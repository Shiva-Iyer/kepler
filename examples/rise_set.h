/*
 * rise_set.h - Declarations for the kepler rise/set example program
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

#ifndef _RISE_SET_H_
#define _RISE_SET_H_

#include <kepler.h>
#include <julian_date.h>
#include <coordinates.h>

enum {
  PLUTO = NEPTUNE + 1,
  SUN,
  MOON
};

#define TIME_STEP               6                    /* hours */

#define SUN_REFRACTION		(-3000 * ACS_TO_RAD) /* -50 arc minutes */
#define MOON_REFRACTION		(450 * ACS_TO_RAD)   /* +7.5 arc minutes */
#define PLANET_REFRACTION	(-2040 * ACS_TO_RAD) /* -34 arc minutes */

void get_equatorial(int pla, struct julian_date *jd,
		    struct equatorial_coordinates *equ);

char *format_time(double t, char *buf, u_short sec);

void parse_command_line(int argc, char *argv[], int *year, int *month,
			int *day, double *longitude, double *latitude,
			double *ut_off);

void display_usage();

void display_copyright();

#endif
