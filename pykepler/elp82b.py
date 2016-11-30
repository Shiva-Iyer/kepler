# elp82b.py - Wrapper for ELP2000-82B routines
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
from .julian_date import *
from .coordinates import *
from pykepler import _libkepler

def elp82b_coordinates(tdb):
    """
    Calculate the Moon's geocentric rectangular coordinates using the
    ELP 2000-82B lunar theory in its entirety.

    tdb -- TDB to be used for calculations. TT may be used for all
    but the most exacting applications.

    Return 1: The Moon's geocentric rectangular coordinates in KM.
    The reference frame is the equinox & ecliptic of J2000.
    """

    rectangular = RectangularCoordinates()

    _libkepler.elp82b_coordinates(byref(tdb), byref(rectangular))

    return rectangular

def elp82b_ecliptic_to_equator(ecliptic):
    """
    Rotate the Moon's coordinates from the ecliptic frame of J2000 to
    the equatorial frame of J2000/FK5.

    ecliptic -- The coordinates to be rotated in-place
    """

    _libkepler.elp82b_ecliptic_to_equator(byref(ecliptic))

_libkepler.elp82b_coordinates.restype = None
_libkepler.elp82b_coordinates.argtypes = [
    POINTER(JulianDate),
    POINTER(RectangularCoordinates)
]

_libkepler.elp82b_ecliptic_to_equator.restype = None
_libkepler.elp82b_ecliptic_to_equator.argtypes = [
    POINTER(RectangularCoordinates)
]

__all__ = [
    "elp82b_coordinates",
    "elp82b_ecliptic_to_equator"
]
