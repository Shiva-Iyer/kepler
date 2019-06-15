# __init__.py - Package definition file for pykepler
# Copyright (C) 2016-2019 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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

"""This package provides a Python interface for the routines in the
kepler library. Refer to the kepler documentation for the software
prerequisites.

Modules:

aberration -- Ron-Vondrak theory of aberration
constants -- Constants and utility routines
coordinates -- Coordinate transformations and rotations
delta_t -- Approximate value of Delta-T (TT - UT1)
earth_figure -- Values related to the oblateness of the Earth
eclipse -- Calculate the circumstances of solar and lunar eclipses
elp82b -- ELP2000-82B theory for lunar coordinates
equisols -- Equinox and solstice calculations
fund_args -- Fundamental lunisolar/planetary/precession arguments
iau2000a_nutation -- Nutation calculations using the IAU 2000A model
iau2006_precession -- Precession calculations using the IAU 2006 model
julian_date -- Julian date conversions
magnitude -- Apparent magnitude of minor planets and comets
moonphase -- Dates and times of the phases of the Moon
mpc_file -- MPC orbit database parser
orbital_elements -- Reduction of orbital elements to ephemeris
parallax -- Values for the geocentric parallax of celestial bodies
pluto -- PLUTO95 theory for Pluto's coordinates
riseset -- Rise/transit/set times
sidereal_time -- Sidereal time calculations using IERS 2003 conventions 
vsop87 -- VSOP87 theory for sun/planet coordinates
"""

if __name__ == "__main__":
    exit()

import sys
import os
from ctypes import *

# Load the kepler shared library and save for use in the pykepler
# modules. The library is operating system dependent.
basedir = os.path.dirname(os.path.abspath(__file__))
if os.name == "posix": 
    _libkepler = CDLL(os.path.join(basedir, "libkepler.so"))
elif os.name == "nt":
    _libkepler = CDLL(os.path.join(basedir, "libkepler.dll"))

from .aberration import *
from .constants import *
from .coordinates import *
from .delta_t import *
from .earth_figure import *
from .eclipse import *
from .elp82b import *
from .equisols import *
from .fund_args import *
from .iau2000a_nutation import *
from .iau2006_precession import *
from .julian_date import *
from .magnitude import *
from .moonphase import *
from .mpc_file import *
from .orbital_elements import *
from .parallax import *
from .pluto import *
from .riseset import *
from .sidereal_time import *
from .vsop87 import *

__all__ = []

# Iterate through the pykepler modules and export the public symbols so
# that functions can be called without specifying a module name.
_modules = filter(lambda mod: mod.startswith(__package__ + "."),
                  sys.modules)
for m in _modules:
    try:
        mod = sys.modules[m]
        if mod:
            __all__ += mod.__all__
    except Exception:
        pass
