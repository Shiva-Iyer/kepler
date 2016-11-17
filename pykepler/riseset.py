# riseset.py - Wrapper for celestial body rise/transit/set times
# Copyright (C) 2016 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

if __name__ == "__main__":
    exit()

from ctypes import *
from pykepler import _libkepler

def riseset(df, ra, dec, gast, lon, lat, delt, h0):
    """Calculate rise/transit/set times for an object given its 
    positions in equatorial coordinates.

    df   -- List of day fractions, each in the range [0,1]. Positions 
            must be given for one full day at intervals of 6 hours or
            smaller for satisfactory results, especially for the Moon
	    and Mercury.
    ra   -- RA of the object in radians at the times in <df>.
    dec  -- Declination of the object in radians at the times in <df>.
    gast -- Greenwich apparent sidereal time in radians at <df[0]>.
    lon  -- Observer's longitude in radians, positive east of Greenwich.
    lat  -- Observer's latitude in radians, positive north of the equator.
    delt -- Delta-T in seconds at <df[0]>.
    h0   -- Correction to use for atmospheric refraction in radians.

    Return: rts[0] = rise, rts[1] = transit, rts[2] = setting times,
    all in UTC day fractions in the range [0,1]. Values will be -1
    for objects that don't rise/transit/set.
    """

    N = len(df)
    rts = (c_double*3)()

    _libkepler.riseset(c_int(N),
                       cast((c_double*N)(*df), POINTER(c_double)),
                       cast((c_double*N)(*ra), POINTER(c_double)),
                       cast((c_double*N)(*dec), POINTER(c_double)),
                       c_double(gast),
                       c_double(lon),
                       c_double(lat),
                       c_double(delt),
                       c_double(h0),
                       pointer(rts))

    return(rts[0], rts[1], rts[2])

def interpolate(X, Y, xint):
    """Interpolate using Lagrange's interpolation formula.

    X    -- x-values for interpolation.
    Y    -- y-values for interpolation.
    xint -- Interpolant.

    Return: Interpolated y-value corresponding to <xint>.
    """

    N = len(X)
    return(_libkepler.interpolate(c_int(N),
                                  cast((c_double*N)(*X), POINTER(c_double)),
                                  cast((c_double*N)(*Y), POINTER(c_double)),
                                  c_double(xint)))

_libkepler.riseset.argtypes = [
    c_int,
    POINTER(c_double),
    POINTER(c_double),
    POINTER(c_double),
    c_double,
    c_double,
    c_double,
    c_double,
    c_double,
    POINTER(c_double*3)
]

_libkepler.interpolate.restype = c_double
_libkepler.interpolate.argtypes = [
    c_int,
    POINTER(c_double),
    POINTER(c_double),
    c_double
]

__all__ = [
    "riseset",
    "interpolate"
]
