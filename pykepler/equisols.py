# equisols.py - Wrapper for equinox and solstice calculations.
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

def equisols(year, month):
    """
    Calculate the time of the equinox or solstice in the specified year
    and month.

    year  -- Year number using astronomical reckoning and after 4800BC.
    month -- 3 for March equinox, 6 for June solstice, 9 for September
             equinox, or 12 for December solstice.

    Return: Dynamical time in JDE form at which the requested equinox
            or solstice occurs; -1 if the input parameters are invalid.
    """

    return(_libkepler.equisols(year, month))

_libkepler.equisols.restype = c_double
_libkepler.equisols.argtypes = [
    c_int,
    c_int
]

__all__ = [
    "equisols"
]
