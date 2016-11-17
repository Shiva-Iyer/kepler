/*
 * moonphase.c - Routines to compute the phases of the Moon
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
 * References: Chapters 48 & 49 of Meeus for algorithms to compute the
 * illuminated fraction of the Moon's disk and the dates of Moon phases.
 */

#include <math.h>
#include <kepler.h>
#include <julian_date.h>
#include <moonphase.h>

/*
 * Calculate the fraction of the Moon's disk that is illuminated.
 * 
 * dt    -- Dynamical time for computation.
 *
 * Return: A fraction in the range [0,1] indicating the portion of
 *         the Moon's disk illuminated at <dt>.
 */
double moonlit(struct julian_date *dt)
{
    double T,D,M,N,i;

    T = JULIAN_CENTURIES(dt->date1, dt->date2);

    D = 297.8501921 + (445267.1114034 + (-0.0018819 +
	(1.0/545868.0 - 1.0/113065000.0*T)*T)*T)*T;
    M = 357.5291092 + (35999.0502909 +
        (-0.0001536 + 1.0/24490000.0*T)*T)*T;
    N = 134.9633964 + (477198.8675055 + (0.0087414 +
	(1.0/69699.0 - 1.0/14712000.0*T)*T)*T)*T;

    i = 180 - D - 6.289*sin(N*DEG_TO_RAD) + 2.100*sin(M*DEG_TO_RAD) -
	1.274*sin((2*D-N)*DEG_TO_RAD) - 0.658*sin(2*D*DEG_TO_RAD) -
	0.214*sin(2*N*DEG_TO_RAD) - 0.110*sin(D*DEG_TO_RAD);
    return(round((1 + cos(i*DEG_TO_RAD))*50)/100);
}

/*
 * Calculate the date and time of the specified Moon phase.
 *
 * dt    -- Dynamical time at which the search is to commence.
 * phase -- Enumeration that identifies the phase to be computed.
 *
 * Return: Dynamical time in JDE form corresponding to the time of the
 *         next Moon <phase> on or after <dt>.
 */
double moonphase(struct julian_date *dt, enum MoonPhases phase)
{
    short i;
    double k,T,J,E,M,N,F,O,C1,C2,C3;
    static double A0[14] = {299.77,251.88,251.83,349.42,84.66,141.74,207.14,
			    154.84,34.52,207.19,291.34,161.72,239.56,331.55};
    static double A1[14] = {0.107408,0.016321,26.651886,36.412478,18.206239,
			    53.303771,2.453732,7.306860,27.261239,0.121824,
			    1.844379,24.198154,25.513099,3.592518};
    static short Ac[14] = {325,165,164,126,110,62,60,56,47,42,40,37,35,23};

    k = round((dt->date1+dt->date2-J2000_EPOCH)*12.3685/365.25);
    if (phase == FirstQuarter)
	k += 0.25;
    else if (phase == FullMoon)
	k += 0.50;
    else if (phase == LastQuarter)
	k += 0.75;

    T = k/1236.85;

    J = 2451550.09766 + 29.530588861*k +
	(1.5437E-4*T + (-1.50E-7*T + 7.3E-10*T*T)*T)*T;

    E = 1.0 - (2.516E-3 + 7.4E-6*T)*T;

    M = (2.5534 + 29.10535670*k - (1.4E-6 + 1.1E-7*T)*T*T)*DEG_TO_RAD;
    N = (201.5643 + 385.81693528*k + (0.0107582*T +
        (1.238E-5*T - 5.8E-8*T*T)*T)*T)*DEG_TO_RAD;
    F = (160.7108 + 390.67050284*k + (-0.0016118*T +
        (-2.27E-6*T + 1.1E-8*T*T)*T)*T)*DEG_TO_RAD;
    O = (124.7746 - 1.56375588*k + (0.0020672 + 2.15E-6*T)*T*T)*DEG_TO_RAD;

    if (phase == NewMoon) {
	C1 = -0.40720*sin(N) + 0.17241*E*sin(M) + 0.01608*sin(2*N) +
	     0.01039*sin(2*F) + 7.39E-3*E*sin(N-M) - 5.14E-3*E*sin(N+M) +
	     2.08E-3*E*E*sin(2*M);
	C2 = 0;
    } else if (phase == FullMoon) {
	C1 = -0.40614*sin(N) + 0.17302*E*sin(M) + 0.01614*sin(2*N) +
	     0.01043*sin(2*F) + 7.34E-3*E*sin(N-M) - 5.15E-3*E*sin(N+M) +
	     2.09E-3*E*E*sin(2*M);
	C2 = 0;
    } else {
	C1 = -0.62801*sin(N) + (0.17172*sin(M) - 0.01183*sin(N+M))*E +
	     8.62E-3*sin(2*N) + 8.04E-3*sin(2*F) + (4.54E-3*sin(N-M) +
	     2.04E-3*E*sin(2*M))*E - 1.8E-3*sin(N-2*F) - 7E-4*sin(N+2*F) -
	     4E-4*sin(3*N) + (-3.4E-4*sin(2*N-M) + 3.2E-4*sin(M+2*F) +
	     3.2E-4*sin(M-2*F) - 2.8E-4*E*sin(N+2*M) + 2.7E-4*sin(2*N+M))*E -
	     1.7E-4*sin(O) - 5E-5*sin(N-M-2*F) + 4E-5*(sin(2*N+2*F) -
	     sin(N+M+2*F) + sin(N-2*M)) + 3E-5*(sin(N+M-2*F) + sin(3*M)) +
	     2E-5*(sin(2*N-2*F) + sin(N-M+2*F) - sin(3*N+M));

	C2 = 3.06E-3 - 3.8E-4*E*cos(M) + 2.6E-4*cos(N) -
	     2E-5*(cos(N-M) - cos(N+M) - cos(2*F));
	if (phase == LastQuarter)
	    C2 *= -1;
    }

    if (phase == NewMoon || phase == FullMoon) {
	C1 += -1.11E-3*sin(N-2*F) - 5.7E-4*sin(N+2*F) + 5.6E-4*E*sin(2*N+M) -
	      4.2E-4*sin(3*N) + (4.2E-4*sin(M+2*F) + 3.8E-4*sin(M-2*F) -
	      2.4E-4*sin(2*N-M))*E - 1.7E-4*sin(O) - 7E-5*sin(N+2*M) +
	      4E-5*(sin(2*N-2*F) + sin(3*M)) + 3E-5*(sin(N+M-2*F) +
	      sin(2*N+2*F) - sin(N+M+2*F) + sin(N-M+2*F)) -
	      2E-5*(sin(N-M-2*F) + sin(3*N+M) - sin(4*N));
    }

    C3 = 0;
    A0[0] -= 9.173E-3*T*T;
    for (i = 0; i < 14; i++)
	C3 += sin((A0[i] + A1[i]*k)*DEG_TO_RAD)*Ac[i];
    C3 /= 1E6;

    return(J + C1 + C2 + C3);
}
