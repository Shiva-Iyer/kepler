/*
 * riseset.c - Implementation to calculate rise/transit/set times
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

/*
 * Implement a modified version of the algorithm in Chapter 15 of Meeus
 * using Lagrange interpolation on a larger set of planet positions. The
 * accuracy improvements over the Meeus approach are significant,
 * especially for the Moon and Mercury.
 */

#include <kepler.h>
#include <riseset.h>

/*
 * Calculate rise/transit/set times for an object given its positions in
 * equatorial coordinates.
 *
 * N    -- Number of entries in <df>, <ra>, <dec>
 * df   -- Array of day fractions, each in the range [0,1]
 * ra   -- RA of the object in radians at the times given in <df>
 * dec  -- Declination of the object in radians at the times given in <df>
 * gast -- Greenwich apparent sidereal time in radians at <df[0]>
 * lon  -- Observer's longitude in radians, positive east of Greenwich
 * lat  -- Observer's latitude in radians, positive north of the equator
 * delt -- Delta-T in seconds at <df[0]>
 * h0   -- Correction to use for atmospheric refraction in radians
 * rts  -- On exit: rts[0] = rise, rts[1] = transit, rts[2] = setting times,
 *         all in UTC day fractions in the range [0,1]. Values will be -1
 *         for objects that don't rise/transit/set
 */
void riseset(int N, double *df, double *ra, double *dec, double gast,
	     double lon, double lat, double delt, double h0, double *rts)
{
  int i,j;
  double cH0,m,t0,n,r,d,H,h,dm;

  rts[0] = rts[1] = rts[2] = -1;

  cH0 = (sin(h0) - sin(lat)*sin(dec[0]))/(cos(lat)*cos(dec[0]));
  if (cH0 < -1 || cH0 > 1)
    return;
  cH0 = acos(cH0);

  for (i = 0; i < 3; i++) {
    m = (ra[0] - lon - gast)/TWO_PI;
    if (i == 0)
      m -= cH0/TWO_PI;
    else if (i == 2)
      m += cH0/TWO_PI;

    for (j = 0; j < 10; j++) {
      n = m + delt/86400;
      r = interpolate(N, df, ra, n);
      d = interpolate(N, df, dec, n);

      t0 = gast + 360.985647*DEG_TO_RAD*m;
      H = t0 + lon - r;
      h = asin(sin(lat)*sin(d) + cos(lat)*cos(d)*cos(H));
      if (i == 1)
	dm = -H/TWO_PI;
      else
	dm = (h-h0)/(TWO_PI*cos(d)*cos(lat)*sin(H));

      m += dm;
      if (fabs(dm) <= 1E-5)
	break;
    }
    rts[i] = m;
  }
}

/*
 * Interpolate using Lagrange's interpolation formula.
 *
 * N    -- Number of entries in <X>, <Y>
 * X    -- x-values for interpolation
 * Y    -- y-values for interpolation
 * xint -- Interpolant
 *
 * Return: Interpolated y-value corresponding to <xint>.
 */
double interpolate(int N, double *X, double *Y, double xint)
{
  int i,j;
  double L,yint;

  yint = 0;
  for (i = 0; i < N; i++) {
    L = 1;
    for (j = 0; j < N; j++) {
      if (i != j)
	L *= (xint - X[j])/(X[i] - X[j]);
    }
    yint += L*Y[i];
  }

  return(yint);
}
