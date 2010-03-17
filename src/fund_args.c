/*
 * fund_args.c - Implementation of fundamental planetary/lunisolar arguments
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
 * Reference : US Naval Observatory Circular 179 (2005)
 *             <http://aa.usno.navy.mil/publications/docs/Circular_179.php>
 */

#include <kepler.h>
#include <fund_args.h>

/**
 * Calculates values for the various fundamental arguments used in the planetary,
 * lunar, precession and nutation models.
 *
 * @param[in] arg One of the values from the fund_argument enumeration.
 * @param[in] t Number of Julian centuries of TDB since 2000-01-01 12h TDB. TT
 * may be used for all but the most exacting applications.
 *
 * @return -1 if the arg parameter is invalid. The value of the fundamental 
 * argument in radians otherwise.
 **/
double fundamental_argument(enum fund_argument arg, double t)
{
	double val = -1;

	switch (arg) {

	/* Heliocentric ecliptic longitudes of the eight planets */
	case ARG_LONGITUDE_MERCURY:
		val = 4.402608842 + 2608.7903141574 * t;
		break;

	case ARG_LONGITUDE_VENUS:
		val = 3.176146697 + 1021.3285546211 * t;
		break;

	case ARG_LONGITUDE_EARTH:
		val = 1.753470314 + 628.3075849991 * t;
		break;

	case ARG_LONGITUDE_MARS:
		val = 6.203480913 + 334.0612426700 * t;
		break;

	case ARG_LONGITUDE_JUPITER:
		val = 0.599546497 + 52.9690962641 * t;
		break;

	case ARG_LONGITUDE_SATURN:
		val = 0.874016757 + 21.3299104960 * t;
		break;

	case ARG_LONGITUDE_URANUS:
		val = 5.481293872 + 7.4781598567 * t;
		break;

	case ARG_LONGITUDE_NEPTUNE:
		val = 5.311886287 + 3.8133035638 * t;
		break;

	/* General precession in longitude */
	case ARG_PRECESSION:
		val = (0.024381750 + 0.00000538691 * t) * t;
		break;

	/* Mean anomaly of the Moon (L)*/
	case ARG_ANOMALY_MOON:
		val = (485868.249036 +
			(1717915923.2178 +
			(31.8792 +
			(0.051635 - 0.00024470 * t) * t) * t) * t) * ACS_TO_RAD;
		break;

	/* Mean anomaly of the Sun (Lprime) */
	case ARG_ANOMALY_SUN:
		val = (1287104.79305 +
			(129596581.0481 +
			(-0.5532 +
			(0.000136 - 0.00001149 * t) * t) * t) * t) * ACS_TO_RAD;
		break;

	/* Mean argument of latitude of the Moon (F) */
	case ARG_LATITUDE_MOON:
		val = (335779.526232 +
			(1739527262.8478 +
			(-12.7512 +
			(-0.001037 + 0.00000417 * t) * t) * t) * t) * ACS_TO_RAD;
		break;

	/* Mean elongation of the Moon from the Sun (D) */
	case ARG_ELONGATION_MOON:
		val = (1072260.70369 +
			(1602961601.2090 +
			(-6.3706 +
			(0.006593 - 0.00003169 * t) * t) * t) * t) * ACS_TO_RAD;
		break;

	/* Mean longitude of the Moon's mean ascending node (Omega) */
	case ARG_LONGITUDE_NODE:
		val = (450160.398036 +
			(-6962890.5431 +
			(7.4722 +
			(0.007702 - 0.00005939 * t) * t) * t) * t) * ACS_TO_RAD;
		break;

	/* Mean longitude of the moon (w) */
	case ARG_LONGITUDE_MOON:
		val = (785939.95571 +
			(1732559343.73604 +
			(-5.8883 +
			(0.006604 - 0.00003169 * t) * t) * t) * t) * ACS_TO_RAD;
		break;
	}

	return val;
}
