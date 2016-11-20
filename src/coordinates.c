/*
 * coordinates.c - Routines for coordinate xforms & rotations
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

#include <math.h>
#include <kepler.h>
#include <coordinates.h>
#include <julian_date.h>
#include <vsop87.h>
#include <pluto.h>

/*
 * Converts the heliocentric rectangular coordinates of a celestial body to
 * geocentric spherical coordinates. Since no rotation is involved, the
 * resulting coordinates will be in the same reference frame (xy plane) as the
 * original coordinates.
 *
 * pla -- The body's heliocentric rectangular coordinates. Must be in the same
 *        reference frame and units as ear.
 * ear -- The Earth's heliocentric rectangular coordinates. Must be in the same
 *        reference frame and units as pla.
 * lon -- The body's geocentric longitude in radians.
 * lat -- The body's geocentric latitude in radians.
 * rad -- The body's geocentric distance in the units of pla and ear.
 */
void rectangular_to_spherical(struct rectangular_coordinates *pla,
			      struct rectangular_coordinates *ear,
			      double *lon, double *lat, double *rad)
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

/*
 * Converts the spherical coordinates of a celestial body to rectangular
 * coordinates with the same origin and reference frame (xy plane).
 *
 * lon -- The body's longitude in radians.
 * lat -- The body's latitude in radians.
 * rad -- The body's distance from the central body.
 * rec -- The body's rectangular coordinates in the same units as rad.
 */
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

/*
 * Calculates the Sun-body-Earth phase angle.
 *
 * obj -- The body's heliocentric rectangular coordinates. Must be in
 *        the same reference frame and units as ear.
 * ear -- The Earth's heliocentric rectangular coordinates. Must be in
 *        the same reference frame and units as obj.
 *
 * Return: The Sun-body-Earth phase angle in radians.
 */
double phase_angle(struct rectangular_coordinates *obj,
		   struct rectangular_coordinates *ear)
{
    double a,b,c,pa;

    /* Sun-Earth distance */
    a = sqrt(ear->x * ear->x + ear->y * ear->y + ear->z * ear->z);

    /* Sun-body distance */
    b = sqrt(obj->x * obj->x + obj->y * obj->y + obj->z * obj->z);

    /* Earth-body distance */
    c = sqrt((obj->x - ear->x) * (obj->x - ear->x) +
	     (obj->y - ear->y) * (obj->y - ear->y) +
	     (obj->z - ear->z) * (obj->z - ear->z));

    /* Solve for the phase angle using the law of cosines */
    pa = acos((b * b + c * c - a * a) / (2 * b * c));

    return pa;
}

/*
 * Converts the geocentric equatorial coordinates of a celestial body to
 * geocentric ecliptic coordinates. 
 *
 * equ -- The body's geocentric equatorial coordinates.
 * obl -- The obliquity of the ecliptic in radians.
 * ecl -- The body's geocentric ecliptic coordinates.
 */
void equatorial_to_ecliptic(struct equatorial_coordinates *equ,
			    double obl, struct ecliptic_coordinates *ecl)
{
    double cd,sd,cr,sr,co,so;

    sincos(equ->declination, &sd, &cd);
    sincos(equ->right_ascension, &sr, &cr);
    sincos(obl, &so, &co);

    ecl->longitude = reduce_angle(atan2(sr * cd * co + sd * so, cr * cd), TWO_PI);
    ecl->latitude = asin(sd * co - sr * cd * so);
}

/*
 * Converts the geocentric ecliptic coordinates of a celestial body to
 * geocentric equatorial coordinates. 
 *
 * ecl -- The body's geocentric ecliptic coordinates.
 * obl -- The obliquity of the ecliptic in radians.
 * equ -- The body's geocentric equatorial coordinates.
 */
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

/*
 * Converts the geocentric equatorial coordinates of a celestial body to
 * horizontal coordinates.
 *
 * ha -- The body's local hour angle in radians.
 * decl -- The body's declination in radians.
 * lat -- The observer's geographic latitude in radians.
 * hor -- The body's horizontal coordinates.
 */
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

/*
 * Converts the horizontal coordinates of a celestial body to geocentric
 * equatorial coordinates.
 *
 * hor -- The body's horizontal coordinates.
 * lat -- The observer's geographic latitude in radians.
 * ha -- The body's local hour angle in radians.
 * decl -- The body's declination in radians.
 */
void horizontal_to_equatorial(struct horizontal_coordinates *hor,
			      double lat, double *ha, double *decl)
{
    double ca,sa,cz,sz,ct,st;

    sincos(hor->altitude, &sa, &ca);
    sincos(hor->azimuth, &sz, &cz);
    sincos(lat, &st, &ct);

    *ha = reduce_angle(atan2(sz * ca, cz * ca * st + sa * ct), TWO_PI);
    *decl = asin(sa * st - cz * ca * ct);
}

/*
 * Applies a rotation matrix to a body's rectangular coordinates.
 *
 * mat -- The rotation matrix.
 * pos -- The coordinates to be rotated in-place.
 */
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

/*
 * Applies a rotation matrix to a body's geocentric equatorial coordinates. This
 * function can be used to apply the IAU2006/2000A precession/nutation matrices.
 *
 * mat -- The rotation matrix.
 * pos -- The coordinates to be rotated in-place.
 */
void rotate_equatorial(double mat[3][3], struct equatorial_coordinates *pos)
{
    struct rectangular_coordinates rec;

    /* We arbitrarily assign the body a radius of 1 unit */
    spherical_to_rectangular(pos->right_ascension, pos->declination,1, &rec);

    rotate_rectangular(mat, &rec);

    pos->right_ascension = reduce_angle(atan2(rec.y, rec.x), TWO_PI);
    pos->declination = asin(rec.z);
}

/*
 * Rotates rectangular coordinates in the ecliptic frame to the equatorial frame.
 *
 * obl -- The obliquity of the ecliptic in radians.
 * pos -- The coordinates to be rotated in-place.
 */
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

/*
 * Correct the coordinates of solar system bodies to account for the 
 * finite speed of light.
 *
 * id  -- Object identifier for <obj>. MERCURY through NEPTUNE for
 *        the planets or NEPTUNE+1 for Pluto.
 * dt  -- Dynamical time of coordinates.
 * obj -- Coordinates of the body in AU. Corrected on exit.
 * ear -- Coordinates of the Earth in AU. Corrected on exit if non-NULL.
 */
void lightcor(int id, struct julian_date *dt,
	      struct rectangular_coordinates *obj,
	      struct rectangular_coordinates *ear)
{
    int i;
    double x,y,z,d0,d1;
    struct julian_date t;

    d0 = 0;
    t = *dt;
    for (i = 0; i < 10; i++) {
	if (ear) {
	    x = obj->x - ear->x;
	    y = obj->y - ear->y;
	    z = obj->z - ear->z;
	} else {
	    x = obj->x;
	    y = obj->y;
	    z = obj->z;
	}
	d1 = sqrt(x*x + y*y + z*z);
	if (fabs(d1 - d0) < 1E-8)
	    break;

	t.date2 -= (d1 - d0)/C_AUPERDAY;
	if (ear)
	    vsop87_coordinates(EARTH, &t, ear);
	if (id <= NEPTUNE)
	    vsop87_coordinates(id, &t, obj);
	else
	    pluto_coordinates(&t, obj);

	d0 = d1;
    }
}
