/*
 * orbital_elements.h - Declarations for orbital element support
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

#ifndef _ORBITAL_ELEMENTS_H_
#define _ORBITAL_ELEMENTS_H_

#include <kepler.h>
#include <julian_date.h>
#include <coordinates.h>

#define PARABOLIC_ORBIT			1.00	

#define KEPLER_MAX_ITER			10
#define KEPLER_PRECISION		1e-14

/*
 * Used wherever the osculating orbital elements of a celestial body are
 * expected or returned.
 *
 * These values may be obtained from the circulars of the Minor Planet 
 * Center, the Astronomical Almanac or other publications. See mpc_file.c
 * for some ways in which this structure is used.
 */
struct orbital_elements {
	struct julian_date epoch; /* Epoch of osculation. */

	double mean_anomaly; /* Mean anomaly in radians at epoch. */
	double daily_motion; /* Daily motion in radians/day. */

	double perihelion; /* Perihelion distance in AU. */
	double eccentricity; /* Eccentricity of the orbit. */

	/* The values for the orbit's orientation must be referred to J2000. */
	double arg_perihelion; /* Argument of the perihelion in radians. */
	double lon_asc_node; /* Longitude of the ascending node in radians. */
	double inclination; /* Inclination of the orbit in radians. */
};

/*
 * May be set to limit the number of iterations in the solution of Kepler's
 * equations. The default value is KEPLER_MAX_ITER.
 */
extern u_short kepler_equation_max_iters;

/*
 * May be set to specify the precision expected in the solution Kepler's 
 * equations. The default value is KEPLER_PRECISION.
 */
extern double kepler_equation_precision;

#ifdef _DEBUG
/*
 * Set to the number of iterations required for the solution of Kepler's
 * equations in the most recent call to the relevant functions.
 */
extern u_short kepler_equation_iters;
#endif

int elements_to_ephemeris(struct julian_date *tt, struct orbital_elements *elt,
			struct rectangular_coordinates *eph);

int eccentric_anomaly(double mean_ano, double ecc, double *ecc_ano);

int hyperbolic_anomaly(double mean_ano, double ecc, double *hyp_ano);

#endif
