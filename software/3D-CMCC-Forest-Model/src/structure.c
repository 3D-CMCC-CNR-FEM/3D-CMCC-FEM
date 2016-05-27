/*structure.c*/

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "types.h"
#include "constants.h"
#include "logger.h"

extern logger_t* g_log;

void Annual_numbers_of_layers (CELL *const c)
{
	/*determines number of layer in function of:
	-differences between tree height classes
	-vegetative or un-vegetative period
	to determine crowding competition */

	int height;
	int age;
	int species;
	double current_height;
	double previous_height;

	//height differences in meter to consider trees in two different layers

	logger(g_log, "****ANNUAL_FOREST_STRUCTURE_ROUTINE****\n");
	logger(g_log, "--NUMBER OF ANNUAL LAYERS--\n");

	if (settings->spatial == 'u')
	{
		//the model sorts starting from highest tree class
		qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_asc);

		for ( height = c->heights_count - 1; height >= 0; height-- )
		{
			for ( age = c->heights[height].ages_count - 1 ; age >= 0 ; age-- )
			{
				for (species = 0; species < c->heights[height].ages[age].species_count; species++)
				{
					current_height = c->heights[height].value;
					if (height == c->heights_count - 1 )
					{
						c->annual_layer_number = 1;
						previous_height = current_height;
					}
					else
					{
						if ((previous_height -current_height ) > settings->tree_layer_limit)
						{
							c->annual_layer_number += 1;
							previous_height = current_height;
						}
						else
						{
							previous_height = current_height;
						}
					}
				}
			}
		}
		logger(g_log, "ANNUAL LAYERS NUMBER = %d\n", c->annual_layer_number);

		//ASSIGN ANNUAL Z VALUE
		for ( height = c->heights_count - 1; height >= 0; height-- )
		{
			switch (c->annual_layer_number)
			{
			case 1:
				c->heights[height].z = c->annual_layer_number - 1;
				//logger(g_log, "height %f, z %d\n", c->heights[height].value, c->heights[height].z);
				break;
			case 2:
				if (height == c->heights_count - 1 )
				{
					c->heights[height].z = c->annual_layer_number - 1;
					//logger(g_log, "height %f, z %d\n", c->heights[height].value, c->heights[height].z);
				}
				else
				{
					if ((c->heights[height+1].value - c->heights[height].value) > settings->tree_layer_limit)
					{
						c->heights[height].z = c->heights[height+1].z - 1;
						//logger(g_log, "height = %f, z = %d\n", c->heights[height].value, c->heights[height].z);
					}
					else
					{
						c->heights[height].z = c->heights[height+1].z;
						//logger(g_log, "height = %f, z = %d\n", c->heights[height].value, c->heights[height].z);
					}
				}
				break;
			case 3:
				if (height == c->heights_count - 1)
				{
					c->heights[height].z = c->annual_layer_number - 1;
					//logger(g_log, "height = %f, z = %d\n", c->heights[height].value, c->heights[height].z);
				}
				else
				{
					if ((c->heights[height+1].value - c->heights[height].value) > settings->tree_layer_limit)
					{
						c->heights[height].z = c->heights[height+1].z - 1;
						//logger(g_log, "height = %f, z = %d\n", c->heights[height].value, c->heights[height].z);
					}
					else
					{
						c->heights[height].z = c->heights[height+1].z;
						//logger(g_log, "height = %f, z = %d\n", c->heights[height].value, c->heights[height].z);
					}
				}
				break;
			}
		}
		logger(g_log, "NUMBER OF DIFFERENT LAYERS = %d\n", c->annual_layer_number);
	}
	else
	{
		//for spatial version only one layer
		logger(g_log, "SPATIAL VERSION ONLY ONE LAYER\n");
		c->annual_layer_number = 1;
		logger(g_log, "ANNUAL LAYERS NUMBER = %d\n", c->annual_layer_number);
		for ( height = c->heights_count - 1; height >= 0; height-- )
		{
			c->heights[height].z = 0;
		}

	}
}


void Daily_Forest_structure (CELL *const c,int day,int month,int years)
{
	int height;
	int age;
	int species;
	int tree_number;
	double layer_cover;

	double potential_maximum_crown_diameter,
	potential_minimum_crown_diameter;

	double potential_maximum_crown_area,
	potential_minimum_crown_area;

	double potential_maximum_density,
	potential_minimum_density;


	c->height_class_in_layer_dominant_counter = 0;
	c->height_class_in_layer_dominated_counter = 0;
	c->height_class_in_layer_subdominated_counter = 0;
	c->tree_number_dominant = 0;
	c->tree_number_dominated = 0;
	c->tree_number_subdominated = 0;
	c->layer_cover_dominant = 0;
	c->layer_cover_dominated = 0;
	c->layer_cover_subdominated = 0;

	logger(g_log, "\n\n***FOREST_STRUCTURE***\n");
	if (settings->spatial == 'u')
	{
		for (height = c->heights_count - 1; height >= 0; height-- )
		{
			qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_asc);

			for (age = c->heights[height].ages_count - 1; age >= 0; age --)
			{
				for (species = c->heights[height].ages[age].species_count - 1; species >= 0; species -- )
				{
					/* define numbers of height classes and density for each layer (if present) */

					switch (c->annual_layer_number)
					{
					case 1:
						c->height_class_in_layer_dominant_counter += 1;
						c->tree_number_dominant += c->heights[height].ages[age].species[species].counter[N_TREE];
						c->density_dominant = c->tree_number_dominant / settings->sizeCell;
						break;
					case 2:
						if (c->heights[height].z == c->annual_layer_number- 1)
						{
							c->height_class_in_layer_dominant_counter += 1;
							c->tree_number_dominant += c->heights[height].ages[age].species[species].counter[N_TREE];
							c->density_dominant = c->tree_number_dominant / settings->sizeCell;
						}
						else
						{
							c->height_class_in_layer_dominated_counter += 1;
							c->tree_number_dominated += c->heights[height].ages[age].species[species].counter[N_TREE];
							c->density_dominated = c->tree_number_dominated / settings->sizeCell;
						}
						break;
					case 3:
						if (c->heights[height].z == c->annual_layer_number - 1)
						{
							c->height_class_in_layer_dominant_counter += 1;
							c->tree_number_dominant += c->heights[height].ages[age].species[species].counter[N_TREE];
							c->density_dominant = c->tree_number_dominant / settings->sizeCell;
						}
						else if (c->heights[height].z == c->annual_layer_number - 2)
						{
							c->height_class_in_layer_dominated_counter += 1;
							c->tree_number_dominated += c->heights[height].ages[age].species[species].counter[N_TREE];
							c->density_dominated = c->tree_number_dominated / settings->sizeCell;
						}
						else
						{
							c->height_class_in_layer_subdominated_counter += 1;
							c->tree_number_subdominated  += c->heights[height].ages[age].species[species].counter[N_TREE];
							c->density_subdominated = c->tree_number_subdominated / settings->sizeCell;
						}
						break;
					}
				}
			}
		}

		if (c->annual_layer_number == 1)
		{
			logger(g_log, "Number of adult height classes in layer 0 = %d\n", c->height_class_in_layer_dominant_counter);
			logger(g_log, "Tree number in layer 0 = %d trees\n", c->tree_number_dominant);
			logger(g_log, "Density in layer 0 = %f trees/area\n", c->density_dominant);
		}
		if (c->annual_layer_number == 2)
		{
			logger(g_log, "Number of adult height classes in layer 1 = %d\n", c->height_class_in_layer_dominant_counter);
			logger(g_log, "Number of adult height classes in layer 0 = %d\n", c->height_class_in_layer_dominated_counter);
			logger(g_log, "Tree number in layer 1 = %d \n", c->tree_number_dominant);
			logger(g_log, "Tree number in layer 0 = %d \n", c->tree_number_dominated);
			logger(g_log, "Density in layer 1 = %f trees/area\n", c->density_dominant);
			logger(g_log, "Density in layer 0 = %f trees/area\n", c->density_dominated);
		}
		if (c->annual_layer_number > 2)
		{
			logger(g_log, "Number of adult height classes in layer 2 = %d\n", c->height_class_in_layer_dominant_counter);
			logger(g_log, "Number of adult height classes in layer 1 = %d\n", c->height_class_in_layer_dominated_counter);
			logger(g_log, "Number of adult height classes in layer 0 = %d\n", c->height_class_in_layer_subdominated_counter);
			logger(g_log, "Tree number in layer 2 = %d \n", c->tree_number_dominant);
			logger(g_log, "Tree number in layer 1 = %d \n", c->tree_number_dominated);
			logger(g_log, "Tree number in layer 0 = %d \n", c->tree_number_subdominated);
			logger(g_log, "Density in layer 2 = %f trees/area\n", c->density_dominant);
			logger(g_log, "Density in layer 1 = %f trees/area\n", c->density_dominated);
			logger(g_log, "Density in layer 0 = %f trees/area\n", c->density_subdominated);
		}

		height = 0;

		//todo: control if with a drastic tree number reduction (e.g. management) there's a unrealistic strong variation in DBHDCeffective

		/* Compute class canopy cover */
		for ( height = c->heights_count - 1; height >= 0; height-- )
		{
			qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_asc);

			for (age = c->heights[height].ages_count - 1; age >= 0; age --)
			{
				for (species = c->heights[height].ages[age].species_count - 1; species >= 0; species -- )
				{
					logger(g_log, "\n\n**GET DATA FOR CANOPY HORIZONTAL STRUCTURE BASED ON EFFECTIVE STAND DBH\n\n");


					/* computing maximum and minimum potential crown diameter and area */
					/* assuming that at the beginning of simulation forest structure is at "equilibrium" */

					logger(g_log, "if Low density\n");

					potential_maximum_crown_diameter = c->heights[height].ages[age].species[species].value[DBHDCMAX] *
							c->heights[height].ages[age].species[species].value[AVDBH];
					potential_maximum_crown_area = pow(((potential_maximum_crown_diameter)/2),2)*Pi;
					logger(g_log, "potential maximum crown area with DBHDCMAX = %f m^2\n", potential_maximum_crown_area);

					potential_minimum_density =settings->sizeCell /potential_maximum_crown_area;
					logger(g_log, "number of potential minimum trees with DBHDCMAX = %f\n", potential_minimum_density);

					logger(g_log, "if High density\n");

					potential_minimum_crown_diameter = c->heights[height].ages[age].species[species].value[DBHDCMIN]*
							c->heights[height].ages[age].species[species].value[AVDBH];
					potential_minimum_crown_area = pow(((potential_minimum_crown_diameter)/2),2)*Pi;
					logger(g_log, "potential minimum crown area with DBHDCMIN = %f m^2\n", potential_minimum_crown_area);

					potential_maximum_density = settings->sizeCell /potential_minimum_crown_area;
					logger(g_log, "number of potential maximum trees with DBHDCMIN = %f\n", potential_maximum_density);

					c->heights[height].ages[age].species[species].value[DENMAX] = potential_maximum_density/settings->sizeCell;
					logger(g_log, "potential density with dbhdcmax (low density) = %f (%f tree)\n", c->heights[height].ages[age].species[species].value[DENMAX],
							c->heights[height].ages[age].species[species].value[DENMAX] * settings->sizeCell);

					c->heights[height].ages[age].species[species].value[DENMIN] = potential_minimum_density/settings->sizeCell;
					logger(g_log, "potential density with dbhdcmin (high density) = %f (%f tree)\n", c->heights[height].ages[age].species[species].value[DENMIN],
							c->heights[height].ages[age].species[species].value[DENMIN] * settings->sizeCell);

					logger(g_log, "\n\n**CANOPY COVER from DBH-DC Function layer %d dbh %f species %s **\n", c->heights[height].z,
							c->heights[height].ages[age].species[species].value[AVDBH], c->heights[height].ages[age].species[species].name);

					if (c->heights[height].ages[age].species[species].value[DBHDCMAX] == -9999
							&& c->heights[height].ages[age].species[species].value[DENMIN] == -9999)
					{
						/*compute maximum crown area*/
						//TODO CHECK IF USE IT
						/*for references and variables see "Forest Mensuration" book 4th edition
						 * B. Husch, T.W. Beers, J.A. Kershaw Jr.
						 * edited by John Wiley & Sons, Inc
						 *
						 * and Krajicek, et al., "Crown competition: a measure of density.
						 * For. Sci. 7:36-42
						 *
						 * Lhotka and Loewenstein 2008, Can J For Res
						 */
						c->heights[height].ages[age].species[species].value[MCA] = ((100.0*Pi)/(4*settings->sizeCell))*(9.7344+(11.48612*c->heights[height].ages[age].species[species].value[AVDBH]
						                                                                                                                                                                     +(3.345241*pow(c->heights[height].ages[age].species[species].value[AVDBH], 2))));
						logger(g_log, "-MCA (Maximum Crown Area) = %f m^2\n", c->heights[height].ages[age].species[species].value[MCA]);
						c->heights[height].ages[age].species[species].value[MCD] = 2.0 * sqrt(c->heights[height].ages[age].species[species].value[MCA]/Pi);
						logger(g_log, "-MCD (Maximum Crown Diameter) = %f m\n", c->heights[height].ages[age].species[species].value[MCD]);
						/*recompute DBHDCmax and DENmin from MCA*/
						/*17 Oct 2013*/
						c->heights[height].ages[age].species[species].value[DBHDCMAX] = c->heights[height].ages[age].species[species].value[MCD]
						                                                                                                                    /c->heights[height].ages[age].species[species].value[AVDBH];
						logger(g_log, "-recomputed DBHDCMAX = %f \n", c->heights[height].ages[age].species[species].value[DBHDCMAX]);
						c->heights[height].ages[age].species[species].value[DENMIN] = 1.0/c->heights[height].ages[age].species[species].value[MCA];
						logger(g_log, "-recomputed DENMIN = %f tree/sizecell\n", c->heights[height].ages[age].species[species].value[DENMIN]);
					}


					/* define DBHDC taking into account layer density */
					logger(g_log, "DBHDCMAX = %f\n", c->heights[height].ages[age].species[species].value[DBHDCMAX]);
					logger(g_log, "DBHDCMIN = %f\n", c->heights[height].ages[age].species[species].value[DBHDCMIN]);
					logger(g_log, "DENMAX = %f\n", c->heights[height].ages[age].species[species].value[DENMAX]);
					logger(g_log, "DENMIN = %f\n", c->heights[height].ages[age].species[species].value[DENMIN]);
					logger(g_log, "density in dominant layer = %f \n", c->density_dominant);
					/*only one dominant layer*/
					switch(c->annual_layer_number)
					{
					/* only one layer */
					case 1:
						logger(g_log, "density in dominant layer = %f (%f trees)\n", c->density_dominant,c->density_dominant * settings->sizeCell);
						if (c->density_dominant > c->heights[height].ages[age].species[species].value[DENMAX])
						{
							c->density_dominant = c->heights[height].ages[age].species[species].value[DENMAX];
						}
						if (c->density_dominant < c->heights[height].ages[age].species[species].value[DENMIN])
						{
							c->density_dominant = c->heights[height].ages[age].species[species].value[DENMIN];
						}

						c->heights[height].ages[age].species[species].value[DBHDC_EFF] = ((c->heights[height].ages[age].species[species].value[DBHDCMIN] -
								c->heights[height].ages[age].species[species].value[DBHDCMAX] ) / (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
								* (c->density_dominant - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMAX]);

						logger(g_log, "DBHDC effective to apply for dominant = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
						break;
						/*two layer*/
					case 2:
						/* dominant layers */
						if (c->heights[height].z == 1)
						{
							if (c->density_dominant > c->heights[height].ages[age].species[species].value[DENMAX])
							{
								c->density_dominant = c->heights[height].ages[age].species[species].value[DENMAX];
							}
							if (c->density_dominant < c->heights[height].ages[age].species[species].value[DENMIN])
							{
								c->density_dominant = c->heights[height].ages[age].species[species].value[DENMIN];
							}
							c->heights[height].ages[age].species[species].value[DBHDC_EFF] = (( c->heights[height].ages[age].species[species].value[DBHDCMIN] - c->heights[height].ages[age].species[species].value[DBHDCMAX] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_dominant - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMAX]);
							logger(g_log, "DBHDC effective to apply for dominant = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
						}
						/* dominated layer */
						else
						{
							if (c->density_dominated > c->heights[height].ages[age].species[species].value[DENMAX])
							{
								c->density_dominated = c->heights[height].ages[age].species[species].value[DENMAX];
							}
							if (c->density_dominated < c->heights[height].ages[age].species[species].value[DENMIN])
							{
								c->density_dominated = c->heights[height].ages[age].species[species].value[DENMIN];
							}
							c->heights[height].ages[age].species[species].value[DBHDC_EFF] = (( c->heights[height].ages[age].species[species].value[DBHDCMIN] - c->heights[height].ages[age].species[species].value[DBHDCMAX] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_dominated - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMAX]);
							logger(g_log, "DBHDC effective to apply for dominated = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
						}
						break;
						/*three layers*/
					case 3:
						/* dominant layers */
						if (c->heights[height].z == 2)
						{
							if (c->density_dominant > c->heights[height].ages[age].species[species].value[DENMAX])
							{
								c->density_dominant = c->heights[height].ages[age].species[species].value[DENMAX];
							}
							if (c->density_dominant < c->heights[height].ages[age].species[species].value[DENMIN])
							{
								c->density_dominant = c->heights[height].ages[age].species[species].value[DENMIN];
							}
							c->heights[height].ages[age].species[species].value[DBHDC_EFF] = (( c->heights[height].ages[age].species[species].value[DBHDCMIN] - c->heights[height].ages[age].species[species].value[DBHDCMAX] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_dominant - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMAX]);
							logger(g_log, "DBHDC effective to apply for dominant = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
						}
						/* dominated layer */
						if (c->heights[height].z == 1)
						{
							if (c->density_dominated > c->heights[height].ages[age].species[species].value[DENMAX])
							{
								c->density_dominated = c->heights[height].ages[age].species[species].value[DENMAX];
							}
							if (c->density_dominated < c->heights[height].ages[age].species[species].value[DENMIN])
							{
								c->density_dominated = c->heights[height].ages[age].species[species].value[DENMIN];
							}
							c->heights[height].ages[age].species[species].value[DBHDC_EFF] = (( c->heights[height].ages[age].species[species].value[DBHDCMIN] - c->heights[height].ages[age].species[species].value[DBHDCMAX] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_dominated - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMAX]);
							logger(g_log, "DBHDC effective to apply for dominated = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
						}
						/* subdominant layer */
						else
						{
							if (c->density_subdominated > c->heights[height].ages[age].species[species].value[DENMAX])
							{
								c->density_subdominated = c->heights[height].ages[age].species[species].value[DENMAX];
							}
							if (c->density_subdominated < c->heights[height].ages[age].species[species].value[DENMIN])
							{
								c->density_subdominated = c->heights[height].ages[age].species[species].value[DENMIN];
							}
							c->heights[height].ages[age].species[species].value[DBHDC_EFF] = (( c->heights[height].ages[age].species[species].value[DBHDCMIN] - c->heights[height].ages[age].species[species].value[DBHDCMAX] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_subdominated - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMAX]);
							logger(g_log, "DBHDC effective to apply for subdominated = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
						}
						break;
					}

					//check if DBHDCeffective exceeds maximum or minimum values
					if (c->heights[height].ages[age].species[species].value[DBHDC_EFF] > c->heights[height].ages[age].species[species].value[DBHDCMAX])
					{
						//logger(g_log, "DBHDC effective for Dominant Layer > DBHDCMAX!!!\n");
						c->heights[height].ages[age].species[species].value[DBHDC_EFF] = c->heights[height].ages[age].species[species].value[DBHDCMAX];
						logger(g_log, "DBHDC effective applied is DBHDCMAX = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
					}
					if (c->heights[height].ages[age].species[species].value[DBHDC_EFF] < c->heights[height].ages[age].species[species].value[DBHDCMIN])
					{
						//logger(g_log, "DBHDC effective for Dominant Layer > DBHDCMAX!!!\n");
						c->heights[height].ages[age].species[species].value[DBHDC_EFF] = c->heights[height].ages[age].species[species].value[DBHDCMIN];
						logger(g_log, "DBHDC effective applied is DBHDCMIN = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
					}

					//assuming no reduction in DBHDCeff
					//to prevent reduction in DBHDCeff
					//test 16 MAY 2016 test check removing block that assumes no reduction in DBHDCeff
					//Answer: it causes an high increment in LAI values!!

					if(day == 0 && month == JANUARY && years == 0)
					{
						c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF] = c->heights[height].ages[age].species[species].value[DBHDC_EFF];
					}
					//test 13 May 2016 test why it considers just first day, month and year!!!!!!!
					//in this way seems that dbhdceff can just increase
					else
					{
						//test 13 MAY 2016 test check why model seems to use wrong DBHDCeff values
						logger(g_log, "DBHDC effective applied = %f\n", c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF]);
						logger(g_log, "DBHDC effective applied = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);

						if (c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF] > c->heights[height].ages[age].species[species].value[DBHDC_EFF])
						{
							logger(g_log, "previous = %g\n eff = %g\n", c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF], c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
							c->heights[height].ages[age].species[species].value[DBHDC_EFF] = c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF];
							logger(g_log, "previous = %g\n eff = %g\n", c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF], c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
						}
						else
						{
							logger(g_log, "previous = %g\n eff = %g\n", c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF], c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
							c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF] = c->heights[height].ages[age].species[species].value[DBHDC_EFF];
							logger(g_log, "previous = %g\n eff = %g\n", c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF], c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
						}
					}


					//test 13 MAY 2016 test check why model seems to use wrong DBHDCeff values
					logger(g_log, "DBHDC effective applied = %f\n", c->heights[height].ages[age].species[species].value[PREVIOUS_DBHDC_EFF]);
					logger(g_log, "DBHDC effective applied = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);


					/* Crown Diameter using DBH-DC */
					c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC] = c->heights[height].ages[age].species[species].value[AVDBH] * c->heights[height].ages[age].species[species].value[DBHDC_EFF];
					logger(g_log, "-Crown Diameter from DBHDC function  = %f m\n", c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC]);

					/* Crown Area using DBH-DC */
					c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC] = ( Pi / 4) * pow (c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC], 2 );
					logger(g_log, "-Crown Area from DBHDC function = %f m^2\n", c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC]);

					/* Canopy Cover using DBH-DC */
					c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC] = c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC]
									  * c->heights[height].ages[age].species[species].counter[N_TREE] / settings->sizeCell;
					logger(g_log, "Canopy cover DBH-DC class related = %f\n", c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC]);
				}
			}
		}

		//compute layer cover
		for ( height = c->heights_count - 1; height >= 0; height-- )
		{
			qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_asc);

			for (age = c->heights[height].ages_count - 1; age >= 0; age --)
			{
				for (species = c->heights[height].ages[age].species_count - 1; species >= 0; species -- )
				{
					//define numbers of height classes, tree number and density for each layer
					switch (c->annual_layer_number)
					{
					case 1:
						c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						break;
					case 2:
						if (c->heights[height].z == c->annual_layer_number- 1)
						{
							c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						}
						else
						{
							c->layer_cover_dominated += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						}
						break;
					case 3:
						if (c->heights[height].z == c->annual_layer_number - 1)
						{
							c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						}
						else if (c->heights[height].z == c->annual_layer_number - 2)
						{
							c->layer_cover_dominated += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						}
						else
						{
							c->layer_cover_subdominated += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						}
						break;
					}
				}
			}		//logger(g_log, "Height class = %f is in layer %d \n", c->heights[height].value, c->heights[height].z);
		}

		if (c->annual_layer_number == 1)
		{
			logger(g_log, "\nLayer cover in layer 0 = %f %% \n", c->layer_cover_dominant * 100);

			c->layer_daily_cc[0] = c->layer_monthly_cc[0] = c->layer_annual_cc[0] = c->layer_cover_dominant;
		}
		if (c->annual_layer_number == 2)
		{
			logger(g_log, "\nLayer cover in layer 1 = %f %%\n", c->layer_cover_dominant * 100);
			logger(g_log, "Layer cover in layer 0 = %f %% \n", c->layer_cover_dominated * 100);

			c->layer_daily_cc[1] = c->layer_monthly_cc[1] = c->layer_annual_cc[1] = c->layer_cover_dominant;
			c->layer_daily_cc[0] = c->layer_monthly_cc[0] = c->layer_annual_cc[0] = c->layer_cover_dominated;

		}
		if (c->annual_layer_number > 2)
		{
			logger(g_log, "\nLayer cover in layer 2 = %f %%\n", c->layer_cover_dominant * 100);
			logger(g_log, "Layer cover in layer 1 = %f %% \n", c->layer_cover_dominated * 100);
			logger(g_log, "Layer cover in layer 0 = %f %% \n", c->layer_cover_subdominated * 100);

			c->layer_daily_cc[2] = c->layer_monthly_cc[2] = c->layer_annual_cc[2] = c->layer_cover_dominant;
			c->layer_daily_cc[1] = c->layer_monthly_cc[1] = c->layer_annual_cc[1] = c->layer_cover_dominated;
			c->layer_daily_cc[0] = c->layer_monthly_cc[0] = c->layer_annual_cc[0] = c->layer_cover_subdominated;
		}

		//test 13 MAY 2016 test
		//try to reduce step by step DBHDC up to minimum DBHDC value before call "Layer_cover_mortality" function as a sort of self-thinning

		//the model makes die trees of the lower height class for that layer because
		//it passes through the function sort_by_height_desc the height classes starting from the lowest

		qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_desc);

		for ( height = c->heights_count - 1; height >= 0; height-- )
		{
			for (age = c->heights[height].ages_count - 1; age >= 0; age --)
			{
				for (species = c->heights[height].ages[age].species_count - 1; species >= 0; species -- )
				{
					//define numbers of height classes, tree number and density for each layer
					switch (c->annual_layer_number)
					{
					case 1:
						if (c->layer_cover_dominant >= settings->max_layer_cover)
						{
							//mortality
							layer_cover = c->layer_cover_dominant;
							tree_number = c->tree_number_dominant;
							Layer_cover_mortality (c, height, age, species, layer_cover, tree_number);
							logger(g_log, "Recomputed Layer cover in layer 0 = %f %% \n", c->layer_cover_dominant * 100);
						}
						break;
					case 2:
						if (c->heights[height].z == c->annual_layer_number- 1)
						{
							if (c->layer_cover_dominant >= settings->max_layer_cover)
							{
								//mortality
								layer_cover = c->layer_cover_dominant;
								tree_number = c->tree_number_dominant;
								Layer_cover_mortality (c, height, age, species, layer_cover, tree_number);
								logger(g_log, "Recomputed Layer cover in layer 1 = %f %% \n", c->layer_cover_dominant * 100);
							}
						}
						else
						{
							if (c->layer_cover_dominated >= settings->max_layer_cover)
							{
								//mortality
								layer_cover = c->layer_cover_dominated;
								tree_number = c->tree_number_dominated;
								Layer_cover_mortality (c, height, age, species, layer_cover, tree_number);
								logger(g_log, "Recomputed Layer cover in layer 0 = %f %% \n", c->layer_cover_dominated * 100);
							}
						}

						break;
					case 3:
						if (c->heights[height].z == c->annual_layer_number- 1)
						{
							if (c->layer_cover_dominant >= settings->max_layer_cover)
							{
								//mortality
								layer_cover = c->layer_cover_dominant;
								tree_number = c->tree_number_dominant;
								Layer_cover_mortality (c, height, age, species, layer_cover, tree_number);
								logger(g_log, "Recomputed Layer cover in layer 2 = %f %% \n", c->layer_cover_dominant * 100);
							}
						}
						else if (c->heights[height].z == c->annual_layer_number - 2)
						{
							if (c->layer_cover_dominated >= settings->max_layer_cover)
							{
								//mortality
								layer_cover = c->layer_cover_dominated;
								tree_number = c->tree_number_dominated;
								Layer_cover_mortality (c, height, age, species, layer_cover, tree_number);
								logger(g_log, "Recomputed Layer cover in layer 1 = %f %% \n", c->layer_cover_dominated * 100);
							}
						}
						else
						{
							if (c->layer_cover_subdominated >= settings->max_layer_cover)
							{
								//mortality
								layer_cover = c->layer_cover_subdominated;
								tree_number = c->tree_number_subdominated;
								Layer_cover_mortality (c, height, age, species, layer_cover, tree_number);
								logger(g_log, "Recomputed Layer cover in layer 0 = %f %% \n", c->layer_cover_subdominated * 100);
							}
						}
						break;
					}
				}
			}
		}
	}
	else
	{
		for ( height = c->heights_count - 1; height >= 0; height-- )
		{
			qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_asc);
			for (age = c->heights[height].ages_count - 1; age >= 0; age --)
			{
				for (species = c->heights[height].ages[age].species_count - 1; species >= 0; species -- )
				{
					c->height_class_in_layer_dominant_counter += 1;
					c->tree_number_dominant += c->heights[height].ages[age].species[species].counter[N_TREE];
					c->density_dominant = c->tree_number_dominant / settings->sizeCell;

					c->heights[height].ages[age].species[species].value[DBHDC_EFF] = (( c->heights[height].ages[age].species[species].value[DBHDCMAX] - c->heights[height].ages[age].species[species].value[DBHDCMIN] )
							/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
							* (c->density_dominant - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMIN]);
					logger(g_log, "DBHDC effective to apply = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);


					/* check */
					if (c->heights[height].ages[age].species[species].value[DBHDC_EFF] > c->heights[height].ages[age].species[species].value[DBHDCMAX])
					{
						c->heights[height].ages[age].species[species].value[DBHDC_EFF] = c->heights[height].ages[age].species[species].value[DBHDCMAX];
						logger(g_log, "DBHDC effective to apply = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
					}
					else if (c->heights[height].ages[age].species[species].value[DBHDC_EFF] < c->heights[height].ages[age].species[species].value[DBHDCMIN])
					{
						c->heights[height].ages[age].species[species].value[DBHDC_EFF] = c->heights[height].ages[age].species[species].value[DBHDCMIN];
						logger(g_log, "DBHDC effective to apply = %f\n", c->heights[height].ages[age].species[species].value[DBHDC_EFF]);
					}

					/* Crown Diameter using DBH-DC */

					c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC] = c->heights[height].ages[age].species[species].value[AVDBH] * c->heights[height].ages[age].species[species].value[DBHDC_EFF];
					logger(g_log, "-Crown Diameter from DBHDC function  = %f m\n", c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC]);

					//Crown Area using DBH-DC
					c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC] = ( Pi / 4) * pow (c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC], 2 );
					logger(g_log, "-Crown Area from DBHDC function = %f m^2\n", c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC]);


					//Canopy Cover using DBH-DC

					c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC] = c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC] * c->heights[height].ages[age].species[species].counter[N_TREE] / settings->sizeCell;
					logger(g_log, "-Canopy Cover from DBHDC function = %f \n", c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC]);
					//Canopy Layer Cover
					c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];



				}
			}
		}
		logger(g_log, "Number of adult height classes in layer 0 = %d\n", c->height_class_in_layer_dominant_counter);
		logger(g_log, "Tree number in layer 0 = %d \n", c->tree_number_dominant);
		c->density_dominant = c->tree_number_dominant / settings->sizeCell;
		logger(g_log, "Density in layer 0 = %f trees/area\n", c->density_dominant);

		if (c->layer_cover_dominant >=  settings->max_layer_cover)
		{
			logger(g_log, "Layer cover exceeds max layer cover!!!\n");
		}

	}
	logger(g_log, "*************************************************** \n");
}

//define VEG_UNVEG for deciduous species
void Daily_vegetative_period (CELL *c, const MET_DATA *const met, int month, int day)
{

	static int height;
	static int age;
	static int species;






	/*VEG_UNVEG = 1 for veg period, = 0 for Un-Veg period*/

	c->Veg_Counter = 0;

	logger(g_log, "\n\n\n****GET_DAILY_FOREST_STRUCTURE_ROUTINE for cell (%d, %d)****\n", c->x, c->y);

	//assign value for VEG_UNVEG (1 for veg, 0 for Unveg) and compute number of classes in veg period

	for (height = c->heights_count - 1; height >= 0; height-- )
	{
		for (age = c->heights[height].ages_count - 1 ; age >= 0 ; age-- )
		{
			for (species = 0; species < c->heights[height].ages[age].species_count; species++)
			{

				if (day == 0 && month == 0)
				{
					c->heights[height].ages[age].species[species].counter[LEAF_FALL_COUNTER] = 0;
				}

				/*PHENOLOGY = 0 FOR DECIDUOUS*/
				if (c->heights[height].ages[age].species[species].value[PHENOLOGY] == 0.1 || c->heights[height].ages[age].species[species].value[PHENOLOGY] == 0.2)
				{
					logger(g_log, "-GET ANNUAL VEGETATIVE DAYS for species %s -\n", c->heights[height].ages[age].species[species].name);
					if (settings->spatial == 's')
					{
						//logger(g_log, "Spatial version \n");

						//veg period
						if (met[month].d[day].ndvi_lai > 0.1)
						{
							c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 1;
							c->Veg_Counter += 1;
							logger(g_log, "%s is in veg period\n", c->heights[height].ages[age].species[species].name);
						}
						//unveg period
						else
						{
							c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 0;
							logger(g_log, "%s is in un-veg period\n", c->heights[height].ages[age].species[species].name);
						}
					}
					else
					{

						/*compute annual days of leaf fall*/
						c->heights[height].ages[age].species[species].counter[DAY_FRAC_FOLIAGE_REMOVE] = (int)(c->heights[height].ages[age].species[species].value[LEAF_FALL_FRAC_GROWING]
						                                                                                                                                           * c->heights[height].ages[age].species[species].counter[DAY_VEG_FOR_LITTERFALL_RATE]);
						logger(g_log, "Days of leaf fall for deciduous = %d day\n", c->heights[height].ages[age].species[species].counter[DAY_FRAC_FOLIAGE_REMOVE]);
						//monthly rate of foliage reduction

						//currently the model considers a linear reduction in leaf fall
						//it should be a negative sigmoid function
						//todo: create a sigmoid function
						c->heights[height].ages[age].species[species].value[FOLIAGE_REDUCTION_RATE] = 1.0 / (c->heights[height].ages[age].species[species].counter[DAY_FRAC_FOLIAGE_REMOVE] + 1);
						logger(g_log, "foliage reduction rate = %f,  = %f%%\n", c->heights[height].ages[age].species[species].value[FOLIAGE_REDUCTION_RATE], c->heights[height].ages[age].species[species].value[FOLIAGE_REDUCTION_RATE] * 100);


						//todo decidere se utlizzare growthend o mindaylenght
						//lo stesso approccio deve essere usato anche in Get_Veg_Days func
						//currently model can simulate only forests in boreal hemisphere
						if ((met[month].d[day].thermic_sum >= c->heights[height].ages[age].species[species].value[GROWTHSTART] && month <= 6)
								|| (met[month].d[day].daylength >= c->heights[height].ages[age].species[species].value[MINDAYLENGTH] && month >= 6 && c->north == 0))
						{
							c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 1;
							c->Veg_Counter += 1;
							//FIXME check if it works (daily)
							/* we consider that plants tend to occupy the part of the cell not covered by the others */
							c->cell_cover += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
							if(c->cell_cover > 1.0)
							{
								c->cell_cover = 1.0;
							}
							logger(g_log, "%s is in veg period\n", c->heights[height].ages[age].species[species].name);
							logger(g_log, "cell_cover counter = %f\n", c->cell_cover);
						}
						else
						{
							//check for case 0 of allocation
							if (met[month].d[day].daylength <= c->heights[height].ages[age].species[species].value[MINDAYLENGTH] && month >= 6 && c->north == 0 )
							{

								logger(g_log, "DAY_FRAC_FOLIAGE_REMOVE %d\n", c->heights[height].ages[age].species[species].counter[DAY_FRAC_FOLIAGE_REMOVE]);

								c->heights[height].ages[age].species[species].counter[LEAF_FALL_COUNTER]  += 1;
								//check
								if(c->heights[height].ages[age].species[species].counter[LEAF_FALL_COUNTER]  == 1)
								{
									//assign value of thermic sum
									c->heights[height].ages[age].species[species].value[THERMIC_SUM_FOR_END_VEG] = met[month].d[day].thermic_sum;
									//logger(g_log, "thermic_sum END OF VEG = %f °C\n", c->heights[height].ages[age].species[species].value[THERMIC_SUM_FOR_END_VEG]);
								}

								//check
								if(c->heights[height].ages[age].species[species].counter[LEAF_FALL_COUNTER]  <= (int)c->heights[height].ages[age].species[species].counter[DAY_FRAC_FOLIAGE_REMOVE])
								{
									/*days of leaf fall*/
									c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 1;
									c->Veg_Counter += 1;
								}
								else
								{
									/*outside days of leaf fall*/
									c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 0;
								}

							}
							else
							{
								c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 0;
								logger(g_log, "%s is in un-veg period\n", c->heights[height].ages[age].species[species].name);
							}
						}
					}
				}
				/*PHENOLOGY = 1 FOR EVERGREEN*/
				else
				{
					c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 1;
					logger(g_log, "Veg period = %d \n", c->heights[height].ages[age].species[species].counter[VEG_UNVEG]);
					c->Veg_Counter += 1;
					logger(g_log, "%s is in veg period\n", c->heights[height].ages[age].species[species].name);
				}
			}
		}
	}
	logger(g_log, "classes in veg period = %d\n", c->Veg_Counter);
}

void Daily_numbers_of_layers (CELL *const c)
{
	//determines number of vegetative layer in function of:
	//-differences between tree height classes
	//-vegetative or un-vegetative period
	static int height;
	static int age;
	static int species;
	static double current_height;
	static double previous_height;


	c->daily_layer_number = 0;

	logger(g_log, "\n--GET NUMBER OF DAILY LAYERS (Layer in Veg)--\n");


	qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_asc);

	for ( height = c->heights_count - 1; height >= 0; height-- )
	{
		for ( age = c->heights[height].ages_count - 1 ; age >= 0 ; age-- )
		{
			for (species = 0; species < c->heights[height].ages[age].species_count; species++)
			{
				current_height = c->heights[height].value;

				if (c->heights_count > 1 )
				{
					if (height == c->heights_count -1 && c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
					{
						c->daily_layer_number += 1;
						previous_height = current_height;
					}
					if ((previous_height - current_height ) > settings->tree_layer_limit && c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
					{
						c->daily_layer_number += 1;
						previous_height = current_height;
					}
				}
				if (c->heights_count == 1  && c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
				{
					c->daily_layer_number = 1;
				}
			}
		}
	}
	logger(g_log, "number of vegetative layers = %d\n", c->daily_layer_number);

	//logger(g_log, "height count = %d \n", c->heights_count);
}

void Daily_layer_cover (CELL * c, const MET_DATA *const met, int month, int day)
{

	//compute if is in veg period
	static int height;
	static int age;
	static int species;

	c->layer_cover_dominant = 0;
	c->layer_cover_dominated = 0;
	c->layer_cover_subdominated = 0;


	logger(g_log, "\nDAILY_FOREST_STRUCTURE_ROUTINE\n");

	logger(g_log, "Determine Effective Layer Cover \n");

	for (height = c->heights_count - 1; height >= 0; height -- )
	{
		for (age = c->heights[height].ages_count - 1; age >= 0; age --)
		{
			for (species = c->heights[height].ages[age].species_count - 1; species >= 0; species -- )
			{
				if ( c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
				{
					if (c->daily_layer_number == 3)
					{
						switch (c->heights[height].z)
						{
						case 2:
							//logger(g_log, "z = %d\n", c->heights[height].z);
							c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
							//logger(g_log, "Layer cover in layer 2 = %f %% \n", c->layer_cover_dominant * 100);
							break;
						case 1:
							//logger(g_log, "z = %d\n", c->heights[height].z);
							c->layer_cover_dominated += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
							//logger(g_log, "Layer cover in layer 1 = %f %% \n", c->layer_cover_dominated * 100);
							break;
						case 0:
							//logger(g_log, "z = %d\n", c->heights[height].z);
							c->layer_cover_subdominated  += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
							//logger(g_log, "Layer cover in layer 0 = %f %% \n", c->layer_cover_subdominated * 100);
							break;
						}
					}
					if (c->daily_layer_number == 2)
					{
						switch (c->heights[height].z)
						{
						case 1:
							//logger(g_log, "z = %d\n", c->heights[height].z);
							c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
							//logger(g_log, "Layer cover in layer 1 = %f %% \n", c->layer_cover_dominant * 100);
							break;
						case 0:
							//logger(g_log, "z = %d\n", c->heights[height].z);
							c->layer_cover_dominated  += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
							//logger(g_log, "Layer cover in layer 0 = %f %% \n", c->layer_cover_dominated * 100);
							break;
						}
					}
					else
					{
						//logger(g_log, "z = %d\n", c->heights[height].z);
						c->layer_cover_dominant  += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						//logger(g_log, "Layer cover in layer 0 = %f %% \n", c->layer_cover_dominant * 100);
					}
				}
				else
				{
					//logger(g_log, "Un-Vegetating layers\n");
					//logger(g_log, "z = %d\n", c->heights[height].z);
				}
			}
		}
	}

	logger(g_log, "Daily layer number = %d\n", c->daily_layer_number);

	if (c->daily_layer_number == 1)
	{
		logger(g_log, "Vegetated Layer cover in layer 0 = %f %% \n", c->layer_cover_dominant * 100);
	}
	if (c->daily_layer_number == 2)
	{
		logger(g_log, "Vegetated Layer cover in layer 1 = %f %%\n", c->layer_cover_dominant * 100);
		logger(g_log, "Vegetated Layer cover in layer 0 = %f %% \n", c->layer_cover_dominated * 100);
	}
	if (c->daily_layer_number > 2)
	{
		logger(g_log, "Vegetated Layer cover in layer 2 = %f %%\n", c->layer_cover_dominant * 100);
		logger(g_log, "Vegetated Layer cover in layer 1 = %f %% \n", c->layer_cover_dominated * 100);
		logger(g_log, "Vegetated Layer cover in layer 0 = %f %% \n", c->layer_cover_subdominated * 100);
	}
	logger(g_log, "*************************************************** \n");

}

/*
void Get_top_layer (CELL *const c, int heights_count, HEIGHT *heights)
{

	logger(g_log, " GET_TOP_LAYER_FUNCTION \n");
	int height;

	assert (heights);



	for ( height = heights_count - 1; height >= 0; height-- )
	{
		logger(g_log, "height dominance = %d \n", c->heights[height].dominance);
		if (c->heights[height].dominance == 1)
		{
			c->heights[height].top_layer = c->heights[height].z ;
			logger(g_log, "-Top layer and in Dominant Light is layer with z = %d\n", c->heights[height].top_layer);
			break;
		}
		if ( c->heights[height].top_layer == -1)
		{
			logger(g_log, "--NO TREES IN VEGETATIVE PERIOD!!!\n");
			logger(g_log, "**********************************************\n");
			break;
		}
	}
}
 */

//This function compute if there is a class age in veg period
void Dominant_Light(HEIGHT *heights, CELL* c, const int count, const MET_DATA *const met, const int month, const int DaysInMonth)
{
	int height;
	int age;
	int species;

	assert(heights);

	if (c->daily_layer_number != 0)
	{
		logger(g_log, "-Dominant Light Index Function-\n");

		//highest z value in veg period determines top_layer value
		for ( height = count- 1; height >= 0; height-- )
		{
			qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_desc);
			for ( age = 0; age < heights[height].ages_count; age++ )
			{
				for ( species = 0; species < heights[height].ages[age].species_count; species++ )
				{
					if (heights[height].ages[age].species[species].counter[VEG_UNVEG]==1)
					{

						if (settings->spatial == 'u')
						{
							c->top_layer = c->heights[height].z;
						}
						else
						{
							c->top_layer = 0;
						}
					}
				}
			}
		}
		logger(g_log, "Daily/Monthly Dominant layer is z = %d\n", c->top_layer);
	}
	//logger(g_log, "-Species in veg period = %d\n", c->Veg_Counter);
}


int Number_of_layers (CELL *c)
{
	// ALESSIOR number_of_layers can be uninitialized
	int number_of_layers = 0;
	//compute number of layers
	if (c->dominant_veg_counter > 0 && c->dominated_veg_counter > 0 && c->subdominated_veg_counter > 0)
	{
		number_of_layers = 3;
		logger(g_log, "THREE LAYERS \n");
	}
	else if (c->dominant_veg_counter > 0 && c->dominated_veg_counter > 0 && c->subdominated_veg_counter == 0)
	{
		number_of_layers = 2;
		logger(g_log, "TWO LAYERS \n");
	}
	else if (c->dominant_veg_counter == 0 && c->dominated_veg_counter > 0 && c->subdominated_veg_counter == 0)
	{
		number_of_layers = 1;
		logger(g_log, "ONE LAYER \n");
	}
	return number_of_layers;

}

extern void Daily_veg_counter (CELL *c, SPECIES *s, int height)
{
	switch (c->daily_layer_number)
	{
	case 3:
		if (c->heights[height].z == 2)
		{
			c->dominant_veg_counter += 1;
		}
		if (c->heights[height].z == 1)
		{
			c->dominated_veg_counter += 1;
		}
		else
		{
			c->subdominated_veg_counter += 1;
		}
		break;
	case 2:
		if (c->heights[height].z == 1)
		{
			c->dominant_veg_counter += 1;
		}
		else
		{
			c->dominated_veg_counter += 1;
		}
		break;
	case 1:
		c->dominant_veg_counter += 1;
		break;
	}
}






