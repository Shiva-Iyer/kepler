# earth_figure.py - Wrapper for earth figure routines
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

def earth_figure_values(geog_latitude, height_msl):
    """
    Calculate various values related to the oblateness of the Earth.

    geog_latitude -- The observer's geographic latitude in radians.
    height_msl -- The observer's height above mean sea level in meters.

    Return 1: The observer's geocentric latitude in radians.
    Return 2: The observer's geocentric radius as a fraction of the
              Earth's equatorial radius.
    """

    geocentric_latitude = c_double()
    geocentric_radius = c_double()

    _libkepler.earth_figure_values(geog_latitude, height_msl,
                                   byref(geocentric_latitude),
                                   byref(geocentric_radius))

    return(geocentric_latitude.value, geocentric_radius.value)

def earth_gcdist(lon1, lat1, lon2, lat2):
    """
    Calculate the great circle distance and bearings between two points,
    taking into account the oblateness of the Earth.

    lon1 -- Longitude of pt. 1 in radians, positive east of Greenwich.
    lat1 -- Latitude of pt. 1 in radians, positive north of the equator.
    lon2 -- Longitude of pt. 2 in radians, positive east of Greenwich.
    lat2 -- Latitude of pt. 2 in radians, positive north of the equator.

    Return 1: The great circle distance in meters or -1 if the algorithm
              failed, which usually happens for antipodal points.
    Return 2: The initial bearing on the great circle in radians,
              measured clockwise from true north.
    Return 3: The final bearing on the great circle in radians,
              measured clockwise from true north.
    """

    inb = c_double()
    fib = c_double()

    dis = _libkepler.earth_gcdist(lon1, lat1, lon2, lat2,
                                  byref(inb), byref(fib))

    return(dis, inb.value, fib.value)

def earth_gcend(lon1, lat1, inb, dist):
    """
    Calculate the location on the great circle that lies at the given
    distance and initial bearing from a starting location.

    lon1 -- Longitude of the starting point in radians (E positive).
    lat1 -- Latitude of the starting point in radians (N positive).
    inb  -- The initial bearing on the great circle in radians,
            measured clockwise from true north.
    dist -- The distance along the great circle in meters.

    Return 1: The longitude of the destination in radians.
    Return 2: The latitude of the destination in radians.
    Return 3: The final bearing in radians or -1 if the algorithm
              failed, which usually happens for antipodal points.
    """

    lon2 = c_double()
    lat2 = c_double()

    fib = _libkepler.earth_gcend(lon1, lat1, inb, dist,
                                 byref(lon2), byref(lat2))

    return(lon2.value, lat2.value, fib)

_libkepler.earth_figure_values.restype = None
_libkepler.earth_figure_values.argtypes = [
    c_double,
    c_double,
    POINTER(c_double),
    POINTER(c_double)
]

_libkepler.earth_gcdist.restype = c_double
_libkepler.earth_gcdist.argtypes = [
    c_double,
    c_double,
    c_double,
    c_double,
    POINTER(c_double),
    POINTER(c_double)
]

_libkepler.earth_gcend.restype = c_double
_libkepler.earth_gcend.argtypes = [
    c_double,
    c_double,
    c_double,
    c_double,
    POINTER(c_double),
    POINTER(c_double)
]

__all__ = [
    "earth_figure_values",
    "earth_gcdist",
    "earth_gcend"
]
