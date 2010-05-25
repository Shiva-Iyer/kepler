/*
 * delta-t.c - Implementation for calculating Delta-T
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
 * An implementation of NASA's polynomial expressions for approximating Delta-T
 *
 * Reference : <http://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html>
 */

#include <kepler.h>
#include <delta-t.h>

/*
 * Returns an approximate value for Delta-T (TT - UT1) valid for a limited
 * historical period.
 *
 * year -- Astronomical year number between 2000BC and 3000AD inclusive.
 * month -- Month of the year.
 * delta_t -- An approximate value for Delta-T in seconds.
 * correction -- A correction, in seconds, to be added to Delta-T for years
 *               before 1955AD or after 2005AD and when used in conjunction with
 *               NASA's publication "Five millennium canon of solar eclipses".
 *
 * Return: SUCCESS -- Delta-T calculated successfully.
 *         ERR_INVALID_DATE -- Invalid year or month specified.
 */
int delta_t(int year, int month, double *delta_t, double *correction)
{
	double u,y;

	if (year < -1999 || year > 3000 || month < 1 || month > 12)
		return ERR_INVALID_DATE;

	y = year + (month - 0.5) / 12;

	if (year < -500) {

		u = (year - 1820) / 100;
		*delta_t = -20 + 32 * u * u;

	} else if (year >= -500 && year < 500) {

		u = y / 100;
		*delta_t = 10583.6 +
			(-1014.41 +
			(33.78311 +
			(-5.952053 +
			(-0.1798452 +
			(0.022174192 +
			0.0090316521 * u) * u) * u) * u) * u) * u;

	} else if (year >= 500 && year < 1600) {

		u = (y - 1000) / 100;
		*delta_t = 1574.2 +
			(-556.01 +
			(71.23472 +
			(0.319781 +
			(-0.8503463 +
			(-0.005050998 +
			0.0083572073 * u) * u) * u) * u) * u) * u;

	} else if (year >= 1600 && year < 1700) {

		u = y - 1600;
		*delta_t = 120 + (-0.9808 + (-0.01532 + u / 7129.0) * u) * u;

	} else if (year >= 1700 && year < 1800) {

		u = y - 1700;
		*delta_t = 8.83 +
			(0.1603 +
			(-0.0059285 +
			(0.00013336 - u / 1174000.0) * u) * u) * u;

	} else if (year >= 1800 && year < 1860) {

		u = y - 1800;
		*delta_t = 13.72 +
			(-0.332447 +
			(0.0068612 + 
			(0.0041116 + 
			(-0.00037436 + 
			(0.0000121272 + 
			(-0.0000001699 +
			0.000000000875 * u) * u) * u) * u) * u) * u) * u;

	} else if (year >= 1860 && year < 1900) {

		u = y - 1860;
		*delta_t = 7.62 +
			(0.5737 +
			(-0.251754 +
			(0.01680668 +
			(-0.0004473624 + u / 233174.0) * u) * u) * u) * u;

	} else if (year >= 1900 && year < 1920) {

		u = y - 1900;
		*delta_t = -2.79 +
			(1.494119 +
			(-0.0598939 +
			(0.0061966 - 0.000197 * u) * u) * u) * u;

	} else if (year >= 1920 && year < 1941) {

		u = y - 1920;
		*delta_t = 21.20 +
			(0.84493 +
			(-0.076100 + 0.0020936 * u) * u) * u;

	} else if (year >= 1941 && year < 1961) {

		u = y - 1950;
		*delta_t = 29.07 + (0.407 + (-1.0 / 233.0 + u / 2547.0) * u) * u;

	} else if (year >= 1961 && year < 1986) {

		u = y - 1975;
		*delta_t = 45.45 + (1.067 + (-1.0 / 260.0 - u / 718.0) * u) * u;

	} else if (year >= 1986 && year < 2005) {

		u = y - 2000;
		*delta_t = 63.86 +
			(0.3345 + 
			(-0.060374 + 
			(0.0017275 + 
			(0.000651814 + 0.00002373599 * u) * u) * u) * u) * u;

	} else if (year >= 2005 && year < 2050) {

		u = y - 2000;
		*delta_t = 62.92 + (0.32217 + 0.005589 * u) * u;

	} else if (year >= 2050 && year < 2150) {

		u = (y - 1820) / 100;
		*delta_t = -20 + 32 * u * u - 0.5628 * (2150 - y);

	} else {
		u = (year - 1820) / 100;
		*delta_t = -20 + 32 * u * u;
	}

	if (year < 1955 || year > 2005)
		*correction = -0.000012932 * (y - 1955) * (y - 1955);
	else
		*correction = 0;

	return SUCCESS;
}
