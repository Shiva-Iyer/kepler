/*
 * rise_set.c - Display rise/transit/set times for Sun, Moon, planets
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
#include <time.h>
#include <sidereal_time.h>
#include <delta-t.h>
#include <vsop87.h>
#include <elp82b.h>
#include <pluto.h>
#include <iau2006_precession.h>
#include <riseset.h>
#include <rise_set.h>

int main(int argc, char *argv[])
{
    static char *object_names[] = {"Sun","Moon","Mercury","Venus","Earth",
				   "Mars","Jupiter","Saturn","Uranus",
				   "Neptune","Pluto"};
    int i,j,year,month,day,steps;
    struct julian_date jd0,jd;
    struct equatorial_coordinates equ;
    double longitude,latitude,h0,gast,del_t,del_cor,df[24/TIME_STEP+1],
	rad[24/TIME_STEP+1],decd[24/TIME_STEP+1],rts[3],dist,
	prec[3][3],w;
    char msg[256],rise[10],trans[10],set[10],ra[10],dec[10];

    /* Parse command line parameters */
    parse_command_line(argc, argv, &year, &month,
		       &day, &longitude, &latitude);

    /* Get the Julian Day Number corresponding to user input */
    calendar_to_julian_date(year, month, day, &jd0);
    jd = jd0;

    /* Precession matrix to convert from J2000 to frame of date */
    iau2006_precession_matrix(&jd0, 0, prec);

    /* Greenwich apparent sidereal time */
    gast = apparent_sidereal_time(&jd0, &jd0, 0);

    /* TT/TDB is needed for calculating coordinates, so get delta-T */
    delta_t(year, month, &del_t, &del_cor);
    del_t += del_cor;

    printf("Information for %4d-%02d-%02d (JD %9.1f)"
	   " at lon: %-7.2f & lat: %-7.2f\n", year, month, day,
	   jd0.date1 + jd0.date2, longitude, latitude);
    printf("Rise (UTC), transit (UTC), set (UTC), "
	   "RA (hh:mm:ss), dec. (dd:mm:ss), distance\n");
    printf("RA, dec., distance are at 00:00:00 UTC\n\n");

    longitude *= DEG_TO_RAD;
    latitude *= DEG_TO_RAD;
    steps = sizeof(df)/sizeof(double);
    for (i = SUN; i <= PLUTO; i++) {
	if (i == EARTH)
	    continue;

	/* Compute a list of RA., dec. for the day in question */
	for (j = steps-1; j >= 0; j--) {
	    df[j] = (double)j*TIME_STEP/24;
	    jd.date2 = jd0.date2 + df[j];
	    get_equatorial(i, &jd, &equ, prec, &dist);
	    rad[j] = equ.right_ascension;
	    decd[j] = equ.declination;
	}

	/* Handle the case where RAs wrap around from 2PI to 0 */
	w = 0;
	for (j = 1; j < steps; j++) {
	    if (rad[j-1] > 1.5*PI && rad[j] < 0.5*PI)
		w = TWO_PI;
	    rad[j] += w;
	}

	/* Corrections for atmospheric refraction */
	if (i == SUN)
	    h0 = SUN_REFRACTION;
	else if (i == MOON)
	    h0 = MOON_REFRACTION;
	else
	    h0 = PLANET_REFRACTION;

	/* Compute rise/transit/set times for each planet */
	riseset(steps, df, rad, decd, gast, longitude, latitude,
		del_t, h0, rts);
	for (j = 0; j < 3; j++)
	    rts[j] = reduce_angle(rts[j]*24, 24);

	sprintf(msg,"%10s: %5s, %5s, %5s, %8s, %9s, %9.2f %2s",
		object_names[i+2], format_time(rts[0], rise, 0),
		format_time(rts[1], trans, 0), format_time(rts[2],set,0),
		format_time(rad[0]*RAD_TO_HRS, ra, 1),
		format_time(decd[0]*RAD_TO_DEG, dec, 1), dist,
		(i != MOON) ? "AU" : "KM");
	printf("%s\n", msg);
    }

    return(0);
}

void get_equatorial(int body, struct julian_date *jd,
		    struct equatorial_coordinates *equ,
		    double prec[3][3], double *dist)    
{
    struct rectangular_coordinates ear,rec,sun = {0,0,0};

    vsop87_coordinates(EARTH, jd, &ear);
    if (body >= MERCURY && body <= NEPTUNE) {
	/* Use VSOP87 for all eight major planets */
	vsop87_coordinates(body, jd, &rec);

	/* Correct for finite speed of light */
	lightcor(body, jd, &rec, &ear);

	/* Rotate from the J2000 ecliptic to the equator */
	vsop87_ecliptic_to_equator(&rec);
    } else if (body == PLUTO) {
	/* Pluto has its own analytical theory */
	pluto_coordinates(jd, &rec);
	lightcor(NEPTUNE + 1, jd, &rec, &ear);
    } else if (body == MOON) {
	/* Use ELP 2000-82B for the Moon */
	elp82b_coordinates(jd, &rec);

	/* Rotate from the J2000 ecliptic to the equator */
	elp82b_ecliptic_to_equator(&rec);
    }

    vsop87_ecliptic_to_equator(&ear);

    /* Precess from J2000 frame to frame of date */
    rotate_rectangular(prec, &rec);
    rotate_rectangular(prec, &ear);

    /*
     * Convert heliocentric rectangular coordinates (planets, Pluto)
     * and geocentric rectangular coordinates (the Moon) to geocentric 
     * equatorial coordinates.
     */
    if (body == MOON) {
	/*
	 * The Moon is already in geocentric coordinates, so we just
	 * convert to equatorial, without a Sun->Moon translation.
	 */
	rectangular_to_spherical(&rec, &sun, &equ->right_ascension,
				 &equ->declination, dist);
    } else {
    	rectangular_to_spherical(&rec, &ear, &equ->right_ascension,
				 &equ->declination, dist);
    }
}

char *format_time(double t, char *buf, u_short sec)
{
    struct deg_min_sec dms;

    if (t != -1)
	if (sec) {
	    degrees_to_dms(t, &dms);
	    sprintf(buf, "%02d:%02d:%02d", dms.degrees, abs(dms.minutes),
		    abs(dms.seconds));
	} else {
	    degrees_to_dms(round(t * 60) / 60, &dms);
	    sprintf(buf, "%02d:%02d", dms.degrees, abs(dms.minutes));
	}
    else
	sprintf(buf, " NA  ");

    return(buf);
}

void parse_command_line(int argc, char *argv[], int *year, int *month,
			int *day, double *longitude, double *latitude)
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

    /* Default to the longitude & latitude of Boston, MA */
    *longitude = -DEGREES(71, 3, 42);
    *latitude = DEGREES(42, 21, 28);

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

	sscanf(argv[1], "%d%c%d%c%d", year, &c, month, &c, day);
	if (argc > 2)
	    *longitude = atof(argv[2]);
	if (argc > 3)
	    *latitude = atof(argv[3]);
    }
}

void display_usage()
{
    printf("Usage: rise_set [OPTION] [date] [longitude] [latitude]\n");
    printf("Display rise/transit/set times for the Sun, Moon, planets and Pluto\n\n");
    printf("  -h, --help    Display this help screen and exit\n");
    printf("  -v, --version Display version number and exit\n");
    printf("  date          Date in the Gregorian calendar, in the form yyyy-mm-dd\n");
    printf("  longitude     Longitude in degrees, positive east of Greenwich\n");
    printf("  latitude      Latitude in degrees, positive north of the equator\n\n");
    printf("Defaults to Boston, MA, USA (71W03'42\", 42N21'28\") for today.\n\n");
}
