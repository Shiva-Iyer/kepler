# pluto.py - Wrapper for calculating Pluto's coordinates
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

def pluto_coordinates(tdb):

    rectangular = RectangularCoordinates()

    retval = _libkepler.pluto_coordinates(byref(tdb), byref(rectangular))

    return retval, rectangular

_libkepler.pluto_coordinates.argtypes = [
    POINTER(JulianDate),
    POINTER(RectangularCoordinates)
]

__all__ = [
    "pluto_coordinates"
]
