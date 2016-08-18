/* phenology.h */
#ifndef PHENOLOGY_H_
#define PHENOLOGY_H_

#include "matrix.h"

void phenology(cell_t *const c, const int layer, const int height, const int age, const int species, const meteo_daily_t *const meteo_daily, const int month);

#endif /* PHENOLOGY_H_ */