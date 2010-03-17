/*
 * coordinates.c - Routines for coordinate xforms & rotations
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
#include <coordinates.h>

/**
 * Converts the heliocentric rectangular coordinates of a celestial body to
 * geocentric spherical coordinates.
 *
 * Since no rotation is involved, the resulting coordinates will be in the same
 * reference frame (xy plane) as the original coordinates.
 *
 * @param[in] pla The body's heliocentric rectangular coordinates. Must be in
 * the same reference frame as ear.
 * @param[in] ear The Earth's heliocentric rectangular coordinates. Must be in
 * the same reference frame as pla.
 * @param[out] lon The body's geocentric longitude in radians.
 * @param[out] lat The body's geocentric latitude in radians.
 * @param[out] rad The body's geocentric distance in the units of pla and ear.
 **/
void rectangular_to_spherical(struct rectangular_coordinates *pla,
			struct rectangular_coordinates *ear, double *lon,
			double *lat, double *rad)
{
	double x,y,z;

	/* Convert from heliocentric coordinates to geocentric */
	x = pla->x - ear->x;
	y = pla->y - ear->y;
	z = pla->z - ear->z;

	*lon = reduce_angle(atan2(y, x), TWO_PI);
	*lat = atan2(z, sqrt(x * x + y * y));
	*rad = sqrt(x * x + y * y + z * z);
}

/**
 * Converts the spherical coordinates of a celestial body to rectangular
 * coordinates with the same origin and reference frame (xy plane).
 *
 * @param[in] lon The body's longitude in radians.
 * @param[in] lat The body's latitude in radians.
 * @param[in] rad The body's distance from the central body.
 * @param[out] rec The body's rectangular coordinates in the same units as rad.
 **/
void spherical_to_rectangular(double lon, double lat, double rad,
			struct rectangular_coordinates *rec)
{
	double cd,sd,cr,sr;

	sincos(lon, &sr, &cr);
	sincos(lat, &sd, &cd);

	rec->x = rad * cr * cd;
	rec->y = rad * sr * cd;
	rec->z = rad * sd;
}

/**
 * Converts the geocentric equatorial coordinates of a celestial body to
 * geocentric ecliptic coordinates. 
 *
 * @param[in] equ The body's geocentric equatorial coordinates.
 * @param[in] obl The obliquity of the ecliptic in radians.
 * @param[out] ecl The body's geocentric ecliptic coordinates.
 **/
void equatorial_to_ecliptic(struct equatorial_coordinates *equ, double obl,
			struct ecliptic_coordinates *ecl)
{
	double cd,sd,cr,sr,co,so;

	sincos(equ->declination, &sd, &cd);
	sincos(equ->right_ascension, &sr, &cr);
	sincos(obl, &so, &co);

	ecl->longitude = reduce_angle(atan2(sr * cd * co + sd * so, cr * cd), TWO_PI);
	ecl->latitude = asin(sd * co - sr * cd * so);
}

/**
 * Converts the geocentric ecliptic coordinates of a celestial body to
 * geocentric equatorial coordinates. 
 *
 * @param[in] ecl The body's geocentric ecliptic coordinates.
 * @param[in] obl The obliquity of the ecliptic in radians.
 * @param[out] equ The body's geocentric equatorial coordinates.
 **/
void ecliptic_to_equatorial(struct ecliptic_coordinates *ecl, double obl,
			struct equatorial_coordinates *equ)
{
	double cg,sg,ct,st,co,so;

	sincos(ecl->longitude, &sg, &cg);
	sincos(ecl->latitude, &st, &ct);
	sincos(obl, &so, &co);

	equ->right_ascension = reduce_angle(atan2(sg * ct * co - st * so, cg * ct), TWO_PI);
	equ->declination = asin(st * co + sg * ct * so);
}

/**
 * Converts the geocentric equatorial coordinates of a celestial body to
 * horizontal coordinates.
 *
 * @param[in] ha The body's local hour angle in radians.
 * @param[in] decl The body's declination in radians.
 * @param[in] lat The observer's geographic latitude in radians.
 * @param[out] hor The body's horizontal coordinates.
 **/
void equatorial_to_horizontal(double ha, double decl, double lat,
			struct horizontal_coordinates *hor)
{
	double cd,sd,ch,sh,ct,st;

	sincos(decl, &sd, &cd);
	sincos(ha, &sh, &ch);
	sincos(lat, &st, &ct);

	hor->azimuth = reduce_angle(atan2(cd * sh, cd * st * ch - sd * ct), TWO_PI);
	hor->altitude = asin(sd * st + cd * ct * ch);
}

/**
 * Converts the horizontal coordinates of a celestial body to geocentric
 * equatorial coordinates.
 *
 * @param[in] hor The body's horizontal coordinates.
 * @param[in] lat The observer's geographic latitude in radians.
 * @param[out] ha The body's local hour angle in radians.
 * @param[out] decl The body's declination in radians.
 **/
void horizontal_to_equatorial(struct horizontal_coordinates *hor, double lat,
			double *ha, double *decl)
{
	double ca,sa,cz,sz,ct,st;

	sincos(hor->altitude, &sa, &ca);
	sincos(hor->azimuth, &sz, &cz);
	sincos(lat, &st, &ct);

	*ha = reduce_angle(atan2(sz * ca, cz * ca * st + sa * ct), TWO_PI);
	*decl = asin(sa * st - cz * ca * ct);
}

/**
 * Applies a rotation matrix to a body's rectangular coordinates.
 *
 * @param[in] mat The rotation matrix.
 * @param[in,out] pos The coordinates to be rotated in-place.
 **/
void rotate_rectangular(double mat[3][3], struct rectangular_coordinates *pos)
{
	struct rectangular_coordinates tmp;

	tmp.x = (mat[0][0] * pos->x) + (mat[0][1] * pos->y) + (mat[0][2] * pos->z);
	tmp.y = (mat[1][0] * pos->x) + (mat[1][1] * pos->y) + (mat[1][2] * pos->z);
	tmp.z = (mat[2][0] * pos->x) + (mat[2][1] * pos->y) + (mat[2][2] * pos->z);

	pos->x = tmp.x;
	pos->y = tmp.y;
	pos->z = tmp.z;
}

/**
 * Applies a rotation matrix to a body's geocentric equatorial coordinates. This
 * function can be used to apply the IAU2006/2000A precession/nutation matrices.
 *
 * @param[in] mat The rotation matrix.
 * @param[in,out] pos The coordinates to be rotated in-place.
 **/
void rotate_equatorial(double mat[3][3], struct equatorial_coordinates *pos)
{
	struct rectangular_coordinates rec;

	/* We arbitrarily assign the body a radius of 1 unit */
	spherical_to_rectangular(pos->right_ascension, pos->declination,1, &rec);

	rotate_rectangular(mat, &rec);

	pos->right_ascension = reduce_angle(atan2(rec.y, rec.x), TWO_PI);
	pos->declination = asin(rec.z);
}

/**
 * Rotates rectangular coordinates in the ecliptic frame to the equatorial frame.
 *
 * @param[in] obl The obliquity of the ecliptic in radians.
 * @param[in,out] pos The coordinates to be rotated in-place.
 **/
void rotate_ecliptic_to_equator(double obl, struct rectangular_coordinates *pos)
{
	double co,so;
	struct rectangular_coordinates tmp;

	sincos(obl, &so, &co);

	tmp.y = pos->y * co - pos->z * so;
	tmp.z = pos->y * so + pos->z * co;

	pos->y = tmp.y;
	pos->z = tmp.z;
}
