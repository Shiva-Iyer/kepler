# coordinates.py - Wrapper for coordinate transformations
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

class RectangularCoordinates(Structure):

    _fields_ = [
        ("x", c_double),
        ("y", c_double),
        ("z", c_double)
        ]

    def __init__(self, x = 0.0, y = 0.0, z = 0.0):

        Structure.__init__(self)
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self):

        return dict(x = self.x, y = self.y, z = self.z).__repr__()

class EquatorialCoordinates(Structure):

    _fields_ = [
        ("right_ascension", c_double),
        ("declination", c_double)
        ]

    def __init__(self, right_ascension = 0.0, declination = 0.0):

        Structure.__init__(self)
        self.right_ascension = right_ascension
        self.declination = declination

    def __repr__(self):

        return dict(RA = self.right_ascension, dec = self.declination).__repr__()

class EclipticCoordinates(Structure):

    _fields_ = [
        ("longitude", c_double),
        ("latitude", c_double)
        ]

    def __init__(self, longitude = 0.0, latitude = 0.0):

        Structure.__init__(self)
        self.longitude = longitude
        self.latitude = latitude

    def __repr__(self):

        return dict(lon = self.longitude, lat = self.latitude).__repr__()

class HorizontalCoordinates(Structure):

    _fields_ = [
        ("azimuth", c_double),
        ("altitude", c_double)
        ]

    def __init__(self, azimuth = 0.0, altitude = 0.0):

        Structure.__init__(self)
        self.azimuth = azimuth
        self.altitude = altitude

    def __repr__(self):

        return dict(azi = self.azimuth, alt = self.altitude).__repr__()

def rectangular_to_spherical(body, earth):

    longitude = c_double()
    latitude = c_double()
    radius = c_double()

    _libkepler.rectangular_to_spherical(byref(body), byref(earth),
                                        byref(longitude), byref(latitude),
                                        byref(radius))

    return longitude.value, latitude.value, radius.value

def spherical_to_rectangular(longitude, latitude, radius):

    rectangular = RectangularCoordinates()

    _libkepler.spherical_to_rectangular(longitude, latitude, radius,
                                        byref(rectangular))

    return rectangular

def phase_angle(body, earth):

    return _libkepler.phase_angle(byref(body), byref(earth))

def equatorial_to_ecliptic(equatorial, obliquity):

    ecliptic = EclipticCoordinates()

    _libkepler.equatorial_to_ecliptic(byref(equatorial), obliquity,
                                      byref(ecliptic))

    return ecliptic

def ecliptic_to_equatorial(ecliptic, obliquity):

    equatorial = EquatorialCoordinates()

    _libkepler.ecliptic_to_equatorial(byref(ecliptic), obliquity,
                                      byref(equatorial))

    return equatorial

def equatorial_to_horizontal(hour_angle, declination, latitude):

    horizontal = HorizontalCoordinates()

    _libkepler.equatorial_to_horizontal(hour_angle, declination, latitude,
                                        byref(horizontal))

    return horizontal

def horizontal_to_equatorial(horizontal, latitude):

    hour_angle = c_double()
    declination = c_double()

    _libkepler.horizontal_to_equatorial(byref(horizontal), latitude,
                                        byref(hour_angle), byref(declination))

    return hour_angle.value, declination.value

def rotate_rectangular(rotation_matrix, rectangular):

    mat = (c_double * 9)()
    for i in xrange(3):
        for j in xrange(3):
            mat[i * 3 + j] = rotation_matrix[i][j]

    _libkepler.rotate_rectangular(pointer(mat), byref(rectangular))

def rotate_equatorial(rotation_matrix, equatorial):

    mat = (c_double * 9)()
    for i in xrange(3):
        for j in xrange(3):
            mat[i * 3 + j] = rotation_matrix[i][j]

    _libkepler.rotate_equatorial(pointer(mat), byref(equatorial))

def rotate_ecliptic_to_equator(obliquity, ecliptic):

    _libkepler.rotate_ecliptic_to_equator(obliquity, byref(ecliptic))

_libkepler.rectangular_to_spherical.restype = None
_libkepler.rectangular_to_spherical.argtypes = [
    POINTER(RectangularCoordinates),
    POINTER(RectangularCoordinates),
    POINTER(c_double),
    POINTER(c_double),
    POINTER(c_double)
]

_libkepler.spherical_to_rectangular.restype = None
_libkepler.spherical_to_rectangular.argtypes = [
    c_double,
    c_double,
    c_double,
    POINTER(RectangularCoordinates)
]

_libkepler.phase_angle.restype = c_double
_libkepler.phase_angle.argtypes = [
    POINTER(RectangularCoordinates),
    POINTER(RectangularCoordinates)
]

_libkepler.equatorial_to_ecliptic.restype = None
_libkepler.equatorial_to_ecliptic.argtypes = [
    POINTER(EquatorialCoordinates),
    c_double,
    POINTER(EclipticCoordinates)
]

_libkepler.ecliptic_to_equatorial.restype = None
_libkepler.ecliptic_to_equatorial.argtypes = [
    POINTER(EclipticCoordinates),
    c_double,
    POINTER(EquatorialCoordinates)
]

_libkepler.equatorial_to_horizontal.restype = None
_libkepler.equatorial_to_horizontal.argtypes = [
    c_double,
    c_double,
    c_double,
    POINTER(HorizontalCoordinates)
]

_libkepler.horizontal_to_equatorial.restype = None
_libkepler.horizontal_to_equatorial.argtypes = [
    POINTER(HorizontalCoordinates),
    c_double,
    POINTER(c_double),
    POINTER(c_double)
]

_libkepler.rotate_rectangular.restype = None
_libkepler.rotate_rectangular.argtypes = [
    POINTER(c_double * 9),
    POINTER(RectangularCoordinates)
]

_libkepler.rotate_equatorial.restype = None
_libkepler.rotate_equatorial.argtypes = [
    POINTER(c_double * 9),
    POINTER(EquatorialCoordinates)
]

_libkepler.rotate_ecliptic_to_equator.restype = None
_libkepler.rotate_ecliptic_to_equator.argtypes = [
    c_double,
    POINTER(RectangularCoordinates)
]

__all__ = [
    "RectangularCoordinates",
    "EquatorialCoordinates",
    "EclipticCoordinates",
    "HorizontalCoordinates",
    "rectangular_to_spherical",
    "spherical_to_rectangular",
    "phase_angle",
    "equatorial_to_ecliptic",
    "ecliptic_to_equatorial",
    "equatorial_to_horizontal",
    "horizontal_to_equatorial",
    "rotate_rectangular",
    "rotate_equatorial",
    "rotate_ecliptic_to_equator"
]
