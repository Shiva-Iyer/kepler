/*
 * kepler_test.c - Utility to test kepler functions
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <kepler.h>
#include <julian_date.h>
#include <sidereal_time.h>
#include <coordinates.h>
#include <vsop87.h>
#include <elp82b.h>
#include <pluto.h>
#include <iau2006_precession.h>
#include <iau2000a_nutation.h>
#include <mpc_file.h>
#include <aberration.h>
#include <moonphase.h>
#include <eclipse.h>
#include <equisols.h>
#include <earth_figure.h>

void display_usage()
{
    printf("Usage: kepler_test [OPTION]\n");
    printf("Test all the functions in the kepler library\n\n");
    printf("  -a, --all     display all minor planets and comets\n");
    printf("  -h, --help    display this help screen and exit\n");
    printf("  -v, --version display version number and exit\n");
}

void parse_command_line(int argc, char *argv[], int *show_all)
{
    *show_all = 0;
    if (argc > 1) {
	if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
	    display_usage();
	    exit(0);
	}

	if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
	    printf(PROG_VERSION_STRING);
	    printf(PROG_COPYRIGHT);
	    exit(0);
	}

	if (!strcmp(argv[1], "-a") || !strcmp(argv[1], "--all"))
	    *show_all = 1;
    }
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i,year,month,day,retval,show_all;
    struct julian_date jd;
    struct equatorial_coordinates eq;
    struct rectangular_coordinates xyz[9],zero = {0, 0, 0},moon;
    struct mpc_body inf;
    double dist,df,epsilon,d_psi,d_eps,prec_matrix[3][3],nut_matrix[3][3],
	mst,ast,d_ra,d_dec,gamma,u,inb,fib,lon,lat;
    char buf[256];
    static char *planet_names[] = {"Mercury","Venus","Earth","Mars",
				   "Jupiter","Saturn","Uranus","Neptune",
				   "Pluto"};
    static char *mpc_files[] = {"MPCORB.DAT", "COMET.DAT"};

    parse_command_line(argc, argv, &show_all);

    calendar_to_julian_date(2000, 1, 1, &jd);
    printf("\nJulian date for 2000-01-01 00:00:00 UT = %10.2f\n",
	   jd.date1 + jd.date2);

    jd.date1 = J2000_EPOCH;
    jd.date2 = 0;
    julian_to_calendar_date(&jd, &year, &month, &day, &df);
    printf("\nGregorian date for Julian date %10.2f = "
	   "%04d-%02d-%02d %6.4f\n", jd.date1+jd.date2,
	   year, month, day, df);

    epsilon = iau2000a_mean_obliquity(&jd);
    printf("\nMean obliquity of the ecliptic on 2000-01-01"
	   " 12:00:00 TDB = %6.4f radians\n", epsilon);

    iau2000a_nutation(&jd, &d_psi, &d_eps);
    printf("\nNutation on 2000-01-01 12:00:00 TDB (in rad):"
	   " longitude = %13.10f, obliquity = %13.10f\n", d_psi, d_eps);

    jd.date1 = 2455200.50; 
    jd.date2 = 0;
    printf("\nHeliocentric equatorial coordinates for "
	   "2010-01-04 00:00:00 TDB\n");
    printf("x (AU), y (AU), z (AU)\n\n");
    for (i = MERCURY; i <= NEPTUNE + 1; i++) {
	if (i == NEPTUNE + 1) {
	    pluto_coordinates(&jd, &xyz[i]);
	} else {
	    vsop87_coordinates(i, &jd, &xyz[i]);
	    vsop87_ecliptic_to_equator(&xyz[i]);
	}

	printf("%10s: %8.4f, %8.4f, %8.4f\n",
	       planet_names[i], xyz[i].x, xyz[i].y, xyz[i].z);
    }

    elp82b_coordinates(&jd, &moon);
    elp82b_ecliptic_to_equator(&moon);
    printf("\nGeocentric rectangular coordinates for "
	   "2010-01-04 00:00:00 TDB\n");
    printf("x (KM), y (KM), z (KM)\n\n");
    printf("%10s: %8.4f, %8.4f, %8.4f\n","Moon", moon.x, moon.y, moon.z);

    printf("\nEquatorial coordinates for 2010-01-04 00:00:00 TDB\n");
    printf("RA (hours), Dec. (degrees), distance\n\n");
    for (i = MERCURY; i <= NEPTUNE + 1; i++) {
	if (i == 2)
	    continue;

	if (i == 0) {
	    rectangular_to_spherical(&zero, &xyz[2],
				     &eq.right_ascension,
				     &eq.declination, &dist);
	    printf("%10s: %8.4f, %8.4f, %8.4f AU\n","Sun",
		   eq.right_ascension*RAD_TO_HRS,
		   eq.declination*RAD_TO_DEG, dist);

	    rectangular_to_spherical(&moon, &zero,
				     &eq.right_ascension,
				     &eq.declination, &dist);
	    printf("%10s: %8.4f, %8.4f, %8.1f KM\n","Moon", 
		   eq.right_ascension*RAD_TO_HRS,
		   eq.declination*RAD_TO_DEG, dist);
	}

	rectangular_to_spherical(&xyz[i], &xyz[2], &eq.right_ascension,
				 &eq.declination, &dist);
	printf("%10s: %8.4f, %8.4f, %8.4f AU\n",
	       planet_names[i], eq.right_ascension*RAD_TO_HRS,
	       eq.declination*RAD_TO_DEG, dist);
    }

    jd.date1 = 2469807.50;
    jd.date2 = 0;
    iau2006_precession_matrix(&jd, 0, prec_matrix);
    printf("\nPrecession matrix for J2050 TDB\n\n");
    for (i = 0; i < 3; i++) {
	printf("\t%13.10f\t%13.10f\t%13.10f\n",
	       prec_matrix[i][0], prec_matrix[i][1], prec_matrix[i][2]);
    }

    iau2000a_nutation_matrix(&jd, nut_matrix);
    printf("\nNutation matrix for J2050 TDB\n\n");
    for (i = 0; i < 3; i++) {
	printf("\t%13.10f\t%13.10f\t%13.10f\n",
	       nut_matrix[i][0], nut_matrix[i][1], nut_matrix[i][2]);
    }

    eq.right_ascension = DEGREES(2, 31, 48.704) * HRS_TO_RAD;
    eq.declination     = DEGREES(89, 15, 50.72) * DEG_TO_RAD;
    printf("\nCoordinates of Polaris:      RA = %6.4f, Dec = %6.4f"
	   " rad (epoch & equinox J2000)\n\n",
	   eq.right_ascension, eq.declination);

    eq.right_ascension += (50.0 * DEGREES(0, 0, 0.19877) * HRS_TO_RAD);
    eq.declination     -= (50.0 * DEGREES(0, 0, 0.0152) * DEG_TO_RAD);
    printf("\tAfter proper motion: RA = %6.4f, Dec = %6.4f"
	   " rad (epoch 2050 & equinox J2000)\n",
	   eq.right_ascension, eq.declination);

    rotate_equatorial(prec_matrix, &eq);
    printf("\tAfter precession:    RA = %6.4f, Dec = %6.4f"
	   " rad (epoch & equinox J2050)\n",
	   eq.right_ascension, eq.declination);

    rotate_equatorial(nut_matrix, &eq);
    printf("\tAfter nutation:      RA = %6.4f, Dec = %6.4f"
	   " rad (epoch & equinox J2050)\n",
	   eq.right_ascension, eq.declination);

    jd.date1 = 2455246.0;
    jd.date2 = 0.34723;
    mst = mean_sidereal_time(&jd, &jd,
			     -DEGREES(71,3,42)*DEG_TO_RAD)*RAD_TO_HRS;
    printf("\nMean sidereal time at Boston, MA on 2010-02-18"
	   " 15:20:00 EST     = %6.4f hours\n", mst);

    ast = apparent_sidereal_time(&jd, &jd,
				 -DEGREES(71,3,42)*DEG_TO_RAD)*RAD_TO_HRS;
    printf("\nApparent sidereal time at Boston, MA on 2010-02-18"
	   " 15:20:00 EST = %6.4f hours\n", ast);

    eq.right_ascension = DEGREES(2, 44, 12.9747)*HRS_TO_RAD;
    eq.declination     = DEGREES(49, 13, 39.896)*DEG_TO_RAD;

    jd.date1 = 2462088.69;
    jd.date2 = 0;
    annual_aberration(&jd, &eq, &d_ra, &d_dec);
    printf("\nAnnual aberration for Theta-Persei on 2028-11-13.19 TDB:"
	   " RA = %8.6f, Dec = %8.6f (rad)\n", d_ra, d_dec);

    jd.date1 = 2455200.50;
    jd.date2 = 0;
    printf("\nHeliocentric ecliptic coordinates for"
	   " 2010-01-04 00:00:00 TT\n");
    printf("MP|LP|SP|DC ID [name]: eccentricity, x (AU), y (AU), z (AU)"
	   ", absolute magnitude\n");
    printf("MP = Minor planet, LP = Long-period comet, "
	   "SP = Short-period comet, DC = Defunct comet\n\n");

    for (i = 0; i < sizeof(mpc_files)/sizeof(mpc_files[0]); i++) {
	fp = fopen(mpc_files[i], "r");
	if (fp) {
	    while (fgets(buf, 256, fp)) {
		if (i == 0)
		    retval = minor_planet_info(buf, &jd, &inf);
		else if (i == 1)
		    retval = comet_info(buf, &jd, &inf);

		printf("\t%s %11s [%35s]: %9.7f, %13.9f, "
		       "%13.9f, %13.9f, %5.2f",
		       inf.type == MINOR_PLANET ? "MP" :
		       inf.type == LONG_PERIOD_COMET ? "LP" :
		       inf.type == SHORT_PERIOD_COMET ? "SP" : "DC",
		       inf.id, inf.name, inf.elements.eccentricity,
		       inf.position.x, inf.position.y,
		       inf.position.z, inf.magnitude);

		if (retval == SUCCESS)
		    printf("\n");
		else
		    printf(" error = %d\n", retval);

		if (!show_all)
		    break;
	    }

	    fclose(fp);
	} else
	    printf("\nError opening %s\n", mpc_files[i]);
    }

    calendar_to_julian_date(1992, 4, 12, &jd);
    printf("\nIlluminated fraction of the Moon's disk on"
	   " 1992-04-12 00:00:00 DT = %f\n", moonlit(&jd));

    calendar_to_julian_date(1977, 2, 15, &jd);
    printf("\nTime of New Moon in February 1977  = %f DT\n",
	   moonphase(&jd, NewMoon));

    calendar_to_julian_date(2044, 1, 1, &jd);
    printf("\nTime of first last quarter in 2044 = %f DT\n",
	   moonphase(&jd, LastQuarter));

    calendar_to_julian_date(1993, 5, 21, &jd);
    df = eclipse(&jd, Solar, &gamma, &u);
    printf("\nSolar eclipse of 1993-05-21: %f DT,"
	   " gamma = %7.4f, u = %7.4f\n", df, gamma, u);
    
    calendar_to_julian_date(2009, 7, 22, &jd);
    df = eclipse(&jd, Solar, &gamma, &u);
    printf("\nSolar eclipse of 2009-07-22: %f DT,"
	   " gamma = %7.4f, u = %7.4f\n", df, gamma, u);

    calendar_to_julian_date(1973, 6, 1, &jd);
    df = eclipse(&jd, Lunar, &gamma, &u);
    printf("\nLunar eclipse in June 1973 : %f DT,"
	   " gamma = %7.4f, u = %7.4f\n", df, gamma, u);

    calendar_to_julian_date(1997, 7, 1, &jd);
    df = eclipse(&jd, Lunar, &gamma, &u);
    printf("\nLunar eclipse in July 1997 : %f DT,"
	   " gamma = %7.4f, u = %7.4f\n", df, gamma, u);

    printf("\nEquinoxes and solstices in 1962:\n\n");
    for (i = 3; i <= 12; i += 3)
	printf("\t%02d: %f DT\n", i, equisols(1962, i));

    dist = earth_gcdist(DEGREES(174, 47, 30)*DEG_TO_RAD,
			-DEGREES(37, 0, 29)*DEG_TO_RAD,
			DEGREES(55, 21, 52)*DEG_TO_RAD,
			DEGREES(25, 15, 10)*DEG_TO_RAD, &inb, &fib);
    printf("\nAKL-DXB airport GC dist. = %.3f KM, "
	   "init. bearing = %.1f deg., fin. bearing = %.1f deg.\n",
	   dist/1000, reduce_angle(inb*RAD_TO_DEG, 360),
	   reduce_angle(fib*RAD_TO_DEG, 360));

    dist = 13804;
    inb = 248.8;
    fib = earth_gcend(-DEGREES(97, 2, 16)*DEG_TO_RAD,
		      DEGREES(32, 53, 50)*DEG_TO_RAD,
		      inb*DEG_TO_RAD, dist*1000, &lon, &lat);
    printf("\n%.0f KM at %6.2f deg from DFW airport ->  "
	   "Lon. %7.2f, Lat. %7.2f at %7.2f deg\n", dist, inb,
	   lon*RAD_TO_DEG, lat*RAD_TO_DEG, fib*RAD_TO_DEG);

    printf("\n");
    return(0);
}
