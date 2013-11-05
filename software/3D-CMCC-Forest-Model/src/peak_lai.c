/*peak_lai.c*/

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "constants.h"


extern void Get_peak_lai_from_pipe_model (SPECIES *const s, CELL *const c, int years, int month, int height, int age)
{
	int i;
	//static float oldBasalArea;
	//static float sapwood_perc;

	Log ("\nPEAK LAI FUNCTION\n\n");
	Log("sap leaf = %g\n", s->value[SAP_LEAF]);
	Log("sap area = %g\n", s->value[SAPWOOD_AREA]);
	Log("Leaf Area from Kostner-LPJ = %g m^2\n", (s->value[SAPWOOD_AREA] / 10000) * s->value[SAP_LEAF]);
	Log("Crown diameter = %g m^2\n", s->value[CROWN_DIAMETER_DBHDC_FUNC]);
	Log("Crown Area for Kostner = %g m^2\n", s->value[CROWN_AREA_DBHDC_FUNC]);

	s->value[PEAK_Y_LAI] = ((s->value[SAPWOOD_AREA] / 10000) * s->value[SAP_LEAF]) / s->value[CROWN_AREA_DBHDC_FUNC];
	Log("year %d PEAK LAI from Kostner = %g m^2 m^-2\n",years, s->value[PEAK_Y_LAI]);


	s->value[MAX_BIOMASS_FOLIAGE_CTEM] = ((s->value[PEAK_Y_LAI] * (s->value[CANOPY_COVER_DBHDC] * settings->sizeCell))/ (s->value[SLAmkg]* GC_GDM)) / 1000;
	Log("Maximum foliage biomass = %g tDM/area \n", s->value[MAX_BIOMASS_FOLIAGE_CTEM]);
	//Log("Maximum foliage biomass = %g gDM/tree \n", (s->value[MAX_BIOMASS_FOLIAGE_CTEM] /s->counter[N_TREE])* 1000000);

	s->value[MAX_BIOMASS_FOLIAGE_CTEM] = ((s->value[PEAK_Y_LAI] / (s->value[SLAmkg]* GC_GDM))*(s->value[CANOPY_COVER_DBHDC] * settings->sizeCell)/1000);
	//Log("Maximum foliage biomass = %g Kg/area \n", s->value[MAX_BIOMASS_FOLIAGE_CTEM]/s->counter[N_TREE]);
	//Log("SLA in mod= %g KgC/m^2 \n", s->value[SLAmkg]);


	//DAILY GPP/NPP
	//cell level
	/*
	if (c->heights_count -1  == 0 && c->heights[height].ages_count -1 == 0 && c->heights[height].ages[age].species_count -1 == 0)
	{
		c->annual_peak_lai = s->value[PEAK_Y_LAI];
	}
	*/


	//fixme useful only for one class per layer

	i = c->heights[height].z;

	c->annual_peak_lai[i] = s->value[PEAK_Y_LAI];




}
