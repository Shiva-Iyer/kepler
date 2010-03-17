/*
 * iau2000a_nutation.c - Implementation of the IAU 2000A nutation model
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
#include <fund_args.h>
#include <iau2000a_nutation.h>

/** These arrays in iau2000a_data.c contain the series of terms in the model */
extern struct iau2000a_term1 iau2000a_lunisolar[];
extern struct iau2000a_term2 iau2000a_planetary[];

extern u_short iau2000a_lunisolar_count;
extern u_short iau2000a_planetary_count;

/**
 * Calculates the mean obliquity of the ecliptic using the IAU 2000 formula.
 *
 * @param[in] tdb TDB to be used for calculations. TT may be used for all but
 * the most exacting applications.
 *
 * @return The mean obliquity of the ecliptic in radians.
 **/
double iau2000a_mean_obliquity(struct julian_date *tdb)
{
	double t = JULIAN_CENTURIES(tdb->date1, tdb->date2);

	return (84381.406 +
		(-46.836769 +
		(-0.0001831 +
		(0.00200340 +
		(-0.000000576 - 0.0000000434*t) * t) * t) * t) * t) * ACS_TO_RAD;
}

/**
 * Calculates the nutation in longitude and obliquity using the IAU 2000A 
 * nutation model in its entirety.
 *
 * @param[in] tdb TDB to be used for calculations. TT may be used for all but
 * the most exacting applications.
 * @param[out] d_psi The nutation in longitude expressed in radians.
 * @param[out] d_epsilon The nutation in obliquity expressed in radians.
 **/
void iau2000a_nutation(struct julian_date *tdb, double *d_psi, double *d_epsilon)
{
	int i;
	double t,s_phi,cs,sn,phi[15],psi_lun,psi_pla,eps_lun,eps_pla;

	t = JULIAN_CENTURIES(tdb->date1, tdb->date2);

	/* 
	 * Get fundamental arguments (planetary longitudes, precession,
	 * luni-solar values). We use 1-based indices for phi to make them
	 * consistent with the reference document.
	 */
	for (i = ARG_LONGITUDE_MERCURY; i <= ARG_LONGITUDE_NODE; i++)
		phi[i + 1] = fundamental_argument(i, t);

	psi_lun = 0;
	eps_lun = 0;
	/* To maximize precision,sum lunisolar terms with smallest terms first */
	for (i = iau2000a_lunisolar_count - 1; i >= 0 ; i--) {

		s_phi = iau2000a_lunisolar[i].l * phi[10] +
			iau2000a_lunisolar[i].lp * phi[11] +
			iau2000a_lunisolar[i].f * phi[12] +
			iau2000a_lunisolar[i].d * phi[13] +
			iau2000a_lunisolar[i].om * phi[14];

		sincos(s_phi, &sn, &cs);

		psi_lun += (iau2000a_lunisolar[i].ps +
			iau2000a_lunisolar[i].psd * t) * sn +
			iau2000a_lunisolar[i].pcp * cs;

		eps_lun += (iau2000a_lunisolar[i].ec +
			iau2000a_lunisolar[i].ecd * t) * cs +
			iau2000a_lunisolar[i].esp * sn;
	}

	psi_pla = 0;
	eps_pla = 0;
	/* Sum up planetary terms with the smallest terms first */
	for (i = 0; i < iau2000a_planetary_count; i++) {

		s_phi = iau2000a_planetary[i].mer * phi[1] +
			iau2000a_planetary[i].ven * phi[2] +
			iau2000a_planetary[i].ear * phi[3] +
			iau2000a_planetary[i].mar * phi[4] +
			iau2000a_planetary[i].jup * phi[5] +
			iau2000a_planetary[i].sat * phi[6] +
			iau2000a_planetary[i].ura * phi[7] +
			iau2000a_planetary[i].nep * phi[8] +
			iau2000a_planetary[i].gp * phi[9] +
			iau2000a_planetary[i].l * phi[10] +
			iau2000a_planetary[i].lp * phi[11] +
			iau2000a_planetary[i].f * phi[12] +
			iau2000a_planetary[i].d * phi[13] +
			iau2000a_planetary[i].om * phi[14];

		sincos(s_phi, &sn, &cs);

		psi_pla += iau2000a_planetary[i].ps * sn +
			iau2000a_planetary[i].pcp * cs;

		eps_pla += iau2000a_planetary[i].ec * cs +
			iau2000a_planetary[i].esp * sn;
	}

	/* Sum up the individual contributions */
	*d_psi = (psi_lun + psi_pla) * MAS_TO_RAD;
	*d_epsilon = (eps_lun + eps_pla) * MAS_TO_RAD;
}

/**
 * Calculates the nutation matrix using the IAU 2000A nutation model
 * in its entirety. The resulting matrix may be used in the rotation
 * of coordinates.
 *
 * @param[in] tdb TDB to be used for calculations. TT may be used for all but
 * the most exacting applications.
 * @param[out] nut_matrix The nutation matrix.
 **/
void iau2000a_nutation_matrix(struct julian_date *tdb, double nut_matrix[3][3])
{
	double epsilon,d_psi,d_epsilon,c1,c2,c3,s1,s2,s3;

	epsilon = iau2000a_mean_obliquity(tdb);
	iau2000a_nutation(tdb, &d_psi, &d_epsilon);

	sincos(epsilon, &s1, &c1);
	sincos(d_psi, &s2, &c2);
	s2 *= -1.0;
	sincos(epsilon + d_epsilon, &s3, &c3);
	s3 *= -1.0;

	nut_matrix[0][0] = (c2);
	nut_matrix[0][1] = (s2 * c1);
	nut_matrix[0][2] = (s2 * s1);

	nut_matrix[1][0] = - (s2 * c3);
	nut_matrix[1][1] = (c3 * c2 * c1) - (s1 * s3);
	nut_matrix[1][2] = (c3 * c2 * s1) + (c1 * s3);

	nut_matrix[2][0] = (s2 * s3);
	nut_matrix[2][1] = - (s3 * c2 * c1) - (s1 * c3);
	nut_matrix[2][2] = - (s3 * c2 * s1) + (c3 * c1);
}
