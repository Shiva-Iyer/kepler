# fund_args.py - Wrapper for fundamental argument routines
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

def fundamental_argument(argument, j2000_centuries):

    return __builtin__.libkepler.fundamental_argument(argument, j2000_centuries)

__builtin__.libkepler.fundamental_argument.restype = c_double
__builtin__.libkepler.fundamental_argument.argtypes = [
    c_int,
    c_double
]

__all__ = [
    "fundamental_argument"
]
