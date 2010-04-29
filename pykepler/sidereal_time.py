# sidereal_time.py - Wrapper for sidereal time routines
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

import __builtin__
from ctypes import *
from julian_date import *

def mean_sidereal_time(ut1, tdb, longitude):

    return __builtin__.libkepler.mean_sidereal_time(byref(ut1),
                                                    byref(tdb),
                                                    longitude)

def apparent_sidereal_time(ut1, tdb, longitude):

    return __builtin__.libkepler.apparent_sidereal_time(byref(ut1),
                                                        byref(tdb),
                                                        longitude)

def equation_of_the_equinoxes(tdb):

    return __builtin__.libkepler.equation_of_the_equinoxes(byref(tdb))

__builtin__.libkepler.mean_sidereal_time.restype = c_double
__builtin__.libkepler.mean_sidereal_time.argtypes = [
    POINTER(JulianDate),
    POINTER(JulianDate),
    c_double
]

__builtin__.libkepler.apparent_sidereal_time.restype = c_double
__builtin__.libkepler.apparent_sidereal_time.argtypes = [
    POINTER(JulianDate),
    POINTER(JulianDate),
    c_double
]

__builtin__.libkepler.equation_of_the_equinoxes.restype = c_double
__builtin__.libkepler.equation_of_the_equinoxes.argtypes = [
    POINTER(JulianDate)
]

__all__ = [
    "mean_sidereal_time",
    "apparent_sidereal_time",
    "equation_of_the_equinoxes"
]
