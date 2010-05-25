/*
 * earth_figure.c - Routines for calculating various earth figure values
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

#include <math.h>
#include <kepler.h>
#include <earth_figure.h>

/*
 * Calculates various values related to the oblateness of the Earth.
 *
 * geog_lat -- The observer's geographic latitude in radians.
 * height_msl -- The observer's height above mean sea level in meters.
 * gc_lat -- The observer's geocentric latitude in radians.
 * gc_radius -- The observer's geocentric radius as a fraction of
 *              the Earth's equatorial radius.
 */
void earth_figure_values(double geog_lat, double height_msl, double *gc_lat,
			double *gc_radius)
{
	double cg,sg,x,s,c;

	sincos(geog_lat, &sg, &cg);
	x = atan2(EARTH_POL_RADIUS * sg, EARTH_EQU_RADIUS * cg);
	sincos(x, &s, &c);

	c = c + (height_msl / EARTH_EQU_RADIUS) * cg;
	s = (1.0 - EARTH_FLATTENING) * s + (height_msl / EARTH_EQU_RADIUS) * sg;

	*gc_lat = atan2(s, c);
	*gc_radius = sqrt(c * c + s * s);
}
