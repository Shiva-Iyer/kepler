# moonphase.py - Wrapper for Moon phase routines
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

class MoonPhases:
    FirstQuarter, \
    FullMoon,     \
    LastQuarter,  \
    NewMoon = range(4)

def moonlit(dt):
    """
    Calculate the fraction of the Moon's disk that is illuminated.
  
    dt    -- Dynamical time for computation.
 
    Return: A fraction in the range [0,1] indicating the portion of
            the Moon's disk illuminated at <dt>.
    """

    return(_libkepler.moonlit(byref(dt)))

def moonphase(dt, phase):
    """
    Calculate the date and time of the specified Moon phase.

    dt    -- Dynamical time at which the search is to commence.
    phase -- Enumeration that identifies the phase to be computed.

    Return: Dynamical time in JDE form corresponding to the time of the
            next Moon <phase> on or after <dt>.
    """

    return(_libkepler.moonphase(byref(dt), phase))

_libkepler.moonlit.restype = c_double
_libkepler.moonlit.argtypes = [
    POINTER(JulianDate)
]

_libkepler.moonphase.restype = c_double
_libkepler.moonphase.argtypes = [
    POINTER(JulianDate),
    c_int
]

__all__ = [
    "MoonPhases",
    "moonlit",
    "moonphase"
]
