# sidereal_time.py - Wrapper for sidereal time routines
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

def mean_sidereal_time(ut1, tdb, longitude):
    """
    Calculate the local mean sidereal time.

    ut1 -- UT1 for calculating the Earth Rotation Angle.
    tdb -- TDB to be used for calculating the LMST. TT may be used for
    all but the most exacting applications.
    longitude -- The observer's longitude in radians, positive
    east of Greenwich.

    Return 1: Local mean sidereal time in radians, between 0 and 2*PI.
    """

    return _libkepler.mean_sidereal_time(byref(ut1),
                                         byref(tdb), longitude)

def apparent_sidereal_time(ut1, tdb, longitude):
    """
    Calculate the local apparent sidereal time. Apparent sidereal time
    takes into account the motion of the equinox due to nutation.

    ut1 -- UT1 for calculating the Earth Rotation Angle.
    tdb -- TDB to be used for calculating the LAST. TT may be used
    for all but the most exacting applications.
    longitude -- The observer's longitude in radians, positive
    east of Greenwich.

    Return 1: Local apparent sidereal time in radians,
    between 0 and 2*PI.
    """

    return _libkepler.apparent_sidereal_time(byref(ut1), byref(tdb), longitude)

def equation_of_the_equinoxes(tdb):
    """
    Calculate the value of the equation of the equinoxes which accounts
    for the motion of the equinox due to nutation.

    tdb -- TDB to be used for calculations. TT may be used for all but
    the most exacting applications.

    Return 1: The value of the equation of the equinoxes in radians.
    """

    return _libkepler.equation_of_the_equinoxes(byref(tdb))

_libkepler.mean_sidereal_time.restype = c_double
_libkepler.mean_sidereal_time.argtypes = [
    POINTER(JulianDate),
    POINTER(JulianDate),
    c_double
]

_libkepler.apparent_sidereal_time.restype = c_double
_libkepler.apparent_sidereal_time.argtypes = [
    POINTER(JulianDate),
    POINTER(JulianDate),
    c_double
]

_libkepler.equation_of_the_equinoxes.restype = c_double
_libkepler.equation_of_the_equinoxes.argtypes = [
    POINTER(JulianDate)
]

__all__ = [
    "mean_sidereal_time",
    "apparent_sidereal_time",
    "equation_of_the_equinoxes"
]
