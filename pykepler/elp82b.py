# elp82b.py - Wrapper for ELP2000-82B routines
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
from coordinates import *

def elp82b_coordinates(tdb):

    ecliptic = EclipticCoordinates()
    radius = c_double()

    __builtin__.libkepler.elp82b_coordinates(byref(tdb),
                                             byref(ecliptic),
                                             byref(radius))

    return ecliptic, radius

__builtin__.libkepler.elp82b_coordinates.restype = None
__builtin__.libkepler.elp82b_coordinates.argtypes = [
    POINTER(JulianDate),
    POINTER(EclipticCoordinates),
    POINTER(c_double)
]

__all__ = [
    "elp82b_coordinates"
]
