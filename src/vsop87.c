/*
 * vsop87.c - Routines to implement the VSOP87(A) planetary theory
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

/*
 * Reference : "Planetary Solutions VSOP87" - P. Bretagnon, G. Francou
 *             <ftp://cdsarc.u-strasbg.fr/pub/cats/VI/81/>
 */

#include <math.h>
#include <memory.h>
#include <vsop87.h>

#define PLANET_SERIES_COUNT		18

/* These arrays in vsop87_data.c contain the series of terms for VSOP87(A) */
extern struct vsop87_term *planets_terms[]; 

/*
 * Calculates a major planet's heliocentric rectangular coordinates using the
 * VSOP87 (version A) theory in its entirety.
 *
 * planet Enumeration that identifies the planet for calculations.
 * tdb TDB to be used for calculations. TT may be used for all but
 * the most exacting applications.
 * pos The planet's heliocentric rectangular coordinates in AU.
 * The reference frame is the equinox & ecliptic of J2000.
 *
 * Return: SUCCESS If the coordinates have been calculated successfully.
 * Return: ERR_INVALID_PLANET If the planet's identifier is invalid.
 */
int vsop87_coordinates(enum solar_system_planets planet, struct julian_date *tdb,
		struct rectangular_coordinates *pos)
{
	int i;
	struct vsop87_term *p;
	double t,sum[PLANET_SERIES_COUNT],xyz[3];

	if (planet < MERCURY || planet > NEPTUNE)
		return ERR_INVALID_PLANET;

	t = JULIAN_MILLENNIA(tdb->date1, tdb->date2);
	memset(sum, 0, sizeof(sum));

	/* Sum up the terms a*cos(b + c*t) for each series x0,y0,z0... */
	for (i = 0; i < PLANET_SERIES_COUNT; i++) {
		p = planets_terms[planet * PLANET_SERIES_COUNT + i];
		while (p->a != 0 || p->b != 0 || p->c != 0) {
			sum[i] += (p->a * cos(p->b + p->c * t));
			p++;
		}
	}

	for (i = 0; i < PLANET_SERIES_COUNT; i += 6) {
		xyz[i / 6] = sum[i] +
			(sum[i + 1] +
			(sum[i + 2] +
			(sum[i + 3] +
			(sum[i + 4] + sum[i + 5] * t) * t) * t) * t) * t;
	}

	pos->x = xyz[0];
	pos->y = xyz[1];
	pos->z = xyz[2];

	return SUCCESS;
}

/*
 * Rotates a body's coordinates from the dynamical ecliptic frame of J2000
 * to the equatorial frame of J2000/FK5.
 *
 * pos The coordinates to be rotated in-place
 */
void vsop87_ecliptic_to_equator(struct rectangular_coordinates *pos)
{
	/* Rotation matrix specified in the VSOP87 theory */
	static double rot_matrix[3][3] = {
		{ 1.000000000000, 0.000000440360, -0.000000190919},
		{-0.000000479966, 0.917482137087, -0.397776982902},
		{ 0.000000000000, 0.397776982902,  0.917482137087}
	};

	rotate_rectangular(rot_matrix, pos);
}
