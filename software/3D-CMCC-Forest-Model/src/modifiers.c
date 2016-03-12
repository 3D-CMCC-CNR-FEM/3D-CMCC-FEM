/*modifiers.c*/

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "constants.h"



void Daily_modifiers (SPECIES *const s,  AGE *const a, CELL *const c, const MET_DATA *const met, int month, int day,  int z, int management, int height )
{
	double RelAge;
	double vwc; //soil volumetric water content
	double psi;  //soil matric potential

	/*variables for CO2 modifier computation*/
	double Km;	//affinity coefficients  temperature dependent according to Arrhenius relationship
	double Ea1 = 59.4; //KJ mol^-1
	double A1 = 2.419 * pow(10,13);
	double Ea2 = 109.6;	//KJ mol^-1
	double A2 = 1.976 * pow(10,22);
	double KO;	//inibition constant for 02
	double Ea0 = 13913.5;	//KJ mol^-1
	double A0 = 8240;
	double tao;	// CO2/O2  specifity ratio
	double Eatao = -42869.9;
	double Atao = 7.87 * pow(10,-5);


	Log("\nDAILY_MODIFIERS\n\n");

	/*CO2 MODIFIER FROM C-FIX*/

	if (met[month].d[day].tavg >= 15)
	{
		Km = A1 * exp (-Ea1/(R*TempAbs));
	}
	else
	{
		Km = A2 * exp (-Ea2/(R*TempAbs));
	}
	KO = A0 * exp (-Ea0/(R*TempAbs));

	tao = Atao * exp (-Eatao/(R*TempAbs));

	s->value[F_CO2] = ((site->co2Conc/(O2CONC/(2*tao))) /
			((refCO2CONC/(O2CONC/(2*tao)))) * ((Km*(1+(O2CONC/KO))+refCO2CONC)/(Km*(1+(O2CONC/KO))+refCO2CONC)));
	Log("F_CO2 modifier (NOT_USED)= %g\n", s->value[F_CO2]);



	//LIGHT MODIFIER (Following Makela et al , 2008, Peltioniemi_etal_2012)
	//FIXME chose which type of light use and differentiate for different layers
	//following Nolè should be used apar
	//following Peltioniemi should be used par

	if ( c->heights[height].z == c->top_layer )
		{


		if (s->value[GAMMA_LIGHT] != -9999)
		{
			s->value[F_LIGHT]= 1.0/ ((s->value[GAMMA_LIGHT]* s->value[APAR]) +1.0);
		}
		 else
		 {
			 s->value[F_LIGHT]= 1.0;
		 }
		Log("FLight (NOT USED)= %g\n", s->value[F_LIGHT]);
	}


	/*TEMPERATURE MODIFIER*/

	//Log("--Temperature Average %f °C\n", met[month].d[day].tavg);
	//Log("--Daylight Average Temperature  %f °C\n", met[month].d[day].tday);
	//Log("--Optimum temperature for species %s = %f °C\n", s->name, s->value[GROWTHTOPT]);
	//Log("--Maximum temperature for species %s = %f °C\n", s->name, s->value[GROWTHTMAX]);
	//Log("--Minimum temperature for species %s = %f °C\n", s->name, s->value[GROWTHTMIN]);

	if (met[month].d[day].tday == NO_DATA)
	{
		if ((met[month].d[day].tavg <= s->value[GROWTHTMIN]) || (met[month].d[day].tavg >= s->value[GROWTHTMAX]))
		{
			s->value[F_T] = 0;
			Log("F_T = 0 \n");
		}
		else
		{
			s->value[F_T] = ((met[month].d[day].tavg - s->value[GROWTHTMIN]) / (s->value[GROWTHTOPT] - s->value[GROWTHTMIN])) * pow(((s->value[GROWTHTMAX] - met[month].d[day].tavg) / (s->value[GROWTHTMAX] - s->value[GROWTHTOPT])),
					((s->value[GROWTHTMAX] - s->value[GROWTHTOPT]) / (s->value[GROWTHTOPT] - s->value[GROWTHTMIN])));
		}
	}
	else
	{
		if ((met[month].d[day].tday <= s->value[GROWTHTMIN]) || (met[month].d[day].tday >= s->value[GROWTHTMAX]))
		{
			Log("tday < o > GROWTHTMIN o GROWTHTMAX\n");
			s->value[F_T] = 0;
			Log("F_T = 0 \n");
		}
		else
		{
			s->value[F_T] = ((met[month].d[day].tday - s->value[GROWTHTMIN]) / (s->value[GROWTHTOPT] - s->value[GROWTHTMIN])) * pow(((s->value[GROWTHTMAX] - met[month].d[day].tday) / (s->value[GROWTHTMAX] - s->value[GROWTHTOPT])),
					((s->value[GROWTHTMAX] - s->value[GROWTHTOPT]) / (s->value[GROWTHTOPT] - s->value[GROWTHTMIN])));
		}
	}
	Log("fT = %f\n", s->value[F_T]);

	if (s->value[F_T] > 1)
	{
		Log("ATTENTION fT EXCEEDS 1 \n");
		s->value[F_T] = 1;
	}

	c->daily_f_t = s->value[F_T];


	//average yearly f_vpd modifiers
	s->value[AVERAGE_F_T] += s->value[F_T];



	/*FROST MODIFIER*/
	if(met[month].d[day].tday < s->value[GROWTHTMIN])
	{
		s->value[F_FROST] = 0;
		Log("fFROST - Frost modifier = %f\n", s->value[F_FROST]);
	}
	else
	{
		s->value[F_FROST] = 1;
		Log("fFROST - Frost modifier = %f\n", s->value[F_FROST]);
	}


	/*VPD MODIFIER*/

	//Log("--RH = %f %%\n", met[month].rh);
	//Log("--vpd = %f mbar", vpd);

	//The input VPD data is in KPa
	//if the VPD is in KPa
	//Convert to mbar
	//1 Kpa = 10 mbar
	//1 hPa = 1 mbar
	//s->value[F_VPD] = exp (- s->value[COEFFCOND] * vpd) * 10);
	//convert also COEFFCOND multiply it for
	s->value[F_VPD] = exp (- s->value[COEFFCOND] * met[month].d[day].vpd);
	c->daily_f_vpd = s->value[F_VPD];
	Log("fVPD = %f\n", s->value[F_VPD]);

	//average yearly f_vpd modifiers
	s->value[AVERAGE_F_VPD] += s->value[F_VPD];


	//look the version 1 for 3pg vpd modifier version

	/*AGE MODIFIER*/

	if (a->value != 0)
	{
		if (management == T)
		{
			//for TIMBER
			//AGE FOR TIMBER IS THE EFFECTIVE AGE
			RelAge = (double)a->value / s->value[MAXAGE];
			s->value[F_AGE] = ( 1 / ( 1 + pow ((RelAge / (double)s->value[RAGE]), (double)s->value[NAGE])));
			//Log("--Rel Age = %f years\n", RelAge);
			//Log("--Age = %d years\n", a->value);
			Log("fAge = %f\n", s->value[F_AGE]);
		}
		else
		{
			//for SHOOTS
			//AGE FOR COPPICE IS THE AGE FROM THE COPPICING
			RelAge = (double)a->value / s->value[MAXAGE_S];
			s->value[F_AGE] = ( 1 / ( 1 + pow ((RelAge / (double)s->value[RAGE_S]), (double)s->value[NAGE_S])));
			//Log("--Rel Age = %f years\n", RelAge);
			//Log("--Age = %d years\n", a->value);
			Log("fAge = %f\n", s->value[F_AGE]);

		}
	}
	else
	{
		s->value[F_AGE] = 1;
		Log("no data for age F_AGE = 1\n");
	}

	/*SOIL NUTRIENT MODIFIER*/
	s->value[F_NUTR] = 1.0 - ( 1.0- site->fn0)  * pow ((1.0 - site->fr), site->fnn);
	Log("fNutr = %f\n", s->value[F_NUTR]);

	/*SOIL NUTRIENT MODIFIER*/
	//rev 16 May 2012
	//Log("Soil Nitrogen Content = %f g m^-2 \n", site->sN);

	/*SOIL WATER MODIFIER*/
	//FIXME CHECK IT
	Log("ASW = %f\n", c->asw);
	Log("MIN ASW = %f\n", c->max_asw * site->min_frac_maxasw);
	Log("moist ratio = %f\n", c->soil_moist_ratio);


	s->value[F_SW] = 1.0 / (1.0 + pow(((1.0 - c->soil_moist_ratio) / s->value[SWCONST]), s->value[SWPOWER]));
	if ( s->value[F_SW] > 1  )
	{
		Log("PROBLEM IN fSW !!!!!!!!!!\n");
		s->value[F_SW] = 1;
		Log("fSW - Soil Water modifier layer %d = %f\n", z,  s->value[F_SW]);
	}
	else
	{
		Log("fSW - Soil Water modifier layer %d = %f\n", z,  s->value[F_SW]);
	}



	/*SOIL MATRIC POTENTIAL*/

	//**********************************
	//USABLE ONLY FOR DAILY SIMULATION
	//**********************************

	/* convert kg/m2 or mm  --> m3/m2 --> m3/m3 */
	//100 mm H20 m^-2 = 100 kg H20 m^-2
	Log("available soil water %f mm\n", c->asw);

	/* (DIM) volumetric water content */
	vwc = c->asw / (1000.0 * (site->soil_depth/100));
	Log("volumetric available soil water  = %f (DIM)\n", vwc);
	Log ("vwc_sat = %f (DIM)\n", c->vwc_sat);
	Log ("vwc/vwc_sat = %f \n", vwc / c->vwc_sat);

	/* calculate psi */
	//todo controllare vwc è l'unica variabile che può far variare psi fare delle prove su excel e vedere a quanto dovrebbe essere per avere un valore compreso tra OPEN e CLOSE
	/* (MPa) water potential of soil and leaves */
	psi = c->psi_sat * pow((vwc/c->vwc_sat), c->soil_b);
	Log ("PSI BIOME = %f (MPa)\n", psi);
	Log ("PSI_SAT BIOME = %f (MPa)\n", c->psi_sat);


	if (psi > s->value[SWPOPEN]) /*no water stress*/
	{
		s->value[F_PSI] = 1;
	}
	else if (psi <= s->value[SWPCLOSE]) /* full water stress */
	{
		s->value[F_PSI] = 0;
	}
	else /* partial water stress */
	{
		s->value[F_PSI] = (s->value[SWPCLOSE] - psi)/(s->value[SWPCLOSE] - s->value[SWPOPEN]);
	}
	Log("F_PSI = %f\n", s->value[F_PSI]);

	c->daily_f_psi = s->value[F_PSI];


	//average yearly f_sw modifiers
	s->value[AVERAGE_F_SW] += s->value[F_SW];

	/*PHYSIOLOGICAL MODIFIER*/
	s->value[PHYS_MOD]= Minimum(s->value[F_VPD], s->value[F_SW]) * s->value[F_AGE];
	Log("PhysMod = %f\n", s->value[PHYS_MOD]);
	if (s->value[F_VPD] < s->value[F_SW])
	{
		Log("PHYSMOD uses F_VPD * F_AGE\n");
	}
	else
	{
		Log("PHYSMOD uses F_SW * F_AGE\n");
	}

	s->value[YEARLY_PHYS_MOD] += s->value[PHYS_MOD];
	//Log("Yearly Physmod = %f\n", s->value[YEARLY_PHYS_MOD]);


	/*SOIL DROUGHT MODIFIER*/
	//(see Duursma et al., 2008)rev_Angelo
	/*

	//to put in species.txt
	//numbers are not real just used for compile!!!!!!!!
	double leaf_res = 1; //leaf specific plant hydraulic resistance
	double min_leaf_pot = 1; //minimum leaf water potential


	//to put in site.txt ?????
	double soil_res = 1; //soil hydraulic resistance
	double psi0 = 2; //dry soil water potential in MPa
	double soil_coeff = 1; //empirical soil coefficient

	//soil average dimension particle
	//value are averaged from limits in site.txt
	double clay_dim = 0.001; //clay avg dimension of particle
	double silt_dim =  0.026;//silt avg dimension of particle
	double sand_dim =  1.025;//sand avg dimension of particle





    double bulk_pot; //bulk soil water potential
    double asw_vol; //available soil water in volume
    double eq;
	double eq1;
	double eq2;
	double sat_soil_water_cont; //saturated soil water content (m^3 m^-3)
	double soil_avg_dim; //soil mean particle diameter in mm
	double sigma_g; //geometric standard deviation in particle size distribution (mm)
	double pentry_temp; //soil entry water potential (MPa)
	double pentry; //correction for bulk density effects
	double bsl; //coefficient in soil water release curve (-)
	double soil_water_pot_sat; //soil water potential at saturation
	double sat_conduct; //saturated conductivity
	double specific_soil_cond; //specific soil hydraulic conductance
	double leaf_specific_soil_cond;


	//compute soil hydraulic characteristics from soil granulometry
	//from model Hydrall
	eq1 = (site->clay_perc * log(clay_dim)) + (site->silt_perc * log(silt_dim)) + (site->sand_perc * log(sand_dim));
	Log("eq1 = %f\n", eq1);

	//soil mean particle diameter in mm
	soil_avg_dim = exp(eq1);
	Log("soil_avg_dim = %f\n", soil_avg_dim);


    eq2 = sqrt ((pow ((site->clay_perc * log(clay_dim)),2)) + (pow ((site->sand_perc * log(sand_dim)),2)) + (pow ((site->silt_perc * log(silt_dim)),2)));
    Log("eq2 = %f\n", eq2);

    //geometric standard deviation in particle size distribution (mm)
    sigma_g = exp(eq2);
    Log("sigma_g = %f\n", sigma_g);

    //soil entry water potential (MPa)
    pentry_temp = -0.5 / sqrt(soil_avg_dim)/1000;
    Log("pentry_temp = %f\n", pentry_temp);
    //correction for bulk density effects with dens = 1.49 g/cm^3
    pentry = pentry_temp * pow ((site->bulk_dens / 1.3), (0.67 * bsl));
    Log("pentry = %f\n", pentry);

    bsl = -2 * (pentry * 1000) + 0.2 * sigma_g;
    Log("bsl = %f\n", bsl);

    //saturated soil water content
    sat_soil_water_cont= 1.0 - (site->bulk_dens/2.56);
    Log("soil water content at saturation = %f\n", sat_soil_water_cont);

    eq = pentry * pow ((sat_soil_water_cont / c->soil_moist_ratio), bsl);
    Log("eq = %f\n", eq);

    //compute bulk soil water potential
    //for psi see Magani xls
    bulk_pot = Maximum (eq, min_leaf_pot);
    Log("bulk soil water potential = %f\n", bulk_pot);

    //compute leaf-specific soil hydraulic conductance
	leaf_specific_soil_cond = sat_conduct * pow ((soil_water_pot_sat / bulk_pot), (2 + (3 / soil_coeff)));
	Log("leaf-specific soil hydraulic conductance = %f\n", leaf_specific_soil_cond);


	s->value[F_DROUGHT] = (leaf_res * (bulk_pot - min_leaf_pot)) / (- min_leaf_pot * ((leaf_res + soil_res) * bulk_pot));
	Log("F_DROUGHT = %f\n", s->value[F_DROUGHT]);
	 */

	Log("-------------------\n");
}

