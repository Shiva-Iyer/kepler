/*
 * iau2006_precession.c - Implementation of the IAU 2006 precession model
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
 * Reference : US Naval Observatory Circular 179 (2005)
 *             <http://aa.usno.navy.mil/publications/docs/Circular_179.php>
 */

#include <math.h>
#include <kepler.h>
#include <iau2006_precession.h>

/**
 * Calculates the precession matrix using the IAU 2006 precession model
 * in its entirety. The resulting matrix may be used in the rotation
 * of coordinates.
 *
 * @param[in] tdb TDB to be used for calculations. TT may be used for all but
 * the most exacting applications.
 * @param[in] toJ2000 If 0, the resulting matrix is for precessing from J2000 to
 * another epoch. Otherwise, the precession is from another epoch to J2000.
 * @param[out] prec_matrix The precession matrix.
 **/
void iau2006_precession_matrix(struct julian_date *tdb, int toJ2000,
			double prec_matrix[3][3])
{
	double t,psi,omega,chi,eps0,c1,c2,c3,c4,s1,s2,s3,s4;

	eps0 = 84381.406 * ACS_TO_RAD;
	t = JULIAN_CENTURIES(tdb->date1, tdb->date2);

	psi = ((5038.481507 +
		(-1.0790069 +
		(-0.00114045 +
		(0.000132851 - 0.0000000951*t) * t) * t) * t) * t) * ACS_TO_RAD;

	omega = eps0 +
		((-0.025754 +
		(0.0512623 +
		(-0.00772503 +
		(-0.000000467 + 0.0000003337*t) * t) * t) * t) * t) * ACS_TO_RAD;

	chi = ((10.556403 +
		(-2.3814292 +
		(-0.00121197 +
		(0.000170663 - 0.0000000560*t) * t) * t) * t) * t) * ACS_TO_RAD;

	sincos(eps0, &s1, &c1);
	sincos(psi, &s2, &c2);
	s2 *= -1.0;
	sincos(omega, &s3, &c3);
	s3 *= -1.0;
	sincos(chi, &s4, &c4);

	prec_matrix[0][0] = (c4 * c2)-(s2 * s4 * c3);
	prec_matrix[0][1] = (c4 * s2 * c1)+(s4 * c3 * c2 * c1)-(s1 * s4 * s3);
	prec_matrix[0][2] = (c4 * s2 * s1)+(s4 * c3 * c2 * s1)+(c1 * s4 * s3);

	prec_matrix[1][0] = -(s4 * c2)-(s2 * c4 * c3);
	prec_matrix[1][1] = -(s4 * s2 * c1)+(c4 * c3 * c2 * c1)-(s1 * c4 * s3);
	prec_matrix[1][2] = -(s4 * s2 * s1)+(c4 * c3 * c2 * s1)+(c1 * c4 * s3);

	prec_matrix[2][0] = s2 * s3;
	prec_matrix[2][1] = -(s3 * c2 * c1)-(s1 * c3);
	prec_matrix[2][2] = -(s3 * c2 * s1)+(c3 * c1);

	/* Just transpose the matrix for precession to J2000 */
	if (toJ2000) {
		swap(&prec_matrix[0][1], &prec_matrix[1][0]);
		swap(&prec_matrix[0][2], &prec_matrix[2][0]);
		swap(&prec_matrix[1][2], &prec_matrix[2][1]);
	}
}
