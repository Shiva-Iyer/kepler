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
 * inb  -- On exit, the initial bearing on the great circle in radians,
 *         measured clockwise from true north.
 * fib  -- On exit, the final bearing on the great circle in radians,
 *         measured clockwise from true north.
 *
 * Return: The great circle distance in meters or -1 if the algorithm
 *         failed, which usually happens for antipodal points.
 */
double earth_gcdist(double lon1, double lat1, double lon2, double lat2,
		    double *inb, double *fib)
{
    short iter;
    double a,b,f,u1,u2,L,lam0,lam1,cu1,su1,cu2,su2,cl,sl,x,y,sig;

    a = EARTH_EQU_RADIUS;
    b = EARTH_POL_RADIUS;
    f = EARTH_FLATTENING;

    u1 = atan2((1.0 - f)*sin(lat1), cos(lat1));
    u2 = atan2((1.0 - f)*sin(lat2), cos(lat2));
    sincos(u1, &su1, &cu1);

    L = lon2 - lon1;
    lam0 = L;

    for (iter = 0; iter < 20; iter++) {
	sincos(u2, &su2, &cu2);
	sincos(lam0, &sl, &cl);
	x = cu1*su2 - su1*cu2*cl;
	y = cu2*sl;
	y = sqrt(x*x + y*y);
	x = su1*su2 + cu1*cu2*cl;
	sig = atan2(y, x);

	sl = cu1*cu2*sl/y;
	cl = 1.0 - sl*sl;
	if (cl)
	    cu2 = x - 2.0*su1*su2/cl;
	else
	    cu2 = 0;
	if (isnan(cu2) || !isfinite(cu2))
	    cu2 = 0;

	lam1 = f*cl*(4.0 + f*(4.0 - 3.0*cl))/16.0;
	lam1 = L + (1.0 - lam1)*f*sl*(sig + lam1*y*(
	       cu2 + lam1*x*(2.0*cu2*cu2 - 1.0)));
	if (fabs(lam1 - lam0) <= 1E-12) {
	    u2 = cl*((a*a)/(b*b) - 1.0);
	    cl = 1.0+u2*(u2*(u2*(320.0-175.0*u2)-768.0)+4096.0)/16384.0;
	    sl = u2*(256.0 + u2*(u2*(74.0 - 47.0*u2) - 128.0))/1024.0;
	    u1 = sl*y*(cu2 + 0.25*sl*(x*(2.0*cu2*cu2 - 1.0) -
		   sl*cu2*(4.0*y*y - 3.0)*(4.0*cu2*cu2 - 3.0)/6.0));

	    sincos(atan((1.0 - f)*tan(lat1)), &su1, &cu1);
	    sincos(atan((1.0 - f)*tan(lat2)), &su2, &cu2);
	    sincos(lam1, &y, &x);

	    *inb = reduce_angle(atan2(cu2*y, cu1*su2-su1*cu2*x), TWO_PI);
	    *fib = reduce_angle(atan2(cu1*y, cu1*su2*x-su1*cu2), TWO_PI);
	    return(b*cl*(sig-u1));
	}

	lam0 = lam1;
    }

    *inb = *fib = -1;
    return(-1);
}

/*
 * Calculate the location on the great circle that lies at the given
 * distance and initial bearing from a starting location.
 *
 * lon1 -- Longitude of the starting point in radians (E positive).
 * lat1 -- Latitude of the starting point in radians (N positive).
 * inb  -- The initial bearing on the great circle in radians, measured
 *         clockwise from true north.
 * dist -- The distance along the great circle in meters.
 * lon2 -- On exit, longitude of the destination in radians.
 * lat2 -- On exit, latitude of the destination in radians.
 *
 * Return: The final bearing in radians or -1 if the algorithm failed,
 *         which usually happens for antipodal points.
 */
double earth_gcend(double lon1, double lat1, double inb, double dist,
		   double *lon2, double *lat2)
{
    short iter;
    double a,b,c,f,s,u,x,y,sig1,sig2,dsig,t;

    a = EARTH_EQU_RADIUS;
    b = EARTH_POL_RADIUS;
    f = EARTH_FLATTENING;

    u = (1.0 - f)*tan(lat1);
    sincos(inb, &s, &c);
    sig1 = atan2(u, c);

    u = ((a*a)/(b*b) - 1.0)*(1.0 - s*s/(1.0 + u*u));
    x = 1.0 + u*(u*(u*(320.0 - 175.0*u) - 768.0) + 4096.0)/16384.0;
    y = u*(256.0 + u*(u*(74.0 - 47.0*u) - 128.0))/1024.0;
    sig2 = dist/(b*x);

    for (iter = 0; iter < 20; iter++) {
	u = cos(2*sig1 + sig2);
	sincos(sig2, &s, &c);
	dsig = y*s*(u + y*(c*(2*u*u - 1.0) -
	       y*u*(4.0*s*s - 3.0)*(4.0*u*u - 3.0)/6.0)/4.0);
	t = sig2;
	sig2 = dist/(b*x) + dsig;
	if (fabs(sig2 - t) <= 1E-12) {
	    x = (1.0 - f)*tan(lat1);
	    t = 1.0/sqrt(1.0 + x*x);
	    sincos(inb, &sig1, &dsig);
	    a = sig1*t;
	    b = 1.0 - a*a;
	    sincos(sig2, &s, &c);
	    y = t*(x*s - c*dsig);
	    *lat2 = atan2(t*(x*c + s*dsig),(1.0 - f)*sqrt(a*a + y*y));

	    *lon2 = atan2(s*sig1, t*(c - x*s*dsig));
	    t = f*b*(4.0 + f*(4.0 - 3.0*b))/16.0;
	    *lon2 += lon1-(1.0-t)*f*a*(sig2+t*s*(u+t*c*(2.0*u*u-1.0)));
	    *lon2 = reduce_angle(*lon2 + 3.0*PI, TWO_PI) - PI;

	    return(reduce_angle(atan2(a, -y), TWO_PI));
	}
    }

    *lon2 = lon1;
    *lat2 = lat1;
    return(-1);
}
