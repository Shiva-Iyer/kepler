/*
 * earth_figure.c - Routines for calculating various earth figure values
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
#include <earth_figure.h>

/*
 * References: The great circle distance calculation implements Thaddeus
 * Vincenty's algorithm <http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf>.
 */

/*
 * Calculate values related to the oblateness of the Earth.
 *
 * geog_lat   -- The observer's geographic latitude in radians.
 * height_msl -- The observer's height above mean sea level in meters.
 * gc_lat     -- The observer's geocentric latitude in radians.
 * gc_radius  -- The observer's geocentric radius as a fraction of
 *               the Earth's equatorial radius.
 */
void earth_figure_values(double geog_lat, double height_msl,
			 double *gc_lat, double *gc_radius)
{
    double cg,sg,x,s,c;

    sincos(geog_lat, &sg, &cg);
    x = atan2(EARTH_POL_RADIUS*sg, EARTH_EQU_RADIUS*cg);
    sincos(x, &s, &c);

    c = c + (height_msl/EARTH_EQU_RADIUS)*cg;
    s = (1.0-EARTH_FLATTENING)*s + (height_msl/EARTH_EQU_RADIUS)*sg;

    *gc_lat = atan2(s, c);
    *gc_radius = sqrt(c*c + s*s);
}

/*
 * Calculate the great circle distance and bearings between two points,
 * taking into account the oblateness of the Earth.
 *
 * lon1 -- Longitude of pt. 1 in radians, positive east of Greenwich.
 * lat1 -- Latitude of pt. 1 in radians, positive north of the equator.
 * lon2 -- Longitude of pt. 2 in radians, positive east of Greenwich.
 * lat2 -- Latitude of pt. 2 in radians, positive north of the equator.
 * inb  -- The initial bearing on the great circle path in radians.
 * fib  -- The final bearing on the great circle path in radians.
 *
 * Return: The great circle distance in meters or -1 if the algorithm
 *         failed, which usually happens for antipodal points.
 */
double earth_gcdist(double lon1, double lat1, double lon2, double lat2,
		    double *inb, double *fib)
{
    int iter;
    double a,b,f,u1,u2,L,lam0,lam1,cu1,su1,cu2,su2,cl,sl,x,y,sig;

    a = EARTH_EQU_RADIUS;
    b = EARTH_POL_RADIUS;
    f = EARTH_FLATTENING;

    u1 = atan((1.0-f)*tan(lat1));
    u2 = atan((1.0-f)*tan(lat2));
    L = lon2 - lon1;
    lam0 = L;
    for (iter = 0; iter < 20; iter++) {
	sincos(u1, &su1, &cu1);
	sincos(u2, &su2, &cu2);
	sincos(lam0, &sl, &cl);
	x = cu1*su2 - su1*cu2*cl;
	y = cu2*sl;
	y = sqrt(x*x + y*y);
	x = su1*su2 + cu1*cu2*cl;
	sig = atan2(y, x);

	sl = cu1*cu2*sl/y;
	cl = 1 - sl*sl;
	if (cl)
	    cu2 = x - 2.0*su1*su2/cl;
	else
	    cu2 = 0;
	if (isnan(cu2) || !isfinite(cu2))
	    cu2 = 0;

	lam1 = f*cl*(4.0 + f*(4.0 - 3.0*cl))/16.0;
	lam1 = L + (1 - lam1)*f*sl*(sig + lam1*y*(
	       cu2 + lam1*x*(2.0*cu2*cu2 - 1.0)));
	if (fabs(lam1 - lam0) <= 1E-12) {
	    u2 = cl*((a*a)/(b*b) - 1.0);
	    cl = 1.0+u2*(u2*(u2*(320.0-175.0*u2)-768.0)+4096.0)/16384.0;
	    sl = u2*(256.0 + u2*(u2*(74.0 - 47.0*u2) - 128.0))/1024.0;
	    u1 = sl*y*(cu2 + 0.25*sl*(x*(2.0*cu2*cu2 - 1.0) -
		   sl/6.0*cu2*(4.0*y*y - 3.0)*(4.0*cu2*cu2 - 3.0)));

	    sincos(atan((1.0-f)*tan(lat1)), &su1, &cu1);
	    sincos(atan((1.0-f)*tan(lat2)), &su2, &cu2);
	    sincos(lon2 - lon1, &y, &x);

	    *inb = atan2(cu2*y, cu1*su2 - su1*cu2*x);
	    *fib = atan2(cu1*y, cu1*su2*x - su1*cu2);
	    return(b*cl*(sig-u1));
	}

	lam0 = lam1;
    }

    *inb = *fib = -1;
    return(-1);
}
