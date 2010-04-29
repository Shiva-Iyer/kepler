# iau2006_precession.py - Wrapper for IAU2006 precession routines
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

def iau2006_precession_matrix(tdb, toJ2000):

    prec_matrix = (c_double * 9)()

    __builtin__.libkepler.iau2006_precession_matrix(byref(tdb),
                                                    c_int(1 if toJ2000 else 0),
                                                    pointer(prec_matrix))

    return [prec_matrix[0:3], prec_matrix[3:6], prec_matrix[6:9]]

__builtin__.libkepler.iau2006_precession_matrix.restype = None
__builtin__.libkepler.iau2006_precession_matrix.argtypes = [
    POINTER(JulianDate),
    c_int,
    POINTER(c_double * 9)
]

__all__ = [
    "iau2006_precession_matrix"
]
