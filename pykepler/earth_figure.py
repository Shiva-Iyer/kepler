# earth_figure.py - Wrapper for earth figure routines
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
from pykepler import _libkepler

def earth_figure_values(geog_latitude, height_msl):

    geocentric_latitude = c_double()
    geocentric_radius = c_double()

    _libkepler.earth_figure_values(geog_latitude, height_msl,
                                   byref(geocentric_latitude),
                                   byref(geocentric_radius))

    return geocentric_latitude.value, geocentric_radius.value

_libkepler.earth_figure_values.restype = None
_libkepler.earth_figure_values.argtypes = [
    c_double,
    c_double,
    POINTER(c_double),
    POINTER(c_double)
]

__all__ = [
    "earth_figure_values"
]
