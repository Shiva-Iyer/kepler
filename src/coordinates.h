/*
 * coordinates.h - Structs and declarations for coordinate xforms
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

#ifndef _COORDINATES_H_
#define _COORDINATES_H_

/* 
 * Used for rectangular coordinates. 
 *
 * The origin and orientation of the xy axes are application specific. The xy
 * plane could be the ecliptic or the Earth's equator
 */
struct rectangular_coordinates {
	double x; /* x-coordinate. The unit is application specific. */
	double y; /* y-coordinate. The unit is application specific. */
	double z; /* z-coordinate. The unit is application specific. */
};

/* Used for coordinates in the Earth's equatorial frame. */
struct equatorial_coordinates {
	double right_ascension; /* Right ascension in radians. */
	double declination;     /* Declination in radians. */
};

/* Used for ecliptic coordinates. The origin is application specific. */
struct ecliptic_coordinates {
	double longitude; /* Longitude in radians. */
	double latitude;  /* Latitude in radians. */
};

/* Used for horizontal coordinates. */
struct horizontal_coordinates {
	double azimuth;  /* Azimuth in radians. Clockwise from the South. */
	double altitude; /* Altitude in radians. */
};

void rectangular_to_spherical(struct rectangular_coordinates *pla,
			struct rectangular_coordinates *ear, double *lon,
			double *lat, double *rad);

void spherical_to_rectangular(double lon, double lat, double rad,
			struct rectangular_coordinates *rec);

double phase_angle(struct rectangular_coordinates *obj,
		struct rectangular_coordinates *ear);

void equatorial_to_ecliptic(struct equatorial_coordinates *equ, double obl,
			struct ecliptic_coordinates *ecl);

void ecliptic_to_equatorial(struct ecliptic_coordinates *ecl, double obl,
			struct equatorial_coordinates *equ);

void equatorial_to_horizontal(double ha, double decl, double lat,
			struct horizontal_coordinates *hor);

void horizontal_to_equatorial(struct horizontal_coordinates *hor, double lat,
			double *ha, double *decl);

void rotate_rectangular(double mat[3][3], struct rectangular_coordinates *pos);

void rotate_equatorial(double mat[3][3], struct equatorial_coordinates *pos);

void rotate_ecliptic_to_equator(double obl, struct rectangular_coordinates *pos);

#endif
