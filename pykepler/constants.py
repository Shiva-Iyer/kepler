# constants.py - Constants and utility routines
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

from math import *

class SolarSystemPlanets:
    
    MERCURY, \
    VENUS,   \
    EARTH,   \
    MARS,    \
    JUPITER, \
    SATURN,  \
    URANUS,  \
    NEPTUNE = range(8)

class ReturnValues:

    SUCCESS = 0
    ERR_INVALID_PLANET = -1
    ERR_INVALID_DATE = -2
    ERR_INVALID_ECCENTRICITY = -3
    ERR_CONVERGENCE = -4
    ERR_INVALID_DATA = -5

class Constants:

    PI = 3.141592653589793238462643
    TWO_PI = 6.283185307179586476925287
    SQROOT_TWO = 1.414213562373095048801689

    J2000_EPOCH = 2451545.0
    MJD_EPOCH = 2400000.5

    GAUSS_GRAV_CONSTANT	= 0.01720209895
    AU = 149597870.691

class Conversion:

    DEG_TO_RAD = 0.01745329251994329576923691
    ACS_TO_RAD = 0.000004848136811095359935899141
    MAS_TO_RAD = 0.000000004848136811095359935899141
    UAS_TO_RAD = 0.000000000004848136811095359935899141

    RAD_TO_DEG = 57.295779513082320876798147
    RAD_TO_ACS = 206264.806247096

    DEG_TO_HRS = 0.066666666666666666666667
    HRS_TO_DEG = 15.0

    RAD_TO_HRS = 3.81971863420548805845321
    HRS_TO_RAD = 0.261799387799149436538554

class DegMinSec:
    """ The __init__ method converts an angle expressed as a decimal into degrees,
    minutes and seconds.

    Fields:

    degrees
    minutes
    seconds

    """

    def __init__(self, d = 0.0):

	tmp = fabs(d)
	self.degrees = floor(tmp)
	tmp = (tmp - self.degrees) * 60
	self.minutes = floor(tmp)
	self.seconds = (tmp - self.minutes) * 60

	if (d < 0):
            self.degrees *= -1
            self.minutes *= -1
            self.seconds *= -1

    def __repr__(self):

        rep = dict(deg = self.degrees,
                   min = self.minutes,
                   sec = self.seconds).__repr__()

        return rep

def degrees(d, m, s):
    """Return the decimal value of an angle expressed in degrees, minutes
    and seconds.

    """
    return d + (m / 60.0) + (s / 3600.0)

__all__ = [
    "SolarSystemPlanets",
    "ReturnValues",
    "Constants",
    "Conversion",
    "DegMinSec",
    "degrees"
]
