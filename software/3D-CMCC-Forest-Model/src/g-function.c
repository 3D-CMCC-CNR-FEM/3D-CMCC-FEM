/* g-function.c */
#include <stdio.h>
#include <math.h>
#include "common.h"
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

void carbon_pool_fraction(cell_t *c)
{
	int height;
	int age;
	int species;

	height_t *h;
	age_t *a;
	species_t *s;

	for ( height = 0; height < c->heights_count; height++ )
	{
		h = &c->heights[height];

		for ( age = 0; age < h->ages_count; age++ )
		{
			a = &c->heights[height].ages[age];

			for ( species = 0; species < a->species_count; species ++)
			{
				s = &c->heights[height].ages[age].species[species];

				s->value[FINE_ROOT_LEAF_FRAC] = s->value[FINE_ROOT_LEAF] / (s->value[FINE_ROOT_LEAF]+1.0);
				s->value[STEM_LEAF_FRAC] = s->value[STEM_LEAF] / (s->value[STEM_LEAF]+1.0);
				s->value[COARSE_ROOT_STEM_FRAC] = s->value[COARSE_ROOT_STEM] / (s->value[COARSE_ROOT_STEM]+1.0);
				s->value[LIVE_TOTAL_WOOD_FRAC] = s->value[LIVE_TOTAL_WOOD] / (s->value[LIVE_TOTAL_WOOD]+1.0);
			}
		}
	}
}

void Abscission_DayLength (cell_t *c)
{
	/*to compute day length for stopping growth*/
	//from Schwalm and Ek, 2004
	//but it considers a value independently from species
	c->abscission_daylength = (39132 + (pow (1.088, (g_soil_settings->values[SOIL_LAT] + 60.753))))/(60*60);
	//logger(g_log, "Abscission day length = %f hrs\n", c->abscission_daylength);
}

void tree_period(cell_t* const c, const int layer, const int height, const int age, const int species)
{
	age_t *a;
	species_t *s;

	a = &c->heights[height].ages[age];
	s = &c->heights[height].ages[age].species[species];
	/* Set Tree period */
	/* 0 = adult tree */
	/* 1 = young tree */

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


void Veg_Days(cell_t *const c, const int day, const int month, const int year)
{
	int height;
	int age;
	int species;

	meteo_t *met;
	species_t *s;

	met = c->years[year].m;

	/* compute annual number of vegetative days */

	for (height = c->heights_count - 1; height >= 0; height-- )
	{
		for (age = c->heights[height].ages_count - 1 ; age >= 0 ; age-- )
		{
			for (species = 0; species < c->heights[height].ages[age].species_count; species++)
			{
				s = &c->heights[height].ages[age].species[species];

				if ( g_settings->spatial == 'u' )
				{
					if ( s->value[PHENOLOGY] == 0.1 || s->value[PHENOLOGY] == 0.2 )
					{
						/* reset 'annual day_veg_for_litterfall_rate'*/
						if (day == 0 && month == JANUARY)
						{
							s->counter[DAY_VEG_FOR_LEAF_FALL] = 0;
						}
						//the same approach must be used in "Get_daily_vegetative_period" func

						if ((met[month].d[day].thermic_sum >= s->value[GROWTHSTART] && month <= 6)
								|| (met[month].d[day].daylength >= s->value[MINDAYLENGTH] && month >= 6))
						{
							s->counter[DAY_VEG_FOR_LEAF_FALL] += 1;
							if (s->counter[DAY_VEG_FOR_LEAF_FALL] == 1)
							{
								logger(g_log, "GDD basis = %d\n", GDD_BASIS);
								logger(g_log, "species %s First day of growing season day = %d month = %d\n", s->name, day+1, month+1);
							}
						}
					}
					else
					{
						if ( IS_LEAP_YEAR( c->years[year].year ) )
						{
							s->counter[DAY_VEG_FOR_LEAF_FALL] = 366;
						}
						else
						{
							s->counter[DAY_VEG_FOR_LEAF_FALL] = 365;
						}
					}
					/* compute last year day the number of days for leaf fall */
					if (day == 30 && month == DECEMBER)
					{
						s->counter[DAY_FRAC_FOLIAGE_REMOVE] =  (int) (s->value[LEAF_FALL_FRAC_GROWING] *
								s->counter[DAY_VEG_FOR_LEAF_FALL]);
						s->counter[DAY_FRAC_FINE_ROOT_REMOVE] = (int) (s->value[LEAF_FALL_FRAC_GROWING] *
								s->counter[DAY_VEG_FOR_LEAF_FALL]);
						logger(g_log, "Day frac foliage remove = %d\n", s->counter[DAY_FRAC_FOLIAGE_REMOVE] );
						//add leaf fall days
						if (s->value[PHENOLOGY] == 0.1 || s->value[PHENOLOGY] == 0.2)
						{
							s->counter[DAY_VEG_FOR_LEAF_FALL] += (int)(s->counter[DAY_VEG_FOR_LEAF_FALL] *
									s->value[LEAF_FALL_FRAC_GROWING]);

						}
						logger(g_log, "-species %s annual vegetative days = %d \n", s->name, s->counter[DAY_VEG_FOR_LEAF_FALL]);
					}
				}
				else
				{
					if (!month)
					{
						s->counter[DAY_VEG_FOR_LEAF_FALL] = 0;
					}
					if (s->value[PHENOLOGY] == 0.1 || s->value[PHENOLOGY] == 0.2)
					{
						if (met[month].d[day].ndvi_lai >= 0.5)
						{
							s->counter[DAY_VEG_FOR_LEAF_FALL] +=1;
						}
					}
					else
					{
						if ( IS_LEAP_YEAR( c->years[year].year ) )
						{
							s->counter[DAY_VEG_FOR_LEAF_FALL] = 366;
						}
						else
						{
							s->counter[DAY_VEG_FOR_LEAF_FALL] = 365;
						}
					}
					if (day == 30 && month == DECEMBER)
					{
						logger(g_log, "----- TOTAL VEGETATIVE DAYS = %d \n\n", s->counter[DAY_VEG_FOR_LEAF_FALL]);
					}
				}
			}
		}
	}
}



