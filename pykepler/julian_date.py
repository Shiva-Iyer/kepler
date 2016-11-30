# julian_date.py - Wrapper for Julian date routines
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

class JulianDate(Structure):
    """Used wherever a Julian Day Number (JDN) is expected or returned.

    A given JDN may be partitioned between date1 and date2 in any
    convenient manner. The JDN that corresponds to this class is
    date1 + date2.
    """

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
    """Calculate the Julian Day Number for a date in the proleptic
    Gregorian calendar. No account is taken of the Gregorian calendar
    reform and dates prior to Jan. 1, 4800BC are not supported.

    year -- Year number using astronomical reckoning and after 4800BC.
    month -- Month of the year.
    day -- Day of the month.

    Return 1: SUCCESS -- Julian date calculated successfully.
              ERR_INVALID_DATE -- Gregorian date specified is invalid.
    Return 2: The calculated Julian Day Number.
    """

    jd = JulianDate()

    retval = _libkepler.calendar_to_julian_date(year, month, day, jd)

    return retval, jd

def julian_to_calendar_date(jd):
    """Calculates the date in the proleptic Gregorian calendar that 
    corresponds to the given Julian Day Number. The Gregorian calendar
    reform is ignored.

    jd -- The Julian Day Number. Must correspond to a date on or after
          Jan. 1, 4800BC in the proleptic Gregorian calendar.

    Return 1: SUCCESS -- Gregorian date calculated successfully.
              ERR_INVALID_DATE -- Julian Day Number is out of range.
    Return 2: Year number using astronomical reckoning.
    Return 3: Month of the year.
    Return 4: Day of the month.
    Return 5: Fractional part of the day (0 = midnight, 0.5 = noon).
    """

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
