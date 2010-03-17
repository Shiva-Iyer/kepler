/*
 * sidereal_time.h - Declarations for calculating sidereal time
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

#ifndef _SIDEREAL_TIME_H_
#define _SIDEREAL_TIME_H_

#include <julian_date.h>

/** Used internally to store the series for the equation of the equinoxes */
struct equation_of_equinoxes_term {
	double si;
	double ci;
	short l;
	short lp;
	short f;
	short d;
	short om;
	short l_ve;
	short l_ea;
	short pre;
};

double mean_sidereal_time(struct julian_date *ut1, struct julian_date *tdb,
			double longitude);

double apparent_sidereal_time(struct julian_date *ut1, struct julian_date *tdb,
			double longitude);

double equation_of_the_equinoxes(struct julian_date *tdb);

#endif
