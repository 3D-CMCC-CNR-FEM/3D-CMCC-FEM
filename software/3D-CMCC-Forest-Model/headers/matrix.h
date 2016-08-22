/* matrix.h */
#ifndef MATRIX_H_
#define MATRIX_H_

#include "yos.h"

#define MAXTURNTIME 5000

typedef enum {
	F		/* forest */
	, Z		/* crop */
} e_landuse;

typedef enum {
	T		/* timber */
	, C		/* coppice */
} e_management;

enum {
	/*valori relativi alla specie*/
	/* !!!!! NON SPOSTARE !!!!!!*/
	/* serve questo ordine per l'importazione, vedere species_values dentro matrix.c */
	LIGHT_TOL,
	PHENOLOGY,                  //PHENOLOGY 0=deciduous, 1=evergreen
	ALPHA,                      //Canopy quantum efficiency (molC/molPAR)
	EPSILONgCMJ,                //Light Use Efficiency  (gC/MJ)(used if ALPHA is not available)
	K,                          //Extinction coefficient for absorption of PAR by canopy for Quercus cerris L. (A. Cutini, Ann Sci For, 1996)
	ALBEDO,                     //species-specific albedo (ratio)
	GAMMA_LIGHT,                //parameter for light modifier (not used)
	INT_COEFF,                  //precip interception coefficient LAI-1day-1
	SLA_AVG,                    //AVERAGE Specific Leaf Area m^2/KgC for sunlit/shaded leaves
	SLA_RATIO,                  //(DIM) ratio of shaded to sunlit projected SLA
	LAI_RATIO,                  //(DIM) all-sided to projected leaf area ratio
	FRACBB0,                    //Branch and Bark fraction at age 0 (m^2/kg)
	FRACBB1,                    //Branch and Bark fraction for mature stands (m^2/kg)
	TBB,                        //Age at which fracBB = (FRACBB0 + FRACBB1 )/ 2
	RHOMIN,                     //Minimum Basic Density for young Trees
	RHOMAX,                     //Maximum Basic Density for young Trees (Ferrara-Nol�)
	TRHO,                       //Age at which rho = (RHOMIN + RHOMAX )/2
	COEFFCOND,                  //Define stomatal responsee to VPD in m/sec TO REMOVE
	BLCOND,                     //Canopy Boundary Layer conductance
	MAXCOND,                    //Maximum leaf Conductance in m/sec
	CUTCOND,                    //cuticular conductance in m/sec
	MAXAGE,                     //Determines rate of "physiological decline" of forest
	RAGE,                       //Relative Age to give fAGE = 0.5
	NAGE,                       //Power of relative Age in function for Age
	MAXAGE_S,
	RAGE_S,                     //Relative Age to give fAGE = 0.5
	NAGE_S,                     //Power of relative Age in function for Age
	GROWTHTMIN,                 //Minimum temperature for growth
	GROWTHTMAX,                 //Maximum temperature for growth
	GROWTHTOPT,                 //Optimum temperature fro growth
	GROWTHSTART,                //Thermic sum  value for starting growth in �C
	GROWTHEND,                  //Thermic sum  value for ending growth in �C
	MINDAYLENGTH,               //minimum day length for phenology
	SWPOPEN,
	SWPCLOSE,
	SWCONST,                    //Costant in Soil Water modifier vs Moist Ratio
	SWPOWER,                    //Power in Soil Water modifier vs Moist Ratio
	OMEGA_CTEM,                 //ALLOCATION PARAMETER
	S0CTEM,                     //PARAMETER CONTROLLING ALLOCATION TO STEM
	R0CTEM,                     //PARAMETER CONTROLLING ALLOCATION TO ROOT
	F0CTEM,                     //PARAMETER CONTROLLING ALLOCATION TO FOLIAGE
	RES0CTEM,                   //PARAMETER CONTROLLING ALLOCATION TO RESERVE
	EPSILON_CTEM,               //ALLOCATION PARAMETER IN (Kgcm^2)^-0.6
	KAPPA,                      //ALLOCATION PARAMETER DIMENSIONLESS
	MIN_R0CTEM,                 //MINIMUM RATE TO ROOT AT THE FIRST YEAR AFTER COPPICING
	MAX_S0CTEM,                 //MAXIMUM RATE TO STEM AT THE FIRST YEAR AFTER COPPICING
	YEARS_FOR_CONVERSION,       //years from coppicing to consider tree as a timber
	FRUIT_PERC,                 //percentage of NPP for fruits
	CONES_LIFE_SPAN,            //life life span for cones (for conifers only)
	FINE_ROOT_LEAF,             //allocation new fine root C:new leaf (ratio)
	STEM_LEAF,                  //allocation new stem C:new leaf (ratio)
	COARSE_ROOT_STEM,           //allocation new coarse root C:new stem (ratio)
	LIVE_TOTAL_WOOD,            //new live C:new total wood (ratio)
	CN_LEAVES,                  //CN of leaves (kgC/kgN)
	CN_FALLING_LEAVES,          //CN of falling leaf litter (kgC/kgN)
	CN_FINE_ROOTS,              //CN of fine roots (kgC/kgN)
	CN_LIVE_WOODS,              //CN of live woods (kgC/kgN)
	CN_DEAD_WOODS,              //CN of dead woods (kgC/kgN)
	BUD_BURST,                  //days of bud burst at the beginning of growing season (only for deciduous)
	LEAF_FALL_FRAC_GROWING,     //proportions of the growing season of leaf fall
	LEAF_FINEROOT_TURNOVER,     //Average daily leaves and fine root turnover rate (for evergreen)
	LIVE_WOOD_TURNOVER,         //Average daily live wood turnover rate
	WSX1000,                    //Max stem mass (kg) per tree at 1000 trees/hectare
	THINPOWER,                  //Power in self-thinning rule
	MF,                         //Fraction mean single tree foliage biomass lost per dead tree
	MR,                         //Fraction mean single tree root biomass lost per dead tree
	MS,                         //Fraction mean single tree stem biomass lost per dead tree
	DBHDCMAX,                   //Low Density
	DBHDCMIN,                   //High Density
	SAP_A,                      //a coefficient for sapwood
	SAP_B,                      //b coefficient for sapwood
	SAP_LEAF,                   //sapwood_max leaf area ratio in pipe model
	SAP_WRES,                   //Sapwood-Reserve biomass ratio used if no Wres data are available
	HMAX,                       //Max Height in m
	DMAX,                       //Max Diameter in cm
	HPOWER,                     //Slope of Asymptotic Height from Sortie
	RPOWER,                     //Slope of Asymptotic Crown-Radius from Sortie
	b_RPOWER,
	CHPOWER,                    //Slope of Asymptotic Crown-Height from Sortie
	b_CHPOWER,
	STEMCONST_P,
	STEMPOWER_P,
	CRA,
	CRB,
	CRC,
	HDMAX,                      //Height to Base diameter ratio MAX
	HDMIN,                      //Height to Base diameter ratio MIN
	DENMAX,                     //Maximum density (trees/10000 m^2)
	DENMIN,                     //Minimum density (trees/10000 m^2)
	MINPAREST,                  //Minimum Monthly PAR (W/m^2 hour) for Establishment for the Dominated Layer
	MINRAIN,                    //Minimum annual Rain Precipitation for Establishment
	ADULT_AGE,
	MAXSEED,                    //max numbers of seed per tree (from TREEMIG)
	MASTSEED,                   //ricorrenza anni di pasciona (da TREEMIG)
	WEIGHTSEED,                 //peso frutto in g
	SEXAGE,                     //Age at Sexual Maturity
	GERMCAPACITY,               //Geminability (Lischke H. & Loffler T. J.)
	MINTEMP,                    //Minimum temperature for germination in �C
	ESTMAX,                     //Potential Establishment rate in the absence of competition
	FRACFRUIT,                  //Fraction of NPP to Fruit Production
	ROTATION,
	MINAGEMANAG,                //Minimum age for Managment
	MINDBHMANAG,                //Minimum DBH for Managment
	AV_SHOOT,                   //Average number of shoots produced after coppicing

	/* ALESSIOR:

		AVDBH deve essere sempre il primo perchè prima di lui ci sono i
		parametri con i valori CHE NON CAMBIANO MAI (COSTANTI!!!!!)

		oltretutto l'indice AVDBH viene usato in new_class.c
	 */

	AVDBH,                      //Average DBH in cm
	CROWN_RADIUS_SORTIE,        //Crown Radius in m from Sortie Standard
	CROWN_DIAMETER_SORTIE,      //Crown Diameter in m from Sortie Standard
	CROWN_AREA_SORTIE,          //Crown Area from Sortie diameter
	CROWN_HEIGHT_SORTIE,        //Crown Height in m from Sortie Standard
	TREE_HEIGHT_SORTIE,         //Tree Height in m from Sortie function
	TREE_HEIGHT_LPJ,            //Tree Height in m from LPJ
	CROWN_AREA_LPJ,             //Crown Area in m^2 from LPJ
	CROWN_DIAMETER_LPJ,         //Crown Diameter in m from LPJ
	CANOPY_COVER_LPJ,           //Canopy Cover % of pixel covered from LPJ
	DBHDC_EFF,                  //Crown Diameter from DBH in function of density
	PREVIOUS_DBHDC_EFF,         //previous dbhdc value
	CROWN_DIAMETER_DBHDC,       //Crown Diameter in m from DBHDC function
	CROWN_AREA_DBHDC,           //Crown Area in m^2 from DBHDC function
	CANOPY_COVER_DBHDC,         //Canopy Cover % of pixel covered from DBHDC function
	CANOPY_COVER_P,             //Canopy Cover from Portoghesi Function % of pixel covered
	FREE_CANOPY_COVER_P,
	TREE_HEIGHT_CR,             //Tree Height in m from CHAPMAN-RICHARDS FUNCTION
	HD_EFF,                     //Effective HD ratio to give to Crowding Competition Function
	CC_TREE_HEIGHT,             //Tree Height in m from Crowding Competition Function in m
	CC_AVDBH,                   //Average DBH from Crowding Competition Function in cm
	DENSITY,                    //Numbers of Tree per m^2
	SAPWOOD_AREA,
	SAPWOOD_PERC,
	HEARTWOOD_AREA,
	HEARTWOOD_PERC,
	BASAL_AREA,                 //Basal Area (cm^2/area tree)
	STAND_BASAL_AREA,
	BASAL_AREA_m2,              //Basal Area (cm^2/area tree)
	STAND_BASAL_AREA_m2,
	CROWN_HEIGHT,               //Crown Height (m)
	MASS_DENSITY,               //mass density
	VOLUME,                     //Stem Volume
	TREE_VOLUME,                //Single Tree Volume (m^3/area)
	CAI,                        //Current Annual Increment
	MAI,                        //Mean Annual Volume Increment (m^3/area year)
	STEMCONST,

	/* PAR */
	PAR,                        //Photosynthetically Active Radiation molPAR/m^2/day
	PAR_REFL,                   //Reflected Photosynthetically Active Radiation molPAR/m^2/day OVERALL CANOPY
	APAR,                       //Absorbed Physiological Active Radiation molPAR/m^2/day
	APAR_SUN,                   //Absorbed Physiological Active Radiation molPAR/m^2/day for sun leaves
	APAR_SHADE,                 //Absorbed Physiological Active Radiation molPAR/m^2/day for shaded leaves
	TRANSM_PAR,                 //Transmitted Photosynthetically Active Radiation molPAR/m^2/day
	TRANSM_PAR_SUN,             //Transmitted Photosynthetically Active Radiation molPAR/m^2/day from sun leaves
	TRANSM_PAR_SHADE,           //Transmitted Photosynthetically Active Radiation molPAR/m^2/day from shaded leaves

	/* short wave */
	SW_RAD,                     //Short Wave Radiation in W/m2
	SW_RAD_REFL,                //Reflected Short Wave Radiation W/m2 OVERALL CANOPY
	SW_RAD_ABS,                 //Absorbed Net Short Wave radiation in W/m2
	SW_RAD_ABS_SUN,             //Absorbed Net Short Wave Radiation W/m2 for sun leaves
	SW_RAD_ABS_SHADE,           //Absorbed Net Short Wave Radiation W/m2 for shaded leaves
	SW_RAD_TRANSM,              //Transmitted Net Short Wave Radiation W/m2
	SW_RAD_TRANSM_SUN,          //Transmitted Net Short Wave Radiation W/m2 for sun leaves
	SW_RAD_TRANSM_SHADE,        //Transmitted Net Short Wave Radiation W/m2 for shaded leaves

	/* long wave */
	NET_LW_RAD,                 //Long Wave Radiation in W/m2
	LW_RAD_REFL,                //Reflected Long Wave Radiation W/m2 OVERALL CANOPY
	LW_RAD_EMIT,                //Emitted Long Wave radiation in W/m2
	LW_RAD_EMIT_SUN,            //Emitted Long Wave Radiation W/m2 for sun leaves
	LW_RAD_EMIT_SHADE,          //Emitted Long Wave Radiation W/m2 for shaded leaves
	LW_RAD_ABS,                 //Absorbed Long Wave radiation in W/m2
	LW_RAD_ABS_SUN,             //Absorbed Long Wave Radiation W/m2 for sun leaves
	LW_RAD_ABS_SHADE,           //Absorbed Long Wave Radiation W/m2 for shaded leaves
	LW_RAD_TRANSM,              //Transmitted Long Wave Radiation W/m2
	LW_RAD_TRANSM_SUN,          //Transmitted Long Wave Radiation W/m2 for sun leaves
	LW_RAD_TRANSM_SHADE,        //Transmitted Long Wave Radiation W/m2 for shaded leaves

	/* net radiation */
	NET_RAD,                    //Short Wave Radiation in W/m2
	RAD_REFL,                   //Reflected Radiation W/m2 OVERALL CANOPY
	NET_RAD_ABS,                //Absorbed Net radiation in W/m2
	NET_RAD_ABS_SUN,            //Absorbed Net Radiation W/m2 for sun leaves
	NET_RAD_ABS_SHADE,          //Absorbed Net Radiation W/m2 for shaded leaves
	NET_RAD_TRANSM,             //Transmitted Net Radiation W/m2
	NET_RAD_TRANSM_SUN,         //Transmitted Net Radiation W/m2 for sun leaves
	NET_TRANSM_SHADE,           //Transmitted Net Radiation W/m2 for shaded leaves

	/* PPFD */
	PPFD,                       //Photosynthetic Photon Flux Density umol/m2/sec
	PPFD_REFL,                  //Transmitted Photosynthetic Photon Flux Density umol/m2/sec OVERALL CANOPY
	PPFD_ABS,                   //Absorbed Photosynthetic Photon Flux Density umol/m2/sec
	PPFD_ABS_SUN,               //Absorbed Photosynthetic Photon Flux Density umol/m2/sec for sun leaves
	PPFD_ABS_SHADE,             //Absorbed Photosynthetic Photon Flux Density umol/m2/sec for shaded leaves
	PPFD_TRANSM,                //Transmitted Photosynthetic Photon Flux Density umol/m2/sec
	PPFD_TRANSM_SUN,            //Transmitted Photosynthetic Photon Flux Density umol/m2/sec  from sun leaves
	PPFD_TRANSM_SHADE,          //Transmitted Photosynthetic Photon Flux Density umol/m2/sec  from shaded leaves

	/*modifiers variables*/
	F_VPD,                      //VPD modifier
	F_LIGHT,                    //LIGHT modifier
	F_LIGHT_SUN,                //LIGHT modifier for Sun leaves
	F_LIGHT_SHADE,              //LIGHT modifier for Shaded leaves
	F_AGE,                      //AGE modifier
	F_NUTR,                     //SOIL NUTRIENT Modifier
	F_T,                        //TEMPERATURE modifier
	F_FROST,                    //FROST modifer
	F_SW,                       //SOIL WATER modifier
	F_DROUGHT,                  //SOIL DROUGHT modifier (see Duursma et al., 2008)
	F_PSI,                      //SOIL WATER modifier using PSI, see Biome
	F_CO2,                      //CO2 fert effect
	PHYS_MOD,                   //Physmod
	YEARLY_PHYS_MOD,
	AVERAGE_PHYS_MOD,
	AVERAGE_F_VPD,
	AVERAGE_F_NUTR,
	AVERAGE_F_T,
	AVERAGE_F_SW,

	/*water variables*/
	CANOPY_CONDUCTANCE,
	CANOPY_BLCOND,              //Canopy Boundary Layer conductance (m s-1)
	FRAC_DAYTIME_TRANSP,        //fraction of daytime for transpiration (and photosynthesis)
	CANOPY_INT,
	CANOPY_EVAPO,
	CANOPY_WET,                 //it should'nt be reset every day
	CANOPY_TRANSP,
	CANOPY_EVAPO_TRANSP,
	CANOPY_INT_SNOW,
	CANOPY_WATER,               //it should'nt be reset every day
	OLD_CANOPY_WATER,           //it should'nt be reset every day
	CANOPY_SNOW,                //it should'nt be reset every day
	OLD_CANOPY_SNOW,            //it should'nt be reset every day
	CANOPY_FRAC_DAY_TRANSP,     //Fraction of daily canopy transpiration (DIM)
	CANOPY_LATENT_HEAT,
	CANOPY_SENSIBLE_HEAT,

	/*LAI*/
	LAI,                        //LAI (m^2/m2)
	LAI_SUN,
	LAI_SHADE,
	MAX_LAI,                    //lai at the very first day of senescence: parameter of the sigmoid function
	PEAK_LAI,                   //PEAK LAI
	LEAF_TEMP_K,
	CANOPY_TEMP_K,
	CANOPY_TEMP_K_OLD,

	/*carbon variables*/
	C_FLUX,
	DAILY_GPP_gC,               //Daily Gross Primary Production gC/m^2 day
	MONTHLY_GPP_gC,             //Class Monthly Gross Primary Production gC/m^2 month
	YEARLY_GPP_gC,              //Yearly GPP
	NPP_gC,                     //Net Primary Production in grams of C
	MONTHLY_NPP_gC,
	YEARLY_NPP_gC,
	NPP_tC,
	MONTHLY_NPP_tC,
	YEARLY_NPP_tC,

	/*biomass variables*/
	/* carbon to carbon pools in gC/m2/day */
	C_TO_LEAF,
	C_TO_ROOT,
	C_TO_FINEROOT,
	C_TO_COARSEROOT,
	C_TO_TOT_STEM,
	C_TO_STEM,
	C_TO_BRANCH,
	C_TO_RESERVE,
	C_TO_FRUIT,
	C_TO_LITTER,
	C_LEAF_TO_RESERVE,
	C_FINEROOT_TO_RESERVE,
	C_LEAF_TO_LITTER,
	C_FINE_ROOT_TO_LITTER,
	C_FRUIT_TO_LITTER,
	C_STEM_LIVEWOOD_TO_DEADWOOD,
	C_COARSEROOT_LIVE_WOOD_TO_DEADWOOD,
	C_BRANCH_LIVE_WOOD_TO_DEAD_WOOD,

	/* carbon pools in tons of C/area */
	LEAF_C,
	OLD_LEAF_C,
	MAX_LEAF_C,
	TOT_ROOT_C,
	COARSE_ROOT_C,
	FINE_ROOT_C,
	MAX_FINE_ROOT_C,
	MAX_BUD_BURST_C,
	STEM_C,
	BRANCH_C,
	TOT_STEM_C,
	MIN_RESERVE_C,
	RESERVE_C,
	FRUIT_C,
	LITTER_C,
	LIVE_WOOD_C,
	DEAD_WOOD_C,
	TOT_WOOD_C,
	TOTAL_C,
	STEM_LIVE_WOOD_C,
	STEM_DEAD_WOOD_C,
	COARSE_ROOT_LIVE_WOOD_C,
	COARSE_ROOT_DEAD_WOOD_C,
	BRANCH_LIVE_WOOD_C,
	BRANCH_DEAD_WOOD_C,
	RESERVE_FOLIAGE_TO_RETRANSL_C,
	RESERVE_FINEROOT_TO_RETRANSL_C,
	STEM_SAPWOOD_C,
	STEM_HEARTWOOD_C,
	COARSE_ROOT_SAPWOOD_C,
	COARSE_ROOT_HEARTWOOD_C,
	BRANCH_SAPWOOD_C,
	BRANCH_HEARTWOOD_C,
	TOT_SAPWOOD_C,

	EFF_LIVE_TOTAL_WOOD_FRAC,

	/* per tree  in KgC */
	AV_LEAF_MASS_KgC,
	AV_STEM_MASS_KgC,
	AV_TOT_STEM_MASS_KgC,
	AV_ROOT_MASS_KgC,
	AV_FINE_ROOT_MASS_KgC,
	AV_COARSE_ROOT_MASS_KgC,
	AV_RESERVE_MASS_KgC,
	AV_MIN_RESERVE_KgC,
	AV_BRANCH_MASS_KgC,
	AV_LIVE_STEM_MASS_KgC,
	AV_DEAD_STEM_MASS_KgC,
	AV_LIVE_COARSE_ROOT_MASS_KgC,
	AV_DEAD_COARSE_ROOT_MASS_KgC,
	AV_LIVE_BRANCH_MASS_KgC,
	AV_DEAD_BRANCH_MASS_KgC,
	AV_LIVE_WOOD_MASS_KgC,
	AV_DEAD_WOOD_MASS_KgC,
	AV_TOT_WOOD_MASS_KgC,

	//CTEM CARBON
	//carbon biomass increment
	DEL_ROOTS_TOT,
	DEL_STEMS,
	DEL_FOLIAGE,
	DEL_RESERVE,
	DEL_FRUIT,
	DEL_BB,
	DEL_TOT_STEM,
	DEL_ROOTS_FINE,
	DEL_ROOTS_COARSE,
	DAILY_DEL_LITTER,

	//total cell carbon biomass pools in MgDM
	BIOMASS_FOLIAGE_tDM,
	BIOMASS_ROOTS_TOT_tDM,
	BIOMASS_COARSE_ROOT_tDM,
	BIOMASS_FINE_ROOT_tDM,
	BIOMASS_STEM_tDM,
	BIOMASS_BRANCH_tDM,
	BIOMASS_TOT_STEM_tDM, //STEM + BRANCH AND BARK
	RESERVE_tDM,
	BIOMASS_FRUIT_tDM,   //only for coniferous
	BIOMASS_LIVE_WOOD_tDM,
	BIOMASS_DEAD_WOOD_tDM,
	BIOMASS_STEM_LIVE_WOOD_tDM,
	BIOMASS_STEM_DEAD_WOOD_tDM,
	BIOMASS_COARSE_ROOT_LIVE_WOOD_tDM,
	BIOMASS_COARSE_ROOT_DEAD_WOOD_tDM,
	BIOMASS_STEM_BRANCH_LIVE_WOOD_tDM,
	BIOMASS_STEM_BRANCH_DEAD_WOOD_tDM,
	RESERVE_FOLIAGE_TO_RETRANSL_tDM,
	RESERVE_FINEROOT_TO_RETRANSL_tDM,
	MIN_RESERVE_tDM,

	/* per tree  in KgDM */
	AV_FOLIAGE_MASS_KgDM,
	AV_TOT_STEM_MASS_KgDM,
	AV_STEM_MASS_KgDM,
	AV_ROOT_MASS_KgDM,
	AV_FINE_ROOT_MASS_KgDM,
	AV_COARSE_ROOT_MASS_KgDM,
	AV_RESERVE_MASS_KgDM,
	AV_MIN_RESERVE_KgDM,
	AV_BRANCH_MASS_KgDM,
	AV_LIVE_STEM_MASS_KgDM,
	AV_DEAD_STEM_MASS_KgDM,
	AV_LIVE_COARSE_ROOT_MASS_KgDM,
	AV_DEAD_COARSE_ROOT_MASS_KgDM,
	AV_LIVE_WOOD_MASS_KgDM,
	AV_DEAD_WOOD_MASS_KgDM,
	AV_LIVE_BRANCH_MASS_KgDM,
	AV_DEAD_BRANCH_MASS_KgDM,
	CLASS_AGB,
	CLASS_BGB,
	//converted biome fraction for turnover
	LIVE_TOTAL_WOOD_TURNOVER_FRAC,
	MAX_BIOMASS_BUDBURST_tDM,
	MAX_BIOMASS_FOLIAGE_tDM,
	MAX_BIOMASS_FINE_ROOTS_tDM,
	WS_sap_tDM,
	WS_heart_tDM,
	WRC_sap_tDM,
	WRC_heart_tDM,
	WBB_sap_tDM,
	WBB_heart_tDM,
	WTOT_sap_tDM,
	//Branch and bark fraction --- currently the model doesn't accumulate it!!!!!!!
	W_SEED,                        //Seed Biomass
	DEL_TOTAL_W,                   //Total Biomass Increment
	TOTAL_W,
	DEL_Y_WTS,
	DEL_Y_WS,                      //Yearly cumulated stem biomass increment
	DEL_Y_WF,                      //Yearly cumulated foliage biomass increment
	DEL_Y_WR,                      //Yearly cumulated root biomass increment
	DEL_Y_WFR,
	DEL_Y_WCR,
	DEL_Y_WRES,                    //Yearly cumulated reserve biomass increment
	DEL_Y_BB,

	//converted biome fraction for allocation
	FINE_ROOT_LEAF_FRAC,
	STEM_LEAF_FRAC,
	COARSE_ROOT_STEM_FRAC,
	LIVE_TOTAL_WOOD_FRAC,
	//FINE/COARSE RATIO
	FR_CR,                         //FINE-COARSE ROOT RATIO
	FRACBB,

	//MONTHLY PHENOLOGY
	FRAC_MONTH_FOLIAGE_REMOVE,
	MONTH_FRAC_FOLIAGE_REMOVE,
	//DAILY PHENOLOGY
	FRAC_DAY_FOLIAGE_REMOVE,      //number of leaf fall days/tot number of veg days
	FOLIAGE_REDUCTION_RATE,
	DAILY_FOLIAGE_BIOMASS_TO_REMOVE,
	//Marconi
	DAILY_FINEROOT_BIOMASS_TO_REMOVE,
	DAILY_LEAVES_BIOMASS_TO_REMOVE,
	OLD_BIOMASS_ROOTS_COARSE,
	OLD_BIOMASS_FINE_ROOT_tDM,
	OLD_BIOMASS_STEM,
	OLD_BIOMASS_BRANCH,
	OLD_BIOMASS_LEAVES,

	OLD_BIOMASS_STEM_LIVE_WOOD,
	OLD_BIOMASS_COARSE_ROOT_LIVE_WOOD,
	OLD_BIOMASS_STEM_BRANCH_LIVE_WOOD,
	FIRST_DAY_LAI,

	/*Maintenance respiration*/
	DAILY_LEAF_MAINT_RESP,
	NIGHTLY_LEAF_MAINT_RESP,
	TOT_DAY_LEAF_MAINT_RESP,
	FINE_ROOT_MAINT_RESP,
	STEM_MAINT_RESP,
	COARSE_ROOT_MAINT_RESP,
	BRANCH_MAINT_RESP,
	LIVE_STEM_MAINT_RESP,
	LIVE_COARSE_ROOT_MAINT_RESP,
	TOTAL_MAINT_RESP,

	/*Growth respiration*/
	LEAF_GROWTH_RESP,
	FINE_ROOT_GROWTH_RESP,
	STEM_GROWTH_RESP,
	BRANCH_GROWTH_RESP,
	COARSE_ROOT_GROWTH_RESP,
	TOTAL_GROWTH_RESP,

	//Autotrophic respiration
	LEAF_AUT_RESP,
	FINE_ROOT_AUT_RESP,
	STEM_AUT_RESP,
	COARSE_ROOT_AUT_RESP,
	BRANCH_AUT_RESP,
	TOTAL_AUT_RESP,

	/* nitrogen pools (tN/area) */
	LEAF_N,
	LEAF_FALL_N,
	FINE_ROOT_N,
	COARSE_ROOT_N,
	STEM_N,
	BRANCH_N,

	//3PG MORTALITY FUNCTION
	WS_MAX,                       //Maximum stem mass per tree at 1000 trees/ha
	//LPJ MORTALITY FUNCTION
	AGEMORT,                      //Age probability mortality function

	//NATURAL REGENERATION INDIVIDUAL BIOMASS VALUES FOR SAPLING PASSING TO ADULT
	AVDBH_SAPLING,
	LAI_SAPLING,
	TREE_HEIGHT_SAPLING,
	LEAF_C_SAPLING,
	COARSE_ROOT_C_SAPLING,
	FINE_ROOT_C_SAPLING,
	STEM_C_SAPLING,
	PERC,

	VALUES
};

enum {

	N_TREE,                      /* Numbers of Adult Tree per class */
	N_STUMP,                     /* Number of stumps for coppice per class */
	N_SEED,                      /* Seeds number per class */
	DEAD_STEMS,                  /* Dead Trees per class */
	CUT_TREES,                   /* Cut trees for Management per class */
	N_TREE_SAP,                  /* Numbers of Saplings per class */
	VEG_UNVEG,                   /* vegetative-un vegetative period per class */
	VEG_DAYS,                    /* day of vegetative period for class */
	BUD_BURST_COUNTER,           /* days of budburst per class */
	DAY_FRAC_FOLIAGE_REMOVE,     /* days of leaf fall per class */
	DAY_FRAC_FINE_ROOT_REMOVE,   /* days of "fine root fall" per class */
	DAY_VEG_FOR_LEAF_FALL,       /* days for leaf fall */
	LEAF_FALL_COUNTER            /* counter for leaf fall*/,
	SENESCENCE_DAYONE,  //Sergio's

	COUNTERS
};

//SERGIO
typedef struct {
	double fineroot[MAXTURNTIME];
	double leaves[MAXTURNTIME];
	double coarseRoots[MAXTURNTIME];
	double branch[MAXTURNTIME];
	double stem[MAXTURNTIME];

	int FINERTOVER;
	int COARSERTOVER;
	int STEMTOVER;
	int BRANCHTOVER;
} turnover_t;

typedef struct {

	turnover_t* turnover;
	e_management management;

	char *name;
	int period;            /* period = 0 for adult tree ; 1 for very young tree */
	double value[VALUES];
	int counter[COUNTERS];
	int phenology_phase;

} species_t;

typedef struct
{
	species_t* species;

	int value;
	int species_count;
} age_t;


enum {
	T_LAYER_VALUE_COVER
	, T_LAYER_VALUE_DENSITY
	, T_LAYER_VALUE_CANOPY_COVER
	, T_LAYER_VALUE_LAYER_COVER
	, T_LAYER_VALUE_BASAL_AREA
	, T_LAYER_VALUE_SHORT_WAVE_RADIATION_UPWARD_MJ
	, T_LAYER_VALUE_SHORT_WAVE_RADIATION_UPWARD_W
	, T_LAYER_VALUE_NET_SHORT_WAVE_RADIATION_MJ  
	, T_LAYER_VALUE_NET_SHORT_WAVE_RADIATION_W   
	, T_LAYER_VALUE_LONG_WAVE_RADIATION_UPWARD_MJ
	, T_LAYER_VALUE_LONG_WAVE_RADIATION_UPWARD_W 
	, T_LAYER_VALUE_NET_LONG_WAVE_RADIATION_MJ   
	, T_LAYER_VALUE_NET_LONG_WAVE_RADIATION_W    
	, T_LAYER_VALUE_SHORT_WAVE_ABSORBED          
	, T_LAYER_VALUE_SHORT_WAVE_REFLECTED         
	, T_LAYER_VALUE_LONG_WAVE_ABSORBED           
	, T_LAYER_VALUE_LONG_WAVE_REFLECTED          
	, T_LAYER_VALUE_LONG_WAVE_EMITTED            
	, T_LAYER_VALUE_SOIL_LONG_WAVE_EMITTED       
	, T_LAYER_VALUE_NET_LW_RAD_FOR_SOIL          
	, T_LAYER_VALUE_NET_RAD_FOR_SOIL             
	, T_LAYER_VALUE_SW_RAD_REFL                  
	, T_LAYER_VALUE_SW_RAD_FOR_SOIL_REFL         
	, T_LAYER_VALUE_NET_SW_RAD                   
	, T_LAYER_VALUE_NET_SW_RAD_ABS               
	, T_LAYER_VALUE_NET_SW_RAD_TRANSM            
	, T_LAYER_VALUE_NET_SW_RAD_FOR_SOIL          
	, T_LAYER_VALUE_PAR
	, T_LAYER_VALUE_APAR
	, T_LAYER_VALUE_PAR_TRANSM
	, T_LAYER_VALUE_PAR_REFLECTED
	, T_LAYER_VALUE_PAR_FOR_DOMINATED
	, T_LAYER_VALUE_PAR_FOR_SUBDOMINATED
	, T_LAYER_VALUE_PAR_FOR_SOIL
	, T_LAYER_VALUE_PAR_REFLECTED_SOIL
	, T_LAYER_VALUE_PAR_FOR_ESTABLISHMENT
	, T_LAYER_VALUE_PPFD                         
	, T_LAYER_VALUE_PPFD_ABS                     
	, T_LAYER_VALUE_PPFD_TRANSM                  
	, T_LAYER_VALUE_PPFD_REFLECTED               
	, T_LAYER_VALUE_PPFD_FOR_SOIL
	, T_LAYER_VALUE_PPFD_REFLECTED_SOIL

	, T_LAYER_VALUE_DAILY_GPP
	, T_LAYER_VALUE_MONTHLY_GPP
	, T_LAYER_VALUE_ANNUAL_GPP     //IN G OF C M^2
	, T_LAYER_VALUE_DAILY_NPP_GC
	, T_LAYER_VALUE_MONTHLY_NPP_GC
	, T_LAYER_VALUE_ANNUAL_NPP_GC	//IN G OF C M^2
	, T_LAYER_VALUE_DAILY_NPP_TDM
	, T_LAYER_VALUE_MONTHLY_NPP_TDM
	, T_LAYER_VALUE_ANNUAL_NPP_TDM     //IN TONNES OF DM PER HECTARE
	, T_LAYER_VALUE_DAILY_AUT_RESP
	, T_LAYER_VALUE_MONTHLY_AUT_RESP
	, T_LAYER_VALUE_ANNUAL_AUT_RESP
	, T_LAYER_VALUE_DAILY_AUT_RESP_TC
	, T_LAYER_VALUE_MONTHLY_AUT_RESP_TC
	, T_LAYER_VALUE_ANNUAL_AUT_RESP_TC
	, T_LAYER_VALUE_DAILY_MAINT_RESP
	, T_LAYER_VALUE_MONTHLY_MAINT_RESP
	, T_LAYER_VALUE_ANNUAL_MAINT_RESP
	, T_LAYER_VALUE_DAILY_GROWTH_RESP
	, T_LAYER_VALUE_MONTHLY_GROWTH_RESP
	, T_LAYER_VALUE_ANNUAL_GROWTH_RESP
	, T_LAYER_VALUE_DAILY_R_ECO
	, T_LAYER_VALUE_MONTHLY_R_ECO
	, T_LAYER_VALUE_ANNUAL_R_ECO
	, T_LAYER_VALUE_DAILY_HET_RESP
	, T_LAYER_VALUE_MONTHLY_HET_RESP
	, T_LAYER_VALUE_ANNUAL_HET_RESP
	, T_LAYER_VALUE_DAILY_C_FLUX
	, T_LAYER_VALUE_MONTHLY_C_FLUX
	, T_LAYER_VALUE_ANNUAL_C_FLUX
	, T_LAYER_VALUE_LITTER
	, T_LAYER_VALUE_TER                                                            /*TOTAL ECOSYSTEM RESPIRATION */
	, T_LAYER_VALUE_DAILY_NEE
	, T_LAYER_VALUE_MONTHLY_NEE
	, T_LAYER_VALUE_ANNUAL_NEE
	, T_LAYER_VALUE_DAILY_LEAF_CARBON                                              /* DAILY CARBON ASSIMILATED TO C POOL IN GC/M2/DAY */
	, T_LAYER_VALUE_DAILY_STEM_CARBON                                              /* DAILY CARBON ASSIMILATED TO C POOL IN GC/M2/DAY  */
	, T_LAYER_VALUE_DAILY_FINE_ROOT_CARBON                                         /* DAILY CARBON ASSIMILATED TO C POOL IN GC/M2/DAY */
	, T_LAYER_VALUE_DAILY_COARSE_ROOT_CARBON                                       /* DAILY CARBON ASSIMILATED TO C POOL IN GC/M2/DAY */
	, T_LAYER_VALUE_DAILY_ROOT_CARBON                                              /* DAILY CARBON ASSIMILATED TO C POOL IN GC/M2/DAY */
	, T_LAYER_VALUE_DAILY_BRANCH_CARBON                                            /* DAILY CARBON ASSIMILATED TO C POOL IN GC/M2/DAY */
	, T_LAYER_VALUE_DAILY_RESERVE_CARBON                                           /* DAILY CARBON ASSIMILATED TO C POOL IN GC/M2/DAY */
	, T_LAYER_VALUE_DAILY_LITTER_CARBON                                            /* DAILY CARBON ASSIMILATED TO LITTER C POOL IN GC/M2/DAY */
	, T_LAYER_VALUE_DAILY_FRUIT_CARBON                                             /* DAILY CARBON ASSIMILATED TO FRUIT C POOL IN GC/M2/DAY */
	, T_LAYER_VALUE_DAILY_LEAF_CARBON_TC                                           /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_STEM_CARBON_TC                                           /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_FINE_ROOT_CARBON_TC                                      /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_COARSE_ROOT_CARBON_TC                                    /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_BRANCH_CARBON_TC                                         /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_RESERVE_CARBON_TC                                        /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_ROOT_CARBON_TC                                           /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_LITTER_CARBON_TC                                         /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_FRUIT_CARBON_TC                                          /* DAILY CARBON ASSIMILATED TO C POOL IN TC/CELL/DAY */
	, T_LAYER_VALUE_DAILY_LEAF_DRYMATTER
	, T_LAYER_VALUE_DAILY_STEM_DRYMATTER
	, T_LAYER_VALUE_DAILY_FINE_ROOT_DRYMATTER
	, T_LAYER_VALUE_DAILY_COARSE_ROOT_DRYMATTER
	, T_LAYER_VALUE_DAILY_BRANCH_DRYMATTER
	, T_LAYER_VALUE_DAILY_RESERVE_DRYMATTER
	, T_LAYER_VALUE_DAILY_LEAF_MAINT_RESP
	, T_LAYER_VALUE_DAILY_STEM_MAINT_RESP
	, T_LAYER_VALUE_DAILY_FINE_ROOT_MAINT_RESP
	, T_LAYER_VALUE_DAILY_BRANCH_MAINT_RESP
	, T_LAYER_VALUE_DAILY_COARSE_ROOT_MAINT_RESP
	, T_LAYER_VALUE_DAILY_LEAF_GROWTH_RESP
	, T_LAYER_VALUE_DAILY_STEM_GROWTH_RESP
	, T_LAYER_VALUE_DAILY_FINE_ROOT_GROWTH_RESP
	, T_LAYER_VALUE_DAILY_BRANCH_GROWTH_RESP
	, T_LAYER_VALUE_DAILY_COARSE_ROOT_GROWTH_RESP
	, T_LAYER_VALUE_DAILY_LEAF_AUT_RESP
	, T_LAYER_VALUE_DAILY_STEM_AUT_RESP
	, T_LAYER_VALUE_DAILY_BRANCH_AUT_RESP
	, T_LAYER_VALUE_DAILY_FINE_ROOT_AUT_RESP
	, T_LAYER_VALUE_DAILY_COARSE_ROOT_AUT_RESP
	, T_LAYER_VALUE_DAILY_F_SW
	, T_LAYER_VALUE_DAILY_F_PSI
	, T_LAYER_VALUE_DAILY_F_T
	, T_LAYER_VALUE_DAILY_F_VPD
	, T_LAYER_VALUE_DAILY_LITTERFALL
	, T_LAYER_VALUE_MONTHLY_LITTERFALL
	, T_LAYER_VALUE_ANNUAL_LITTERFALL
	, T_LAYER_VALUE_AV_GPP
	, T_LAYER_VALUE_AV_NPP
	, T_LAYER_VALUE_STAND_AGB
	, T_LAYER_VALUE_STAND_BGB

	, T_LAYER_VALUE_DAILY_C_INT_WATT
	, T_LAYER_VALUE_DAILY_C_TRANSP_WATT
	, T_LAYER_VALUE_DAILY_C_EVAPO_WATT
	, T_LAYER_VALUE_DAILY_C_EVAPOTRANSP_WATT
	, T_LAYER_VALUE_DAILY_C_LATENT_HEAT_FLUX
	, T_LAYER_VALUE_DAILY_LATENT_HEAT_FLUX
	, T_LAYER_VALUE_MONTHLY_LATENT_HEAT_FLUX
	, T_LAYER_VALUE_ANNUAL_LATENT_HEAT_FLUX
	, T_LAYER_VALUE_DAILY_C_SENSIBLE_HEAT_FLUX
	, T_LAYER_VALUE_DAILY_SENSIBLE_HEAT_FLUX
	, T_LAYER_VALUE_MONTHLY_SENSIBLE_HEAT_FLUX
	, T_LAYER_VALUE_ANNUAL_SENSIBLE_HEAT_FLUX

	, T_LAYER_VALUES_COUNT
};

enum {
	T_LAYER_COUNTER_VEG
	, T_LAYER_COUNTER_N_TREE
	, T_LAYER_COUNTER_DAILY
	, T_LAYER_COUNTER_MONTHLY
	, T_LAYER_COUNTER_ANNUAL
	, T_LAYER_COUNTER_DAILY_DEAD_TREE
	, T_LAYER_COUNTER_MONTHLY_DEAD_TREE
	, T_LAYER_COUNTER_ANNUAL_DEAD_TREE

	, T_LAYER_COUNTERS_COUNT
};

typedef struct
{
	int layer_z;
	int layer_n_height_class;  /* number of height class per layer */
	int layer_n_trees;         /* number of trees per layer */
	double layer_density;      /* tree density per layer (n_tree/sizecell) */
	double layer_cover;        /* layer canopy cover per layer */

	double value[T_LAYER_VALUES_COUNT];
	int counter[T_LAYER_COUNTERS_COUNT];
} tree_layer_t;

typedef struct
{
	age_t *ages;
	int ages_count;

	double value;
	int height_z;
	double height_density;      /* tree density per height class (n_tree/sizecell) */
	double height_cover;        /* height class cover per height class (n_tree/sizecell) */

} height_t;

typedef struct
{
	// ALLESSIOR:
	// VC++ 2008 do not allow empty structure
	// TODO: REMOVE
	int foo;
} soil_t;

enum
{
	S_LAYER_VALUE_PREVIOUS_AVAILABLE_SOIL_WATER
	, S_LAYER_VALUE_WATER_BALANCE
	, S_LAYER_VALUE_OLD_WATER_BALANCE
	, S_LAYER_VALUE_SOIL_MOIST_RATIO
	, S_LAYER_VALUE_AV_SOIL_MOIST_RATIO
	, S_LAYER_VALUE_SWC//VOLUMETRIC SOIL WATER CONTENT (%VOL)
	, S_LAYER_VALUE_PSI_SAT//SOIL SATURATED MATRIC POTENTIAL
	, S_LAYER_VALUE_VWC
	, S_LAYER_VALUE_VWC_FC
	, S_LAYER_VALUE_VWC_SAT//SOIL SATURATED VOLUMETRIC WATER CONTENT
	, S_LAYER_VALUE_SOIL_B //SOIL MOISTURE PARAMETER
	, S_LAYER_VALUE_SOILW_SAT //(KGH2O/M2) SOILWATER AT SATURATION
	, S_LAYER_VALUE_SOILW_FC //(KGH2O/M2) SOILWATER AT FIELD CAPACITY
	, S_LAYER_VALUE_DAILY_SOIL_EVAPORATION_WATT
	, S_LAYER_VALUE_DAILY_SOIL_LATENT_HEAT_FLUX
	, S_LAYER_VALUE_DAILY_SOIL_SENSIBLE_HEAT_FLUX

	, S_LAYER_VALUES_COUNT
};

typedef struct {
	soil_t* soils;
	int soils_count;
	double value[S_LAYER_VALUES_COUNT];

	//ALESSIOC ALESSIOR SERGIOS STUFFS SEE IF REMOVE AND USE ARRAYS
	/**************************************************************************************************
	 * 	SOIL PART; DEVELOPMENT OF DAILY SOIL CARBON NITROGEN AND PHOSPHORUS BALANCE
	 * 			for any issue contact Sergio Marconi (sergio.marconi@cmcc.it)
	 *
	 **************************************************************************************************/
	double sph;
	double soilMoisture;
	double soilTemp;
	double ice;
	double soc;
	double doc;
	double rcvl;
	double rcr;
	double rcl;
	double CRB1;
	double CRB2;
	double crhl;
	double crhr;
	double dphum;
	double no3;
	double no2;
	double nh4;
	double nh3;
	//double Eh;
	double co2;
	double day_O2;
	double dcbavai;
	double drcvl;
	double DRCB1, DRCB2;
	double soilDepth;
	double clay_nh4;
	double inert_C;
	double LabP;
	double AdsP;
	//unknown
	double sts,mmm;
	double no, n2o;
	double litco22, litco23;

	double initialOrganicC;
	double waterContent;
	double CEC;
	/*********************************************************************************************/
} soil_layer_t;

typedef struct
{
	height_t* heights;
	int heights_count;

	tree_layer_t* t_layers;
	int t_layers_count;

	soil_layer_t* s_layers;
	int s_layers_count;

	int years_count;
	yos_t *years;

	e_landuse landuse;                                  /* LandUse type */

	int x;                                              /* cell index within the matrix */
	int y;                                              /* cell index within the matrix */

	/* forest structure variables */
	int annual_tree_layer_number;                       /* annual number of tree layer within the cell */

	/* general variables */
	double abscission_daylength;                        /* day length for starting abscission (not used) */
	double av_yearly_daylength;                         /* average yearly daylength for renovation function */
	double av_yearly_par_soil;                          /* average yearly par for soil for renovation function */
	int north;                                          /* northern hemisphere north = 0, south hemisphere south = 1 */

	/* annual met values */
	double annual_tavg;                                 /* annual avg daily avg temperature (°C) */
	double annual_tmin;                                 /* annual avg daily min temperature (°C) */
	double annual_tmax;                                 /* annual avg daily max temperature (°C) */
	double annual_tday;                                 /* annual avg daily daytime temperature (°C) */
	double annual_tnight;                               /* annual avg daily nighttime temperature (°C) */
	double annual_tsoil;                                /* annual avg daily soil temperature (°C) */
	double annual_solar_rad;                            /* annual avg daily solar radiation (short wave) (MJ/m2/day) */
	double annual_precip;                               /* annual cumulate daily precipitation (mm) */
	double annual_vpd;                                  /* annual avg daily vpd (hPa) */

	/*forest structure variables*/	
	int Veg_Counter;                                    /* number of class in vegetative period  in the cell */
	int top_layer;                                      /* index for dominant layer */
	int saplings_counter;                               /* number of class as sapling */
	double cell_cover;                                  /* canopy cover (ratio) */
	int cell_n_trees;                                   /* number of trees */

	int daily_dead_tree;                                /* daily number of dead tree */
	double monthly_dead_tree;                           /* monthly number of dead tree */
	double annual_dead_tree ;                           /* annual number of dead tree */
	double basal_area;                                  /* cumulated basal area at cell level */
	double agb;                                         /* current above ground biomass at cell level (tC/cell) */
	double bgb;                                         /* current below ground biomass at cell level (tC/cell) */

	/* radiation variables */
	/* short wave radiation */
	double sw_rad_down_MJ;                              /* Downward short wave radiation flux (MJ/m2/day) */
	double sw_rad_down_W;                               /* Downward short wave radiation flux (W/m2) */
	double short_wave_radiation_upward_MJ;              /* Upward short wave radiation flux (MJ/m2/day) */
	double short_wave_radiation_upward_W;               /* Upward short wave radiation flux (W/m2) */
	double sw_rad_refl;                                 /* Short Wave radiation reflected (W/m2) */
	double sw_rad_for_soil_refl;                        /* Short Wave radiation reflected from soil level (W/m2) */
	double sw_rad_abs;                                  /* Short Wave radiation absorbed (W/m2) */
	double sw_rad_transm;                               /* Short Wave radiation transmitted (W/m2) */
	double sw_rad_for_soil;                             /* Short Wave radiation incoming to soil level (W/m2) */

	/* long wave radiation */
	double long_wave_radiation_upward_MJ;               /* Upward long wave radiation flux (MJ/m2/day) */
	double long_wave_radiation_upward_W;                /* Upward long wave radiation flux (W/m2) */
	double net_long_wave_radiation_MJ;                  /* Net long wave radiation flux (MJ/m2/day) */
	double net_long_wave_radiation_W;                   /* Net long wave radiation flux (W/m2) */
	double long_wave_absorbed;                          /* Total absorbed long wave wave radiation flux (W/m2) */
	double long_wave_reflected;                         /* Total reflected long wave radiation flux (W/m2) */
	double long_wave_emitted;                           /* Total emitted long wave radiation flux (W/m2) */
	double soil_long_wave_emitted;                      /* Soil emitted long wave radiation flux (W/m2) */
	double net_lw_rad_for_soil;                         /* Net Long Wave radiation to soil level (W/m2) */
	double net_rad_for_soil;                            /* Net radiation to soil level (W/m2) */

	/* PAR radiation */
	double par;                                         /* incoming PAR at cell level (molPAR/m2/day) */
	double apar;                                        /* cumulated absorbed PAR at cell level (molPAR/m2/day) */
	double par_transm;                                  /* cumulated transmitted PAR at cell level (molPAR/m2/day) */
	double par_refl;                                    /* cumulated reflected PAR at cell level (molPAR/m2/day) */
	double par_for_soil;                                /* available PAR for soil (molPAR/m2/day) */
	double par_refl_soil;                               /* reflected PAR by the soil (molPAR/m2/day) */
	double par_for_establishment;                       /* available PAR for establishment (molPAR/m2/day) */

	/* PPFD radiation */
	double ppfd;                                        /* Photosynthetic photon flux density (umol/m2/sec) */
	double ppfd_abs;                                    /* Absorbed Photosynthetic photon flux density (umol/m2/sec) */
	double ppfd_transm;                                 /* Transmitted Photosynthetic photon flux density (umol/m2/sec) */
	double ppfd_refl;                                   /* Reflected Photosynthetic photon flux density (umol/m2/sec) */
	double ppfd_for_soil;                               /* PPFD available for soil (umol/m2/sec) */
	double ppfd_refl_soil;                              /* PPFD reflected by the soil (umol/m2/day) */

	double canopy_temp;                                 /* daily canopy temperature (°C) */
	double canopy_temp_k;                               /* daily canopy temperature (K) */
	double canopy_temp_diff;                            /* daily canopy temperature (K) */

	/*carbon variables*/
	double daily_gpp, monthly_gpp, annual_gpp;                            /* daily, monthly and annual GPP at cell level (gC/m2/ ) */
	double daily_npp_gC, monthly_npp_gC, annual_npp_gC;                   /* daily, monthly and annual NPP at cell level (gC/m2/ ) */
	double daily_npp_tC, monthly_npp_tC, annual_npp_tC;                   /* daily, monthly and annual NPP at cell level (tC/cell/ ) */
	double daily_npp_tDM, monthly_npp_tDM, annual_npp_tDM;                /* daily, monthly and annual NPP at cell level (tDM/cell/ ) */
	double daily_aut_resp, monthly_aut_resp, annual_aut_resp;             /* daily, monthly and annual aut resp at cell level (gC/m2/ ) */
	double daily_aut_resp_tC, monthly_aut_resp_tC, annual_aut_resp_tC;    /* daily, monthly and annual aut resp at cell level (tC/cell/ ) */
	double daily_maint_resp, monthly_maint_resp, annual_maint_resp;       /* daily, monthly and annual maint resp at cell level (gC/m2/ ) */
	double daily_growth_resp, monthly_growth_resp, annual_growth_resp;    /* daily, monthly and annual growth resp at cell level (gC/m2/ ) */
	double daily_r_eco, monthly_r_eco, annual_r_eco;                      /* daily, monthly and annual ecosystem resp at cell level (gC/m2/ ) */
	double daily_het_resp, monthly_het_resp, annual_het_resp;             /* daily, monthly and annual heterotrophic resp at cell level (gC/m2/ ) */
	double daily_C_flux, monthly_C_flux, annual_C_flux;                   /* daily, monthly and annual carbon fluxes at cell level (gC/m2/ ) */
	double daily_nee, monthly_nee, annual_nee;                            /* daily, monthly and annual NEE at cell level (gC/m2/ ) */
	double daily_leaf_carbon;                                             /* daily carbon assimilated to c pool at cell level (gC/m2/day) */
	double daily_stem_carbon;                                             /* daily carbon assimilated to c pool at cell level (gC/m2/day)  */
	double daily_fine_root_carbon;                                        /* daily carbon assimilated to c pool at cell level (gC/m2/day) */
	double daily_coarse_root_carbon;                                      /* daily carbon assimilated to c pool at cell level (gC/m2/day) */
	double daily_root_carbon;                                             /* daily carbon assimilated to c pool at cell level (gC/m2/day) */
	double daily_branch_carbon;                                           /* daily carbon assimilated to c pool at cell level (gC/m2/day) */
	double daily_reserve_carbon;                                          /* daily carbon assimilated to c pool at cell level (gC/m2/day) */
	double daily_litter_carbon;                                           /* daily carbon assimilated to litter c pool at cell level (gC/m2/day) */
	double daily_fruit_carbon;                                            /* daily carbon assimilated to fruit c pool at cell level (gC/m2/day) */
	double daily_leaf_carbon_tC;                                          /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_stem_carbon_tC;                                          /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_fine_root_carbon_tC;                                     /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_coarse_root_carbon_tC;                                   /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_branch_carbon_tC;                                        /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_reserve_carbon_tC;                                       /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_root_carbon_tC;                                          /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_litter_carbon_tC;                                        /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_fruit_carbon_tC;                                         /* daily carbon assimilated to c pool at cell level (tC/cell/day) */
	double daily_leaf_maint_resp;                                         /* daily leaf maint resp at cell level (gC/m2/day) */
	double daily_stem_maint_resp;                                         /* daily stem maint resp at cell level (gC/m2/day) */
	double daily_fine_root_maint_resp;                                    /* daily fine root maint resp at cell level (gC/m2/day) */
	double daily_branch_maint_resp;                                       /* daily branch and bark maint resp at cell level (gC/m2/day) */
	double daily_coarse_root_maint_resp;                                  /* daily coarse root maint resp at cell level (gC/m2/day) */
	double daily_leaf_growth_resp;                                        /* daily leaf growth resp at cell level (gC/m2/day) */
	double daily_stem_growth_resp;                                        /* daily stem growth resp at cell level (gC/m2/day) */
	double daily_fine_root_growth_resp;                                   /* daily fine root growth resp at cell level (gC/m2/day) */
	double daily_branch_growth_resp;                                      /* daily branch and bark growth resp at cell level (gC/m2/day) */
	double daily_coarse_root_growth_resp;                                 /* daily coarse root growth resp at cell level (gC/m2/day) */
	double daily_leaf_aut_resp;                                           /* daily leaf aut resp at cell level (gC/m2/day) */
	double daily_stem_aut_resp;                                           /* daily stem aut resp at cell level (gC/m2/day) */
	double daily_branch_aut_resp;                                         /* daily branch and bark aut resp at cell level (gC/m2/day) */
	double daily_fine_root_aut_resp;                                      /* daily fine root aut resp at cell level (gC/m2/day) */
	double daily_coarse_root_aut_resp;                                    /* daily coarse root aut resp at cell level (gC/m2/day) */

	/*water variables*/
	double wilting_point;                                                 /* volumetric water content at wilting point (mm/m2) */
	double field_capacity;                                                /* volumetric water content at field capacity (mm/m2) */
	double sat_hydr_conduct;                                              /* saturated hydraulic conductivity (mm/m2) */
	double bulk_density;                                                  /* soil bulk density (g/cm3) */

	double asw;                                                           /* current available soil water (mm/volume) */
	double old_asw;                                                       /* old available soil water (mm/volume) */
	double max_asw_fc;                                                    /* max available soil water at field capacity mmKgH2O/m3*/
	double max_asw_sat;                                                   /* max available soil water at field capacity mmKgH2O/m3*/
	double psi;                                                           /* water potential of soil and leaves (MPa) */
	double soil_moist_ratio;                                              /* soil moisture ratio */
	double vwc;                                                           /* volumetric water content (mm/m2) */
	double vwc_fc;                                                        /* volumetric water content at field capacity (mm/m2) */
	double vwc_sat;                                                       /* volumetric water content at saturation (mm/m2) */
	double psi_sat;                                                       /* soil matric potential (MPa) */
	double soil_b;                                                        /* slope of log(psi) vs log(rwc) (DIM) */
	double soilw_fc;                                                      /* soil water at field capacity (kgH2O/m2) */
	double soilw_sat;                                                     /* soil water at saturation (kgH2O/m2) */
	double swc;

	double rain;
	double snow;
	int days_since_rain;                                                  /* consecutive days without rain */
	double rain_intercepted;                                              /* current amount of intercted rain (mm/m2) */
	double rain_to_soil;                                                  /* current rain to soil (mm/m2) */
	double snow_to_soil;                                                  /* current snow to soil (mm/m2) */
	double vapour_to_atmosphere;                                          /* current water flux water to atmosphere (mm/m2) */
	double snow_pack;                                                     /* current amount of snow (Kg/m2) */
	double snow_melt;                                                     /* current amount of melted snow (mm/m2) */
	double snow_subl;                                                     /* current amount of sublimated snow (mm/m2) */
	double old_snow_pack;                                                 /* old amount of snow (Kg/m2) */
	double out_flow;                                                      /* current amount of water outflow (mm/m2) */
	double daily_c_rain_int, monthly_c_rain_int, annual_c_rain_int;       /* daily, monthly and canopy rain interception at cell level (mm/m2/ ) */
	double daily_c_snow_int, monthly_c_snow_int, annual_c_snow_int;       /* daily, monthly and canopy snow interception at cell level (mm/m2/ ) */
	double daily_c_transp, monthly_c_transp, annual_c_transp;             /* daily, monthly and canopy transpiration at cell level (mm/m2/ ) */
	double daily_c_evapo, monthly_c_evapo, annual_c_evapo;                /* daily, monthly and canopy evaporation at cell level (mm/m2/ ) */
	double daily_c_water_stored, monthly_c_water_stored, annual_c_water_stored;/* daily, monthly and canopy water stored at cell level (mm/m2/ ) */
	double daily_c_evapotransp, monthly_c_evapotransp, annual_c_evapotransp;/* daily, monthly and canopy evapotranspiration at cell level (mm/m2/ ) */
	double daily_soil_evapo, monthly_soil_evapo, annual_soil_evapo;       /* daily, monthly and soil evaporation at cell level (mm/m2/ ) */
	double daily_et, monthly_et, annual_et;                               /* daily, monthly and evapotranspiration at cell level (mm/m2/ ) */
	double daily_tot_w_flux, monthly_tot_w_flux, annual_tot_w_flux;       /* daily, monthly and annual water fluxes at cell level (mm/m2/ ) */
	double old_daily_c_water_stored;

	double daily_soil_evaporation_watt;                                   /* current daily soil evaporation in watt at cell level (W/m2) */
	double daily_soil_latent_heat_flux;                                   /* current daily soil latent heat flux  at cell level (W/m2) */
	double daily_soil_sensible_heat_flux;                                 /* current daily soil sensible heat flux at cell level (W/m2) */

	/*energy balance*/
	double daily_c_transp_watt;                                           /* daily canopy transpiration at cell level (W/m2) */
	double daily_c_evapo_watt;                                            /* daily canopy evaporation at cell level (W/m2) */
	double daily_c_evapotransp_watt;                                      /* daily canopy evapotranspiration at cell level (W/m2) */
	double daily_c_latent_heat_flux;                                      /* daily canopy latent heat flux at cell level  (W/m2) */
	double daily_latent_heat_flux, monthly_latent_heat_flux, annual_latent_heat_flux;/* daily, monthly and annual latent heat flux at cell level (W/m2) */
	double daily_c_sensible_heat_flux;                                    /* current daily canopy sensible heat flux (W/m2) */
	double daily_sensible_heat_flux, monthly_sensible_heat_flux, annual_sensible_heat_flux;/* daily, monthly and annual sensible heat flux at cell level (W/m2) */

	double annual_peak_lai[10];

	/**************************************************************************************************
	 * 	SOIL PART; DEVELOPMENT OF DAILY SOIL CARBON NITROGEN AND PHOSPHORUS BALANCE
	 * 			for any issue contact Sergio Marconi (sergio.marconi@cmcc.it)
	 *
	 **************************************************************************************************/
	//daily soil variables; 3D-CMCC-SGM
	double soil_ph;
	//sergio; not clear till concepts and dynamics; what's that and how to replace&introcude in 3d-cmcc
	double till_fact, tilq;
	//dC
	double leafLittering, fineRootLittering,stemBrancLittering,stemLittering, coarseRootLittering;
	double leaflitN, fineRootlitN,stemBranclitN,stemlitN, coarseRootlitN;
	double day_C_mine;
	double day_N_mine;
	double day_N_assim;
	double day_soil_nh3;
	double day_clay_N, day_clayn;
	double wrcvl, wrcl, wrcr, wcrb, wcrh,whumus;
	double wtcavai,w_no2,w_no3,w_n2o, w_nh4, w_nh3, wpool_no;
	double wsoc;
	double leach_u, day_no2, flux_no2;
	//double wFreezedoc;
	double End_SON;
	double runoff_N;
	double previousSoilT;
	double soilSurfaceT;
	double temp_avet;
	double yr_avet;
	double base_clay_N, max_clay_N;
	double AddC, AddCN, AddC1, AddC2, AddC3;

	//potentially already existent
	int doy, dos;

	//todo to be removed used just to evaluate total biomass fluctuations in the several different compartments
	double leafBiomass, stemBiomass, fineRootBiomass, coarseRootBiomass,stemBranchBiomass;
	double vpSat[365], maxVpSat;

	/*
		those variables are referred to cell, they must be same for each layers
	 */

} cell_t;

typedef struct {
	cell_t *cells;
	int cells_count;
	int x_cells_count;
	int y_cells_count;
} matrix_t;

matrix_t* matrix_create(const char* const filename);
void matrix_free(matrix_t *m);
void simulation_summary(const matrix_t* const m);
void site_summary(const matrix_t* const m);
void topo_summary(const matrix_t* const m);
void soil_summary(const matrix_t* const m, const cell_t* const cell);
void forest_summary(const matrix_t* const m, const int cell, const int day, const int month, const int year);

#endif /* MATRIX_H_ */
