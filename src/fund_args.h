/*
 * fund_args.h - Declarations for fundamental planetary/lunisolar arguments
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

#ifndef _FUND_ARGS_H_
#define _FUND_ARGS_H_

/** 
 * Enum for the various fundamental arguments used throughout the program. These
 * values are parameters to the function fundamental_argument().
 **/
enum fund_argument {
	ARG_LONGITUDE_MERCURY = 0,
	ARG_LONGITUDE_VENUS,
	ARG_LONGITUDE_EARTH,
	ARG_LONGITUDE_MARS,
	ARG_LONGITUDE_JUPITER,
	ARG_LONGITUDE_SATURN,
	ARG_LONGITUDE_URANUS,
	ARG_LONGITUDE_NEPTUNE,
	ARG_PRECESSION,
	ARG_ANOMALY_MOON,
	ARG_ANOMALY_SUN,
	ARG_LATITUDE_MOON,
	ARG_ELONGATION_MOON,
	ARG_LONGITUDE_NODE,
	ARG_LONGITUDE_MOON
};

double fundamental_argument(enum fund_argument arg, double t);

#endif
