/*
 * magnitude.c - Routines for the apparent magnitude of celestial bodies
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
 * Reference 1 : HIPPARCOS photometry of minor planets
 *               <http://adsabs.harvard.edu/full/1993IAUS..156...25M>
 */

#include <math.h>
#include <magnitude.h>

/*
 * Calculates the apparent magnitude of a minor planet.
 *
 * obj -- The minor planet's heliocentric rectangular coordinates. Must be in the
 *        same reference frame and units as ear (AU).
 * ear -- The Earth's heliocentric rectangular coordinates. Must be in the same
 *        reference frame and units as obj (AU).
 * abs_mag -- The mean absolute visual magnitude of the minor planet, H.
 * slope -- The magnitude slope parameter of the minor planet, G.
 *
 * Return: The minor planet's apparent magnitude.
 */
double minor_planet_magnitude(struct rectangular_coordinates *obj,
			struct rectangular_coordinates *ear, double abs_mag,
			double slope)
{
	double ds,de,pa,phi1,phi2,m;

	/* Sun-body distance */
	ds = sqrt(obj->x * obj->x + obj->y * obj->y + obj->z * obj->z);

	/* Earth-body distance */
	de = sqrt((obj->x - ear->x) * (obj->x - ear->x) +
		(obj->y - ear->y) * (obj->y - ear->y) +
		(obj->z - ear->z) * (obj->z - ear->z));

	/* The Sun-body-Earth phase angle */
	pa = phase_angle(obj, ear);

	/* Bowell's formula for the apparent magnitude (Reference 1) */
	phi1 = exp(-3.33 * pow(tan(pa / 2), 0.63));
	phi2 = exp(-1.87 * pow(tan(pa / 2), 1.22));

	m = abs_mag + 5 * log10(ds * de) -
		2.5 * log10(phi1 + (phi2 - phi1) * slope);

	return m;
}

/*
 * Calculates the apparent magnitude of a comet.
 *
 * obj -- The comet's heliocentric rectangular coordinates. Must be in the
 *        same reference frame and units as ear (AU).
 * ear -- The Earth's heliocentric rectangular coordinates. Must be in the same
 *        reference frame and units as obj (AU).
 * abs_mag -- The mean absolute visual magnitude of the comet, H.
 * slope -- The magnitude slope parameter of the comet, G.
 *
 * Return: The comet's apparent magnitude.
 */
double comet_magnitude(struct rectangular_coordinates *obj,
		struct rectangular_coordinates *ear, double abs_mag,
		double slope)
{
	double ds,de,m;

	/* Sun-comet distance */
	ds = sqrt(obj->x * obj->x + obj->y * obj->y + obj->z * obj->z);

	/* Earth-comet distance */
	de = sqrt((obj->x - ear->x) * (obj->x - ear->x) +
		(obj->y - ear->y) * (obj->y - ear->y) +
		(obj->z - ear->z) * (obj->z - ear->z));

	/* The comet's apparent magnitude */
	m = abs_mag + 5 * log10(de) + 2.5 * slope * log10(ds);

	return m;
}
