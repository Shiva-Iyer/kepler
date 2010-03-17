/*
 * julian_date.c - Implementation of routines for julian date conversions
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

#include <math.h>
#include <kepler.h>
#include <julian_date.h>

/*
 * Calculates the Julian Day Number for a date in the proleptic Gregorian 
 * calendar.
 *
 * No account is taken of the Gregorian calendar reform and dates prior to
 * Jan. 1, 4800BC are not supported.
 *
 * year Year number using astronomical reckoning and after 4800BC.
 * month Month of the year.
 * day Day of the month.
 * jd The calculated Julian Day Number.
 *
 * Return: SUCCESS Julian date calculated successfully.
 * Return: ERR_INVALID_DATE Gregorian date specified is invalid or out of range.
 */
int calendar_to_julian_date(int year, int month, int day, struct julian_date *jd)
{
	int mm;

	if (year < -4799 || month < 1 || month > 12 || day < 1 || day > 31)
		return ERR_INVALID_DATE;

	mm = (month - 14) / 12;

	jd->date1 = MJD_EPOCH;
	jd->date2 = day +
		(1461 * (year + 4800 + mm)) / 4 +
		(367 * (month - 2 - mm * 12)) / 12 -
		(3 * ((year + 4900 + mm) / 100)) / 4 - 2432076;

	return SUCCESS;
}

/*
 * Calculates the date in the proleptic Gregorian calendar that corresponds
 * to the given Julian Day Number.
 *
 * No account is taken of the Gregorian calendar reform.
 *
 * jd The Julian Day Number. Must correspond to a date on or after
 * Jan. 1, 4800BC in the proleptic Gregorian calendar.
 * year Year number using astronomical reckoning.
 * month Month of the year.
 * day Day of the month.
 * day_fraction Fractional part of day (0 = midnight, 0.5 = noon).
 *
 * Return: SUCCESS Gregorian date calculated successfully.
 * Return: ERR_INVALID_DATE Julian Day Number specified is out of range.
 */
int julian_to_calendar_date(struct julian_date *jd, int *year, int *month,
			int *day, double *day_fraction)
{
	double j;
	int a,da,b,db,c,dc,g,dg,y,m,d;

	j = jd->date1 + jd->date2;

	if (j < -32044.5)
		return ERR_INVALID_DATE;

	j += 32044.5;
	g = j / 146097;
	dg = fmod(j, 146097);
	c = ((dg / 36524 + 1) * 3) / 4;
	dc = dg - c * 36524;
	b = dc / 1461;
	db = fmod(dc, 1461);
	a = ((db / 365 + 1) * 3) / 4;
	da = db - a * 365;
	y = g * 400 + c * 100 + b * 4 + a;
	m = (da * 5 + 308) / 153 - 2;
	d = da - ((m + 4) * 153) / 5 + 122;

	*year = y - 4800 + (m + 2) / 12;
	*month = fmod(m + 2, 12) + 1;
	*day = d + 1;
	*day_fraction = fmod(jd->date1 + jd->date2 + 0.5, 1.0);

	return SUCCESS;
}
