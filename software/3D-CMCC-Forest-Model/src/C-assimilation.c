/*
 * C-assimilation.c
 *
 *  Created on: 14/ott/2013
 *      Author: alessio
 */
/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "constants.h"


void Get_carbon_assimilation (SPECIES *const s, CELL *const c, int years, int month, int day, int height)
{
	int i;

	Log ("\nGET_C-ASSIMILATION_ROUTINE\n");


	if (s->counter[VEG_UNVEG] == 1)
	{
		Log("GPP = %g\n", s->value[GPP_g_C]);
		Log("Reserve biomass = %g\n", s->value[BIOMASS_RESERVE_CTEM]);
		Log("Total aut respiration = %g\n", s->value[TOTAL_AUT_RESP]);
		Log("Fraction of respiration = %g %%\n", (s->value[TOTAL_AUT_RESP]*100.0)/s->value[GPP_g_C]);

		/*for principle of conservation of mass*/
		/*used if previous day NPP is negative to conserve mass assuming the loss
		of reserve*/
		if (s->value[GPP_g_C] < s->value[TOTAL_AUT_RESP])
		{
			//fixme remove after insert reserve also for coniferous
			if (s->value[PHENOLOGY] == 0.1 || s->value[PHENOLOGY] == 0.2)
			{
				/*following Barbaroux et al., 2003*/
				s->value[BIOMASS_RESERVE_CTEM] -=((s->value[TOTAL_AUT_RESP] * GC_GDM) / 1000000) * (s->value[CANOPY_COVER_DBHDC] * settings->sizeCell);

				if (s->value[BIOMASS_RESERVE_CTEM] < 0.0)
				{
					s->value[BIOMASS_RESERVE_CTEM] = 0;
					Log("All reserve has been consumed for respiration!!!\n");
				}

				s->value[NPP_g_C] = 0;
			}
			//fixme see for correct use of reserve for evergreen
			if (s->value[PHENOLOGY] == 1.1 || s->value[PHENOLOGY] == 1.2)
			{
				/*following Barbaroux et al., 2003*/
				//FIXME AS IT SHOULD BE
				/*
				s->value[BIOMASS_RESERVE_CTEM] -=((s->value[TOTAL_AUT_RESP] * GC_GDM) / 1000000) * (s->value[CANOPY_COVER_DBHDC] * settings->sizeCell);

				if (s->value[BIOMASS_RESERVE_CTEM] < 0.0)
				{
					s->value[BIOMASS_RESERVE_CTEM] = 0;
					Log("All reserve has been consumed for respiration!!!\n");
				}
				*/
				s->value[NPP_g_C] = 0;
			}

			s->value[NPP] = 0;
		}
		else
		{
			s->value[NPP_g_C] = s->value[GPP_g_C] - s->value[TOTAL_AUT_RESP];
			Log("NPP_g_C = %g\n", s->value[NPP_g_C]);
			//upscale class NPP to class cell level
			s->value[NPP] = ((s->value[NPP_g_C] * GC_GDM) / 1000000) * (s->value[CANOPY_COVER_DBHDC] * settings->sizeCell);
			//s->value[NPP] = (((s->value[GPP_g_C] * settings->sizeCell * GC_GDM)-(s->value[TOTAL_AUT_RESP])) / 1000000);
		}

		if (settings->time == 'm')
		{
			//Monthly layer GPP in grams of C/m^2
			//Convert molC into grams
			Log("Monthly NPP = %g gC/m^2\n",  s->value[NPP_g_C]);
			Log("Monthly NPP = %g tDM/area\n", s->value[NPP]);

			//DailyNPP = s->value[NPP] / DaysInMonth;
			//Log("Daily GPP in grams of C for this layer = %g molC/m^2 day\n", DailyGPPgC);
		}
		else
		{
			Log("Daily NPP = %g gC/m^2\n", s->value[NPP_g_C]);
			Log("Daily NPP = %g gC/area\n", s->value[NPP_g_C]* (s->value[CANOPY_COVER_DBHDC] * settings->sizeCell));
			Log("Daily NPP = %g tDM/area\n",  s->value[NPP]);
			//MonthlyNPP += s->value[NPP];
			//Log("Monthly NPP (per area covered) for layer %d = %g tDM/area\n", c->heights[height].z, MonthlyNPP);
		}
	}
	else
	{
		/*for principle of conservation of mass*/
		/*used if previous day NPP is negative to conserve mass assuming the loss
		of reserve*/
		if (s->value[TOTAL_AUT_RESP] > 0.0)
		{
			Log("Reserve Biomass = %g tDM/area\n", s->value[BIOMASS_RESERVE_CTEM]);
			s->value[BIOMASS_RESERVE_CTEM] -= ((s->value[TOTAL_AUT_RESP] * GC_GDM)/1000000) * (s->value[CANOPY_COVER_DBHDC]* settings->sizeCell);
			Log("Reserve consumed for respiration = %g tDM/cell \n", (s->value[TOTAL_AUT_RESP] * GC_GDM /1000000) * (s->value[CANOPY_COVER_DBHDC]* settings->sizeCell));
			Log("Reserve Biomass = %g tDM/area\n", s->value[BIOMASS_RESERVE_CTEM]);
		}
		s->value[NPP_g_C] = 0.0;
		s->value[NPP] = 0.0;
		Log("Daily/Monthly NPP = %g gC/m^2\n", s->value[NPP_g_C]);
		Log("Daily/Monthly NPP (per area covered) = %g  tDM/sizecell yr\n", s->value[NPP]);
	}

	i = c->heights[height].z;

	c->daily_npp[i] += s->value[NPP];
	c->monthly_npp[i] += s->value[NPP];
	c->annual_npp[i] += s->value[NPP];

	c->daily_tot_npp += s->value[NPP];
	c->monthly_tot_npp += s->value[NPP];
	c->annual_tot_npp += s->value[NPP];

	Log("***************************** ANNUAL NPP *************************** \n");

	Log("*********************** CLASS LEVEL ANNUAL NPP ********************** \n");
	//class level
	s->value[YEARLY_NPP] += s->value[NPP];
	Log("-CLASS LEVEL\n");
	Log("-CLASS LEVEL Yearly NPP (per area covered) = %g tDM/sizecell yr\n", s->value[YEARLY_NPP]);

	Log("*********************** STAND LEVEL ANNUAL NPP ********************** \n");

	//cell level
	c->npp += s->value[NPP];
	Log("-CELL LEVEL\n");
	Log("-CELL LEVEL Yearly NPP (per area covered) = %g tDM/sizecell yr\n", c->npp);

}


