/* g-function.c */
#include <stdio.h>
#include <math.h>
#include "soil_settings.h"
#include "constants.h"
#include "settings.h"
#include "yos.h"
#include "logger.h"
#include "matrix.h"

extern settings_t *g_settings;
extern logger_t* g_log;
extern soil_settings_t *g_soil_settings;

int sort_by_years(const void *a, const void *b)
{
	if ( ((yos_t*)a)->year < ((yos_t*)b)->year )
	{
		return -1;
	} else if ( ((yos_t*)a)->year > ((yos_t*)b)->year )
	{
		return 1;
	} else
	{
		return 0;
	}
}
/* todo : implement a better comparison for equality */
int sort_by_heights_asc(const void * a, const void * b)
{
	if ( ((height_t *)a)->value < ((height_t *)b)->value )
	{
		return -1;
	} else if ( ((height_t *)a)->value > ((height_t *)b)->value )
	{
		return 1;
	} else
	{
		return 0;
	}
}

/* todo : implement a better comparison for equality */
int sort_by_heights_desc(const void * a, const void * b)
{
	if ( ((height_t *)a)->value < ((height_t *)b)->value )
	{
		return 1;
	} else if ( ((height_t *)a)->value > ((height_t *)b)->value )
	{
		return -1;
	} else
	{
		return 0;
	}
}

void Pool_fraction(species_t *const s)
{
	s->value[FINE_ROOT_LEAF_FRAC] = s->value[FINE_ROOT_LEAF] / (s->value[FINE_ROOT_LEAF]+1.0);
	s->value[STEM_LEAF_FRAC] = s->value[STEM_LEAF] / (s->value[STEM_LEAF]+1.0);
	s->value[COARSE_ROOT_STEM_FRAC] = s->value[COARSE_ROOT_STEM] / (s->value[COARSE_ROOT_STEM]+1.0);
	s->value[LIVE_TOTAL_WOOD_FRAC] = s->value[LIVE_TOTAL_WOOD] / (s->value[LIVE_TOTAL_WOOD]+1.0);
}

void Abscission_DayLength (cell_t * c)
{
	/*to compute dayleght for stopping growth*/
	//from Schwalm and Ek, 2004
	//but it considers a value independently from species
	c->abscission_daylength = (39132 + (pow (1.088, (g_soil_settings->values[SOIL_LAT] + 60.753))))/(60*60);
	//logger(g_log, "Abscission day length = %f hrs\n", c->abscission_daylength);
}

void Tree_period(cell_t* const c, const int layer, const int height, const int age, const int species)
{
	age_t *a;
	species_t *s;

	a = &c->heights[height].ages[age];
	s = &c->heights[height].ages[age].species[species];
	/*Set Tree period*/
	// 0 = adult tree
	// 1 = young tree

	if ( a->value > s->value[ADULT_AGE])
	{
		s->period = 0;

		if (!c->saplings_counter)
		{
			c->saplings_counter -= 1;
		}
	}
	else
	{
		s->period = 1;
	}
}


void Veg_Days(cell_t *const c,const int day, const int month, const int year)
{
	static int height;
	static int age;
	static int species;

	meteo_t *met;
	species_t *s;

	met = c->years[year].m;
	s = &c->heights[height].ages[age].species[species];

	/* compute annual number of vegetative days */

	logger(g_log, "VEG_DAYS_for year %d\n", year);


	for (height = c->heights_count - 1; height >= 0; height-- )
	{
		for (age = c->heights[height].ages_count - 1 ; age >= 0 ; age-- )
		{
			for (species = 0; species < c->heights[height].ages[age].species_count; species++)
			{
				if (g_settings->spatial == 'u')
				{
					if (s->value[PHENOLOGY] == 0.1 || s->value[PHENOLOGY] == 0.2)
					{
						/* reset 'annual day_veg_for_litterfall_rate'*/
						if (day == 0 && month == JANUARY)
						{
							s->counter[DAY_VEG_FOR_LITTERFALL_RATE] = 0;
						}
						//the same approach must be used in "Get_daily_vegetative_period" func

						if ((met[month].d[day].thermic_sum >= s->value[GROWTHSTART] && month <= 6)
								|| (met[month].d[day].daylength >= s->value[MINDAYLENGTH] && month >= 6))
						{
							s->counter[DAY_VEG_FOR_LITTERFALL_RATE] += 1;
							if (s->counter[DAY_VEG_FOR_LITTERFALL_RATE] == 1)
							{
								logger(g_log, "GDD basis = %d\n", GDD_BASIS);
								logger(g_log, "species %s First day of growing season day = %d month = %d\n", s->name, day+1, month+1);
							}
						}
					}
					else
					{
						s->counter[DAY_VEG_FOR_LITTERFALL_RATE] = 365;
					}
					/* compute last year day the number of days for leaf fall */
					if (day == 30 && month == DECEMBER)
					{
						s->counter[DAY_FRAC_FOLIAGE_REMOVE] =  (int) (s->value[LEAF_FALL_FRAC_GROWING] *
								s->counter[DAY_VEG_FOR_LITTERFALL_RATE]);
						s->counter[DAY_FRAC_FINE_ROOT_REMOVE] = (int) (s->value[LEAF_FALL_FRAC_GROWING] *
								s->counter[DAY_VEG_FOR_LITTERFALL_RATE]);
						logger(g_log, "Day frac foliage remove = %d\n", s->counter[DAY_FRAC_FOLIAGE_REMOVE] );
						//add leaf fall days
						if (s->value[PHENOLOGY] == 0.1 || s->value[PHENOLOGY] == 0.2)
						{
							s->counter[DAY_VEG_FOR_LITTERFALL_RATE] += (int)(s->counter[DAY_VEG_FOR_LITTERFALL_RATE] *
									s->value[LEAF_FALL_FRAC_GROWING]);

						}
						logger(g_log, "-species_t %s TOTAL VEGETATIVE DAYS = %d \n", s->name, s->counter[DAY_VEG_FOR_LITTERFALL_RATE]);
					}
				}
				else
				{
					if (!month)
					{
						s->counter[DAY_VEG_FOR_LITTERFALL_RATE] = 0;
					}
					if (s->value[PHENOLOGY] == 0.1 || s->value[PHENOLOGY] == 0.2)
					{
						if (met[month].d[day].ndvi_lai >= 0.5)
						{
							s->counter[DAY_VEG_FOR_LITTERFALL_RATE] +=1;
						}
					}
					else
					{
						s->counter[DAY_VEG_FOR_LITTERFALL_RATE] = 365;
					}
					if (day == 30 && month == DECEMBER)
					{
						logger(g_log, "----- TOTAL VEGETATIVE DAYS = %d \n\n", s->counter[DAY_VEG_FOR_LITTERFALL_RATE]);
					}
				}
			}
		}
	}
}



