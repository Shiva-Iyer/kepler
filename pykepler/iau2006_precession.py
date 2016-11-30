# iau2006_precession.py - Wrapper for IAU2006 precession routines
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

def iau2006_precession_matrix(tdb, toJ2000):
    """
    Calculate the precession matrix using the IAU 2006 precession model
    in its entirety. The resulting matrix may be used in the rotation of
    coordinates.

    tdb -- TDB to be used for calculations. TT may be used for all but the
    most exacting applications.
    toJ2000 -- If False, the resulting matrix is for precessing from J2000
    to another epoch. If True, precession is from another epoch to J2000.

    Return 1: The precession matrix in the form 
              [[m11, m12, m13], [m21, m22, m23], [m31, m32, m33]]
    """

    prec_matrix = (c_double * 9)()

    _libkepler.iau2006_precession_matrix(byref(tdb),
                                         c_int(1 if toJ2000 else 0),
                                         pointer(prec_matrix))

    return [prec_matrix[0:3], prec_matrix[3:6], prec_matrix[6:9]]

_libkepler.iau2006_precession_matrix.restype = None
_libkepler.iau2006_precession_matrix.argtypes = [
    POINTER(JulianDate),
    c_int,
    POINTER(c_double * 9)
]

__all__ = [
    "iau2006_precession_matrix"
]
