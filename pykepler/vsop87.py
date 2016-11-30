# vsop87.py - Wrapper for VSOP87 routines
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

def vsop87_coordinates(planet_number, tdb):
    """
    Calculate a major planet's heliocentric rectangular coordinates
    using the VSOP87 (version A) theory in its entirety.

    planet_number -- A constant from the class SolarSystemPlanets,
    identifying the planet.
    tdb -- TDB to be used for calculations. TT may be used for all
    but the most exacting applications.

    Return 1: SUCCESS -- Coordinates were calculated successfully.
              ERR_INVALID_PLANET -- Planet's identifier is invalid.
    Return 2: The planet's heliocentric rectangular coordinates
    in AU. The reference frame is the equinox & ecliptic of J2000.
    """

    rectangular = RectangularCoordinates()

    retval = _libkepler.vsop87_coordinates(planet_number, byref(tdb),
                                           byref(rectangular))

    return retval, rectangular

def vsop87_ecliptic_to_equator(ecliptic):
    """
    Rotate a body's coordinates from the dynamical ecliptic frame
    of J2000 to the equatorial frame of J2000/FK5.

    ecliptic -- The coordinates to be rotated in-place
    """

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
