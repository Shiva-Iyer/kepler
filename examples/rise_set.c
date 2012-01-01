/*
 * rise_set.c - Display rise/set times for the Sun, Moon, planets & Pluto
 * Copyright (C) 2010-2012 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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
 * See README for limitations and other known issues
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sidereal_time.h>
#include <delta-t.h>
#include <vsop87.h>
#include <elp82b.h>
#include <pluto.h>
#include <iau2000a_nutation.h>
#include <rise_set.h>

int main(int argc, char *argv[])
{
	static char *object_names[] = {"Mercury","Venus","Earth","Mars",
				       "Jupiter","Saturn","Uranus","Neptune",
				       "Pluto","Sun","Moon"};
	int i,year,month,day;
	struct julian_date jd;
	struct equatorial_coordinates equ;
	double longitude,latitude,ut_off,dist,obliquity,ha,lmst,ut_trans,
		ut_rise,ut_set,del_t,corr;
	char msg[256],rise[10],trans[10],set[10],ra[10],dec[10];

	/* Parse command line parameters */
	parse_command_line(argc, argv, &year, &month,
			&day, &longitude, &latitude, &ut_off);

	/* Get obliquity of the ecliptic for the J2000 epoch */
	jd.date1 = J2000_EPOCH;
	jd.date2 = 0;
	obliquity = iau2000a_mean_obliquity(&jd);

	/* Get the Julian Day Number corresponding to user input */
	calendar_to_julian_date(year, month, day, &jd);

	printf("Information for %4d-%02d-%02d (JD %9.1f) at lon: %-7.2f & lat: %-7.2f\n",
		year, month, day, jd.date1 + jd.date2, longitude, latitude);
	printf("Rise, transit, set, RA (h:m:s), declination (d:m:s), distance (AU)\n");
	printf("RA and declination are for UT 0h on the date specified\n\n");

	/* kepler expects all angles to be in radians */
	longitude *= DEG_TO_RAD;
	latitude *= DEG_TO_RAD;

	/* Get the local mean sidereal time */
	lmst = mean_sidereal_time(&jd, &jd, longitude);

	/* TT/TDB is needed for calculating coordinates, so get delta-T */
	delta_t(year, month, &del_t, &corr);
	jd.date2 += (del_t / 86400);

	for (i = MERCURY; i <= MOON; i++) {
		if (i == EARTH)
			continue;

		get_equatorial(i, &jd, obliquity, &equ, &dist);

		/* Since LST is the RA of any body at upper culmination */
		ut_trans = (equ.right_ascension - lmst) * RAD_TO_HRS + ut_off;

		if (i == SUN)
			ha = sin(SUN_REFRACTION);
		else if (i == MOON)
			ha = sin(MOON_REFRACTION);
		else
			ha = sin(PLANET_REFRACTION);

		/* LHA = angle the earth turned from planet rise to transit */
		ha = (ha - sin(latitude) * sin(equ.declination)) /
			(cos(latitude) * cos(equ.declination));

		if (ha >= -1.0 && ha <= 1.0) {
			ha = acos(ha) * RAD_TO_HRS;

			/* Subtract/add LHA from transit time to get rise/set */
			ut_rise = reduce_angle(ut_trans - ha, 24.0);
			ut_set = reduce_angle(ut_trans + ha, 24.0);
			ut_trans = reduce_angle(ut_trans, 24);

			sprintf(msg,"%10s: %5s, %5s, %5s, %8s, %9s, %11.8f",
				object_names[i],
				format_time(ut_rise, rise, 0),
				format_time(ut_trans, trans, 0),
				format_time(ut_set, set, 0),
				format_time(equ.right_ascension * RAD_TO_HRS, ra, 1),
				format_time(equ.declination * RAD_TO_DEG, dec, 1),
				dist);

			printf("%s\n", msg);
		} else {
			printf("%10s: Does not rise and/or set\n", object_names[i]);
		}
	}

	return 0;
}

void get_equatorial(int pla, struct julian_date *jd, double obl,
		struct equatorial_coordinates *equ, double *dist)
{
	struct rectangular_coordinates rec,zero = {0, 0, 0};
	static struct julian_date jd_saved = {0, 0};
	static struct rectangular_coordinates ear_saved;

	/* We need the Earth for coordinate conversions. Save in a static. */
	if (jd->date1 + jd->date2 != jd_saved.date1 + jd_saved.date2) {
		jd_saved.date1 = jd->date1;
		jd_saved.date2 = jd->date2;

		vsop87_coordinates(EARTH, jd, &ear_saved);
		vsop87_ecliptic_to_equator(&ear_saved);
	}

	if (pla < PLUTO) {
		/* Use VSOP87 for all eight major planets */
		vsop87_coordinates(pla, jd, &rec);

		/* Rotate from the J2000 ecliptic to the equator */
		vsop87_ecliptic_to_equator(&rec);
	} else if (pla == PLUTO) {
		/* Pluto has its own analytical theory */
		pluto_coordinates(jd, &rec);
	} else if (pla == SUN) {
		/* The Sun's heliocentric coordinates are zero (duh) */
		rec.x = 0;
		rec.y = 0;
		rec.z = 0;
	} else {
		/* Use ELP 2000-82B for the Moon */
		elp82b_coordinates(jd, &rec);

		/* Rotate from the J2000 ecliptic to the equator */
		elp82b_ecliptic_to_equator(&rec);
	}

	/*
	 * Convert heliocentric rectangular coordinates (planets, Pluto)
	 * and geocentric rectangular coordinates (the Moon) to geocentric 
	 * equatorial coordinates.
	 */
	if (pla == MOON) {
		/*
		 * The Moon is already in geocentric coordinates, so we just
		 * convert to equatorial, without a Sun->Moon translation.
		 */
		rectangular_to_spherical(&rec, &zero, &equ->right_ascension,
					&equ->declination, dist);

		/* Convert the Moon's distance to AU */
		*dist = *dist / AU;
	} else {
		rectangular_to_spherical(&rec, &ear_saved, &equ->right_ascension,
					&equ->declination, dist);
	}
}

char *format_time(double t, char *buf, u_short sec)
{
	struct deg_min_sec dms;

	if (sec) {
		degrees_to_dms(t, &dms);
		sprintf(buf, "%02d:%02d:%02d", dms.degrees, abs(dms.minutes),
			abs(dms.seconds));
	} else {
		degrees_to_dms(round(t * 60) / 60, &dms);
		sprintf(buf, "%02d:%02d", dms.degrees, abs(dms.minutes));
	}

	return buf;
}

void parse_command_line(int argc, char *argv[], int *year, int *month,
			int *day, double *longitude, double *latitude,
			double *ut_off)
{
	char c;
	time_t t;
	struct tm *tt;

	/* Default to today's date */
	time(&t);
	tt = localtime(&t);
	*year = 1900 + tt->tm_year;
	*month = 1 + tt->tm_mon;
	*day = tt->tm_mday;

#ifdef tm_gmtoff
	*ut_off = tt->tm_gmtoff / 3600.0;
#else
	*ut_off = 0;
#endif

	/* Default to the longitude & latitude of Boston, MA */
	*longitude = -DEGREES(71, 3, 42);
	*latitude = DEGREES(42, 21, 28);

	if (argc > 1) {
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			display_usage();
			exit(0);
		}

		if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
			display_copyright();
			exit(0);
		}

		sscanf(argv[1], "%d%c%d%c%d", year, &c, month, &c, day);

		if (argc > 2)
			*longitude = atof(argv[2]);

		if (argc > 3)
			*latitude = atof(argv[3]);

		if (argc > 4)
			*ut_off = atof(argv[4]);
	}
}

void display_usage()
{
	printf("Usage: rise_set [OPTION] [date] [longitude] [latitude] [UTC_offset]\n");
	printf("Display rise / set times for the Sun, Moon, planets and Pluto\n\n");
	printf("  -h, --help    Display this help screen and exit\n");
	printf("  -v, --version Display version number and exit\n");
	printf("  date          Date in the Gregorian calendar, expressed as yyyy-mm-dd\n");
	printf("  longitude     Observer's longitude in degrees, positive east of Greenwich\n");
	printf("  latitude      Observer's latitude in degrees, positive north of the equator\n");
	printf("  UTC_offset    Observer's offset from UTC in hours, positive east of Greenwich\n\n");
	printf("If no parameters are provided, information for today's date is displayed for\n");
	printf("Boston, MA, USA (71W03'42\", 42N21'28\"). No validation is done on parameters.\n\n");
	printf("See README in the kepler source for limitations and other known issues.\n");
}

void display_copyright()
{
	printf(PROG_VERSION_STRING);
	printf(PROG_COPYRIGHT);
}
