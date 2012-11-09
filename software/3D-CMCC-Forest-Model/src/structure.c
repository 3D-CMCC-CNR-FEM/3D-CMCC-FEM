/*structure.c*/

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "math.h"
#include "types.h"

/* todo : implement a better comparison for equality */
int sort_by_heights_asc(const void * a, const void * b)
{
	if ( ((HEIGHT *)a)->value < ((HEIGHT *)b)->value )
	{
		return -1;
	}
	else if ( ((HEIGHT *)a)->value > ((HEIGHT *)b)->value )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* todo : implement a better comparison for equality */
int sort_by_heights_desc(const void * a, const void * b)
{
	if ( ((HEIGHT *)a)->value < ((HEIGHT *)b)->value )
	{
		return 1;
	}
	else if ( ((HEIGHT *)a)->value > ((HEIGHT *)b)->value )
	{
		return -1;
	}
	else
	{
		return 0;
	}
}



void Get_annual_numbers_of_layers (CELL *const c)
{
	//determines number of layer in function of:
	//-differences between tree height classes
	//-vegetative or un-vegetative period
	//to determine crowding competition
	int height;
	int age;
	int species;
	float current_height;
	float previous_height;



	//height differences in meter to consider trees in two different layers


	Log("--GET NUMBER OF ANNUAL LAYERS--\n");

	//the model sorts starting from highest tree
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
					if ((previous_height -current_height ) > settings->layer_limit)
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
	Log("ANNUAL LAYERS NUMBER = %d\n", c->annual_layer_number);


	//ASSIGN ANNUAL Z VALUE
	for ( height = c->heights_count - 1; height >= 0; height-- )
	{
		switch (c->annual_layer_number)
		{
		case 1:
			c->heights[height].z = c->annual_layer_number - 1;
			Log("height %g, z %d\n", c->heights[height].value, c->heights[height].z);
			break;
		case 2:
			if (height == c->heights_count - 1 )
			{
				c->heights[height].z = c->annual_layer_number - 1;
				Log("height %g, z %d\n", c->heights[height].value, c->heights[height].z);
			}
			else
			{
				if ((c->heights[height+1].value - c->heights[height].value) > settings->layer_limit)
				{
					c->heights[height].z = c->heights[height+1].z - 1;
					Log("height = %g, z = %d\n", c->heights[height].value, c->heights[height].z);
				}
				else
				{
					c->heights[height].z = c->heights[height+1].z;
					Log("height = %g, z = %d\n", c->heights[height].value, c->heights[height].z);
				}
			}
			break;
		case 3:
			if (height == c->heights_count - 1)
			{
				c->heights[height].z = c->annual_layer_number - 1;
				Log("height = %g, z = %d\n", c->heights[height].value, c->heights[height].z);
			}
			else
			{
				if ((c->heights[height+1].value - c->heights[height].value) > settings->layer_limit)
				{
					c->heights[height].z = c->heights[height+1].z - 1;
					Log("height = %g, z = %d\n", c->heights[height].value, c->heights[height].z);
				}
				else
				{
					c->heights[height].z = c->heights[height+1].z;
					Log("height = %g, z = %d\n", c->heights[height].value, c->heights[height].z);
				}
			}
			break;
		}
	}
	Log("NUMBER OF DIFFERENT LAYERS = %d\n", c->annual_layer_number);
}


void Get_annual_forest_structure (CELL *const c, HEIGHT *const h)
{
	int height;
	int age;
	int species;
	float DBHDCeffective;
	int tree_number;
	float layer_cover;

	c->height_class_in_layer_dominant_counter = 0;
	c->height_class_in_layer_dominated_counter = 0;
	c->height_class_in_layer_subdominated_counter = 0;
	c->tree_number_dominant = 0;
	c->tree_number_dominated = 0;
	c->tree_number_subdominated = 0;
	c->layer_cover_dominant = 0;
	c->layer_cover_dominated = 0;
	c->layer_cover_subdominated = 0;

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
		}		//Log("Height class = %g is in layer %d \n", c->heights[height].value, c->heights[height].z);
	}

	if (c->heights_count == 1)
	{
		Log("Number of adult height classes in layer 1 = %d\n", c->height_class_in_layer_dominant_counter);
		Log("Tree number in layer 1 = %d \n", c->tree_number_dominant);
		Log("Density in layer 1 = %g trees/ha\n", c->density_dominant);
	}
	if (c->heights_count == 2)
	{
		Log("Number of adult height classes in layer 1 = %d\n", c->height_class_in_layer_dominant_counter);
		Log("Number of adult height classes in layer 0 = %d\n", c->height_class_in_layer_dominated_counter);
		Log("Tree number in layer 1 = %d \n", c->tree_number_dominant);
		Log("Tree number in layer 0 = %d \n", c->tree_number_dominated);
		Log("Density in layer 1 = %g trees/ha\n", c->density_dominant);
		Log("Density in layer 0 = %g trees/ha\n", c->density_dominated);
	}
	if (c->heights_count > 2)
	{
		Log("Number of adult height classes in layer 2 = %d\n", c->height_class_in_layer_dominant_counter);
		Log("Number of adult height classes in layer 1 = %d\n", c->height_class_in_layer_dominated_counter);
		Log("Number of adult height classes in layer 0 = %d\n", c->height_class_in_layer_subdominated_counter);
		Log("Tree number in layer 2 = %d \n", c->tree_number_dominant);
		Log("Tree number in layer 1 = %d \n", c->tree_number_dominated);
		Log("Tree number in layer 0 = %d \n", c->tree_number_subdominated);
		Log("Density in layer 2 = %g trees/ha\n", c->density_dominant);
		Log("Density in layer 1 = %g trees/ha\n", c->density_dominated);
		Log("Density in layer 0 = %g trees/ha\n", c->density_subdominated);
	}


	height = 0;

	//compute class canopy cover
	for ( height = c->heights_count - 1; height >= 0; height-- )
	{
		qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_asc);

		for (age = c->heights[height].ages_count - 1; age >= 0; age --)
		{
			for (species = c->heights[height].ages[age].species_count - 1; species >= 0; species -- )
			{
				Log("** CANOPY COVER from DBH-DC Function FOR LAYER %d  species %s **\n", c->heights[height].z, c->heights[height].ages[age].species[species].name);


				if (c->heights_count == 1)
				{
					DBHDCeffective = (( c->heights[height].ages[age].species[species].value[DBHDCMAX] - c->heights[height].ages[age].species[species].value[DBHDCMIN] )
							/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
							* (c->density_dominant - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMIN]);
					Log("DBHDC effective to apply = %g\n", DBHDCeffective);
				}
				else
				{
					if (c->heights_count >= 3 )//3 heights classes or more
					{
						if (c->heights[height].z == 2)
						{
							//settings->dominant
							DBHDCeffective = (( c->heights[height].ages[age].species[species].value[DBHDCMAX] - c->heights[height].ages[age].species[species].value[DBHDCMIN] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_dominant - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMIN]);
							Log("DBHDC effective to apply = %g\n", DBHDCeffective);
						}
						else if (c->heights[height].z == 1)
						{
							DBHDCeffective = (( c->heights[height].ages[age].species[species].value[DBHDCMAX] - c->heights[height].ages[age].species[species].value[DBHDCMIN] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_dominated - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMIN]);
							Log("DBHDC effective to apply = %g\n", DBHDCeffective);
						}
						else
						{
							//SUBDOMINATED
							DBHDCeffective = (( c->heights[height].ages[age].species[species].value[DBHDCMAX] - c->heights[height].ages[age].species[species].value[DBHDCMIN] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_subdominated - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMIN]);
							Log("DBHDC effective to apply = %g\n", DBHDCeffective);;

						}
					}
					else //2 height classes
					{
						if (c->heights[height].z == 2)
						{
							//settings->dominant
							DBHDCeffective = (( c->heights[height].ages[age].species[species].value[DBHDCMAX] - c->heights[height].ages[age].species[species].value[DBHDCMIN] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_dominant - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMIN]);
							Log("DBHDC effective to apply = %g\n", DBHDCeffective);
						}
						else
						{
							DBHDCeffective = (( c->heights[height].ages[age].species[species].value[DBHDCMAX] - c->heights[height].ages[age].species[species].value[DBHDCMIN] )
									/ (c->heights[height].ages[age].species[species].value[DENMAX] - c->heights[height].ages[age].species[species].value[DENMIN] )
									* (c->density_dominated - c->heights[height].ages[age].species[species].value[DENMIN] ) + c->heights[height].ages[age].species[species].value[DBHDCMIN]);
							Log("DBHDC effective to apply = %g\n", DBHDCeffective);
						}
					}
				}



				if (DBHDCeffective > c->heights[height].ages[age].species[species].value[DBHDCMAX])
				{
					//Log("DBHDC effective for Dominant Layer > DBHDCMAX!!!\n");
					DBHDCeffective = c->heights[height].ages[age].species[species].value[DBHDCMAX];
					Log("DBHDC effective applied = %g\n", DBHDCeffective);
				}


				//Crown Diameter using DBH-DC

				c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC] = c->heights[height].ages[age].species[species].value[AVDBH] * DBHDCeffective;
				Log("-Crown Diameter from DBHDC function  = %g m\n", c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC]);

				//Crown Area using DBH-DC
				c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC] = ( Pi / 4) * pow (c->heights[height].ages[age].species[species].value[CROWN_DIAMETER_DBHDC_FUNC], 2 );
				Log("-Crown Area from DBHDC function = %g m^2\n", c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC]);


				//Canopy Cover using DBH-DC

				c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC] = c->heights[height].ages[age].species[species].value[CROWN_AREA_DBHDC_FUNC] * c->heights[height].ages[age].species[species].counter[N_TREE] / settings->sizeCell;

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
		}		//Log("Height class = %g is in layer %d \n", c->heights[height].value, c->heights[height].z);
	}

	if (c->heights_count == 1)
	{
		Log("Layer cover in layer 1 = %g %% \n", c->layer_cover_dominant * 100);
	}
	if (c->heights_count == 2)
	{
		Log("Layer cover in layer 1 = %g %%\n", c->layer_cover_dominant * 100);
		Log("Layer cover in layer 0 = %g %% \n", c->layer_cover_dominated * 100);
	}
	if (c->heights_count > 2)
	{
		Log("Layer cover in layer 2 = %g %%\n", c->layer_cover_dominant * 100);
		Log("Layer cover in layer 1 = %g %% \n", c->layer_cover_dominated * 100);
		Log("Layer cover in layer 0 = %g %% \n", c->layer_cover_subdominated * 100);
	}


	//the model makes die trees of the lower height class for that layer because
	//it passes through the function sort_by_height_desc the height classes starting from the lowest
	for ( height = c->heights_count - 1; height >= 0; height-- )
	{
		qsort (c->heights, c->heights_count, sizeof (HEIGHT), sort_by_heights_desc);

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
						Get_layer_cover_mortality (&c->heights[height].ages[age].species[species], layer_cover, tree_number, c->heights[height].z);
					}
					else
					{
						Log ("NO MORTALITY BASED ON HIGH CANOPY COVER layer %d !!!\n", c->heights[height].z);
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
								Get_layer_cover_mortality (&c->heights[height].ages[age].species[species], layer_cover, tree_number, c->heights[height].z);
							}
							else
							{
								Log ("NO MORTALITY BASED ON HIGH CANOPY COVER layer %d !!!\n", c->heights[height].z);
							}
						}
						else
						{
							if (c->layer_cover_dominated >= settings->max_layer_cover)
							{
								//mortality
								layer_cover = c->layer_cover_dominated;
								tree_number = c->tree_number_dominated;
								Get_layer_cover_mortality (&c->heights[height].ages[age].species[species], layer_cover, tree_number, c->heights[height].z);
							}
							else
							{
								Log ("NO MORTALITY BASED ON HIGH CANOPY COVER layer %d !!!\n", c->heights[height].z);
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
							Get_layer_cover_mortality (&c->heights[height].ages[age].species[species], layer_cover, tree_number, c->heights[height].z);
						}
						else
						{
							Log ("NO MORTALITY BASED ON HIGH CANOPY COVER layer %d !!!\n", c->heights[height].z);
						}
					}
					else if (c->heights[height].z == c->annual_layer_number - 2)
					{
						if (c->layer_cover_dominated >= settings->max_layer_cover)
						{
							//mortality
							layer_cover = c->layer_cover_dominated;
							tree_number = c->tree_number_dominated;
							Get_layer_cover_mortality (&c->heights[height].ages[age].species[species], layer_cover, tree_number, c->heights[height].z);
						}
						else
						{
							Log ("NO MORTALITY BASED ON HIGH CANOPY COVER layer %d !!!\n", c->heights[height].z);
						}
					}
					else
					{
						if (c->layer_cover_subdominated >= settings->max_layer_cover)
						{
							//mortality
							layer_cover = c->layer_cover_subdominated;
							tree_number = c->tree_number_subdominated;
							Get_layer_cover_mortality (&c->heights[height].ages[age].species[species], layer_cover, tree_number, c->heights[height].z);
						}
						else
						{
							Log ("NO MORTALITY BASED ON HIGH CANOPY COVER layer %d !!!\n", c->heights[height].z);
						}
					}
					break;
				}
			}
		}		//Log("Height class = %g is in layer %d \n", c->heights[height].value, c->heights[height].z);
	}
	Log("*************************************************** \n");
}


void Get_monthly_vegetative_period (CELL *const c, const MET_DATA *const met, int month)
{

	static int height;
	static int age;
	static int species;
	int counter;

	Log("--GET VEGETATIVE PERIOD--\n");

	//assign value for VEG_UNVEG (1 for veg, 0 for Unveg) and compute number of classes in veg period

	for ( height = c->heights_count - 1; height >= 0; height-- )
	{
		for ( age = c->heights[height].ages_count - 1 ; age >= 0 ; age-- )
		{
			for (species = 0; species < c->heights[height].ages[age].species_count; species++)
			{
				if (c->heights[height].ages[age].species[species].phenology == D)
				{
					if (settings->version == 's')
					{
						//Log("Spatial version \n");

						//veg period
						if (met[month].ndvi_lai > 0.1)
						{
							c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 1;
							counter += 1;
						}
						//unveg period
						else
						{
							c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 0;
						}
					}
					else
					{
						if((met[month].tav >= c->heights[height].ages[age].species[species].value[GROWTHSTART] && month < 6)
								|| (met[month].tav >= c->heights[height].ages[age].species[species].value[GROWTHEND] && month >= 6))
						{
							c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 1;
							counter += 1;
						}
						else
						{
							c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 0;
						}
					}
				}
				else
				{
					c->heights[height].ages[age].species[species].counter[VEG_UNVEG] = 1;
					Log("Veg period = %d \n", c->heights[height].ages[age].species[species].counter[VEG_UNVEG]);
					counter += 1;
				}
			}
		}
	}
	Log("species in veg period = %d\n", counter);
}




void Get_monthly_numbers_of_layers (CELL *const c)
{
	//determines number of layer in function of:
	//-differences between tree height classes
	//-vegetative or un-vegetative period
	static int height;
	static int age;
	static int species;
	float current_height;
	float previous_height;

	//height differences in meter to consider trees in two different layers


	Log("--GET NUMBER OF MONTHLY LAYERS (Layer in Veg)--\n");

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
					if ((previous_height - current_height ) > settings->layer_limit && c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
					{
						c->monthly_layer_number += 1;
					}
					previous_height = current_height;
				}
				else if (c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
				{
					c->monthly_layer_number = 1;
				}
			}
		}
	}
	Log("number of vegetative layers = %d\n", c->monthly_layer_number);

	//ASSIGN MONTHLY Z VALUE

}



void Get_monthly_layer_cover (CELL *const c, HEIGHT *const h, const MET_DATA *const met, int month)
{

	//compute if is in veg period
	int height;
	int age;
	int species;


	c->layer_cover_dominant = 0;
	c->layer_cover_dominated = 0;
	c->layer_cover_subdominated = 0;


	Log("\nGET_MONTHLY_FOREST_STRUCTURE_ROUTINE, MONTH = %d\n", month+1);

	Log("Determines Effective Layer Cover \n");
	for (height = c->heights_count - 1; height >= 0; height -- )
	{
		for (age = c->heights[height].ages_count - 1; age >= 0; age --)
		{
			for (species = c->heights[height].ages[age].species_count - 1; species >= 0; species -- )
			{
				if (c->monthly_layer_number == 1 && c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
				{
					c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
				}
				else
				{
					if (c->monthly_layer_number >= 3 )//3 heights classes or more
					{
						if (c->heights[height].z == 2 && c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
						{
							//settings->dominant
							c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						}
						else if (c->heights[height].z == 1 && c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
						{
							//settings->dominated
							c->layer_cover_dominated += c->heights[height].ages[age].species[species].counter[CANOPY_COVER_DBHDC];
						}
						else if (c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
						{
							//SUBDOMINATED
							c->layer_cover_subdominated  += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];

						}
						else
						{
							//Log("No species in veg period \n");
						}
					}
					else //2 height classes
					{
						if (c->heights[height].z == 2 && c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
						{
							//settings->dominant
							c->layer_cover_dominant += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						}
						else if (c->heights[height].ages[age].species[species].counter[VEG_UNVEG] == 1)
						{
							c->layer_cover_dominated += c->heights[height].ages[age].species[species].value[CANOPY_COVER_DBHDC];
						}
						else
						{
							//Log("No species in veg period \n");
						}

					}
				}
			}
		}
	}


	switch (c->monthly_layer_number)
	{

	case 1:
		Log("Layer cover in layer 1 = %g %% \n", c->layer_cover_dominant * 100);
		break;
	case 2:
		Log("Layer cover in layer 1 = %g %%\n", c->layer_cover_dominant * 100);
		Log("Layer cover in layer 0 = %g %% \n", c->layer_cover_dominated * 100);
		break;
	case 3:
		Log("Layer cover in layer 2 = %g %%\n", c->layer_cover_dominant * 100);
		Log("Layer cover in layer 1 = %g %% \n", c->layer_cover_dominated * 100);
		Log("Layer cover in layer 0 = %g %% \n", c->layer_cover_subdominated * 100);
		break;

	}

	Log("*************************************************** \n");

}


/*
void Get_top_layer (CELL *const c, int heights_count, HEIGHT *heights)
{

	Log(" GET_TOP_LAYER_FUNCTION \n");
	int height;

	assert (heights);



	for ( height = heights_count - 1; height >= 0; height-- )
	{
		Log("height dominance = %d \n", c->heights[height].dominance);
		if (c->heights[height].dominance == 1)
		{
			c->heights[height].top_layer = c->heights[height].z ;
			Log("-Top layer and in Dominant Light is layer with z = %d\n", c->heights[height].top_layer);
			break;
		}
		if ( c->heights[height].top_layer == -1)
		{
			Log("--NO TREES IN VEGETATIVE PERIOD!!!\n");
			Log("**********************************************\n");
			break;
		}
	}
}
*/

//This function compute if there is a class age in veg period
extern void Get_Dominant_Light(HEIGHT *heights, CELL* c, const int count, const MET_DATA *const met, const int month, const int DaysInMonth)
{
	int height;
	int age;
	int species;
	int stop;
	int z;
	static int top_layer;
	int Light_codominance;
	/*counter for height class vegetating in each layer*/
	static int dominant_veg_counter;
	static int dominated_veg_counter;
	static int subdominated_veg_counter;
	static float PAR_for_Soil;


	stop = 0;
	Light_codominance = 0;
	c->Veg_Counter = 0;
	dominant_veg_counter = 0;
	dominated_veg_counter = 0;
	subdominated_veg_counter = 0;

	assert(heights);


	Log("-Dominant Light Index Function-\n");
	for ( height = count- 1; height >= 0; height-- )
	{
		for ( age = 0; age < heights[height].ages_count; age++ )
		{
			for ( species = 0; species < heights[height].ages[age].species_count; species++ )
			{
				if (count == 1)
				{
					z = 0;
				}
				else
				{
					if (count >= 3)
					{
						if (heights[height].value >= settings->dominant)
						{
							z = 2;
						}
						else if (heights[height].value < settings->dominant && heights[height].value >= settings->dominated)
						{
							z = 1;
						}
						else
						{
							z = 0;
						}
					}
					else
					{
						if (heights[height].value >= settings->dominant)
						{
							z = 1;
						}
						else
						{
							z = 0;
						}
					}
				}


				if ( D == heights[height].ages[age].species[species].phenology )
				{
					//Log("Phenology = Deciduous \n");

					if((met[month].tav >= heights[height].ages[age].species[species].value[GROWTHSTART] && month < JULY)
							|| (met[month].tav >=heights[height].ages[age].species[species].value[GROWTHEND] && month >= JULY))
						//vegetative period for deciduous
					{
						if (!stop || top_layer == z)
						{
							heights[height].dominance = 1;
							stop = 1;
							top_layer = z;
							Light_codominance += 1;
							//Log("Height = %g m \n", heights[height].value);
							//Log("dominance = %d\n", heights[height].dominance);
							//Log("TOP _LAYER = %d\n", top_layer);
						}
					}
					else
					{
						heights[height].dominance = -1;
						//Log("Height = %g m \n", heights[height].value);
						//Log (" no dominance = %d\n", heights[height].dominance);
					}
				}
				else
					//evergreen
				{
					if (!stop || top_layer == z)
					{
						heights[height].dominance = 1;
						stop = 1;
						top_layer = z;
						Light_codominance += 1;
						//Log("Height = %g m \n", heights[height].value);
						//Log("dominance = %d\n", heights[height].dominance);
						//Log("TOP _LAYER = %d\n", top_layer);

					}
					else
					{
						heights[height].dominance = -1;
						//Log("Height = %g m \n", heights[height].value);
						//Log (" no dominance = %d\n", heights[height].dominance);
					}
				}
			}
		}
	}


	if (Light_codominance > 1)
	{
		Log("-Codominance between species for Light\n");
		Light_codominance = 1;
	}
	else
	{
		Log("-No Codominance between species for Light\n");
		Light_codominance = -1;
	}

	Log("-Number of Height Class in vegetative period = %d\n", c->Veg_Counter);
	if (c->Veg_Counter != 0)
	{
		if (count == 1)
		{
			Log("-Number of Height Class in vegetative period in Dominant Layer = %d\n", dominant_veg_counter);
		}
		else
		{
			if (count >= 3)
			{
				Log("-Number of Height Class in vegetative period in Dominant Layer = %d\n", dominant_veg_counter);
				Log("-Number of Height Class in vegetative period in Dominated Layer = %d\n", dominated_veg_counter);
				Log("-Number of Height Class in vegetative period in Subdominated Layer = %d\n", subdominated_veg_counter);
			}
			else
			{
				Log("-Number of Height Class in vegetative period in Dominant Layer = %d\n", dominant_veg_counter);
				Log("-Number of Height Class in vegetative period in Dominated Layer = %d\n", dominated_veg_counter);
			}
		}



		/*compute number of layers*/
		if (dominant_veg_counter >= 1 && dominated_veg_counter >= 1 && subdominated_veg_counter >= 1 )
		{
			Log("-Three vegetative layers\n");
		}
		else if (dominant_veg_counter >= 1 && dominated_veg_counter >= 1 && subdominated_veg_counter == 0 )
		{
			Log("-Two vegetative layers\n");
		}
		else if (dominant_veg_counter >= 1 && dominated_veg_counter == 0 && subdominated_veg_counter >= 1 )
		{
			Log("-Two vegetative layers\n");
		}
		else if (dominant_veg_counter == 0 && dominated_veg_counter >= 1 && subdominated_veg_counter >= 1 )
		{
			Log("-Two vegetative layers\n");
		}
		else
		{
			Log("-One vegetative layer\n");
		}
	}
	else
	{
		Log("-No vegetative layers all species are in un-vegetative period \n");
		//PAR_for_Soil
		//Log("DAYs in month = %d day/month\n", DaysInMonth);
		//Log("Solar Radiation = %g MJ/m^2/day\n", met[month].solar_rad);
		PAR_for_Soil = DaysInMonth * met[month].solar_rad * MOLPAR_MJ;
		Log("PAR FOR SOIL in UNVEGETATIVE PERIOD = %g MOLPAR/m^2/month\n", PAR_for_Soil);
	}

	//Log("-Species in veg period = %d\n", c->Veg_Counter);
}


int Get_number_of_layers (CELL *c)
{
	int number_of_layers;
	//compute number of layers
	if (c->dominant_veg_counter > 0 && c->dominated_veg_counter > 0 && c->subdominated_veg_counter > 0)
	{
		number_of_layers = 3;
		Log("THREE LAYERS \n");
	}
	else if (c->dominant_veg_counter > 0 && c->dominated_veg_counter > 0 && c->subdominated_veg_counter == 0)
	{
		number_of_layers = 2;
		Log("TWO LAYERS \n");
	}
	else if (c->dominant_veg_counter == 0 && c->dominated_veg_counter > 0 && c->subdominated_veg_counter == 0)
	{
		number_of_layers = 1;
		Log("ONE LAYER \n");
	}
	return number_of_layers;

}
