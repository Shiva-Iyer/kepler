# mpc_file.py - Wrapper for MPCORB database routines
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
from coordinates import *
from orbital_elements import *

class MpcBody(Structure):

    _fields_ = [
        ("id", c_char * 12),
        ("name", c_char * 36),
        ("type", c_int),
        ("magnitude", c_double),
        ("slope", c_double),
        ("elements", OrbitalElements),
        ("position", RectangularCoordinates)
        ]

    def __repr__(self):

        rep = dict(id = self.id,
                   name = self.name,
                   type = self.type,
                   x = self.position.x,
                   y = self.position.y,
                   z = self.position.z,
                   magnitude = self.magnitude,
                   slope = self.slope).__repr__()

        return rep

def minor_planet_info(mpcorb_record, jd_TT):

    mpc_body = MpcBody()

    retval = __builtin__.libkepler.minor_planet_info(mpcorb_record,
                                                     byref(jd_TT),
                                                     byref(mpc_body))

    return retval, mpc_body

def comet_info(mpcorb_record, jd_TT):

    mpc_body = MpcBody()

    retval = __builtin__.libkepler.comet_info(mpcorb_record,
                                              byref(jd_TT),
                                              byref(mpc_body))

    return retval, mpc_body

__builtin__.libkepler.minor_planet_info.argtypes = [
    c_char_p,
    POINTER(JulianDate),
    POINTER(MpcBody)
]

__builtin__.libkepler.comet_info.argtypes = [
    c_char_p,
    POINTER(JulianDate),
    POINTER(MpcBody)
]

__all__ = [
    "MpcBody",
    "minor_planet_info",
    "comet_info"
]
