# magnitude.py - Wrapper for apparent magnitude routines
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
from .coordinates import *
from pykepler import _libkepler

def minor_planet_magnitude(body_coords, earth_coords, abs_mag, slope):
    """
    Calculate the apparent magnitude of a minor planet.

    body_coords -- The minor planet's heliocentric rectangular
    coordinates. Must be in the same reference frame and units
    as ear (AU).
    earth_coords -- The Earth's heliocentric rectangular coordinates.
    Must be in the same reference frame and units as obj (AU).
    abs_mag -- The mean absolute visual magnitude of the minor planet, H.
    slope -- The magnitude slope parameter of the minor planet, G.

    Return 1: The minor planet's apparent magnitude.
    """

    return _libkepler.minor_planet_magnitude(byref(body_coords),
                                             byref(earth_coords),
                                             abs_mag, slope)

def comet_magnitude(body_coords, earth_coords, abs_mag, slope):
    """
    Calculates the apparent magnitude of a comet.

    body_coords -- The comet's heliocentric rectangular coordinates.
    Must be in the same reference frame and units as ear (AU).
    earth_coords -- The Earth's heliocentric rectangular coordinates.
    Must be in the same reference frame and units as obj (AU).
    abs_mag -- The mean absolute visual magnitude of the comet, H.
    slope -- The magnitude slope parameter of the comet, G.

    Return 1: The comet's apparent magnitude.
    """

    return _libkepler.comet_magnitude(byref(body_coords), byref(earth_coords),
                                      abs_mag, slope)

_libkepler.minor_planet_magnitude.restype = c_double
_libkepler.minor_planet_magnitude.argtypes = [
    POINTER(RectangularCoordinates),
    POINTER(RectangularCoordinates),
    c_double,
    c_double
]

_libkepler.comet_magnitude.restype = c_double
_libkepler.comet_magnitude.argtypes = [
    POINTER(RectangularCoordinates),
    POINTER(RectangularCoordinates),
    c_double,
    c_double
]

__all__ = [
    "minor_planet_magnitude",
    "comet_magnitude"
]
