/*
 * eclipse.c - Routines to compute the time of solar and lunar eclipses
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
 * References: Chapter 54 of Meeus. The algorithm does not yield high
 * accuracy or the local circumstances for solar eclipses. 
 */

#include <math.h>
#include <kepler.h>
#include <julian_date.h>
#include <eclipse.h>

/*
 * Calculate the date and time of solar and lunar eclipses.
 *
 * dt   -- Dynamical time at which the search is to commence.
 * type -- The type of eclipse to be searched for.
 * g    -- On exit, extra eclipse data. See Meeus for details on gamma.
 * u    -- On exit, additional eclipse data. See Meeus for details.
 *
 * Return: Dynamical time in JDE form corresponding to the time of the
 *         next <type> eclipse on or after <dt> at its maximum.
 */
double eclipse(struct julian_date *dt, enum EclipseTypes type,
	       double *g, double *u)
{
    double k,T,J,E,M,N,F,O,A,P,Q;

    k = round((dt->date1+dt->date2-J2000_EPOCH)*12.3685/365.25);
    if (type == Lunar)
	k += 0.50;

    do {
	T = k/1236.85;
	F = (160.7108 + 390.67050284*k + (-0.0016118*T +
	    (-2.27E-6*T + 1.1E-8*T*T)*T)*T)*DEG_TO_RAD;
	k += 1.0;
    } while(fabs(sin(F)) > 0.36);

    k -= 1.0;
    J = 2451550.09766 + 29.530588861*k +
	(1.5437E-4*T + (-1.50E-7*T + 7.3E-10*T*T)*T)*T;

    E = 1.0 - (2.516E-3 + 7.4E-6*T)*T;

    M = (2.5534 + 29.10535670*k - (1.4E-6 + 1.1E-7*T)*T*T)*DEG_TO_RAD;
    N = (201.5643 + 385.81693528*k + (0.0107582*T +
        (1.238E-5*T - 5.8E-8*T*T)*T)*T)*DEG_TO_RAD;
    O = (124.7746 - 1.56375588*k + (2.0672E-3 + 2.15E-6*T)*T*T)*DEG_TO_RAD;
    F = F - 0.02665*sin(O)*DEG_TO_RAD;
    A = (299.77 + 0.107408*k - 9.173E-3*T*T)*DEG_TO_RAD;

    J += ((type == Solar) ? -0.4075 : -0.4065)*sin(N) +
	((type == Solar) ? 0.1721 : 0.1727)*E*sin(M) +
	0.0161*sin(2*N) - 9.7E-3*sin(2*F) + 7.3E-3*E*sin(N-M) -
	5.0E-3*E*sin(N+M) - 2.3E-3*sin(N-2*F) + 2.1E-3*E*sin(2*M) +
	1.2E-3*sin(N+2*F) + 6E-4*E*sin(2*N+M) - 4E-4*sin(3*N) -
	3E-4*(E*sin(M+2*F) - sin(A)) - 2E-4*(E*sin(M-2*F) +
	E*sin(2*N-M) + sin(O));

    P = 0.2070*E*sin(M) + 2.4E-3*E*sin(2*M) - 0.0392*sin(N) +
	0.0116*sin(2*N) - 7.3E-3*E*sin(N+M) + 6.7E-3*E*sin(N-M) +
	0.0118*sin(2*F);
    Q = 5.2207 - 4.8E-3*E*cos(M) + 2.0E-3*E*cos(2*M) -
	0.3299*cos(N) - 6.0E-3*E*cos(N+M) + 4.1E-3*E*cos(N-M);

    *g = (P*cos(F) + Q*sin(F))*(1 - 4.8E-3*fabs(cos(F)));
    *u = 5.9E-3 + 4.6E-3*E*cos(M) - 0.0182*cos(N) +
	4E-4*cos(2*N) - 5E-4*cos(M+N);
    return(J);
}
