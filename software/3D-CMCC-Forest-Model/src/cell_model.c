/*
 * cell_model.c
 *
 *  Created on: 31 ago 2016
 *      Author: alessio
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"
#include "constants.h"
#include "logger.h"
#include "canopy_radiation_lw_band.h"
#include "canopy_radiation_sw_band.h"
#include "evapotranspiration.h"
#include "heat_fluxes.h"
#include "fluxes.h"
#include "check_balance.h"

extern logger_t* g_debug_log;

int Cell_model_daily (matrix_t *const m, const int cell, const int day, const int month, const int year)
{

	/* shortcuts */
	cell_t *c;
	meteo_daily_t *meteo_daily;

	/* assign shortcuts */
	c = &m->cells[cell];
	meteo_daily = &m->cells[cell].years[year].m[month].d[day];

	/* check parameters */
	assert(m);

	logger (g_debug_log, "\n*********CELL_MODEL_DAILY*********\n");

	logger(g_debug_log, "-number of tree heights (if present) = %d\n", c->heights_count);
	logger(g_debug_log, "-number of tree layer   (if present) = %d\n", c->tree_layers_count);

	/* OVERALL CELL */

	/* compute cell level evapotranspiration */
	evapotranspiration ( c );

	/* compute cell level latent heat fluxes */
	latent_heat_flux ( c, meteo_daily );

	/* compute cell level sensible heat fluxes */
	sensible_heat_flux ( c, meteo_daily );

	/* compute cell level water fluxes */
	water_fluxes ( c, meteo_daily );

	/*******************************************************************************************************/

	/* CHECK FOR BALANCE CLOSURE */

	/* CHECK FOR RADIATIVE FLUX BALANCE CLOSURE */
	/* 1 */ if ( ! check_cell_radiation_flux_balance ( c, meteo_daily ) ) return 0;

	/* CHECK FOR CARBON FLUX BALANCE CLOSURE */
	/* 2 */ if ( ! check_cell_carbon_flux_balance    ( c ) ) return 0;

	/* CHECK FOR CARBON MASS BALANCE CLOSURE */
	/* 3 */ if ( ! check_cell_carbon_mass_balance    ( c ) ) return 0;

	/* CHECK FOR WATER FLUX BALANCE CLOSURE */
	/* 4 */ if ( ! check_cell_water_flux_balance     ( c, meteo_daily ) ) return 0;

	/*******************************************************************************************************/

	/* ok */
	return 1;

}
