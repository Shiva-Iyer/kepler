# orbital_elements.py - Wrapper for orbital elements routines
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
from julian_date import *
from coordinates import *
from pykepler import _libkepler

class OrbitalElements(Structure):

    _fields_ = [
        ("epoch", JulianDate),
        ("mean_anomaly", c_double),
        ("daily_motion", c_double),
        ("perihelion", c_double),
        ("eccentricity", c_double),
        ("arg_perihelion", c_double),
        ("lon_asc_node", c_double),
        ("inclination", c_double)
        ]

    def __init__(self, epoch = JulianDate(), mean_anomaly = 0.0,
                 daily_motion = 0.0, perihelion = 0.0, eccentricity = 0.0,
                 arg_perihelion = 0.0, lon_asc_node = 0.0, inclination = 0.0):

        Structure.__init__(self)
        self.epoch = epoch
        self.mean_anomaly = mean_anomaly
        self.daily_motion = daily_motion
        self.perihelion = perihelion
        self.eccentricity = eccentricity
        self.arg_perihelion = arg_perihelion
        self.lon_asc_node = lon_asc_node
        self.inclination = inclination

    def __repr__(self):

        rep = dict(T = self.epoch.date1 + self.epoch.date2,
                   M = self.mean_anomaly,
                   n = self.daily_motion,
                   q = self.perihelion,
                   e = self.eccentricity,
                   w = self.arg_perihelion,
                   W = self.lon_asc_node,
                   i = self.inclination).__repr__()

        return rep

def elements_to_ephemeris(jd_TT, orb_elements):

    coordinates = RectangularCoordinates()

    retval = _libkepler.elements_to_ephemeris(byref(jd_TT), byref(orb_elements),
                                              byref(coordinates))

    return retval, coordinates

def eccentric_anomaly(mean_ano, eccentricity):

    ecc_anomaly = c_double()

    retval = _libkepler.eccentric_anomaly(mean_ano, eccentricity,
                                          byref(ecc_anomaly))

    return retval, ecc_anomaly.value

def hyperbolic_anomaly(mean_ano, eccentricity):

    hyp_anomaly = c_double()

    retval = _libkepler.hyperbolic_anomaly(mean_ano, eccentricity,
                                           byref(hyp_anomaly))

    return retval, hyp_anomaly.value

_libkepler.elements_to_ephemeris.argtypes = [
    POINTER(JulianDate),
    POINTER(OrbitalElements),
    POINTER(RectangularCoordinates)
]

_libkepler.eccentric_anomaly.argtypes = [
    c_double,
    c_double,
    POINTER(c_double)
]

_libkepler.hyperbolic_anomaly.argtypes = [
    c_double,
    c_double,
    POINTER(c_double)
]

__all__ = [
    "OrbitalElements",
    "elements_to_ephemeris",
    "eccentric_anomaly",
    "hyperbolic_anomaly"
]
