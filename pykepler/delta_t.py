# delta_t.py - Wrapper for delta-T routines
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
from pykepler import _libkepler

def delta_t(year, month):
    """Return an approximate value for Delta-T (TT - UT1) valid for a
    limited historical period.

    year -- Year number in astronomical reckoning, between
    2000BC and 3000AD.
    month -- Month of the year.

    Return 1: SUCCESS -- Delta-T calculated successfully.
              ERR_INVALID_DATE -- Invalid year or month specified.
    Return 2: An approximate value for Delta-T in seconds.
    Return 3: A correction, in seconds, to be added to Delta-T for years
    before 1955AD or after 2005AD and when used in conjunction with
    NASA's publication "Five millennium canon of solar eclipses".
    """

    del_t = c_double()
    correction = c_double()

    retval = _libkepler.delta_t(year, month, byref(del_t),
                                byref(correction))

    return retval, del_t.value, correction.value

_libkepler.delta_t.argtypes = [
    c_int,
    c_int,
    POINTER(c_double),
    POINTER(c_double)
]

__all__ = [
    "delta_t"
]
