/*
 * littering.c
 *
 *  Created on: 12 apr 2017
 *      Author: alessio
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "common.h"
#include "leaf_fall.h"
#include "constants.h"
#include "settings.h"
#include "logger.h"

extern settings_t* g_settings;

void littering ( cell_t *const c, species_t *const s )
{
	/* daily flux for litter production */

	/** carbon littering **/

	/* cell level litter carbon fluxes (gC/m2/day) */
	c->daily_leaf_to_litrC        += s->value[C_LEAF_TO_LITR]  * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litrC       += s->value[C_FROOT_TO_LITR] * 1e6 / g_settings->sizeCell;
	c->daily_to_litrC             += ((s->value[C_LEAF_TO_LITR] + s->value[C_FROOT_TO_LITR]) *
			(1e6 / g_settings->sizeCell));

	/* cell level cwd carbon fluxes (gC/m2/day) */
	c->daily_stem_cwdC         += s->value[C_STEM_TO_CWD]    * 1e6 / g_settings->sizeCell;
	c->daily_croot_cwdC        += s->value[C_CROOT_TO_CWD]   * 1e6 / g_settings->sizeCell;
	c->daily_branch_cwdC       += s->value[C_BRANCH_TO_CWD]  * 1e6 / g_settings->sizeCell;
	c->daily_reserve_cwdC      += s->value[C_RESERVE_TO_CWD] * 1e6 / g_settings->sizeCell;
	c->daily_fruit_cwdC        += s->value[C_FRUIT_TO_CWD]   * 1e6 / g_settings->sizeCell;
	c->daily_cwdC              += (s->value[C_STEM_TO_CWD]  +
			s->value[C_CROOT_TO_CWD]                        +
			s->value[C_BRANCH_TO_CWD]                       +
			s->value[C_RESERVE_TO_CWD]                      +
			s->value[C_FRUIT_TO_CWD])                       *
			1e6 / g_settings->sizeCell;

	/* cell level soil carbon fluxes (gC/m2/day) */
	c->daily_soilC             += 0. ;

//	/* cell level litter carbon pools (gC/m2) */
//	c->litrC                   += (s->value[C_LEAF_TO_LITR] +
//			s->value[C_FROOT_TO_LITR]) *
//			1e6 / g_settings->sizeCell;
//	c->cwdC                    += (s->value[C_STEM_TO_CWD]  +
//			s->value[C_CROOT_TO_CWD]                        +
//			s->value[C_BRANCH_TO_CWD]                       +
//			s->value[C_RESERVE_TO_CWD]                      +
//			s->value[C_FRUIT_TO_CWD])                       *
//			1e6 / g_settings->sizeCell;
//	c->soilC                   += 0. ;

	/* leaf litter carbon fluxes */
	s->value[C_LEAF_TO_LITR1C]  = s->value[C_LEAF_TO_LITR]  * s->value[LEAF_LITR_LAB_FRAC];
	s->value[C_LEAF_TO_LITR2C]  = s->value[C_LEAF_TO_LITR]  * s->value[LEAF_LITR_USCEL_FRAC];
	s->value[C_LEAF_TO_LITR3C]  = s->value[C_LEAF_TO_LITR]  * s->value[LEAF_LITR_SCEL_FRAC];
	s->value[C_LEAF_TO_LITR4C]  = s->value[C_LEAF_TO_LITR]  * s->value[LEAF_LITR_LIGN_FRAC];

	/* fine root litter carbon fluxes */
	s->value[C_FROOT_TO_LITR1C] = s->value[C_FROOT_TO_LITR] * s->value[FROOT_LITR_LAB_FRAC];
	s->value[C_FROOT_TO_LITR2C] = s->value[C_FROOT_TO_LITR] * s->value[FROOT_LITR_USCEL_FRAC];
	s->value[C_FROOT_TO_LITR3C] = s->value[C_FROOT_TO_LITR] * s->value[FROOT_LITR_SCEL_FRAC];
	s->value[C_FROOT_TO_LITR4C] = s->value[C_FROOT_TO_LITR] * s->value[FROOT_LITR_LIGN_FRAC];

	CHECK_CONDITION ( fabs(s->value[C_LEAF_TO_LITR1C]  + s->value[C_LEAF_TO_LITR2C]  + s->value[C_LEAF_TO_LITR3C]  + s->value[C_LEAF_TO_LITR4C])  -	s->value[C_LEAF_TO_LITR]  , >, eps );
	CHECK_CONDITION ( fabs(s->value[C_FROOT_TO_LITR1C] + s->value[C_FROOT_TO_LITR2C] + s->value[C_FROOT_TO_LITR3C] + s->value[C_FROOT_TO_LITR4C]) -	s->value[C_FROOT_TO_LITR] , >, eps );

	/* cell level litter carbon fluxes */
	c->daily_leaf_to_litr1C       += s->value[C_LEAF_TO_LITR1C]  * 1e6 / g_settings->sizeCell;
	c->daily_leaf_to_litr2C       += s->value[C_LEAF_TO_LITR2C]  * 1e6 / g_settings->sizeCell;
	c->daily_leaf_to_litr3C       += s->value[C_LEAF_TO_LITR3C]  * 1e6 / g_settings->sizeCell;
	c->daily_leaf_to_litr4C       += s->value[C_LEAF_TO_LITR4C]  * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litr1C      += s->value[C_FROOT_TO_LITR1C] * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litr2C      += s->value[C_FROOT_TO_LITR2C] * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litr3C      += s->value[C_FROOT_TO_LITR3C] * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litr4C      += s->value[C_FROOT_TO_LITR4C] * 1e6 / g_settings->sizeCell;
	c->daily_litr1C               += c->daily_leaf_to_litr1C + c->daily_froot_to_litr1C;
	c->daily_litr2C               += c->daily_leaf_to_litr2C + c->daily_froot_to_litr2C;
	c->daily_litr3C               += c->daily_leaf_to_litr3C + c->daily_froot_to_litr3C;
	c->daily_litr4C               += c->daily_leaf_to_litr4C + c->daily_froot_to_litr4C;


	/* cell level litter carbon pools (tC/sizecell) */
//	c->leaf_litr1C             += s->value[C_LEAF_TO_LITR1C]  * 1e6 / g_settings->sizeCell;
//	c->leaf_litr2C             += s->value[C_LEAF_TO_LITR2C]  * 1e6 / g_settings->sizeCell;
//	c->leaf_litr3C             += s->value[C_LEAF_TO_LITR3C]  * 1e6 / g_settings->sizeCell;
//	c->leaf_litr4C             += s->value[C_LEAF_TO_LITR4C]  * 1e6 / g_settings->sizeCell;
//	c->froot_litr1C            += s->value[C_FROOT_TO_LITR1C] * 1e6 / g_settings->sizeCell;
//	c->froot_litr2C            += s->value[C_FROOT_TO_LITR2C] * 1e6 / g_settings->sizeCell;
//	c->froot_litr3C            += s->value[C_FROOT_TO_LITR3C] * 1e6 / g_settings->sizeCell;
//	c->froot_litr4C            += s->value[C_FROOT_TO_LITR4C] * 1e6 / g_settings->sizeCell;
	//todo
	//c->daily_cwd1C              += ();

	/****************************************************************************************************************/

	/** nitrogen littering **/

	/* cell level litter nitrogen fluxes (gN/m2/day) */
	c->daily_leaf_to_litrN        += s->value[N_LEAF_TO_LITR]  * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litrN       += s->value[N_FROOT_TO_LITR] * 1e6 / g_settings->sizeCell;
	c->daily_to_litrN             += ((s->value[N_LEAF_TO_LITR] + s->value[N_FROOT_TO_LITR]) *
			(1e6 / g_settings->sizeCell));

	/* cell level cwd nitrogen fluxes (gN/m2/day) */
	c->daily_stem_cwdN         += s->value[N_STEM_TO_CWD]    * 1e6 / g_settings->sizeCell;
	c->daily_croot_cwdN        += s->value[N_CROOT_TO_CWD]   * 1e6 / g_settings->sizeCell;
	c->daily_branch_cwdN       += s->value[N_BRANCH_TO_CWD]  * 1e6 / g_settings->sizeCell;
	c->daily_reserve_cwdN      += s->value[N_RESERVE_TO_CWD] * 1e6 / g_settings->sizeCell;
	c->daily_fruit_cwdN        += s->value[N_FRUIT_TO_CWD]   * 1e6 / g_settings->sizeCell;
	c->daily_cwdN              += ((s->value[N_STEM_TO_CWD] +
			s->value[N_CROOT_TO_CWD]                        +
			s->value[N_BRANCH_TO_CWD]                       +
			s->value[N_RESERVE_TO_CWD]                      +
			s->value[N_FRUIT_TO_CWD])                       *
			(1e6 / g_settings->sizeCell));

//	/* cell level soil nitrogen fluxes (gN/m2/day) */
//	c->daily_soilN             += 0. ;
//
//	/* cell level litter nitrogen pools (gN/m2) */
//	c->litrN                   += ((s->value[N_LEAF_TO_LITR] +
//			s->value[N_FROOT_TO_LITR]) *
//			(1e6 / g_settings->sizeCell));
//
//	c->cwdN                    += ((s->value[N_STEM_TO_CWD] +
//			s->value[N_CROOT_TO_CWD]                        +
//			s->value[N_BRANCH_TO_CWD]                       +
//			s->value[N_RESERVE_TO_CWD]                      +
//			s->value[N_FRUIT_TO_CWD])                       *
//			(1e6 / g_settings->sizeCell));
//	c->soilN                   += 0.;

	/* leaf litter nitrogenfluxes */
	s->value[N_LEAF_TO_LITR1N]  = s->value[N_LEAF_TO_LITR]  * s->value[LEAF_LITR_LAB_FRAC];
	s->value[N_LEAF_TO_LITR2N]  = s->value[N_LEAF_TO_LITR]  * s->value[LEAF_LITR_USCEL_FRAC];
	s->value[N_LEAF_TO_LITR3N]  = s->value[N_LEAF_TO_LITR]  * s->value[LEAF_LITR_SCEL_FRAC];
	s->value[N_LEAF_TO_LITR4N]  = s->value[N_LEAF_TO_LITR]  * s->value[LEAF_LITR_LIGN_FRAC];

	/* fine root litter nitrogenfluxes */
	s->value[N_FROOT_TO_LITR1N] = s->value[N_FROOT_TO_LITR] * s->value[FROOT_LITR_LAB_FRAC];
	s->value[N_FROOT_TO_LITR2N] = s->value[N_FROOT_TO_LITR] * s->value[FROOT_LITR_USCEL_FRAC];
	s->value[N_FROOT_TO_LITR3N] = s->value[N_FROOT_TO_LITR] * s->value[FROOT_LITR_SCEL_FRAC];
	s->value[N_FROOT_TO_LITR4N] = s->value[N_FROOT_TO_LITR] * s->value[FROOT_LITR_LIGN_FRAC];

	/* cell level litter nitrogenfluxes */
	c->daily_leaf_to_litr1N       += s->value[N_LEAF_TO_LITR1N]  * 1e6 / g_settings->sizeCell;
	c->daily_leaf_to_litr2N       += s->value[N_LEAF_TO_LITR2N]  * 1e6 / g_settings->sizeCell ;
	c->daily_leaf_to_litr3N       += s->value[N_LEAF_TO_LITR3N]  * 1e6 / g_settings->sizeCell;
	c->daily_leaf_to_litr4N       += s->value[N_LEAF_TO_LITR4N]  * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litr1N      += s->value[N_FROOT_TO_LITR1N] * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litr2N      += s->value[N_FROOT_TO_LITR2N] * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litr3N      += s->value[N_FROOT_TO_LITR3N] * 1e6 / g_settings->sizeCell;
	c->daily_froot_to_litr4N      += s->value[N_FROOT_TO_LITR4N] * 1e6 / g_settings->sizeCell;
	c->daily_litr1N               += c->daily_leaf_to_litr1N + c->daily_froot_to_litr1N;
	c->daily_litr2N               += c->daily_leaf_to_litr2N + c->daily_froot_to_litr2N;
	c->daily_litr3N               += c->daily_leaf_to_litr3N + c->daily_froot_to_litr3N;
	c->daily_litr4N               += c->daily_leaf_to_litr4N + c->daily_froot_to_litr4N;

//	/* cell level litter nitrogenpools (tC/sizecell) */
//	c->leaf_litr1N             += s->value[N_LEAF_TO_LITR1N]  * 1e6 / g_settings->sizeCell;
//	c->leaf_litr2N             += s->value[N_LEAF_TO_LITR2N]  * 1e6 / g_settings->sizeCell;
//	c->leaf_litr3N             += s->value[N_LEAF_TO_LITR3N]  * 1e6 / g_settings->sizeCell;
//	c->leaf_litr4N             += s->value[N_LEAF_TO_LITR4N]  * 1e6 / g_settings->sizeCell;
//	c->froot_litr1N            += s->value[N_FROOT_TO_LITR1N] * 1e6 / g_settings->sizeCell;
//	c->froot_litr2N            += s->value[N_FROOT_TO_LITR2N] * 1e6 / g_settings->sizeCell;
//	c->froot_litr3N            += s->value[N_FROOT_TO_LITR3N] * 1e6 / g_settings->sizeCell;
//	c->froot_litr4N            += s->value[N_FROOT_TO_LITR4N] * 1e6 / g_settings->sizeCell;
	//todo
	//c->daily_cwd1C              += ();

}
