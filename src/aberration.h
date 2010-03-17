/*
 * aberration.h - Declarations for calculating annual aberration
 * Copyright (C) 2010 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ABERRATION_H_
#define _ABERRATION_H_

#include <julian_date.h>
#include <coordinates.h>

/*
 * Used internally to store the terms in the trigonometric aberration series
 */
struct aberration_term1_1 {
	short l_ea;
	int x_sin;
	int x_sin_t;
	int x_cos;
	int x_cos_t;
	int x_cos_t2;
	int y_sin;
	int y_sin_t;
	int y_sin_t2;
	int y_cos;
	int y_cos_t;
	int z_sin;
	int z_sin_t;
	int z_sin_t2;
	int z_cos;
	int z_cos_t;
};

/*
 * Used internally to store the terms in the trigonometric aberration series
 */
struct aberration_term1_2 {
	short l_me;
	short l_ve;
	short l_ea;
	short l_ma;
	short l_ju;
	short l_sa;
	short x_sin;
	short x_cos;
	short y_sin;
	short y_cos;
	short z_sin;
	short z_cos;
};

/*
 * Used internally to store the terms in the trigonometric aberration series
 */
struct aberration_term2 {
	short l_ve;
	short l_ea;
	short l_ju;
	short l_sa;
	short l_ur;
	short l_ne;
	short x_sin;
	short x_cos;
	short y_sin;
	short y_cos;
	short z_sin;
	short z_cos;
};

/*
 * Used internally to store the terms in the trigonometric aberration series
 */
struct aberration_term3 {
	short w;
	short d;
	short lp;
	short l;
	short f;
	short x_sin;
	short y_cos;
	short z_cos;
};

void aberration_earth_velocity(struct julian_date *tdb,
			struct rectangular_coordinates *vel);

void annual_aberration(struct julian_date *tdb, struct equatorial_coordinates *e,
		double *d_ra, double *d_dec);

#endif
