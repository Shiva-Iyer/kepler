/*
 * equisols.c - Equinox and solstice calculations.
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
 * References: Chapter 27 in Meeus. This implementation interpolates
 * the apparent geocentric longitude of the Sun from the full VSOP87
 * theory to time equinoxes and solstices.
 */

#include <math.h>
#include <equisols.h>
#include <kepler.h>
#include <julian_date.h>
#include <coordinates.h>
#include <vsop87.h>
#include <iau2006_precession.h>
#include <iau2000a_nutation.h>
#include <riseset.h>

/*
 * Calculate the time of the equinox or solstice in the specified year
 * and month.
 *
 * year  -- Year number using astronomical reckoning and after 4800BC.
 * month -- 3 for March equinox, 6 for June solstice, 9 for September
 *          equinox, or 12 for December solstice.
 *
 * Return: Dynamical time in JDE form at which the requested equinox
 *         or solstice occurs; -1 if the input parameters are invalid.
 */
double equisols(int year, int month)
{
    int steps,i;
    double X,w,nutl,nuto,df[24/TIME_STEP+1],lon[24/TIME_STEP+1],
	prec[3][3];
    struct julian_date jd0,jd;
    struct rectangular_coordinates ear,sun = {0,0,0};
    struct ecliptic_coordinates ecl;
    struct equatorial_coordinates equ;

    if (month == 3)
	X = TWO_PI;
    else if (month == 6)
	X = 0.5*PI;
    else if (month == 9)
	X = 1.0*PI;
    else if (month == 12)
	X = 1.5*PI;
    else
	return(-1);

    calendar_to_julian_date(year, month, 20, &jd0);
    jd = jd0;

    steps = sizeof(df)/sizeof(double);
    for (i = 0; i < steps; i++) {
	df[i] = (double)i*TIME_STEP/24;
	jd.date2 = jd0.date2 + df[i];
	vsop87_coordinates(EARTH, &jd, &ear);

	lightcor(EARTH, &jd, &ear, NULL);

	vsop87_ecliptic_to_equator(&ear);

	iau2006_precession_matrix(&jd, 0, prec);
	rotate_rectangular(prec, &ear);

	rectangular_to_spherical(&sun, &ear, &equ.right_ascension,
				 &equ.declination, &w);

	iau2000a_nutation(&jd, &nutl, &nuto);
	equatorial_to_ecliptic(&equ, iau2000a_mean_obliquity(&jd)+nuto,
			       &ecl);
	lon[i] = ecl.longitude + nutl;
    }

    w = 0;
    for (i = 1; i < steps; i++) {
	if (lon[i-1] > 1.5*PI && lon[i] < 0.5*PI)
	    w = TWO_PI;
	lon[i] += w;
    }

    return(jd0.date1 + jd0.date2 + interpolate(steps, lon, df, X));
}
