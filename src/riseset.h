/*
 * riseset.h - Declarations for calculating rise/transit/setting times
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

#ifndef _RISESET_H_
#define _RISESET_H_

void riseset(int N, double *df, double *ra, double *dec, double gast,
	     double lon, double lat, double delt, double h0, double *rts);

double interpolate(int N, double *X, double *Y, double xint);

#endif
