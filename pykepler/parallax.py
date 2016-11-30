# parallax.py - Wrapper for geocentric parallax routines
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

def geocentric_parallax(hour_angle, declination, distance,
			geog_latitude, height_msl):
    """
    Calculate the geocentric parallax in the equatorial coordinates of a
    celestial body.

    hour_angle -- The body's local hour angle in radians.
    declination -- The body's declination in radians.
    distance -- The body's distance from the Earth in AU. 
    geog_latitude -- The observer's geographic latitude in radians.
    height_msl -- The observer's height above mean sea level in meters.

    Return 1: The parallax in right ascension expressed in radians.
    Return 2: The parallax in declination expressed in radians.
    """

    d_RA = c_double()
    d_declination = c_double()

    _libkepler.geocentric_parallax(hour_angle, declination, distance,
                                   geog_latitude, height_msl,
                                   byref(d_RA), byref(d_declination))

    return d_RA.value, d_declination.value

_libkepler.geocentric_parallax.restype = None
_libkepler.geocentric_parallax.argtypes = [
    c_double,
    c_double,
    c_double,
    c_double,
    c_double,
    POINTER(c_double),
    POINTER(c_double)
]

__all__ = [
    "geocentric_parallax"
]
