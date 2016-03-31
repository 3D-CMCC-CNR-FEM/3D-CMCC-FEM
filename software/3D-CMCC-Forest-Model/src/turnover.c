/*turnover.c*/

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "constants.h"



void Turnover(SPECIES *s)
{
	/*following biome turnover occurs only during growing season*/
	if (s->counter[VEG_UNVEG] == 1)
	{
		Log("\n*DAILY_TURNOVER*\n\n");
		/*daily leaf turnover for DECIDUOUS, leaffall*/
		if (s->value[PHENOLOGY] == 1.1 || s->value[PHENOLOGY] == 1.2)
		{
			Log("****leaf turnover for evergreen****\n");
			s->value[LITTERFALL_RATE] = s->value[LEAVES_FINERTTOVER]/365.0;
			Log("Daily litter fall rate = %f\n", s->value[LITTERFALL_RATE]);

			/* reduce daily amount of leaf linearly */
			s->value[LEAF_C] -= (s->value[LEAF_C] * s->value[LITTERFALL_RATE]);
			s->value[C_TO_LITTER] = s->value[LEAF_C] * s->value[LITTERFALL_RATE];
			Log("Leaf to litter = %f tC/cell/day\n", s->value[C_TO_LITTER]);
			Log("Leaf poool after turnover = %f tC/cell\n", s->value[LEAF_C]);
		}
		/* daily stem turnover live to dead wood*/
		Log("****Stem turnover****\n");
		s->value[STEM_LIVE_WOOD_C] -= (s->value[C_TO_STEM] * (s->value[LIVE_WOOD_TURNOVER]/(int)s->counter[DAY_VEG_FOR_LITTERFALL_RATE]));
		Log("Live Stem Biomass = %f tC/cell\n", s->value[STEM_LIVE_WOOD_C]);
		s->value[STEM_DEAD_WOOD_C] = s->value[STEM_C] - s->value[STEM_LIVE_WOOD_C];
		//s->value[STEM_DEAD_WOOD_C] += (s->value[C_TO_STEM] * (s->value[LIVE_WOOD_TURNOVER]/(int)s->counter[DAY_VEG_FOR_LITTERFALL_RATE]));
		Log("Dead Stem Biomass = %f tC/cell\n", s->value[STEM_DEAD_WOOD_C]);

		Log("Live stem + dead stem = %f tC/cell\n", s->value[STEM_LIVE_WOOD_C] + s->value[STEM_DEAD_WOOD_C]);
		Log("Total stem = %f tC/cell\n", s->value[STEM_C]);
		Log("difference in stem = %f tC/cell\n", (s->value[STEM_LIVE_WOOD_C] + s->value[STEM_DEAD_WOOD_C]) - s->value[STEM_C]);
		//ALESSIOR
		CHECK_CONDITION(fabs((s->value[STEM_LIVE_WOOD_C] + s->value[STEM_DEAD_WOOD_C]) - s->value[STEM_C]), >1e-4)

		/* daily coarse root turnover live to dead wood */
		Log("****Coarse root turnover****\n");
		s->value[COARSE_ROOT_LIVE_WOOD_C] -= (s->value[C_TO_COARSEROOT] * (s->value[LIVE_WOOD_TURNOVER]/(int)s->counter[DAY_VEG_FOR_LITTERFALL_RATE]));
		Log("Live Coarse root Biomass = %f tC/cell\n", s->value[BIOMASS_COARSE_ROOT_LIVE_WOOD_tDM]);
		s->value[COARSE_ROOT_DEAD_WOOD_C] = s->value[COARSE_ROOT_C] - s->value[COARSE_ROOT_LIVE_WOOD_C];
		//s->value[COARSE_ROOT_DEAD_WOOD_C] += (s->value[C_TO_COARSEROOT] * (s->value[LIVE_WOOD_TURNOVER]/(int)s->counter[DAY_VEG_FOR_LITTERFALL_RATE]));
		Log("Dead Coarse root Biomass = %f tC/cell\n", s->value[COARSE_ROOT_DEAD_WOOD_C]);

		/* adding a daily turnover that goes to litter */
		//		s->value[COARSE_ROOT_DEAD_WOOD_C] -= (s->value[COARSE_ROOT_C] * s->value[COARSERTTOVER]/s->counter[VEG_DAYS]);
		//		Log("Dead Coarse root Biomass = %f tC/cell\n", s->value[COARSE_ROOT_DEAD_WOOD_C]);
		//		s->value[COARSE_ROOT_C] -= (s->value[COARSE_ROOT_C] * s->value[COARSERTTOVER]/s->counter[VEG_DAYS]);
		//		Log("Coarse root Biomass = %f tC/cell\n", s->value[COARSE_ROOT_DEAD_WOOD_C]);
		//		s->value[C_COARSE_ROOT_TO_LITTER] = s->value[COARSE_ROOT_C] * s->value[COARSERTTOVER]/s->counter[VEG_DAYS];
		//		Log("Coarse root Biomass to litter = %f tC/cell\n", s->value[C_COARSE_ROOT_TO_LITTER]);

		Log("Live coarse + dead coarse = %f tC/cell\n", s->value[COARSE_ROOT_LIVE_WOOD_C] + s->value[COARSE_ROOT_DEAD_WOOD_C]);
		Log("Total coarse = %f tC/cell\n", s->value[COARSE_ROOT_C]);
		Log("difference in coarse root = %f tC/cell\n", (s->value[COARSE_ROOT_LIVE_WOOD_C] + s->value[COARSE_ROOT_DEAD_WOOD_C]) - s->value[COARSE_ROOT_C]);
		//ALESSIOR
		CHECK_CONDITION(fabs((s->value[COARSE_ROOT_LIVE_WOOD_C] + s->value[COARSE_ROOT_DEAD_WOOD_C]) - s->value[COARSE_ROOT_C]), >1e-4)


		/* daily branch turnover live to dead wood */
		Log("****Stem branch turnover****\n");
		s->value[BRANCH_LIVE_WOOD_C] -= (s->value[C_TO_BRANCH] * (s->value[LIVE_WOOD_TURNOVER]/(int)s->counter[DAY_VEG_FOR_LITTERFALL_RATE]));
		Log("Live Stem branch Biomass = %f tC/cell\n", s->value[BRANCH_LIVE_WOOD_C]);
		s->value[BRANCH_DEAD_WOOD_C] = s->value[BRANCH_C] - s->value[BRANCH_LIVE_WOOD_C];
		//s->value[BRANCH_DEAD_WOOD_C] += (s->value[C_TO_BRANCH] * (s->value[LIVE_WOOD_TURNOVER]/(int)s->counter[DAY_VEG_FOR_LITTERFALL_RATE]));
		Log("Dead Stem Biomass = %f tC/cell\n", s->value[BRANCH_DEAD_WOOD_C]);

		/* adding a daily turnover that goes to litter */
		//		s->value[BRANCH_DEAD_WOOD_C] -= (s->value[C_TO_BRANCH] * s->value[BRANCHTTOVER]/s->counter[VEG_DAYS]);
		//		Log("Dead Branch Biomass = %f tC/cell\n", s->value[BRANCH_DEAD_WOOD_C]);
		//		s->value[BRANCH_C] -= (s->value[C_TO_BRANCH] * s->value[BRANCHTTOVER]/s->counter[VEG_DAYS]);
		//		Log("Branch Biomass = %f tC/cell\n", s->value[BRANCH_C]);
		//		s->value[C_BRANCH_TO_LITTER] = s->value[C_TO_BRANCH] * s->value[BRANCHTTOVER]/s->counter[VEG_DAYS];
		//		Log("Branch Biomass to litter = %f tC/cell\n", s->value[C_BRANCH_TO_LITTER]);

		Log("Live branch + dead branch = %f tC/cell\n", s->value[BRANCH_LIVE_WOOD_C] + s->value[BRANCH_DEAD_WOOD_C]);
		Log("Total branch = %f\n tC/cell", s->value[BRANCH_C]);
		Log("difference in branch = %f tC/cell\n", (s->value[BRANCH_LIVE_WOOD_C] + s->value[BRANCH_DEAD_WOOD_C]) - s->value[BRANCH_C]);
		//ALESSIOR
		CHECK_CONDITION(fabs((s->value[BRANCH_LIVE_WOOD_C] + s->value[BRANCH_DEAD_WOOD_C]) - s->value[BRANCH_C]), >1e-4);

		/* daily fruit turnover */
		if(s->value[PHENOLOGY] == 1.2 && s->value[FRUIT_C] != 0.0)
		{
			s->value[FRUIT_C] -= (s->value[FRUIT_C] * (1 / s->value[CONES_LIFE_SPAN]));
		}
	}
}
