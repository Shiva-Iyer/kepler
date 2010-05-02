# aberration.py - Wrapper for annual aberration routines
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

def aberration_earth_velocity(tdb):

    earth_velocity = RectangularCoordinates()

    _libkepler.aberration_earth_velocity(byref(tdb), byref(earth_velocity))

    return earth_velocity

def annual_aberration(tdb, equ_coords):

    d_RA = c_double()
    d_declination = c_double()

    _libkepler.annual_aberration(byref(tdb), byref(equ_coords), byref(d_RA),
                                 byref(d_declination))

    return d_RA.value, d_declination.value

_libkepler.aberration_earth_velocity.restype = None
_libkepler.aberration_earth_velocity.argtypes = [
    POINTER(JulianDate),
    POINTER(RectangularCoordinates)
]

_libkepler.annual_aberration.restype = None
_libkepler.annual_aberration.argtypes = [
    POINTER(JulianDate),
    POINTER(EquatorialCoordinates),
    POINTER(c_double),
    POINTER(c_double)
]

__all__ = [
    "aberration_earth_velocity",
    "annual_aberration"
]
