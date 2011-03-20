/*
 * elp82b.h - Structs & declarations for the ELP 2000-82B lunar theory
 * Copyright (C) 2010-2011 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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

#ifndef _ELP82B_H_
#define _ELP82B_H_

#include <julian_date.h>
#include <coordinates.h>

/* Used internally to store the series of terms in the theory */
struct elp82b_term1 {
	short i1;
	short i2;
	short i3;
	short i4;
	double a;
	double b1;
	double b2;
	double b3;
	double b4;
	double b5;
};

/* Used internally to store the series of terms in the theory */
struct elp82b_term2 {
	short i1;
	short i2;
	short i3;
	short i4;
	short i5;
	double phi;
	double a;
};

/* Used internally to store the series of terms in the theory */
struct elp82b_term3 {
	short i1;
	short i2;
	short i3;
	short i4;
	short i5;
	short i6;
	short i7;
	short i8;
	short i9;
	short i10;
	short i11;
	double phi;
	double a;
};

void elp82b_coordinates(struct julian_date *tdb, struct rectangular_coordinates *pos);

void elp82b_ecliptic_to_equator(struct rectangular_coordinates *pos);

#endif
