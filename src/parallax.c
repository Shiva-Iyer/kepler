/*
 * parallax.c - Implementation for calculating geocentric parallax
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
#include <parallax.h>

/*
 * Calculates the geocentric parallax in the equatorial coordinates of a
 * celestial body.
 *
 * hr_ang -- The body's local hour angle in radians.
 * decl -- The body's declination in radians.
 * distance -- The body's distance from the Earth in AU. 
 * geog_lat -- The observer's geographic latitude in radians.
 * height_msl -- The observer's height above mean sea level in meters.
 * d_ra -- The parallax in right ascension expressed in radians.
 * d_decl -- The parallax in declination expressed in radians.
 */
void geocentric_parallax(double hr_ang, double decl, double distance,
			double geog_lat, double height_msl, double *d_ra,
			double *d_decl)
{
	double gclat,gcrad,hpx,cd,sd,ch,sh,cl,sl;

	/* Get the observer's earth figure numbers */
	earth_figure_values(geog_lat, height_msl, &gclat, &gcrad);

	/* The horizontal parallax of the body */
	hpx = sin(8.794 * ACS_TO_RAD) / distance;

	sincos(hr_ang, &sh, &ch);
	sincos(decl, &sd, &cd);
	sincos(gclat, &sl, &cl);

	*d_ra = atan2(-gcrad * cl * hpx * sh, cd - gcrad * cl * hpx * ch);

	*d_decl = atan2((sd - gcrad * sl * hpx) * cos(*d_ra),
			cd - gcrad * cl * hpx * ch) - decl;
}
