# __init__.py - Package definition file for pykepler
# Copyright (C) 2010-2012 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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

"""This package provides a Python interface for the routines in the kepler
library. Refer to the kepler documentation for the software prerequisites.

Modules:

julian_date -- Julian date conversions
delta_t -- Approximate value of Delta-T (TT - UT1)
coordinates -- Coordinate transformations and rotations
fund_args -- Values of fundamental lunisolar/planetary/precession arguments
vsop87 -- VSOP87 theory for sun/planet coordinates
elp82b -- ELP2000-82B theory for lunar coordinates
pluto -- PLUTO95 theory for Pluto's coordinates
sidereal_time -- Sidereal time calculations using IERS 2003 conventions 
iau2006_precession -- Precession calculations using the IAU 2006 model
iau2000a_nutation -- Nutation calculations using the IAU 2000A model
orbital_elements -- Reduction of orbital elements to ephemeris
mpc_file -- MPC orbit database parser
aberration -- Ron-Vondrak theory of aberration
earth_figure -- Values related to the oblateness of the Earth
parallax -- Values for the geocentric parallax of celestial bodies
magnitude -- Apparent magnitude of minor planets and comets
constants -- Constants and utility routines

"""

if __name__ == "__main__":
    exit()

import sys
from ctypes import *

import os

# Load the kepler shared library and save for use in the pykepler modules.
# The library is operating system dependent.
if os.name == "posix": 
    _libkepler = CDLL("libkepler.so.1")
elif os.name == "nt":
    _libkepler = CDLL("libkepler.dll")

from julian_date import *
from delta_t import *
from coordinates import *
from fund_args import *
from vsop87 import *
from elp82b import *
from pluto import *
from sidereal_time import *
from iau2006_precession import *
from iau2000a_nutation import *
from orbital_elements import *
from mpc_file import *
from aberration import *
from earth_figure import *
from parallax import *
from magnitude import *
from constants import *

__all__ = []

# Iterate through the pykepler modules and export the public symbols so that
# functions can be called without specifying a module name.
_modules = filter(lambda mod: mod.startswith(__package__ + "."), sys.modules)
for m in _modules:
    try:
        mod = sys.modules[m]
        if mod:
            __all__ += mod.__all__
    except Exception:
        True
