# vsop87.py - Wrapper for VSOP87 routines
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

def vsop87_coordinates(planet_number, tdb):

    rectangular = RectangularCoordinates()

    retval = _libkepler.vsop87_coordinates(planet_number, byref(tdb),
                                           byref(rectangular))

    return retval, rectangular

def vsop87_ecliptic_to_equator(ecliptic):

    _libkepler.vsop87_ecliptic_to_equator(byref(ecliptic))

_libkepler.vsop87_coordinates.argtypes = [
    c_int,
    POINTER(JulianDate),
    POINTER(RectangularCoordinates)
]

_libkepler.vsop87_ecliptic_to_equator.restype = None
_libkepler.vsop87_ecliptic_to_equator.argtypes = [
    POINTER(RectangularCoordinates)
]

__all__ = [
    "vsop87_coordinates",
    "vsop87_ecliptic_to_equator"
]
