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
#include <memory.h>
#include <sidereal_time.h>
#include <delta-t.h>
#include <vsop87.h>
#include <elp82b.h>
#include <pluto.h>
#include <iau2000a_nutation.h>
#include <riseset.h>
#include <rise_set.h>

int main(int argc, char *argv[])
{
  static char *object_names[] = {"Mercury","Venus","Earth","Mars",
				 "Jupiter","Saturn","Uranus","Neptune",
				 "Pluto","Sun","Moon"};
  int i,j,year,month,day,steps;
  struct julian_date jd0,jd;
  struct equatorial_coordinates equ;
  double longitude,latitude,ut_off,h0,gast,del_t,corr,
    *df,*rad,*decd,rts[3];
  char msg[256],rise[10],trans[10],set[10],ra[10],dec[10];

  /* Parse command line parameters */
  parse_command_line(argc, argv, &year, &month,
		     &day, &longitude, &latitude, &ut_off);

  /* Get the Julian Day Number corresponding to user input */
  calendar_to_julian_date(year, month, day, &jd0);
  jd = jd0;

  /* Greenwich apparent sidereal time */
  gast = apparent_sidereal_time(&jd0, &jd0, 0);

  /* TT/TDB is needed for calculating coordinates, so get delta-T */
  delta_t(year, month, &del_t, &corr);

  printf("Information for %4d-%02d-%02d (JD %9.1f) at lon: %-7.2f & lat: %-7.2f\n",
	 year, month, day, jd0.date1 + jd0.date2, longitude, latitude);
  printf("Rise, transit, set, RA (h:m:s), dec. (d:m:s)\n");
  printf("RA, dec. are for 0h UT on the given date\n\n");

  steps = 24/TIME_STEP + 1;
  df = malloc(steps*sizeof(double));
  rad = malloc(steps*sizeof(double));
  decd = malloc(steps*sizeof(double));

  /* kepler expects all angles to be in radians */
  longitude *= DEG_TO_RAD;
  latitude *= DEG_TO_RAD;
  for (i = MERCURY; i <= MOON; i++) {
    if (i == EARTH)
      continue;

    for (j = 0; j < steps; j++) {
      df[j] = (double)j*TIME_STEP/24;
      jd.date2 = jd0.date2 + df[j];
      get_equatorial(i, &jd, &equ);
      rad[j] = equ.right_ascension;
      decd[j] = equ.declination;
    }

    if (i == SUN)
      h0 = SUN_REFRACTION;
    else if (i == MOON)
      h0 = MOON_REFRACTION;
    else
      h0 = PLANET_REFRACTION;

    riseset(steps, df, rad, decd, gast, longitude, latitude, del_t, h0, rts);
    for (j = 0; j < 3; j++)
      rts[j] = reduce_angle(rts[j]*24 + ut_off, 24);

    sprintf(msg,"%10s: %5s, %5s, %5s, %8s, %9s",
	    object_names[i], format_time(rts[0], rise, 0),
	    format_time(rts[1], trans, 0), format_time(rts[2], set, 0),
<<<<<<< HEAD
	    format_time(rad[0]*RAD_TO_HRS, ra, 1),
	    format_time(decd[0]*RAD_TO_DEG, dec, 1));
=======
	    format_time(rad[1]*RAD_TO_HRS, ra, 1),
	    format_time(decd[1]*RAD_TO_DEG, dec, 1));
>>>>>>> 51ac667367b3c4e51de3dab5da8eb5f99453406f
    printf("%s\n", msg);
  }

  free(df);
  free(rad);
  free(decd);
  return(0);
}

void get_equatorial(int pla, struct julian_date *jd,
		    struct equatorial_coordinates *equ)
{
  double dist;
  struct rectangular_coordinates ear,rec = {0, 0, 0},zero = {0, 0, 0};

  if (pla < PLUTO) {
    /* Use VSOP87 for all eight major planets */
    vsop87_coordinates(pla, jd, &rec);

    /* Rotate from the J2000 ecliptic to the equator */
    vsop87_ecliptic_to_equator(&rec);
  } else if (pla == PLUTO) {
    /* Pluto has its own analytical theory */
    pluto_coordinates(jd, &rec);
  } else if (pla == MOON) {
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
			     &equ->declination, &dist);
  } else {
    vsop87_coordinates(EARTH, jd, &ear);
    vsop87_ecliptic_to_equator(&ear);
    rectangular_to_spherical(&rec, &ear, &equ->right_ascension,
			     &equ->declination, &dist);
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
