# delta_t.py - Wrapper for delta-T routines
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

def delta_t(year, month):

    del_t = c_double()
    correction = c_double()

    retval = __builtin__.libkepler.delta_t(year,
                                           month,
                                           byref(del_t),
                                           byref(correction))

    return retval, del_t, correction

__builtin__.libkepler.delta_t.argtypes = [
    c_int,
    c_int,
    POINTER(c_double),
    POINTER(c_double)
]

__all__ = [
    "delta_t"
]
