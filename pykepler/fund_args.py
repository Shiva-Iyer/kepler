# fund_args.py - Wrapper for fundamental argument routines
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

class FundArgs:
    """
    Constants for the various fundamental arguments used throughout the
    library. These values are parameters to the function
    fundamental_argument().
    """

    ARG_LONGITUDE_MERCURY, \
    ARG_LONGITUDE_VENUS,   \
    ARG_LONGITUDE_EARTH,   \
    ARG_LONGITUDE_MARS,    \
    ARG_LONGITUDE_JUPITER, \
    ARG_LONGITUDE_SATURN,  \
    ARG_LONGITUDE_URANUS,  \
    ARG_LONGITUDE_NEPTUNE, \
    ARG_PRECESSION,        \
    ARG_ANOMALY_MOON,      \
    ARG_ANOMALY_SUN,       \
    ARG_LATITUDE_MOON,     \
    ARG_ELONGATION_MOON,   \
    ARG_LONGITUDE_NODE,    \
    ARG_LONGITUDE_MOON = range(15)

def fundamental_argument(argument, j2000_centuries):
    """
    Calculates values for the various fundamental arguments used in the
    planetary, lunar, precession and nutation models.

    arguments -- One of the constants from the FundArgs class.
    j2000_centuries -- Julian centuries of TDB since 2000-01-01 12h TDB.
    TT may be used for all but the most exacting applications.

    Return 1: -1 if the argument parameter is invalid. The value of the
              fundamental argument in radians otherwise.
    """

    return _libkepler.fundamental_argument(argument, j2000_centuries)

_libkepler.fundamental_argument.restype = c_double
_libkepler.fundamental_argument.argtypes = [
    c_int,
    c_double
]

__all__ = [
    "FundArgs",
    "fundamental_argument"
]
