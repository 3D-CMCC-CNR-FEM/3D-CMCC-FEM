/*
 * phenology.c
 *
 *  Created on: 21/mag/2013
 *      Author: alessio
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "types.h"
#include "constants.h"

extern void Get_phenology_phase (CELL * c, const MET_DATA *const met, const int years, const int month, const int day, const int years_of_simulation)
{
	int cell;
	int height;
	int age;
	int species;

	for ( height = c->heights_count -1 ; height >= 0; height-- )
	{
		for ( age = c->heights[height].ages_count - 1 ; age >= 0 ; age-- )
		{
			for (species = 0; species < c->heights[height].ages[age].species_count; species++)
			{
				Log("--GET_DAILY PHENOLOGY for SPECIES %s --\n", c->heights[height].ages[age].species[species].name);
				//defining phenology phase
				if(settings->time == 'd') //for daily version
				{
					if (c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
					{
						if (met[month].d[day].daylength < c->heights[height].ages[age].species[species].value[MINDAYLENGTH] && month > 6/*c->abscission_daylength*/)
						{
							//Leaf fall
							c->heights[height].ages[age].species[species].phenology_phase = 0;
						}
						else
						{
							//Beginning of growing season
							if (c->heights[height].ages[age].species[species].value[LAI] <= c->heights[height].ages[age].species[species].value[PEAK_Y_LAI] * 0.5 )
							{
								c->heights[height].ages[age].species[species].phenology_phase = 1;
							}
							//arealf of beginning of growing season
							if (c->heights[height].ages[age].species[species].value[LAI] > (c->heights[height].ages[age].species[species].value[PEAK_Y_LAI] * 0.5)
									&& c->heights[height].ages[age].species[species].value[LAI] < c->heights[height].ages[age].species[species].value[PEAK_Y_LAI])
							{
								c->heights[height].ages[age].species[species].phenology_phase = 2;
							}
							//Full growing season
							if(fabs (c->heights[height].ages[age].species[species].value[LAI] - c->heights[height].ages[age].species[species].value[PEAK_Y_LAI]) < 0.1)
							{
								c->heights[height].ages[age].species[species].phenology_phase = 3;
							}
						}
					}
					else
					{
						//Unvegetative period
						c->heights[height].ages[age].species[species].phenology_phase = 4;
					}
				}
				else //for monthly version
				{
					if (met[month].daylength < c->heights[height].ages[age].species[species].value[MINDAYLENGTH] && month > 6)
					{
						//Leaf fall
						c->heights[height].ages[age].species[species].phenology_phase = 0;
					}
					else
					{
						if (c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
						{
							//Beginning of growing season
							if (c->heights[height].ages[age].species[species].value[LAI] <= c->heights[height].ages[age].species[species].value[PEAK_Y_LAI] * 0.5 )
							{
								c->heights[height].ages[age].species[species].phenology_phase = 1;
							}
							//arealf of beginning of growing season
							if (c->heights[height].ages[age].species[species].value[LAI] > (c->heights[height].ages[age].species[species].value[PEAK_Y_LAI] * 0.5)
									&& c->heights[height].ages[age].species[species].value[LAI] < c->heights[height].ages[age].species[species].value[PEAK_Y_LAI])
							{
								c->heights[height].ages[age].species[species].phenology_phase = 2;
							}
							//Full growing season
							if(fabs (c->heights[height].ages[age].species[species].value[LAI] - c->heights[height].ages[age].species[species].value[PEAK_Y_LAI]) < 0.1)
							{
								c->heights[height].ages[age].species[species].phenology_phase = 3;
							}
						}
						//Unvegetative period
						else
						{
							c->heights[height].ages[age].species[species].phenology_phase = 4;
						}
					}
				}
				Log("phenology phase = %d\n", c->heights[height].ages[age].species[species].phenology_phase);
			}
		}
	}

}

