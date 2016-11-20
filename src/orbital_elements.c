/*
 * orbital_elements.c - Routines to generate ephemeris from orbital elements
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

/*
 * Reference 1: "An improved algorithm due to Laguerre for the solution of 
 *              Kepler's equation" - Bruce Conway
 *              <http://www.springerlink.com/content/p122000960815647/fulltext.pdf>
 *
 * Reference 2: "An efficient method for solving Barker's equation" - R. Meire
 *              <http://articles.adsabs.harvard.edu/full/1985JBAA...95..113M>
 */

#include <math.h>
#include <orbital_elements.h>

u_short kepler_equation_max_iters = KEPLER_MAX_ITER;

double kepler_equation_precision = KEPLER_PRECISION;

#ifdef _DEBUG
u_short kepler_equation_iters = 0;
#endif

/*
 * Calculates the heliocentric rectangular coordinates of a celestial 
 * body given its osculating orbital elements.
 *
 * tt  -- TT to be used for calculations.
 * elt -- The body's osculating orbital elements.
 * eph -- The body's heliocentric rectangular coordinates in AU. The
 *        reference frame is the equinox & ecliptic of J2000.
 *
 * Return: SUCCESS -- If the coordinates were calculated successfully.
 *         ERR_INVALID_ECCENTRICITY -- If the eccentricity is negative.
 *         ERR_CONVERGENCE -- If Kepler's equation didn't converge to the
 *         required precision within the specified number of iterations.
 */
int elements_to_ephemeris(struct julian_date *tt,
			  struct orbital_elements *elt,
			  struct rectangular_coordinates *eph)
{
    int retval;
    double t,ma,ea,ha,w,y,x,r,ta,a,b,c,d;

    t = (tt->date1 - elt->epoch.date1) + (tt->date2 - elt->epoch.date2);

    if (elt->eccentricity < 0) {
	return(ERR_INVALID_ECCENTRICITY);
    } else if (elt->eccentricity == 0) {
	/* Simple case of a circular orbit */
	r  = elt->perihelion;
	ta = elt->mean_anomaly + elt->daily_motion*t;
    } else if (elt->eccentricity < PARABOLIC_ORBIT) {
	/* Solve for elliptic orbits */
	ma = reduce_angle(elt->mean_anomaly+elt->daily_motion*t, TWO_PI);

	retval = eccentric_anomaly(ma, elt->eccentricity, &ea);
	if (retval != SUCCESS)
	    return(retval);

	sincos(ea / 2, &x, &y);

	r  = elt->perihelion * (1.0 - elt->eccentricity*(y*y - x*x)) /
	    (1.0 - elt->eccentricity);

	ta = atan2(sqrt((1.0 + elt->eccentricity) /
			(1.0 - elt->eccentricity))*x, y)*2;
    } else if (elt->eccentricity == PARABOLIC_ORBIT) {
	/* Solve Barker's equation for parabolic orbits (Reference 2) */
	w = (t * GAUSS_GRAV_CONSTANT * 1.5)/sqrt(2);
	y = cbrt(w + sqrt(w*w + 1));
	x = y - 1/y;

	r  = elt->perihelion*(x*x + 1);
	ta = atan(x)*2;
    } else {
	/* Solve Kepler's equation for hyperbolic orbits */
	ma = reduce_angle(elt->mean_anomaly+elt->daily_motion*t, TWO_PI);

	retval = hyperbolic_anomaly(ma, elt->eccentricity, &ha);
	if (retval != SUCCESS)
	    return(retval);

	r  = elt->perihelion * (1 - elt->eccentricity*cosh(ha));

	ta = atan2(sqrt(elt->eccentricity*elt->eccentricity - 1) *
		   sinh(ha), elt->eccentricity - cosh(ha));
    }

    sincos(ta + elt->arg_perihelion, &a, &b);
    sincos(elt->lon_asc_node, &c, &d);
    sincos(elt->inclination, &x, &y);

    eph->x = r * (d * b - c * a * y);
    eph->y = r * (c * b + d * a * y);
    eph->z = r * a * x;

    return(SUCCESS);
}

/*
 * Solves Kepler's equation for elliptic orbits and returns the resulting
 * value of the eccentric anomaly.
 *
 * mean_ano -- The celestial body's mean anomaly in radians.
 * ecc -- The eccentricity of the orbit.
 * ecc_ano -- On success, the body's eccentric anomaly in radians.
 *
 * Return: SUCCESS -- If Kepler's equation was solved successfully.
 *         ERR_CONVERGENCE -- If Kepler's equation didn't converge to the
 *         required precision within the specified number of iterations.
 */
int eccentric_anomaly(double mean_ano, double ecc, double *ecc_ano)
{
    u_short i;
    const u_short N = 6;
    double ea,dea,c_ea,s_ea,x,y;

    /*
     * Solve Kepler's equation for elliptic orbits using the 
     * Laguerre method (Reference 1).
     */
    ea = mean_ano + ecc*sin(mean_ano);

    for (i = 0; i < kepler_equation_max_iters; i++) {
	sincos(ea, &s_ea, &c_ea);

	dea = N * (mean_ano - ea  + ecc*s_ea);
	x = 1 - ecc * c_ea;
	y = sqrt(fabs((N - 1)*((N - 1)*x*x + ecc*s_ea*dea)));

	if (fabs(x + y) > fabs(x - y))
	    dea = dea/(x + y);
	else
	    dea = dea/(x - y);

	ea += dea;
	if (fabs(dea) < kepler_equation_precision)
	    break;
    }

#ifdef _DEBUG
    kepler_equation_iters = (i < kepler_equation_max_iters) ?
	i + 1 : kepler_equation_max_iters;
#endif

    *ecc_ano = ea;
    return(i < kepler_equation_max_iters ? SUCCESS : ERR_CONVERGENCE);
}

/*
 * Solves Kepler's equation for hyperbolic orbits and returns the
 * resulting value of the hyperbolic anomaly.
 *
 * mean_ano -- The celestial body's mean anomaly in radians.
 * ecc -- The eccentricity of the orbit.
 * hyp_ano -- On success, the body's hyperbolic anomaly in radians.
 *
 * Return: SUCCESS -- If Kepler's equation was solved successfully.
 *         ERR_CONVERGENCE -- If Kepler's equation didn't converge to the
 *         required precision within the specified number of iterations.
 */
int hyperbolic_anomaly(double mean_ano, double ecc, double *hyp_ano)
{
    u_short i;
    double ha,dha,x,y;
    const u_short N = 6;

    /*
     * Solve Kepler's equation for hyperbolic orbits using the
     * Laguerre method (Reference 1).
     */
    ha = mean_ano;
    for (i = 0; i < kepler_equation_max_iters; i++) {
	dha = N * (mean_ano + ha - ecc * sinh(ha));
	x = ecc * cosh(ha) - 1;
	y = sqrt(fabs((N - 1)*((N - 1)*x*x + dha*ecc*sinh(ha))));
 
	if (fabs(x + y) > fabs(x - y))
	    dha = dha/(x + y);
	else
	    dha = dha/(x - y);

	ha += dha;
	if (fabs(dha) < kepler_equation_precision)
	    break;
    }

#ifdef _DEBUG
    kepler_equation_iters = (i < kepler_equation_max_iters) ?
	i + 1 : kepler_equation_max_iters;
#endif

    *hyp_ano = ha;
    return(i < kepler_equation_max_iters ? SUCCESS : ERR_CONVERGENCE);
}
