/*management.c*/

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <math.h>
#include "management.h"
#include "constants.h"
#include "settings.h"
#include "common.h"
#include "logger.h"
#include "new_forest_tree_class.h"

extern settings_t* g_settings;
extern logger_t* g_log;

void forest_management (cell_t *const c, const int layer, const int height, const int age, const int species, const int year)
{
	static int rotation_counter;

	species_t *s;

	s = &c->heights[height].ages[age].species[species];

	/* this function handles all other management functions */

	if ( !string_compare_i (g_settings->management, "on") && !year )
	{
		rotation_counter = (int)s->value[ROTATION];

		/* if model is set to ask which management */
		//choose_management ( s , year);
	}
	else if ( ! string_compare_i (g_settings->management, "on") && year > 0 )
	{
		if ( year == (int)s->value[ROTATION] )
		{
			logger(g_log,"**FOREST MANAGEMENT**\n");

			clearcut_timber_without_request ( c, layer, height, age, species, year );

			/* add multiples */
			s->value[ROTATION] += rotation_counter;

			if(g_settings->replanted_tree != 0.0)
			{
				//ALESSIOC ALESSIOR remove toggles when bug will be fixed!!
//				if ( ! add_new_tree_class( c, height, age, species ) )
//				{
//					logger(g_log, "unable to add new height class! (exit)\n");
//					exit(1);
//				}
			}
		}
	}
}


void clearcut_timber_upon_request(cell_t *const c, const int layer, const int height, const int age, const int species)
{
	int removed_tree;
	int layer_to_remove_tree;
	double IndWf,
	IndWs,
	IndWrf,
	IndWrc,
	IndWbb,
	IndWres;

	tree_layer_t *l;
	species_t *s;

	l = &c->t_layers[layer];
	s = &c->heights[height].ages[age].species[species];

	IndWf = s->value[BIOMASS_FOLIAGE_tDM] / s->counter[N_TREE];
	IndWs = s->value[BIOMASS_STEM_tDM] / s->counter[N_TREE];
	IndWrc = s->value[BIOMASS_COARSE_ROOT_tDM] / s->counter[N_TREE];
	IndWrf = s->value[BIOMASS_FINE_ROOT_tDM] / s->counter[N_TREE];
	IndWbb = s->value[BIOMASS_BRANCH_tDM] / s->counter[N_TREE];
	IndWres = s->value[RESERVE_tDM] / s->counter[N_TREE];

	/* CLEARCUT FOR TIMBER */
	logger(g_log, "CLEARCUT FOR TIMBER FUNCTION \n");
	printf ("Number of trees removed = ?\n");
	scanf ("%d", &removed_tree);

	if ( removed_tree != 0 )
	{
		if ( c->t_layers_count > 1 )
		{
			printf("Layer from which of %d layer(s) remove trees = ?\n", c->t_layers_count);

			scanf ("%d", &layer_to_remove_tree);
			printf ("layer choiced = %d \n", layer_to_remove_tree);

			if ( layer_to_remove_tree == 0 || layer_to_remove_tree > c->t_layers_count )
			{
				puts("NO CHOICED VALUE CAN BE ACCEPTED");
			}

			//fixme continue..........depending on which layer is chosen

			//fixme continue..........
			//
			//			logger(g_log, "Number of trees removed = %d trees/ha \n", removed_tree);
			//
			//			s->counter[N_TREE] = s->counter[N_TREE] - removed_tree;
			//			logger(g_log, "Number of mother trees after management = %d \n", s->counter[N_TREE] );
			//
			//			//Recompute Biomass
			//			s->value[BIOMASS_FOLIAGE_tDM] = IndWf * s->counter[N_TREE];
			//			s->value[BIOMASS_STEM_tDM] = IndWs * s->counter[N_TREE];
			//			s->value[BIOMASS_COARSE_ROOT_tDM] = IndWrc * s->counter[N_TREE];
			//			s->value[BIOMASS_FINE_ROOT_tDM] = IndWrf * s->counter[N_TREE];
			//			logger(g_log, "Biomass after management:\nWf = %f\nWs = %f\nWrf = %f\nWrc = %f\n",
			//					s->value[BIOMASS_FOLIAGE_tDM],
			//					s->value[BIOMASS_STEM_tDM],
			//					s->value[BIOMASS_FINE_ROOT_tDM],
			//					s->value[BIOMASS_COARSE_ROOT_tDM]);
			//
			//			BiomRem = s->value[TOTAL_W] - (s->value[BIOMASS_FOLIAGE_tDM] + s->value[BIOMASS_STEM_tDM] /* ??? m->lpCell[index].Wr??*/);
			//			logger(g_log, "Total Biomass harvested from ecosystem = %f\n", BiomRem);
			//			// Total Biomass at the end
			//			s->value[TOTAL_W] = s->value[BIOMASS_FOLIAGE_tDM] + s->value[BIOMASS_COARSE_ROOT_tDM] + s->value[BIOMASS_FINE_ROOT_tDM] + s->value[BIOMASS_STEM_tDM];
			//			logger(g_log, "Total 'live' Biomass = %f tDM/ha\n", s->value[TOTAL_W]);
			//
			//
			//			//DA QUI DEVE NASCERE UNA NUOVA CLASSE (se pollonifera) !!!!!!!
			//			//compute number of shoot produced after coppicing
			//			shoots_number = removed_tree * s->value[AV_SHOOT];
			//			logger(g_log, "Number of shoots produced after coppicing = %d shoots/ha \n", shoots_number);

		}
		else
		{

			//			logger(g_log, "Number of trees removed = %d trees/ha \n", removed_tree);
			//
			//			s->counter[N_TREE] = s->counter[N_TREE] - removed_tree;
			//			logger(g_log, "Number of mother trees after management = %d \n", s->counter[N_TREE] );
			//
			//			//Recompute Biomass
			//			s->value[BIOMASS_FOLIAGE_tDM] = IndWf * s->counter[N_TREE];
			//			s->value[BIOMASS_STEM_tDM] = IndWs * s->counter[N_TREE];
			//			s->value[BIOMASS_COARSE_ROOT_tDM] = IndWrc * s->counter[N_TREE];
			//			s->value[BIOMASS_FINE_ROOT_tDM] = IndWrf * s->counter[N_TREE];
			//			logger(g_log, "Biomass after management:\nWf = %f\nWs = %f\nWrf = %f\nWrc = %f\n",
			//					s->value[BIOMASS_FOLIAGE_tDM],
			//					s->value[BIOMASS_STEM_tDM],
			//					s->value[BIOMASS_FINE_ROOT_tDM],
			//					s->value[BIOMASS_COARSE_ROOT_tDM]);
			//
			//			BiomRem = s->value[TOTAL_W] - (s->value[BIOMASS_FOLIAGE_tDM] + s->value[BIOMASS_STEM_tDM] /* ??? m->lpCell[index].Wr??*/);
			//			logger(g_log, "Total Biomass harvested from ecosystem = %f\n", BiomRem);
			//			// Total Biomass at the end
			//			s->value[TOTAL_W] = s->value[BIOMASS_FOLIAGE_tDM] + s->value[BIOMASS_COARSE_ROOT_tDM] + s->value[BIOMASS_FINE_ROOT_tDM] + s->value[BIOMASS_STEM_tDM];
			//			logger(g_log, "Total 'live' Biomass = %f tDM/ha\n", s->value[TOTAL_W]);
			//
			//
			//			//DA QUI DEVE NASCERE UNA NUOVA CLASSE (se pollonifera) !!!!!!!
			//			//compute number of shoot produced after coppicing
			//			shoots_number = removed_tree * s->value[AV_SHOOT];
			//			logger(g_log, "Number of shoots produced after coppicing = %d shoots/ha \n", shoots_number);
		}
	}
}

void clearcut_coppice(cell_t *const c, const int layer, const int height, const int age, const int species)
{
	int removed_tree;
	int layer_to_remove_tree;
	double IndWf,
	IndWs,
	IndWrf,
	IndWrc;
	//double BiomRem;
	//int shoots_number;

	tree_layer_t *l;
	species_t *s;

	l = &c->t_layers[layer];
	s = &c->heights[height].ages[age].species[species];

	IndWf = s->value[BIOMASS_FOLIAGE_tDM] / s->counter[N_TREE];
	IndWs = s->value[BIOMASS_STEM_tDM] / s->counter[N_TREE];
	IndWrc = s->value[BIOMASS_COARSE_ROOT_tDM] / s->counter[N_TREE];
	IndWrf = s->value[BIOMASS_FINE_ROOT_tDM] / s->counter[N_TREE];
	//logger(g_log, "Individual Biomass:\nWf = %f\nWs = %f\nWr = %f\n", IndWf, IndWs, IndWr);

	/* CLEARCUT FOR COPPICE */
	logger(g_log, "CLEARCUT FOR COPPICE FUNCTION \n");
	logger(g_log, "Layer modelled z = %d \n", layer);
	logger(g_log, "Numbers of layers = %d \n", c->t_layers_count);
	logger(g_log, "Number of stools = %d \n", s->counter[N_STUMP]);

	printf ("Number of trees removed = ?\n");
	scanf ("%d", &removed_tree);

	if ( removed_tree != 0 )
	{
		if ( c->t_layers_count > 1 )
		{
			printf("Layer from which of %d layer(s) remove trees = ?\n", c->t_layers_count);

			scanf ("%d", &layer_to_remove_tree);
			printf ("layer choiced = %d \n", layer_to_remove_tree);

			if ( layer_to_remove_tree == 0 || layer_to_remove_tree > c->t_layers_count )
			{
				puts("NO CHOICED VALUE CAN BE ACCEPTED");
			}

			//fixme continue..........depending on which layer is chosen

			//fixme continue..........
			//			logger(g_log, "Number of trees removed = %d trees/ha \n", removed_tree);
			//
			//			s->counter[N_TREE] = s->counter[N_TREE] - removed_tree;
			//			logger(g_log, "Number of trees after management = %d \n", s->counter[N_TREE] );
			//			logger(g_log, "Number of stools = %d \n", s->counter[N_STUMP]);
			//
			//			//Recompute Biomass
			//			s->value[BIOMASS_FOLIAGE_tDM] = IndWf * s->counter[N_TREE];
			//			s->value[BIOMASS_STEM_tDM] = IndWs * s->counter[N_TREE];
			//			s->value[BIOMASS_COARSE_ROOT_tDM] = IndWrc * s->counter[N_TREE];
			//			s->value[BIOMASS_FINE_ROOT_tDM] = IndWrf * s->counter[N_TREE];
			//			logger(g_log, "Biomass after management:\nWf = %f\nWs = %f\nWrc = %f\n Wrf = %f\n",
			//					s->value[BIOMASS_FOLIAGE_tDM],
			//					s->value[BIOMASS_STEM_tDM],
			//					s->value[BIOMASS_COARSE_ROOT_tDM],
			//					s->value[BIOMASS_FINE_ROOT_tDM]);
			//
			//			BiomRem = s->value[TOTAL_W] - (s->value[BIOMASS_FOLIAGE_tDM] + s->value[BIOMASS_STEM_tDM] /* ??? m->lpCell[index].Wr??*/);
			//			logger(g_log, "Total Biomass harvested from ecosystem = %f\n", BiomRem);
			//			// Total Biomass at the end
			//			s->value[TOTAL_W] = s->value[BIOMASS_FOLIAGE_tDM] + s->value[BIOMASS_COARSE_ROOT_tDM] + s->value[BIOMASS_FINE_ROOT_tDM] + s->value[BIOMASS_STEM_tDM];
			//			logger(g_log, "Total 'live' Biomass = %f tDM/ha\n", s->value[TOTAL_W]);
			//
			//
			//
			//			//DA QUI DEVE NASCERE UNA NUOVA CLASSE DI ETA' = 1 !!!!!!!
			//			//compute number of shoot produced after coppicing
			//			shoots_number = removed_tree * s->value[AV_SHOOT];
			//			logger(g_log, "Number of shoots produced after coppicing = %f shoots/ha \n", shoots_number);
			//
			//
			//		}
			//		else
			//		{
			//			logger(g_log, "Layer uncutted \n");
			//		}
		}
	}
}

void choose_management(species_t *const s, const int years)
{
	if ( !years )
	{
		int Manag;
		printf("END OF FIRST YEAR RUN \n");
		//printf("INSERT VALUE FOR MANAGEMENT (T = timber; C = Coppice): ");
		//scanf ("%c",&Manag);
		//logger(g_log, "Management routine choiced = %c \n", Manag);

		/* Management */
		if ( s->management == T )
		{
			logger(g_log, "- Management type = TIMBER\n");
			printf("SELECT TYPE OF MANAGEMENT: \n"
					"-CLEARCUT = 1 \n"
					"-........ = 2 \n"
					"-........ = 3 \n"
					"-........ = 4 \n"
					"-........ = 5 \n");

			scanf ("%d",&Manag);

			switch ( Manag )
			{
			case 1 :
				logger(g_log, "Case CLEARCUT choiced \n");

				//Clearcut_Timber_upon_request (s,  years, c->heights[height].z, c->annual_layer_number);

				break;

			case 2 :
				logger(g_log, "Case ....... choiced \n");

				//fixme call function

				break;

			case 3 :
				logger(g_log, "Case .......  choiced \n");

				//fixme call function

				break;

			case 4 :
				logger(g_log, "Case .......  choiced \n");

				//fixme call function

				break;

			}

		}
		else
		{
			logger(g_log, "- Management type = COPPICE\n");
			printf("SELECT TYPE OF MANAGEMENT: \n"
					"-CLEARCUT = 1 \n"
					"-........ = 2 \n"
					"-........ = 3 \n"
					"-........ = 4 \n"
					"-........ = 5 \n");

			scanf ("%d",&Manag);

			switch (Manag)
			{
			case 1 :
				logger(g_log, "Case CLEARCUT choiced \n");

				//Clearcut_Coppice (s, years, c->heights[height].z, c->annual_layer_number);

				break;

			case 2 :
				logger(g_log, "Case ....... choiced \n");

				//fixme call function

				break;

			case 3 :
				logger(g_log, "Case .......  choiced \n");

				//fixme call function

				break;

			case 4 :
				logger(g_log, "Case .......  choiced \n");

				//fixme call function

				break;
			}
		}
	}
}


//fixme usefull??
void Management (age_t * const a, const int height, const int age, const int species, int years)
{

	species_t *s;
	s = &a->species[species];

	if ( a->value >= s->value[MINAGEMANAG] /* && MINDBHMANAG*/)
	{
		if ( years && !(years % (int)s->value[ROTATION]))
		{
			double IndWf,
			IndWs,
			IndWr,
			BiomRem;

			logger(g_log, "**MANAGEMENT **\n");
			logger(g_log, "ROTATION = %d years\n",s->value[ROTATION]);
			//Only for Dominant Layer
			//Individual Biomass

			IndWf = s->value[BIOMASS_FOLIAGE_tDM] / s->counter[N_TREE];
			IndWs = s->value[BIOMASS_STEM_tDM] / s->counter[N_TREE];
			IndWr = s->value[BIOMASS_FINE_ROOT_tDM] / s->counter[N_TREE];
			IndWr = s->value[BIOMASS_COARSE_ROOT_tDM] / s->counter[N_TREE];
			logger(g_log, "Individual Biomass:\nWf = %f\nWs = %f\nWr = %f\n", IndWf, IndWs, IndWr);

			//Percentage of Cutted Trees for Managment
			//Roots should remains in ecosystem!!
			//fixme
			//s->counter[CUT_TREES] = g_settings->harvested_tree * s->counter[N_TREE];
			s->counter[N_TREE] = s->counter[N_TREE] - s->counter[CUT_TREES];
			logger(g_log, "Management Cutted Trees = %d trees/ha\n", s->counter[CUT_TREES]);
			logger(g_log, "Number of Trees  after Management = %d trees\n", s->counter[N_TREE]);
			//Recompute Biomass
			s->value[BIOMASS_FOLIAGE_tDM] = IndWf * s->counter[N_TREE];
			s->value[BIOMASS_STEM_tDM] = IndWs * s->counter[N_TREE];
			s->value[BIOMASS_COARSE_ROOT_tDM] = IndWr * s->counter[N_TREE];
			s->value[BIOMASS_FINE_ROOT_tDM] = IndWr * s->counter[N_TREE];
			logger(g_log, "Biomass after management:\nWf = %f\nWs = %f\nWrf = %f\nWrc = %f\n",
					s->value[BIOMASS_FOLIAGE_tDM],
					s->value[BIOMASS_STEM_tDM],
					s->value[BIOMASS_FINE_ROOT_tDM],
					s->value[BIOMASS_COARSE_ROOT_tDM]);

			BiomRem = s->value[TOTAL_W] - (s->value[BIOMASS_FOLIAGE_tDM] + s->value[BIOMASS_STEM_tDM] /* ??? m->lpCell[index].Wr??*/);
			logger(g_log, "Total Biomass harvested from ecosystem = %f\n", BiomRem);
			// Total Biomass at the end
			s->value[TOTAL_W] = s->value[BIOMASS_FOLIAGE_tDM] + s->value[BIOMASS_COARSE_ROOT_tDM] + s->value[BIOMASS_FINE_ROOT_tDM] + s->value[BIOMASS_STEM_tDM];
			logger(g_log, "Total 'live' Biomass = %f tDM/ha\n", s->value[TOTAL_W]);
		}
		else
		{
			logger(g_log, "NO ROTATION in Dominant layer\n");
			logger(g_log, "NO MANAGEMENT in Dominant layer\n");
			s->counter[CUT_TREES] = 0;
		}
		logger(g_log, "NO ROTATION in Dominant layer\n");
		logger(g_log, "NO MANAGEMENT in Dominant layer\n");
		s->counter[CUT_TREES] = 0;
	}
	logger(g_log, "NO ROTATION in Dominant layer\n");
	logger(g_log, "NO MANAGEMENT in Dominant layer\n");
	s->counter[CUT_TREES] = 0;
}

void clearcut_timber_without_request (cell_t *const c, const int layer, const int height, const int age, const int species, const int year)
{
	int removed_tree = 0;
	double IndWf,
	IndWs,
	IndWrf,
	IndWrc,
	IndWbb,
	IndWres,
	IndWslive,
	IndWsdead,
	IndWrclive,
	IndWrcdead,
	IndWbblive,
	IndWbbdead;
	double stand_basal_area_to_remove;

	species_t *s;
	s = &c->heights[height].ages[age].species[species];

	//CLEARCUT FOR TIMBER (Taglio raso)
	logger(g_log, "CLEARCUT FOR TIMBER FUNCTION \n");

	IndWf = s->value[LEAF_C] / s->counter[N_TREE];
	IndWs = s->value[STEM_C] / s->counter[N_TREE];
	IndWrc = s->value[COARSE_ROOT_C] / s->counter[N_TREE];
	IndWrf = s->value[FINE_ROOT_C] / s->counter[N_TREE];
	IndWbb = s->value[BRANCH_C] / s->counter[N_TREE];
	IndWres = s->value[RESERVE_C] / s->counter[N_TREE];
	IndWslive = s->value[STEM_LIVE_WOOD_C] / s->counter[N_TREE];
	IndWsdead = s->value[STEM_DEAD_WOOD_C] / s->counter[N_TREE];
	IndWrclive = s->value[COARSE_ROOT_LIVE_WOOD_C] / s->counter[N_TREE];
	IndWrcdead = s->value[COARSE_ROOT_DEAD_WOOD_C] / s->counter[N_TREE];
	IndWbblive = s->value[BRANCH_LIVE_WOOD_C] / s->counter[N_TREE];
	IndWbbdead = s->value[BRANCH_DEAD_WOOD_C] / s->counter[N_TREE];

	stand_basal_area_to_remove = g_settings->removing_basal_area * s->value[STAND_BASAL_AREA];
	logger(g_log, "basal area to remove = %f\n", stand_basal_area_to_remove);
	removed_tree = ROUND(stand_basal_area_to_remove / s->value[BASAL_AREA]);
	logger(g_log, "removed trees = %d\n", removed_tree);

	s->counter[N_TREE] -= removed_tree;
	logger(g_log, "Number of trees after management = %d \n", s->counter[N_TREE]);

	//Recompute Biomass
	s->value[LEAF_C] = IndWf * s->counter[N_TREE];
	s->value[STEM_C] = IndWs * s->counter[N_TREE];
	s->value[COARSE_ROOT_C] = IndWrc * s->counter[N_TREE];
	s->value[FINE_ROOT_C] = IndWrf * s->counter[N_TREE];
	s->value[BRANCH_C] = IndWbb * s->counter[N_TREE];
	s->value[RESERVE_C] = IndWres * s->counter[N_TREE];
	logger(g_log, "Biomass after management:\nWf = %f\nWs = %f\nWrf = %f\nWrc = %f\nWrBB = %f\n Wres = %f\n",
			s->value[LEAF_C],
			s->value[STEM_C],
			s->value[FINE_ROOT_C],
			s->value[COARSE_ROOT_C],
			s->value[BRANCH_C],
			s->value[RESERVE_C]);

	// Total Biomass at the end
	s->value[TOTAL_W] = s->value[LEAF_C] + s->value[COARSE_ROOT_C] + s->value[FINE_ROOT_C] + s->value[STEM_C] + s->value[BRANCH_C] + s->value[RESERVE_C];
	logger(g_log, "Total Biomass = %f tC/ha\n", s->value[TOTAL_W]);

	/* update stand trees */
	c->n_tree -= removed_tree;
	c->annual_dead_tree += removed_tree;

	/* adding coarse and fine root and leaf to litter pool */
	c->daily_litter_carbon_tC +=  (IndWrc * removed_tree) + (IndWrf * removed_tree) + (IndWf * removed_tree);

}
