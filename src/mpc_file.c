/*
 * mpc_file.c - Routines for parsing MPC Orbit Database files
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

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <kepler.h>
#include <mpc_file.h>

/*
 * Parses data in the MPC's Orbit Database for minor planets. For details on the
 * format, see <http://www.cfa.harvard.edu/iau/info/MPOrbitFormat.html>.
 * Download the database from <http://www.cfa.harvard.edu/iau/MPCORB.html>.
 *
 * data -- A line of data from the MPCORB minor planet database file.
 * tt -- TT to be used for calculations.
 * info -- The minor planet's details and heliocentric coordinates. The reference
 *         frame is the equinox & ecliptic of J2000.
 *
 * Return: SUCCESS -- The data was parsed and the coordinates were calculated.
 *         ERR_INVALID_DATA -- The data was in an unrecognized format.
 *         ERR_CONVERGENCE -- The solution to Kepler's equation didn't converge
 *         to the required precision within the specified number of iterations.
 */
int minor_planet_info(char *data, struct julian_date *tt, struct mpc_body *info)
{
	size_t i,l;
	char tmp[16];
	int year,month,day;

	l = strlen(data);
	if (l < 160)
		return ERR_INVALID_DATA;

	if (data[2] == 'S') {
		/* Use survey designations as-is */
		strncpyn(info->id, data, 7);
	} else {
		/* Unpack the year part of the ID */
		year = MPC_UNPACK(data[0]) * 100 + MPC_UNPACK(data[1]) * 10 +
			MPC_UNPACK(data[2]);
		sprintf(info->id, "%d", year);

		if (isdigit(data[3])) {
			/* Use ID as-is */
			strncat(info->id, data + 3, 2);
		} else {
			/* Unpack month code and cycle number */
			strcat(info->id, " ");
			strncat(info->id, data + 3, 1);
			strncat(info->id, data + 6, 1);

			month = MPC_UNPACK(data[4]) * 10 + MPC_UNPACK(data[5]);
			if (month) {
				sprintf(tmp, "%d", month);
				strcat(info->id, tmp);
			}
		}
	}

	info->name[0] = '\0';
	if (l > 166) {
		/* Strip leading and trailing spaces in name (if provided) */
		for (i = 166; i < l && isspace(data[i]); i++) ;

		strncpyn(info->name, data + i, 194 - i);
		for (i = strlen(info->name) - 1; i >= 0 && isspace(info->name[i]); i--)
			info->name[i] = '\0';
	}

	info->type = MINOR_PLANET;

	/* Parse the absolute magnitude, H and slope parameter, G */
	info->magnitude = atof(strncpyn(tmp, data + 8, 5));
	info->slope = atof(strncpyn(tmp, data + 14, 5));

	/* Unpack epoch of osculation */
	year = MPC_UNPACK(data[20]) * 100 + MPC_UNPACK(data[21]) * 10 +
		MPC_UNPACK(data[22]);
	month = MPC_UNPACK(data[23]);
	day = MPC_UNPACK(data[24]);
	calendar_to_julian_date(year, month, day, &info->elements.epoch);

	/* Parse all the other orbital elements */
	info->elements.mean_anomaly = atof(strncpyn(tmp, data + 26, 9)) * DEG_TO_RAD;
	info->elements.daily_motion = atof(strncpyn(tmp, data + 80, 11)) * DEG_TO_RAD;
	info->elements.eccentricity = atof(strncpyn(tmp, data + 70, 9));
	info->elements.arg_perihelion = atof(strncpyn(tmp, data + 37, 9)) * DEG_TO_RAD;
	info->elements.lon_asc_node = atof(strncpyn(tmp, data + 48, 9)) * DEG_TO_RAD;
	info->elements.inclination = atof(strncpyn(tmp, data + 59, 9)) * DEG_TO_RAD;

	/*
	 * The MPC provides only the semi-major axis for minor planets, so the
	 * perihelion distance must be calculated
	 */
	info->elements.perihelion = fabs(atof(strncpyn(tmp, data + 92, 11)) *
					(1 - info->elements.eccentricity));

	return elements_to_ephemeris(tt, &info->elements, &info->position);
}

/*
 * Parses data in the MPC's Orbit Database for comets. For details on the
 * format, see <http://www.cfa.harvard.edu/iau/info/CometOrbitFormat.html>.
 * Download the database from <http://www.cfa.harvard.edu/iau/MPCORB.html>.
 *
 * data -- A line of data from the MPCORB comet database file.
 * tt -- TT to be used for calculations.
 * info -- The comet's details and heliocentric coordinates. The reference frame
 *         is the equinox & ecliptic of J2000.
 *
 * Return: SUCCESS -- The data was parsed and the coordinates were calculated.
 *         ERR_INVALID_DATA -- The data was in an unrecognized format.
 *         ERR_CONVERGENCE -- The solution to Kepler's equation didn't converge
 *         to the required precision within the specified number of iterations.
 */
int comet_info(char *data, struct julian_date *tt, struct mpc_body *info)
{
	size_t i,l;
	char tmp[16];
	int year,month;
	double day,sma;
	struct julian_date pp;

	l = strlen(data);
	if (l < 103)
		return ERR_INVALID_DATA;

	if (isdigit(data[0])) {
		/* For numbered periodic comets, just use the number */
		sprintf(info->id, "%d", atoi(strncpyn(tmp, data, 4)));
	} else {
		/* Unpack the year part of the ID */
		year = MPC_UNPACK(data[5]) * 100 + MPC_UNPACK(data[6]) * 10 +
			MPC_UNPACK(data[7]);
		sprintf(info->id, "%d", year);

		/* Parse the month name in the ID */
		strcat(info->id, " ");
		strncat(info->id, data + 8, 1);
		if (isupper(data[11]))
			strncat(info->id, data + 11, 1);

		/* Unpack the month cycle number in the ID */
		month = MPC_UNPACK(data[9]) * 10 + MPC_UNPACK(data[10]);
		if (month) {
			sprintf(tmp, "%d", month);
			strcat(info->id, tmp);
		}

		/* Parse the fragment number in the ID */
		if (islower(data[11])) {
			sprintf(tmp, "-%c", toupper(data[11]));
			strcat(info->id, tmp);
		}
	}

	/* Strip leading and trailing spaces in the name */
 	for (i = 102; i < l && isspace(data[i]); i++) ;

	strncpyn(info->name, data + i, sizeof(info->name) - 1);
	for (i = strlen(info->name) - 1; i >= 0 && isspace(info->name[i]); i--)
		info->name[i] = '\0';

	switch (data[4]) {
	case 'C':
		info->type = LONG_PERIOD_COMET;
		break;
	case 'P':
		info->type = SHORT_PERIOD_COMET;
		break;
	case 'D':
		info->type = DEFUNCT_COMET;
		break;
	case 'X':
		info->type = UNCERTAIN_COMET;
		break;
	case 'A':
		info->type = MINOR_PLANET;
		break;
	}

	/* Parse the absolute magnitude, H and slope parameter, G */
	info->magnitude = atof(strncpyn(tmp, data + 91, 4));
	info->slope = atof(strncpyn(tmp, data + 96, 5));

	/* Get epoch of perihelion passage */
	year = atoi(strncpyn(tmp, data + 14, 4));
	month = atoi(strncpyn(tmp, data + 19, 2));
	day = atof(strncpyn(tmp, data + 22, 7));
	calendar_to_julian_date(year, month, day, &pp);
	pp.date2 += fmod(day, 1.0);

	if (isdigit(data[81])) {
		/* For perturbed solutions, use relevant epoch */
		year = atoi(strncpyn(tmp, data + 81, 4));
		month = atoi(strncpyn(tmp, data + 85, 2));
		day = atof(strncpyn(tmp, data + 87, 2));
		calendar_to_julian_date(year, month, day, &info->elements.epoch);
	} else {
		/* For non-perturbed solutions, use perihelion passage epoch */
		info->elements.epoch.date1 = pp.date1;
		info->elements.epoch.date2 = pp.date2;
	}

	info->elements.eccentricity = atof(strncpyn(tmp, data + 41, 8));
	info->elements.perihelion = atof(strncpyn(tmp, data + 30, 9));

	/*
	 * The MPC provides only the perihelion distance for comets, so the
	 * semi-major axis must be calculated
	 */
	sma = fabs(info->elements.perihelion / (1 - info->elements.eccentricity));

	/* Calculate the daily motion using standard equations of dynamics */
	info->elements.daily_motion = GAUSS_GRAV_CONSTANT / (sma * sqrt(sma));

	/* Mean anomaly is just the motion from perihelion passage to epoch */
	info->elements.mean_anomaly = reduce_angle(info->elements.daily_motion *
				((info->elements.epoch.date1 - pp.date1) +
				(info->elements.epoch.date2 - pp.date2)), TWO_PI);

	/* Parse all the other orbital elements */
	info->elements.arg_perihelion = atof(strncpyn(tmp, data + 51, 8)) * DEG_TO_RAD;
	info->elements.lon_asc_node = atof(strncpyn(tmp, data + 61, 8)) * DEG_TO_RAD;
	info->elements.inclination = atof(strncpyn(tmp, data + 71, 8)) * DEG_TO_RAD;

	return elements_to_ephemeris(tt, &info->elements, &info->position);
}
