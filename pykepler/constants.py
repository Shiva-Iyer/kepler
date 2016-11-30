# constants.py - Constants and utility routines
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
    TWO_PI = 2.0*PI

    J2000_EPOCH = 2451545.0
    MJD_EPOCH = 2400000.5

    GAUSS_GRAV_CONSTANT	= 0.01720209895
    AU = 149597870.691

class Conversion:

    DEG_TO_RAD = Constants.PI/180.0
    ACS_TO_RAD = DEG_TO_RAD/3600.0
    MAS_TO_RAD = ACS_TO_RAD/1000.0
    UAS_TO_RAD = MAS_TO_RAD/1000.0

    RAD_TO_DEG = 180.0/Constants.PI
    RAD_TO_ACS = 1.0/ACS_TO_RAD

    HRS_TO_DEG = 15.0
    DEG_TO_HRS = 1.0/HRS_TO_DEG

    RAD_TO_HRS = RAD_TO_DEG*DEG_TO_HRS
    HRS_TO_RAD = 1.0/RAD_TO_HRS

class DegMinSec:
    """ The __init__ method converts an angle expressed as a 
    decimal into degrees, minutes and seconds.

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
