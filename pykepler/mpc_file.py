# mpc_file.py - Wrapper for MPCORB database routines
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
from .coordinates import *
from .orbital_elements import *
from pykepler import _libkepler

class MpcBody(Structure):
    """
    Used to store the details of a celestial body in the MPCORB Database.

    Fields:

    id -- The body's ID or unpacked provisional designation.
    name -- The body's name, if it is included in the file.
    type -- The type of body.
    magnitude -- The body's absolute magnitude, H.
    slope -- Slope parameter, G.
    elements -- The body's orbital elements.
    position --	The body's coordinates at the desired epoch. For minor
    planets and comets, the origin is the Sun and the reference frame is
    the equinox and ecliptic of J2000.
    """

    MINOR_PLANET,       \
    LONG_PERIOD_COMET,  \
    SHORT_PERIOD_COMET, \
    DEFUNCT_COMET,      \
    UNCERTAIN_COMET = range(5)

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
    """
    Parse data in the MPC's Orbit Database for minor planets. For
    details on the format, see
    <http://www.cfa.harvard.edu/iau/info/MPOrbitFormat.html>.
    Download the database from
    <http://www.cfa.harvard.edu/iau/MPCORB.html>.

    mpcorb_record -- A line of data from the MPCORB minor planet
    database file.
    jd_TT -- TT to be used for calculations.

    Return 1: SUCCESS -- Data was parsed and coordinates were calculated.
              ERR_INVALID_DATA -- The data was in an unrecognized format.
              ERR_CONVERGENCE -- The solution to Kepler's equation didn't
              converge to the required precision.
    Return 2: The minor planet's details and heliocentric coordinates.
    The reference frame is the equinox & ecliptic of J2000.
    """

    body = MpcBody()

    retval = _libkepler.minor_planet_info(mpcorb_record, byref(jd_TT),
                                          byref(body))

    return retval, body

def comet_info(mpcorb_record, jd_TT):
    """
    Parse data in the MPC's Orbit Database for comets. For details,
    see <http://www.cfa.harvard.edu/iau/info/CometOrbitFormat.html>.
    Download from <http://www.cfa.harvard.edu/iau/MPCORB.html>.

    mpcorb_record -- A line of data from the MPCORB comet database file.
    jd_TT -- TT to be used for calculations.

    Return 1: SUCCESS -- Data was parsed and coordinates were calculated.
              ERR_INVALID_DATA -- The data was in an unrecognized format.
              ERR_CONVERGENCE -- The solution to Kepler's equation didn't
              converge to the required precision.
    Return 2: The comet's details and heliocentric coordinates. The
    reference frame is the equinox & ecliptic of J2000.
    """

    body = MpcBody()

    retval = _libkepler.comet_info(mpcorb_record, byref(jd_TT),
                                   byref(body))

    return retval, body

_libkepler.minor_planet_info.argtypes = [
    c_char_p,
    POINTER(JulianDate),
    POINTER(MpcBody)
]

_libkepler.comet_info.argtypes = [
    c_char_p,
    POINTER(JulianDate),
    POINTER(MpcBody)
]

__all__ = [
    "MpcBody",
    "minor_planet_info",
    "comet_info"
]
