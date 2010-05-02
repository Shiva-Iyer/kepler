# julian_date.py - Wrapper for Julian date routines
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

class JulianDate(Structure):

    _fields_ = [
        ("date1", c_double),
        ("date2", c_double)
        ]

    def __init__(self, date1 = 0.0, date2 = 0.0):

        Structure.__init__(self)
        self.date1 = date1
        self.date2 = date2

    def __repr__(self):

        return dict(date1 = self.date1, date2 = self.date2).__repr__()

def calendar_to_julian_date(year, month, day):

    jd = JulianDate()

    retval = _libkepler.calendar_to_julian_date(year, month, day, jd)

    return retval, jd

def julian_to_calendar_date(jd):

    year = c_int()
    month = c_int()
    day = c_int()
    day_frac = c_double()

    retval = _libkepler.julian_to_calendar_date(byref(jd), byref(year),
                                                byref(month), byref(day),
                                                byref(day_frac))

    return retval, year.value, month.value, day.value, day_frac.value

_libkepler.calendar_to_julian_date.argtypes = [
    c_int,
    c_int,
    c_int,
    POINTER(JulianDate)
]

_libkepler.julian_to_calendar_date.argtypes = [
    POINTER(JulianDate),
    POINTER(c_int),
    POINTER(c_int),
    POINTER(c_int),
    POINTER(c_double)
]

__all__ = [
    "JulianDate",
    "calendar_to_julian_date",
    "julian_to_calendar_date"
]
