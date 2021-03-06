/*
 * kepler.h - Constants, macros, utility routines
 * Copyright (C) 2016 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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

#ifndef _KEPLER_H_
#define _KEPLER_H_

#include <math.h>
#include <string.h>

#define PROG_NAME		"kepler"
#define PROG_VERSION		"2.0.0"
#define PROG_VERSION_STRING	PROG_NAME " version " PROG_VERSION "\n"
#define PROG_COPYRIGHT		\
    "Copyright (C) 2016 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>\n\n" \
    "This program is free software: you may redistribute and/or modify it\n" \
    "under the terms of the GNU GPL <http://www.gnu.org/licenses/gpl.html>.\n\n" \
    "This program is distributed WITHOUT ANY WARRANTY; without even the\n" \
    "implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"

#define SUCCESS                 	0

#define ERR_INVALID_PLANET         	-1
#define ERR_INVALID_DATE        	-2

#define ERR_INVALID_ECCENTRICITY	-3
#define ERR_CONVERGENCE			-4

#define ERR_INVALID_DATA		-5

#define PI			3.141592653589793238462643
#define TWO_PI			(2.0*PI)

#define DEG_TO_RAD	        (PI/180.0)
#define ACS_TO_RAD	        (DEG_TO_RAD/3600.0)
#define MAS_TO_RAD		(ACS_TO_RAD/1000.0)
#define UAS_TO_RAD		(MAS_TO_RAD/1000.0)

#define RAD_TO_DEG	        (180.0/PI)
#define RAD_TO_ACS              (1.0/ACS_TO_RAD)

#define HRS_TO_DEG	        15.0
#define DEG_TO_HRS	        (1.0/HRS_TO_DEG)

#define RAD_TO_HRS	        (RAD_TO_DEG*DEG_TO_HRS)
#define HRS_TO_RAD	        (1.0/RAD_TO_HRS)

#define DEGREES(d, m, s)	(d + (m/60.0) + (s/3600.0))

#define GAUSS_GRAV_CONSTANT	0.01720209895

#define AU			149597870.691 /* AU in KM */

#define EARTH_FLATTENING	(1.0/298.25642)
#define EARTH_EQU_RADIUS	6378136.6 /* Earth equ. radius (m) */
#define EARTH_POL_RADIUS	(EARTH_EQU_RADIUS*(1.0-EARTH_FLATTENING))

typedef unsigned short 		u_short;

/* Used in conversions from decimal degrees to d/m/s form. */
struct deg_min_sec {
    int degrees;
    int minutes;
    double seconds;
};

/*
 * Identifiers for the planets in the solar system. Following the IAU's
 * resolutions of 2006, Pluto is not considered a planet.
 */
enum solar_system_planets {
    MERCURY,
    VENUS,
    EARTH,
    MARS,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE
};

/*
 * Returns an angle that is between 0 and limit.
 *
 * angle -- The angle to be reduced.
 * limit -- The maximum value for the angle.
 *
 * Return: An angle that is between 0 and limit.
 */
static __inline__ double reduce_angle(double angle, double limit)
{
    while (angle < 0)
	angle += limit;
    while (angle > limit)
	angle -= limit;

    return(angle);
}

/*
 * Converts an angle in decimal degrees to degrees/minutes/seconds form.
 *
 * d -- The angle to convert.
 * out -- Converted value. If d is negative, the members of out
 *        will be negative.
 */
static __inline__ void degrees_to_dms(double d, struct deg_min_sec *out)
{
    double tmp;

    tmp = fabs(d);
    out->degrees = floor(tmp);
    tmp = (tmp - out->degrees) * 60;
    out->minutes = floor(tmp);
    out->seconds = (tmp - out->minutes) * 60;

    if (d < 0) {
	out->degrees *= -1;
	out->minutes *= -1;
	out->seconds *= -1;
    }
}

/*
 * Swaps values.
 *
 * x -- Will contain *y on return.
 * y -- Will contain *x on return.
 */
static __inline__ void swap(double *x, double *y)
{
    double tmp;

    tmp = *x;
    *x = *y;
    *y = tmp;
}

/*
 * Version of the standard C function strncpy that always
 * NULL terminates.
 *
 * dest -- Destination string.
 * src -- Source string.
 * len -- Maximum number of characters to copy.
 */
static __inline__ char *strncpyn(char *dest, const char *src, size_t len)
{
    strncpy(dest, src, len);
    dest[len] = '\0';
    return(dest);
}

#ifndef __USE_GNU

/*
 * The MinGW implementation of gcc lacks the sincos() function, and so we
 * define our own. This is not an efficient implementation, and will need
 * to be revisited.
 */
static __inline__ void sincos(double x, double *sinx, double *cosx)
{
    *sinx = sin(x);
    *cosx = cos(x);
}

#endif

#endif
