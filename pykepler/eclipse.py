# eclipse.py - Wrapper for eclipse calculations
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

class EclipseTypes:
    Solar, \
    Lunar = range(2)

def eclipse(dt, ecl):
    """
    Calculate the date and time of solar and lunar eclipses.

    dt  -- Dynamical time at which the search is to commence.
    ecl -- The type of eclipse to be searched for.

    Return 1: Dynamical time in JDE form corresponding to the time of the
    next <type> eclipse on or after <dt> at its maximum.
    Return 2: Extra eclipse data. See Meeus for details on <gamma>.
    Return 3: Additional eclipse data. See Meeus for details on <u>.
    """

    g = c_double()
    u = c_double()
    t = _libkepler.eclipse(byref(dt), ecl, byref(g), byref(u))

    return(t, g.value, u.value)

_libkepler.eclipse.restype = c_double
_libkepler.eclipse.argtypes = [
    POINTER(JulianDate),
    c_int,
    POINTER(c_double),
    POINTER(c_double)
]

__all__ = [
    "EclipseTypes",
    "eclipse"
]
