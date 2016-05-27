/*
 * canopy_evaporation.c
 *
 *  Created on: 07/nov/2013
 *      Author: alessio
 */


/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "constants.h"
#include "logger.h"

extern logger_t* g_log;


void Canopy_evapotranspiration (SPECIES *const s, CELL *const c, int height)
{

	static int i;
	logger(g_log, "\nCANOPY_EVAPOTRANSPIRATION_ROUTINE\n");
	/*it computes canopy evaporation + canopy transpiration*/

	/*dominant layer*/
	if (c->heights[height].z == c->top_layer)
	{
		s->value[CANOPY_EVAPOTRANSPIRATION] = s->value[DAILY_TRANSP] + s->value[CANOPY_EVAPORATION];
		logger(g_log, "Canopy evapotranspiration = %f mm\n", s->value[CANOPY_EVAPOTRANSPIRATION]);
		/*last height dominant class processed*/
		if (c->dominant_veg_counter == c->height_class_in_layer_dominant_counter)
		{
			/*control*/
			if (c->asw < c->layer_daily_c_evapotransp[c->top_layer])
			{
				logger(g_log, "ATTENTION DAILY EVAPOTRANSPIRATION EXCEEDS AVAILABLE SOIL WATER!!!\n");
				c->layer_daily_c_evapotransp[c->top_layer] = c->asw;
				exit(1);
			}
		}
	}
	/*dominated*/
	else
	{
		/*dominated layer*/
		if (c->heights[height].z == c->top_layer-1)
		{
			s->value[CANOPY_EVAPOTRANSPIRATION] = s->value[DAILY_TRANSP] + s->value[CANOPY_EVAPORATION];
			logger(g_log, "Canopy evapotranspiration = %f mm\n", s->value[CANOPY_EVAPOTRANSPIRATION]);
			/*last height dominant class processed*/
			if (c->dominated_veg_counter == c->height_class_in_layer_dominated_counter)
			{
				/*control*/
				if (c->asw < c->layer_daily_c_evapotransp[c->top_layer-1])
				{
					logger(g_log, "ATTENTION DAILY EVAPOTRANSPIRATION EXCEEDS AVAILABLE SOIL WATER!!!\n");
					c->layer_daily_c_evapotransp[c->top_layer-1] = c->asw;
					exit(1);
				}
			}
		}
		/*subdominated layer*/
		else
		{
			s->value[CANOPY_EVAPOTRANSPIRATION] = s->value[DAILY_TRANSP] + s->value[CANOPY_EVAPORATION];
			logger(g_log, "Canopy evapotranspiration = %f mm\n", s->value[CANOPY_EVAPOTRANSPIRATION]);
			/*last height dominant class processed*/
			if (c->subdominated_veg_counter == c->height_class_in_layer_subdominated_counter)
			{
				/*control*/
				if (c->asw < c->layer_daily_c_evapotransp[c->top_layer-2])
				{
					logger(g_log, "ATTENTION DAILY EVAPOTRANSPIRATION EXCEEDS AVAILABLE SOIL WATER!!!\n");
					c->layer_daily_c_evapotransp[c->top_layer-2] = c->asw;
					exit(1);
				}
			}
		}
	}

	i = c->heights[height].z;

	c->layer_daily_c_evapotransp[i] += s->value[CANOPY_EVAPOTRANSPIRATION];
	c->layer_monthly_c_evapotransp[i] += s->value[CANOPY_EVAPOTRANSPIRATION];
	c->layer_annual_c_evapotransp[i] += s->value[CANOPY_EVAPOTRANSPIRATION];

	c->daily_c_evapotransp += s->value[CANOPY_EVAPOTRANSPIRATION];
	c->monthly_c_evapotransp += s->value[CANOPY_EVAPOTRANSPIRATION];
	c->annual_c_evapotransp += s->value[CANOPY_EVAPOTRANSPIRATION];


	/*compute energy balance evapotranspiration from canopy*/
	c->daily_c_evapotransp_watt = c->daily_c_transp_watt + c->daily_c_int_watt;
	logger(g_log, "Energy for canopy evapotranspiration = %f W/m^2\n", c->daily_c_evapotransp_watt);

}



