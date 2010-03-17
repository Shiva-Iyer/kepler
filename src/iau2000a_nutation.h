/*
 * iau2000a_nutation.h - Declarations for the IAU 2000A nutation model
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

#ifndef _IAU2000A_NUTATION_H_
#define _IAU2000A_NUTATION_H_

#include <julian_date.h>

/* Used internally to store the series of terms in the theory */
struct iau2000a_term1 {
	short l;
	short lp;
	short f;
	short d;
	short om;
	double ps;
	double psd;
	double pcp;
	double ec;
	double ecd;
	double esp;
};

/* Used internally to store the series of terms in the theory */
struct iau2000a_term2 {
	short mer;
	short ven;
	short ear;
	short mar;
	short jup;
	short sat;
	short ura;
	short nep;
	short gp;
	short l;
	short lp;
	short f;
	short d;
	short om;
	double ps;
	double pcp;
	double ec;
	double esp;
};

double iau2000a_mean_obliquity(struct julian_date *tdb);

void iau2000a_nutation(struct julian_date *tdb,double *d_psi, double *d_epsilon);

void iau2000a_nutation_matrix(struct julian_date *tdb, double nut_matrix[3][3]);

#endif
