/*
 * sidereal_time.c - Implementation for calculating sidereal time
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
 * Reference 1 : US Naval Observatory Circular 179 (2005)
 *               <http://aa.usno.navy.mil/publications/docs/Circular_179.php>
 *
 * Reference 2 : IERS Conventions (2003) [Technical note 32]
 *               <http://www.iers.org/nn_11216/IERS/EN/Publications/TechnicalNotes/tn32.html>
 */

#include <math.h>
#include <kepler.h>
#include <fund_args.h>
#include <iau2000a_nutation.h>
#include <sidereal_time.h>

/* Series for calculating the equation of the equinoxes (Reference 2) */
struct equation_of_equinoxes_term eoe_series[] = {
	{2640.96, -0.39,  0,  0,  0,  0,  1,  0,  0,  0},
	{  63.52, -0.02,  0,  0,  0,  0,  2,  0,  0,  0},
	{  11.75,  0.01,  0,  0,  2, -2,  3,  0,  0,  0},
	{  11.21,  0.01,  0,  0,  2, -2,  1,  0,  0,  0},
	{  -4.55,  0.00,  0,  0,  2, -2,  2,  0,  0,  0},
	{   2.02,  0.00,  0,  0,  2,  0,  3,  0,  0,  0},
	{   1.98,  0.00,  0,  0,  2,  0,  1,  0,  0,  0},
	{  -1.72,  0.00,  0,  0,  0,  0,  3,  0,  0,  0},
	{  -1.41, -0.01,  0,  1,  0,  0,  1,  0,  0,  0},
	{  -1.26, -0.01,  0,  1,  0,  0, -1,  0,  0,  0},
	{  -0.63,  0.00,  1,  0,  0,  0, -1,  0,  0,  0},
	{  -0.63,  0.00,  1,  0,  0,  0,  1,  0,  0,  0},
	{   0.46,  0.00,  0,  1,  2, -2,  3,  0,  0,  0},
	{   0.45,  0.00,  0,  1,  2, -2,  1,  0,  0,  0},
	{   0.36,  0.00,  0,  0,  4, -4,  4,  0,  0,  0},
	{  -0.24, -0.12,  0,  0,  1, -1,  1, -8, 12,  0},
	{   0.32,  0.00,  0,  0,  2,  0,  0,  0,  0,  0},
	{   0.28,  0.00,  0,  0,  2,  0,  2,  0,  0,  0},
	{   0.27,  0.00,  1,  0,  2,  0,  3,  0,  0,  0},
	{   0.26,  0.00,  1,  0,  2,  0,  1,  0,  0,  0},
	{  -0.21,  0.00,  0,  0,  2, -2,  0,  0,  0,  0},
	{   0.19,  0.00,  0,  1, -2,  2, -3,  0,  0,  0},
	{   0.18,  0.00,  0,  1, -2,  2, -1,  0,  0,  0},
	{  -0.10,  0.05,  0,  0,  0,  0,  0,  8,-13, -1},
	{   0.15,  0.00,  0,  0,  0,  2,  0,  0,  0,  0},
	{  -0.14,  0.00,  2,  0, -2,  0, -1,  0,  0,  0},
	{   0.14,  0.00,  1,  0,  0, -2,  1,  0,  0,  0},
	{  -0.14,  0.00,  0,  1,  2, -2,  2,  0,  0,  0},
	{   0.14,  0.00,  1,  0,  0, -2, -1,  0,  0,  0},
	{   0.13,  0.00,  0,  0,  4, -2,  4,  0,  0,  0},
	{  -0.11,  0.00,  0,  0,  2, -2,  4,  0,  0,  0},
	{   0.11,  0.00,  1,  0, -2,  0, -3,  0,  0,  0},
	{   0.11,  0.00,  1,  0, -2,  0, -1,  0,  0,  0}
};

/*
 * Calculates the local mean sidereal time using the latest IAU/IERS conventions
 * as described in Reference 1.
 *
 * ut1 -- UT1 for calculating the Earth Rotation Angle.
 * tdb -- TDB to be used for calculating the LMST. TT may be used  for all but
 *        the most exacting applications.
 * longitude -- The observer's longitude in radians, positive east of Greenwich.
 *
 * Return: The local mean sidereal time in radians, between 0 and 2*PI.
 */
double mean_sidereal_time(struct julian_date *ut1, struct julian_date *tdb,
			double longitude)
{
	double dut,era,t,mst;

	dut = (ut1->date1 - J2000_EPOCH) + ut1->date2;
	t = JULIAN_CENTURIES(tdb->date1, tdb->date2);

	/* Earth rotation angle */
	era = (0.7790572732640 + 0.00273781191135448 * dut +
		fmod(ut1->date1, 1.0) + fmod(ut1->date2, 1.0)) * TWO_PI;

	/* Greenwich mean sidereal time */
	mst = era +
		(0.014506 +
		(4612.156534 +
		(1.3915817 +
		(-0.00000044 +
		(-0.000029956 -
		0.0000000368 * t) * t) * t) * t) * t) * ACS_TO_RAD;
 
	/* Local mean sidereal time */
	mst = reduce_angle(mst + longitude, TWO_PI);

	return mst;
}

/*
 * Calculates the local apparent sidereal time using the latest IAU/IERS
 * conventions as described in Reference 1. Apparent sidereal time takes
 * into account the motion of the equinox due to nutation.
 *
 * ut1 -- UT1 for calculating the Earth Rotation Angle.
 * tdb -- TDB to be used for calculating the LAST. TT may be used  for all but
 *        the most exacting applications.
 * longitude -- The observer's longitude in radians, positive east of Greenwich.
 *
 * Return: The local apparent sidereal time in radians, between 0 and 2*PI.
 */
double apparent_sidereal_time(struct julian_date *ut1, struct julian_date *tdb,
			double longitude)
{
	double mst,ast;

	/* Get the local mean sidereal time */
	mst = mean_sidereal_time(ut1, tdb, longitude);

	/* Add equation of equinoxes to get local apparent sidereal time */
	ast = reduce_angle(mst + equation_of_the_equinoxes(tdb), TWO_PI);

	return ast;
}

/*
 * Calculates the value of the equation of the equinoxes using the model
 * described in Reference 2. This accounts for the motion of the equinox
 * due to nutation.
 *
 * tdb -- TDB to be used for calculations. TT may be used for all but the most
 *        exacting applications.
 *
 * Return: The value of the equation of the equinoxes, in radians.
 */
double equation_of_the_equinoxes(struct julian_date *tdb)
{
	int i;
	double t,obl,d_lon,d_obl,l,lp,f,d,om,l_ve,l_ea,pre,phi,eqe;

	/* Fundamental arguments for the EE */
	t    = JULIAN_CENTURIES(tdb->date1, tdb->date2);
	l    = fundamental_argument(ARG_ANOMALY_MOON, t);
	lp   = fundamental_argument(ARG_ANOMALY_SUN, t);
	f    = fundamental_argument(ARG_LATITUDE_MOON, t);
	d    = fundamental_argument(ARG_ELONGATION_MOON, t);
	om   = fundamental_argument(ARG_LONGITUDE_NODE, t);
	l_ve = fundamental_argument(ARG_LONGITUDE_VENUS, t);
	l_ea = fundamental_argument(ARG_LONGITUDE_EARTH, t);
	pre  = fundamental_argument(ARG_PRECESSION, t);

	/* Sum up the series for the equation of the equinoxes */
	eqe = 0;
	for (i = 0; i < sizeof(eoe_series) / sizeof(eoe_series[0]); i++) {
		phi  = eoe_series[i].l * l +
			eoe_series[i].lp * lp +
			eoe_series[i].f * f +
			eoe_series[i].d * d +
			eoe_series[i].om * om +
			eoe_series[i].l_ve * l_ve +
			eoe_series[i].l_ea * l_ea +
			eoe_series[i].pre * pre;

		eqe += eoe_series[i].si * sin(phi) +
			eoe_series[i].ci * cos(phi) -
			0.87 * t * sin(om);
	}
	eqe *= UAS_TO_RAD;

	/* Add the term for obliquity and nutation in longitude */
	obl = iau2000a_mean_obliquity(tdb);
	iau2000a_nutation(tdb, &d_lon, &d_obl);

	eqe += d_lon * cos(obl);

	return eqe;
}
