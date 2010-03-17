/*
 * elp82b.c - Routines to implement the ELP 2000-82B lunar theory 
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
 * Reference : "Lunar Solution ELP 2000-82B" - M. Chapront-Touze, J. Chapront
 *             <ftp://cdsarc.u-strasbg.fr/pub/cats/VI/79/>
 */

#include <math.h>
#include <memory.h>
#include <kepler.h>
#include <fund_args.h>
#include <elp82b.h>

/* These arrays in elp82b_data.c contain the series of terms in the theory */
extern void *elp_terms[];
extern u_short elp_term_count[];

/*
 * Calculates the Moon's geocentric ecliptic coordinates and distance using the
 * ELP 2000-82B lunar theory in its entirety.
 *
 * tdb TDB to be used for calculations. TT may be used for all but
 * the most exacting applications.
 * ecl The Moon's geocentric ecliptic coordinates. The reference
 * frame is the equinox & ecliptic of J2000.
 * radius The Moon's distance from the Earth in KM.
 */
void elp82b_coordinates(struct julian_date *tdb,
			struct ecliptic_coordinates *ecl, double *radius)
{
	int i,j,k;
	struct elp82b_term1 *p1;
	struct elp82b_term2 *p2;
	struct elp82b_term3 *p3;
	double t,w,T,d,l,lp,f,m,alpha2_3m,d_nu,dnu2_3nu,zeta,me,ve,ma,ju,sa,
		ur,ne,x,y,lbr1[3],lbr2[3],lbr3[3];

	t = JULIAN_CENTURIES(tdb->date1, tdb->date2);

	/* Delaunay arguments */
	d  = fundamental_argument(ARG_ELONGATION_MOON, t);
	l  = fundamental_argument(ARG_ANOMALY_MOON, t);
	f  = fundamental_argument(ARG_LATITUDE_MOON, t);
	lp = fundamental_argument(ARG_ANOMALY_SUN, t);

	/*
	 * Mean longitude of the earth. A slightly different expression from the
	 * fundamental argument ARG_LONGITUDE_EARTH used by the new IAU 
	 * precession/nutation theories.
	 */
	T = (361679.22059 +
		(129597742.2758 +
		(-0.0202 +
		(0.000009 + 0.00000015 * t) * t) * t) * t) * ACS_TO_RAD;

	/* Mean longitude of the moon */
	w = fundamental_argument(ARG_LONGITUDE_MOON, t);

	/* Constants needed for additive corrections to A in the main problem */
	m = 129597742.2758 / 1732559343.73604;
	alpha2_3m = 0.00514376267 / (3.0 * m);
	d_nu = (-0.06424 - m * 0.55604) / 1732559343.73604;
	dnu2_3nu = 1.11208 / 5197678031.20812;

	/* 
	 * The following constants, fitted to DE200/LE200, are used to correct A
	 * in the main problem. b2, b3 and b4 in elp82b_data.c have already been
	 * multiplied with these values.
	 * del_e (0".01789), del_eprime (-0".12879), del_g (-0".08066)
	 */
 
	/*
	 * Angle of mean ecliptic of date wrt equinox of J2000, corrected with
	 * the precessional constant.
	 */
	zeta = (785939.95571 + 1732564372.83264 * t) * ACS_TO_RAD;

	/* Longitudes of the planets, used in calculating perturbations */
	me = fundamental_argument(ARG_LONGITUDE_MERCURY, t);
	ve = fundamental_argument(ARG_LONGITUDE_VENUS, t);
	ma = fundamental_argument(ARG_LONGITUDE_MARS, t);
	ju = fundamental_argument(ARG_LONGITUDE_JUPITER, t);
	sa = fundamental_argument(ARG_LONGITUDE_SATURN, t);
	ur = fundamental_argument(ARG_LONGITUDE_URANUS, t);
	ne = fundamental_argument(ARG_LONGITUDE_NEPTUNE, t);

	memset(lbr1, 0, sizeof(lbr1));
	memset(lbr2, 0, sizeof(lbr2));
	memset(lbr3, 0, sizeof(lbr3));

	/* Main problem, files elp1 to elp3 */
	for (i = 1; i <= 3; i++) {
		k = i - 1;
		p1 = elp_terms[i];
		for (j = elp_term_count[i] - 1; j >= 0; j--) {
			x = p1->i1 * d + p1->i2 * lp + p1->i3 * l + p1->i4 * f;
			y = p1->a + (p1->b1 + p1->b5 * alpha2_3m) * d_nu +
				p1->b2 + p1->b3 + p1->b4;
			if (i != 3) {
				lbr1[k] += y * sin(x);
			} else {
				y -= p1->a * dnu2_3nu;
				lbr1[k] += y * cos(x);
			}
			p1++;
		}
	}

	/* 
	 * Perturbations: Earth (elp4-elp9), tidal effects (elp22-elp27),
	 * Moon (elp28-elp30), relativistic (elp31-elp33), planetary effects
	 * on solar eccentricity (elp34-elp36).
	 */
	for (i = 4; i <= 36; i++) {
		if (i >= 10 && i <= 21)
			continue;

		k = (i - 1) % 3;
		p2 = elp_terms[i];
		for (j = elp_term_count[i] - 1; j >= 0; j--) {
			x = p2->i1 * zeta + p2->i2 * d + p2->i3 * lp +
				p2->i4 * l + p2->i5 * f + p2->phi;
			y = p2->a;
			if ((i >= 7 && i <= 9) || (i >= 25 && i <= 27))
				y *= t;
			if (i >= 34 && i <= 36)
				y = (y * t) * t;
			lbr2[k] += y * sin(x);
			p2++;
		}
	}

	/* Planetary perturbations, files elp10 to elp21 */
	for (i = 10; i <= 21; i++) {
		k = (i - 1) % 3;
		p3 = elp_terms[i];
		for (j = elp_term_count[i] - 1; j >= 0; j--) {
			x = p3->phi + p3->i1 * me + p3->i2 * ve + p3->i3 * T +
				p3->i4 * ma + p3->i5 * ju + p3->i6 * sa +
				p3->i7 * ur + p3->i10 * l + p3->i11 * f;
			if (i <= 15) {
				x += p3->i8 * ne + p3->i9 * d;
			} else {
				x += p3->i8 * d + p3->i9 * lp;
			}
			y = p3->a;
			if ((i >= 13 && i <= 15) || (i >= 19 && i <= 21))
				y *= t;
			lbr3[k] += y * sin(x);
			p3++;
		}
	}

	/* Sum up the individual contributions */
	ecl->longitude = reduce_angle(w + (lbr1[0] + lbr2[0] + lbr3[0]) * ACS_TO_RAD, TWO_PI);
	ecl->latitude = (lbr1[1] + lbr2[1] + lbr3[1]) * ACS_TO_RAD;
	*radius = lbr1[2] + lbr2[2] + lbr3[2];
}
