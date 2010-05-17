# elp82b.py - Wrapper for ELP2000-82B routines
# Copyright (C) 2010 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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
from julian_date import *
from coordinates import *
from pykepler import _libkepler

def elp82b_coordinates(tdb):
    """
    Calculate the Moon's geocentric ecliptic coordinates and distance using the
    ELP 2000-82B lunar theory in its entirety.

    tdb -- TDB to be used for calculations. TT may be used for all but the most
           exacting applications.

    Return 1: The Moon's geocentric ecliptic coordinates. The reference frame is
              the equinox & ecliptic of J2000.
    Return 2: The Moon's distance from the Earth in KM.

    """
    ecliptic = EclipticCoordinates()
    radius = c_double()

    _libkepler.elp82b_coordinates(byref(tdb), byref(ecliptic), byref(radius))

    return ecliptic, radius.value

_libkepler.elp82b_coordinates.restype = None
_libkepler.elp82b_coordinates.argtypes = [
    POINTER(JulianDate),
    POINTER(EclipticCoordinates),
    POINTER(c_double)
]

__all__ = [
    "elp82b_coordinates"
]
