# iau2000a_nutation.py - Wrapper for IAU2000A nutation routines
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
from pykepler import _libkepler

def iau2000a_mean_obliquity(tdb):
    """
    Calculate the mean obliquity of the ecliptic using the
    IAU 2000 formula.

    tdb -- TDB to be used for calculations. TT may be used for
    all but the most exacting applications.

    Return 1: The mean obliquity of the ecliptic in radians.
    """

    return _libkepler.iau2000a_mean_obliquity(byref(tdb))

def iau2000a_nutation(tdb):
    """
    Calculate the nutation in longitude and obliquity using the IAU 2000A
    nutation model in its entirety.

    tdb -- TDB to be used for calculations. TT may be used for all 
    but the most exacting applications.

    Return 1: The nutation in longitude expressed in radians.
    Return 2: The nutation in obliquity expressed in radians.
    """

    nut_longitude = c_double()
    nut_latitude = c_double()

    _libkepler.iau2000a_nutation(byref(tdb), byref(nut_longitude),
                                 byref(nut_latitude))

    return nut_longitude.value, nut_latitude.value

def iau2000a_nutation_matrix(tdb):
    """
    Calculate the nutation matrix using the IAU 2000A nutation model
    in its entirety. The resulting matrix may be used in the rotation
    of coordinates.

    tdb -- TDB to be used for calculations. TT may be used for all
    but the most exacting applications.

    Return 1: The nutation matrix in the form
              [[m11, m12, m13], [m21, m22, m23], [m31, m32, m33]]
    """

    nut_matrix = (c_double * 9)()

    _libkepler.iau2000a_nutation_matrix(byref(tdb), pointer(nut_matrix))

    return [nut_matrix[0:3], nut_matrix[3:6], nut_matrix[6:9]]

_libkepler.iau2000a_mean_obliquity.restype = c_double
_libkepler.iau2000a_mean_obliquity.argtypes = [
    POINTER(JulianDate)
]

_libkepler.iau2000a_nutation.restype = None
_libkepler.iau2000a_nutation.argtypes = [
    POINTER(JulianDate),
    POINTER(c_double),
    POINTER(c_double)
]

_libkepler.iau2000a_nutation_matrix.restype = None
_libkepler.iau2000a_nutation_matrix.argtypes = [
    POINTER(JulianDate),
    POINTER(c_double * 9)
]

__all__ = [
    "iau2000a_mean_obliquity",
    "iau2000a_nutation",
    "iau2000a_nutation_matrix"
]
