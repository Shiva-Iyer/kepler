/*
 * julian_date.h - Constants & declarations for julian date conversions
 * Copyright (C) 2010 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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

#ifndef _JULIAN_DATE_H_
#define _JULIAN_DATE_H_

/**
 * This structure is used wherever a Julian Day Number (JDN) is expected or
 * returned. Two doubles are used to preserve precision.
 *
 * A given JDN may be partitioned between date1 & date2 in any convenient 
 * manner. The JDN that corresponds to this structure is date1 + date2.
 **/
struct julian_date {
	double date1;
	double date2;
};

#define J2000_EPOCH 		2451545.0
#define MJD_EPOCH 		2400000.5

#define JULIAN_CENTURY_LENGTH		36525.0
#define JULIAN_MILLENNIUM_LENGTH	365250.0

#define JULIAN_CENTURIES(d1, d2)	\
	(((d1 - J2000_EPOCH) + d2) / JULIAN_CENTURY_LENGTH)

#define JULIAN_MILLENNIA(d1, d2)	\
	(((d1 - J2000_EPOCH) + d2) / JULIAN_MILLENNIUM_LENGTH)

int calendar_to_julian_date(int year, int month, int day,struct julian_date *jd);

int julian_to_calendar_date(struct julian_date *jd, int *year, int *month,
			int *day, double *day_fraction);

#endif
