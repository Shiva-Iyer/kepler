/*
 * mpc_file.h - Declarations for parsing MPC Orbit Database files
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

#ifndef _MPC_FILE_H_
#define _MPC_FILE_H_

#include <julian_date.h>
#include <coordinates.h>
#include <orbital_elements.h>

#define MPC_UNPACK(c)			\
	(c >= 97 ? c - 61 : (c >= 65 ? c - 55 : c - 48))

/*
 * The types of celestial bodies included in the MPCORB Database.
 */
enum mpc_body_types {
	MINOR_PLANET,
	LONG_PERIOD_COMET,
	SHORT_PERIOD_COMET,
	DEFUNCT_COMET,
	UNCERTAIN_COMET
};

/*
 * Used to store the details of a celestial body in the MPCORB Database.
 */
struct mpc_body {
	char id[12]; /* The body's ID or unpacked provisional designation. */
	char name[36]; /* The body's name, if it is included in the file. */
	enum mpc_body_types type; /* The type of body. */

	double magnitude; /* The body's absolute magnitude, H. */
	double slope; /* Slope parameter, G. */

	struct orbital_elements elements; /* The body's orbital elements. */

	/*
	 * The body's coordinates at the desired epoch. For minor planets and
	 * comets, the origin is the Sun and the reference frame is the equinox
	 * and ecliptic of J2000.
	 */
	struct rectangular_coordinates position;
};

int minor_planet_info(char *data, struct julian_date *tt, struct mpc_body *info);

int comet_info(char *data, struct julian_date *tt, struct mpc_body *info);

#endif
