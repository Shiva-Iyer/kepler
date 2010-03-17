/*
 * aberration.c - Implementation of Ron-Vondrak theory of aberration
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

/*
 * Implements the algorithm developed by C. Ron & J. Vondrak to calculate
 * the annual aberration using expansions of trigonometric series.
 *
 * Reference : <http://adsabs.harvard.edu/full/1986BAICz..37...96R>
 */

#include <math.h>
#include <memory.h>
#include <fund_args.h>
#include <aberration.h>

/**
 * Terms for velocity components of the EMB's heliocentric motion #1
 **/
struct aberration_term1_1 aberration_series1_1[] = {
	{1,-1719919,-2,-25,0,0,25,-13,-1,1578094,156,10,32,1,684187,-358},
	{2,6434,141,28007,-107,-1,25697,-95,-1,-5904,-130,11141,-48,0,-2559,-55},
	{3,486,-5,-236,-4,0,-216,-4,0,-446,5,-94,-2,0,-193,2},
};

/**
 * Terms for velocity components of the EMB's heliocentric motion #2
 **/
struct aberration_term1_2 aberration_series1_2[] = {
	{0,0,2,0,-1,0,31,1,1,-28,0,-12},
	{0,0,3,-8,3,0,8,-28,25,8,11,3},
	{0,0,5,-8,3,0,8,-28,-25,-8,-11,-3},
	{0,1,0,0,0,-25,0,0,23,0,10},
	{0,2,-1,0,0,0,21,0,0,-19,0,-8},
	{0,0,1,0,-2,0,16,0,0,15,1,7},
	{0,0,1,0,1,0,11,-1,-1,-10,-1,-5},
	{0,2,-2,0,0,0,0,-11,-10,0,-4,0},
	{0,0,1,0,-1,0,-11,-2,-2,9,-1,4},
	{0,0,4,0,0,0,-7,-8,-8,6,-3,3},
	{0,0,3,0,-2,0,-10,0,0,9,0,4},
	{0,1,-2,0,0,0,-9,0,0,-9,0,-4},
	{0,2,-3,0,0,0,-9,0,0,-8,0,-4},
	{0,2,-3,0,0,0,0,-9,8,0,3,0},
	{0,0,3,-2,0,0,8,0,0,-8,0,-3},
	{0,8,-12,0,0,0,-4,-7,-6,4,-3,2},
	{0,8,-14,0,0,0,-4,-7,6,-4,3,-2},
	{0,0,0,2,0,0,-6,-5,-4,5,-2,2},
	{0,3,-4,0,0,0,-1,-1,-2,-7,1,-4},
	{0,0,2,0,-2,0,4,-6,-5,-4,-2,-2},
	{0,3,-3,0,0,0,0,-7,-6,0,-3,0},
	{0,0,2,-2,0,0,5,-5,-4,-5,-2,-2},
	{0,3,-6,0,0,0,4,-1,1,4,0,2},
	{0,0,0,0,1,0,-4,0,0,3,0,1},
	{0,0,9,-16,4,5,-1,-3,-3,1,-1,0},
	{0,0,7,-16,4,5,-1,-3,3,-1,1,0},
	{0,0,1,0,-3,0,3,1,0,3,0,1},
	{0,0,2,0,-3,0,3,-1,-1,1,0,1},
	{0,4,-5,0,0,0,-2,0,0,-3,0,-1},
	{0,0,1,-4,0,0,1,-2,2,1,1,1},
	{0,0,3,0,-3,0,-2,-1,0,2,0,1},
	{0,0,3,-4,0,0,1,-2,-2,-1,-1,0},
	{0,3,-2,0,0,0,2,0,0,-2,0,-1},
	{0,0,4,-4,0,0,2,-1,-1,-2,0,-1},
	{0,0,2,0,0,-1,2,0,0,-2,0,-1},
	{0,0,3,-3,0,0,2,-1,-1,-1,0,-1},
	{0,0,3,0,-1,0,0,-2,-1,0,-1,0},
	{0,0,1,0,0,1,0,-1,-1,0,-1,0},
	{0,0,0,0,2,0,-1,-1,-1,1,-1,0},
	{0,0,2,-1,0,0,1,0,0,-1,0,-1},
	{0,0,1,0,0,-1,0,-1,-1,0,-1,0},
	{0,5,-6,0,0,0,-2,0,0,-1,0,0},
	{0,0,1,-3,0,0,1,-1,1,1,0,0},
	{0,3,-6,4,0,0,-1,1,1,1,0,0},
	{0,3,-8,4,0,0,-1,1,-1,-1,0,0},
	{0,0,4,-5,0,0,1,-1,-1,0,0,0},
	{0,1,1,0,0,0,0,1,1,0,0,0},
	{0,3,-5,0,0,0,0,-1,1,0,0,0},
	{0,6,-7,0,0,0,-1,0,0,-1,0,0},
	{0,10,-9,0,0,0,1,0,0,-1,0,0},
	{0,0,2,-8,3,0,1,0,0,1,0,0},
	{0,0,6,-8,3,0,-1,0,0,1,0,0},
	{0,0,1,-2,0,0,1,0,0,1,0,0},
	{0,0,9,-15,0,0,-1,0,0,1,0,0},
	{0,0,1,0,-2,5,1,0,0,-1,0,0},
	{0,0,1,0,2,-5,-1,0,0,1,0,0},
	{0,0,1,0,0,-2,1,0,0,1,0,0},
	{0,0,0,1,0,0,-1,0,0,1,0,0},
	{0,0,7,-15,0,0,-1,0,0,-1,0,0},
	{0,2,0,0,0,0,0,-1,-1,0,0,0},
	{0,0,2,0,2,-5,0,1,1,0,0,0},
	{2,0,-2,0,0,0,0,1,-1,0,0,0},
	{0,0,9,-19,0,3,0,1,-1,0,0,0},
	{0,0,11,-19,0,3,0,1,1,0,0,0},
	{0,0,2,-5,0,0,0,-1,1,0,0,0},
	{0,5,-9,0,0,0,0,1,-1,0,0,0},
	{0,11,-10,0,0,0,1,0,0,0,0,0},
	{0,4,-4,0,0,0,0,1,0,0,0,0},
	{0,0,2,0,-4,0,1,0,0,0,0,0},
	{0,0,5,-6,0,0,0,-1,0,0,0,0},
	{0,5,-5,0,0,0,0,1,0,0,0,0},
	{0,0,4,0,-3,0,-1,0,0,0,0,0},
	{0,4,-6,0,0,0,0,-1,0,0,0,0},
	{0,5,-7,0,0,0,0,0,1,0,0,0},
	{0,0,4,0,-2,0,0,0,1,0,0,0},
	{0,0,3,0,-4,0,0,0,0,1,0,0},
	{0,7,-8,0,0,0,0,0,0,-1,0,0}
};

/**
 * Terms for velocity components of the Sun wrt the solar system barycenter
 **/
struct aberration_term2 aberration_series2[] = {
	{0,0,1,0,0,0,719,0,6,-660,-15,-283},
	{0,0,0,1,0,0,159,0,2,-147,-6,-61},
	{0,0,2,0,0,0,34,-9,-8,-31,-4,-13},
	{0,0,0,0,1,0,17,0,0,-16,0,-7},
	{0,0,0,0,0,1,16,0,1,-15,-3,-6},
	{0,0,0,2,0,0,0,-9,-8,0,-3,1},
	{1,0,0,0,0,0,6,0,0,-6,0,-2},
	{0,1,0,0,0,0,5,0,0,-5,0,-2},
	{0,0,3,0,0,0,2,-1,-1,-2,0,-1},
	{0,0,1,-5,0,0,-2,0,0,-2,0,-1},
	{0,0,3,-5,0,0,-2,0,0,2,0,1},
	{1,0,0,0,0,-2,-1,0,0,-1,0,0},
	{0,0,0,3,0,0,-1,0,0,1,0,0},
	{0,0,2,-6,0,0,1,0,0,1,0,0},
	{0,0,2,-4,0,0,1,0,0,-1,0,0},
	{0,0,0,0,2,0,-1,0,0,1,0,0},
	{0,0,1,0,0,-2,1,0,0,0,0,0}
};

/**
 * Terms for velocity components of the Earth wrt the EMB
 **/
struct aberration_term3 aberration_series3[] = {
	{1,0,0,0,0,715,-656,-285},
	{0,0,0,0,1,0,26,-59},
	{1,0,0,1,0,39,-36,-16},
	{1,2,0,-1,0,8,-7,-3},
	{1,-2,0,0,0,5,-5,-2},
	{1,2,0,0,0,4,-4,-2},
	{0,0,0,1,1,0,1,-3},
	{1,-2,0,1,0,-2,2,1},
	{1,0,0,2,0,2,-2,-1},
	{0,2,0,0,-1,0,1,-2},
	{1,0,0,0,-2,-1,1,1},
	{1,0,1,0,0,-1,1,0},
	{1,0,-1,0,0,1,-1,0},
	{1,4,0,-2,0,1,-1,0},
	{1,-2,0,2,0,-1,1,0},
	{1,2,0,1,0,1,0,0},
	{0,2,0,-1,1,0,0,-1}
};

/**
 * Calculates the components of the earth's velocity
 *
 * @param[in] tdb TDB to be used for calculations. TT may be used for all but
 * the most exacting applications.
 * @param[out] vel The Earth's velocity components in 10**(-8) AU/day.
 * The reference frame is the equinox & equator of J2000.
 **/
void aberration_earth_velocity(struct julian_date *tdb,
			struct rectangular_coordinates *vel)
{
	int i;
	struct rectangular_coordinates v1,v2,v3;
	double t,me,ve,ea,ma,ju,sa,ur,ne,l,lp,f,d,w,phi,c_phi,s_phi;

	/* Fundamental arguments */
	t  = JULIAN_CENTURIES(tdb->date1, tdb->date2);
	me = fundamental_argument(ARG_LONGITUDE_MERCURY, t);
	ve = fundamental_argument(ARG_LONGITUDE_VENUS, t);
	ea = fundamental_argument(ARG_LONGITUDE_EARTH, t);
	ma = fundamental_argument(ARG_LONGITUDE_MARS, t);
	ju = fundamental_argument(ARG_LONGITUDE_JUPITER, t);
	sa = fundamental_argument(ARG_LONGITUDE_SATURN, t);
	ur = fundamental_argument(ARG_LONGITUDE_URANUS, t);
	ne = fundamental_argument(ARG_LONGITUDE_NEPTUNE, t);
	l  = fundamental_argument(ARG_ANOMALY_MOON, t);
	lp = fundamental_argument(ARG_ANOMALY_SUN, t);
	f  = fundamental_argument(ARG_LATITUDE_MOON, t);
	d  = fundamental_argument(ARG_ELONGATION_MOON, t);
	w  = fundamental_argument(ARG_LONGITUDE_MOON, t);

	memset(&v1, 0, sizeof(v1));
	memset(&v2, 0, sizeof(v2));
	memset(&v3, 0, sizeof(v3));

	/*
	 * Sum up each series individually with the smallest terms first
	 * in order to maximize precision
	 */

	/* EMB's heliocentric motion #2 */
	i = sizeof(aberration_series1_2) / sizeof(aberration_series1_2[0]) - 1;
	for (; i >= 0; i--) {
		phi = aberration_series1_2[i].l_me * me +
			aberration_series1_2[i].l_ve * ve +
			aberration_series1_2[i].l_ea * ea +
			aberration_series1_2[i].l_ma * ma +
			aberration_series1_2[i].l_ju * ju +
			aberration_series1_2[i].l_sa * sa;

		sincos(phi, &s_phi, &c_phi);

		v1.x += aberration_series1_2[i].x_sin * s_phi +
			aberration_series1_2[i].x_cos * c_phi;
		v1.y += aberration_series1_2[i].y_sin * s_phi +
			aberration_series1_2[i].y_cos * c_phi;
		v1.z += aberration_series1_2[i].z_sin * s_phi +
			aberration_series1_2[i].z_cos * c_phi;
	}

	/* EMB's heliocentric motion #1 */
	i = sizeof(aberration_series1_1) / sizeof(aberration_series1_1[0]) - 1;
	for (; i >= 0; i--) {
		phi = aberration_series1_1[i].l_ea * ea;

		sincos(phi, &s_phi, &c_phi);

		v1.x += (aberration_series1_1[i].x_sin +
			aberration_series1_1[i].x_sin_t * t) * s_phi;
		v1.x += (aberration_series1_1[i].x_cos +
			(aberration_series1_1[i].x_cos_t +
			aberration_series1_1[i].x_cos_t2 * t) * t) * c_phi;

		v1.y += (aberration_series1_1[i].y_sin +
			(aberration_series1_1[i].y_sin_t +
			aberration_series1_1[i].y_sin_t2 * t) * t) * s_phi;
		v1.y += (aberration_series1_1[i].y_cos +
			aberration_series1_1[i].y_cos_t * t) * c_phi;

		v1.z += (aberration_series1_1[i].z_sin +
			(aberration_series1_1[i].z_sin_t +
			aberration_series1_1[i].z_sin_t2 * t) * t) * s_phi;
		v1.z += (aberration_series1_1[i].z_cos +
			aberration_series1_1[i].z_cos_t * t) * c_phi;
	}

	/* The Sun's motion wrt the solar system barycenter */
	i = sizeof(aberration_series2) / sizeof(aberration_series2[0]) - 1;
	for (; i >= 0; i--) {
		phi = aberration_series2[i].l_ve * ve +
			aberration_series2[i].l_ea * ea +
			aberration_series2[i].l_ju * ju +
			aberration_series2[i].l_sa * sa +
			aberration_series2[i].l_ur * ur +
			aberration_series2[i].l_ne * ne;

		sincos(phi, &s_phi, &c_phi);

		v2.x += aberration_series2[i].x_sin * s_phi +
			aberration_series2[i].x_cos * c_phi;
		v2.y += aberration_series2[i].y_sin * s_phi +
			aberration_series2[i].y_cos * c_phi;
		v2.z += aberration_series2[i].z_sin * s_phi +
			aberration_series2[i].z_cos * c_phi;
	}

	/* The Earth's motion wrt to the EMB */
	i = sizeof(aberration_series3) / sizeof(aberration_series3[0]) - 1;
	for (; i >= 0; i--) {
		phi = aberration_series3[i].w * w +
			aberration_series3[i].d * d +
			aberration_series3[i].lp * lp +
			aberration_series3[i].l * l +
			aberration_series3[i].f * f;

		sincos(phi, &s_phi, &c_phi);

		v3.x += aberration_series3[i].x_sin * s_phi;
		v3.y += aberration_series3[i].y_cos * c_phi;
		v3.z += aberration_series3[i].z_cos * c_phi;
	}

	vel->x = v1.x + v2.x + v3.x;
	vel->y = v1.y + v2.y + v3.y;
	vel->z = v1.z + v2.z + v3.z;
}

/**
 * Calculates the annual aberration in right ascension and declination
 *
 * @param[in] tdb TDB to be used for calculations
 * @param[in] e Equatorial coordinates of the celestial body
 * @param[out] d_ra Aberration in right ascension
 * @param[out] d_dec Aberration in declination
 **/
void annual_aberration(struct julian_date *tdb, struct equatorial_coordinates *e,
		double *d_ra, double *d_dec)
{
	double c,cra,sra,cdec,sdec;
	struct rectangular_coordinates v;

	/* Speed of light in 10**(-8) AU/day */
	c = 17314463348.4;

	/* Get the components of the Earth's velocity */
	aberration_earth_velocity(tdb, &v);

	sincos(e->right_ascension, &sra, &cra);
	sincos(e->declination, &sdec, &cdec);

	*d_ra = (v.y * cra - v.x * sra) / (c * cdec);
	*d_dec = (v.z * cdec - (v.x * cra + v.y * sra) * sdec) / c;
}
