/*
 * kepler_test.c - Testbed for all the kepler routines
 * Copyright (C) 2010-2011 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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

void display_copyright()
{
	printf(PROG_VERSION_STRING);
	printf(PROG_COPYRIGHT);
}

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
			display_copyright();
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
	double dist,df,epsilon,d_psi,d_eps,prec_matrix[3][3],nut_matrix[3][3],mst,ast,d_ra,d_dec;
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
	printf("\nGregorian date for Julian date %10.2f = %04d-%02d-%02d %10.8f\n",
		jd.date1 + jd.date2, year, month, day, df);

	epsilon = iau2000a_mean_obliquity(&jd);
	printf("\nMean obliquity of the ecliptic on 2000-01-01 12:00:00 TDB = %13.10f radians\n",
		epsilon);

	iau2000a_nutation(&jd, &d_psi, &d_eps);
	printf("\nNutation on 2000-01-01 12:00:00 TDB (in rad): longitude = %13.10f, obliquity = %13.10f\n",
		d_psi, d_eps);

	jd.date1 = 2455200.50; 
	jd.date2 = 0;
	printf("\nHeliocentric equatorial coordinates for 2010-01-04 00:00:00 TDB (in AU, AU, AU)\n\n");
	for (i = MERCURY; i <= NEPTUNE + 1; i++) {
		if (i == NEPTUNE + 1) {
			pluto_coordinates(&jd, &xyz[i]);
		} else {
			vsop87_coordinates(i, &jd, &xyz[i]);
			vsop87_ecliptic_to_equator(&xyz[i]);
		}

		printf("%10s: x = %14.10f, y = %14.10f, z = %14.10f\n",
			planet_names[i], xyz[i].x, xyz[i].y, xyz[i].z);
	}

	elp82b_coordinates(&jd, &moon);
	elp82b_ecliptic_to_equator(&moon);
	printf("\nGeocentric rectangular coordinates for 2010-01-04 00:00:00 TDB (in KM, KM, KM)\n\n");
	printf("%10s: x = %14.10f, y = %14.10f, z = %14.10f\n","Moon", moon.x, moon.y, moon.z);

	printf("\nEquatorial coordinates for 2010-01-04 00:00:00 TDB (in hours, degrees, AU)\n\n");
	for (i = MERCURY; i <= NEPTUNE + 1; i++) {
		if (i == 2)
			continue;

		if (i == 0) {
			rectangular_to_spherical(&zero, &xyz[2],
						&eq.right_ascension,
						&eq.declination, &dist);
			printf("%10s: RA = %14.10f, Dec = %14.10f, Distance = %14.10f\n","Sun", 
				eq.right_ascension * RAD_TO_HRS,
				eq.declination * RAD_TO_DEG, dist);

			rectangular_to_spherical(&moon, &zero,
						&eq.right_ascension,
						&eq.declination, &dist);
			printf("%10s: RA = %14.10f, Dec = %14.10f, Distance = %14.7f KM\n","Moon", 
				eq.right_ascension * RAD_TO_HRS,
				eq.declination * RAD_TO_DEG, dist);
		}

		rectangular_to_spherical(&xyz[i], &xyz[2], &eq.right_ascension,
					&eq.declination, &dist);
		printf("%10s: RA = %14.10f, Dec = %14.10f, Distance = %14.10f\n",
			planet_names[i], eq.right_ascension * RAD_TO_HRS,
			eq.declination * RAD_TO_DEG, dist);
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
	printf("\nCoordinates of Polaris:      RA = %13.10f, Dec = %13.10f rad (epoch & equinox J2000)\n\n",
		eq.right_ascension, eq.declination);

	eq.right_ascension += (50.0 * DEGREES(0, 0, 0.19877) * HRS_TO_RAD);
	eq.declination     -= (50.0 * DEGREES(0, 0, 0.0152) * DEG_TO_RAD);
	printf("\tAfter proper motion: RA = %13.10f, Dec = %13.10f rad (epoch 2050 & equinox J2000)\n",
		eq.right_ascension, eq.declination);

	rotate_equatorial(prec_matrix, &eq);
	printf("\tAfter precession:    RA = %13.10f, Dec = %13.10f rad (epoch & equinox J2050)\n",
		eq.right_ascension, eq.declination);

	rotate_equatorial(nut_matrix, &eq);
	printf("\tAfter nutation:      RA = %13.10f, Dec = %13.10f rad (epoch & equinox J2050)\n",
		eq.right_ascension, eq.declination);

	jd.date1 = 2455246.0;
	jd.date2 = 0.34723;
	mst = mean_sidereal_time(&jd, &jd,
			-DEGREES(71, 3, 42) * DEG_TO_RAD) * RAD_TO_HRS;
	printf("\nMean sidereal time at Boston, MA on 02-18-2010 15:20:00 EST     = %13.10f hours\n", mst);

	ast = apparent_sidereal_time(&jd, &jd,
			-DEGREES(71, 3, 42) * DEG_TO_RAD) * RAD_TO_HRS;
	printf("\nApparent sidereal time at Boston, MA on 02-18-2010 15:20:00 EST = %13.10f hours\n", ast);

	eq.right_ascension = DEGREES(2, 44, 12.9747) * HRS_TO_RAD;
	eq.declination     = DEGREES(49, 13, 39.896) * DEG_TO_RAD;

	jd.date1 = 2462088.69;
	jd.date2 = 0;
	annual_aberration(&jd, &eq, &d_ra, &d_dec);
	printf("\nAnnual aberration for Theta-Persei on 2028-11-13.19 TDB: RA = %13.10f, Dec = %13.10f (rad)\n",
		d_ra, d_dec);

	jd.date1 = 2455200.50;
	jd.date2 = 0;
	printf("\nHeliocentric ecliptic coordinates for 2010-01-04 00:00:00 TT\n");
	printf("MP|LP|SP|DC ID [name]: eccentricity, x (AU), y (AU), z (AU), absolute magnitude\n");
	printf("MP = Minor planet, LP = Long-period comet, SP = Short-period comet, DC = Defunct comet\n\n");

	for (i = 0; i < sizeof(mpc_files) / sizeof(mpc_files[0]); i++) {
		fp = fopen(mpc_files[i], "r");
		if (fp) {
			while (fgets(buf, 256, fp)) {
				if (i == 0)
					retval = minor_planet_info(buf, &jd, &inf);
				else if (i == 1)
					retval = comet_info(buf, &jd, &inf);

				printf("\t%s %11s [%35s]: %9.7f, %13.9f, %13.9f, %13.9f, %5.2f",
					inf.type == MINOR_PLANET ? "MP" :
					inf.type == LONG_PERIOD_COMET ? "LP" :
					inf.type == SHORT_PERIOD_COMET ? "SP" : "DC",
					inf.id, inf.name, inf.elements.eccentricity,
					inf.position.x,	inf.position.y,
					inf.position.z,	inf.magnitude);

				if (retval == SUCCESS)
					printf("\n");
				else
					printf(" error = %d\n", retval);

				if (!show_all)
					break;
			}

			fclose(fp);
		} else
			printf("Error opening %s\n", mpc_files[i]);
	}

	return 0;
}
