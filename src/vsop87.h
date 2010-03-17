/*
 * vsop87.h - Structs, constants & declarations for the VSOP87 theory
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

#ifndef _VSOP87_H_
#define _VSOP87_H_

#include <kepler.h>
#include <julian_date.h>
#include <coordinates.h>

/** Used internally to store the series of terms in the theory */
struct vsop87_term {
	double a;
	double b;
	double c;
};

int vsop87_coordinates(enum solar_system_planets planet, struct julian_date *tt,
		struct rectangular_coordinates *pos);

void vsop87_ecliptic_to_equator(struct rectangular_coordinates *pos);

#endif
