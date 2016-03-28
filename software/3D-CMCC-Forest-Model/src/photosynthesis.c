/*photosynthesis.c*/

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "constants.h"


void Phosynthesis (SPECIES *const s, CELL *const c, int month, int day, int DaysInMonth, int height, int age, int species)
{
	int i;
	double Alpha_C;
	double Epsilon;
	double GPPmolC, GPPmolC_sun, GPPmolC_shaded, GPPmolC_tot;
	double cell_coverage;

	Log("\n**PHOTOSYNTHESIS**\n");


	Log("VegUnveg = %d\n", s->counter[VEG_UNVEG]);
	//Log("Phenology = %f\n", s->value[PHENOLOGY]);
	if(s->value[CANOPY_COVER_DBHDC] > 1.0)
	{
		cell_coverage = 1.0;
	}
	else
	{
		cell_coverage = s->value[CANOPY_COVER_DBHDC];
	}

	//Veg period
	//fixme
	//photosynthesis should occurs only in the fraction of the days in which also daily transp occurs
	if (s->counter[VEG_UNVEG] == 1 /* && s->value[DAILY_TRANSP] != 0.0*/)
	{
		if (s->value[ALPHA] > 0.0)
		{
			Alpha_C = (s->value[ALPHA] * /* s->value[F_LIGHT] */ s->value[F_CO2] * s->value[F_NUTR] * s->value[F_T] * s->value[PHYS_MOD] * s->value[F_FROST])
					/* *s->value[FRAC_DAYTIME_TRANSP]*/ ;
			Log("Alpha C (Effective Quantum Canopy Efficiency)= %f molC/molPAR\n", Alpha_C);

			//convert epsilon from gCMJ^-1 to molCmolPAR^-1
			Epsilon = Alpha_C * MOLPAR_MJ * GC_MOL;
		}
		else
		{
			Log("NO ALPHA - MODEL USE EPSILON LIGHT USE EFFICIENCY!!!!\n");

			Epsilon = s->value[EPSILONgCMJ] * /*s->value[F_LIGHT]*/ s->value[F_CO2] * s->value[F_NUTR] * s->value[F_T] * s->value[PHYS_MOD]* s->value[F_FROST];
			Log("Epsilon (LUE) = %f gDM/MJ\n", Epsilon);

			Alpha_C = Epsilon / (MOLPAR_MJ * GC_MOL);
			Log("Alpha C = %f molC/molPAR\n", Alpha_C);
		}

		Log("**************************** GPP-'%c' ************************************ \n", settings->time);

		/* GPP */
		Log("Apar for GPP = %f\n", s->value[APAR]);

		//DailyGPP in mol of Carbon
		GPPmolC = s->value[APAR] * Alpha_C;
		GPPmolC_sun = s->value[APAR_SUN]* Alpha_C;
		GPPmolC_shaded = s->value[APAR_SHADE]* Alpha_C;
		GPPmolC_tot = GPPmolC_sun + GPPmolC_shaded;
		Log("GPPmolC = %f molC/m^2 day/month\n", GPPmolC);
		Log("GPPmolC_sun = %f molC/m^2 day/month\n", GPPmolC_sun);
		Log("GPPmolC_shade = %f molC/m^2 day/month\n", GPPmolC_shaded);
		Log("GPPmolC_tot = %f molC/m^2 day/month\n", GPPmolC_tot);

		if(GPPmolC - GPPmolC_tot < 0.00001)
		{
			GPPmolC = GPPmolC_tot;
		}
		else
		{
			Log("ERROR IN GPP_molC!!!\n");
			exit(1);
		}
		/* Daily GPP in grams of C/m^2 */
		/* Convert molC into grams */
		s->value[DAILY_POINT_GPP_gC] = GPPmolC_tot * GC_MOL;
		Log("DAILY_POINT_GPP_gC = %f gC/m2/day \n", s->value[DAILY_POINT_GPP_gC] );

		/* it converts value of GPP gC/m2/day in gC/m2 area covered/day */
		s->value[DAILY_GPP_gC] =  s->value[DAILY_POINT_GPP_gC] * cell_coverage;
		Log("DAILY_GPP_gC = %f gC/m2 area covered/day\n", s->value[DAILY_GPP_gC]);
	}
	else //Un Veg period
	{
		Log("Un-vegetative period !! \n");
		s->value[DAILY_GPP_gC] = 0;
		s->value[DAILY_POINT_GPP_gC] = 0;
		Log("DAILY_GPP_gC = %f gC/m2 area covered/day\n", s->value[DAILY_GPP_gC]);
	}
	s->value[MONTHLY_GPP_gC] += s->value[DAILY_POINT_GPP_gC];

	i = c->heights[height].z;

	c->layer_daily_gpp[i] += s->value[DAILY_GPP_gC];
	c->layer_monthly_gpp[i] += s->value[DAILY_GPP_gC];
	c->layer_annual_gpp[i] += s->value[DAILY_GPP_gC];

	Log("-CELL LEVEL\n");
	Log("-CELL LEVEL Yearly GPP (absolute) = %f gC/m^2 yr\n", c->daily_gpp);
	c->daily_gpp += s->value[DAILY_GPP_gC];
	c->monthly_gpp += s->value[DAILY_GPP_gC];
	c->annual_gpp += s->value[DAILY_GPP_gC];

	Log("***************************** ANNUAL GPP *************************** \n");

	Log("*********************** CLASS LEVEL ANNUAL GPP ********************** \n");
	//class level
	s->value[YEARLY_POINT_GPP_gC] += s->value[DAILY_GPP_gC];
	Log("-CLASS LEVEL\n");
	Log("-CLASS LEVEL Yearly GPP (absolute) = %f gC/m^2 yr\n", s->value[YEARLY_POINT_GPP_gC]);

	Log("*********************** STAND LEVEL ANNUAL GPP ********************** \n");
}
