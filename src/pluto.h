/*
 * pluto.h - Constants & declarations for an analytical theory for Pluto
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

#ifndef _PLUTO_H_
#define _PLUTO_H_

#include <julian_date.h>
#include <coordinates.h>

#define PLUTO_MIN_DATE          2341972.5
#define PLUTO_MAX_DATE          2488092.5

int pluto_coordinates(struct julian_date *tdb,
		struct rectangular_coordinates *pos);

#endif
