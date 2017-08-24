# coordinates.py - Wrapper for coordinate transformations
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
from pykepler import _libkepler

class RectangularCoordinates(Structure):
    """Used for rectangular coordinates. The origin and orientation of
    the xy axes are application specific.

    Fields:

    x -- x-coordinate. The units are application specific.
    y -- y-coordinate. The units are application specific.
    z -- z-coordinate. The units are application specific.
    """

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
    """Used for coordinates in the Earth's equatorial frame.

    Fields:

    right_ascension -- Right ascension in radians
    declination -- Declination in radians
    """

    _fields_ = [
        ("right_ascension", c_double),
        ("declination", c_double)
    ]

    def __init__(self, right_ascension = 0.0, declination = 0.0):

        Structure.__init__(self)
        self.right_ascension = right_ascension
        self.declination = declination

    def __repr__(self):

        return dict(RA = self.right_ascension,
                    dec = self.declination).__repr__()

class EclipticCoordinates(Structure):
    """Used for ecliptic coordinates. The origin is application specific.

    Fields:

    longitude -- Longitude in radians.
    latitude -- Latitude in radians.
    """

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
    """Used for horizontal coordinates.

    Fields:

    azimuth -- Azimuth measured in radians and clockwise from the south.
    altitude -- Altitude in radians.
    """

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
    """
    Convert the heliocentric rectangular coordinates of a celestial
    body to geocentric spherical coordinates. The resulting coordinates
    will be in the same reference frame (xy plane) as the original
    coordinates.

    body -- The body's heliocentric rectangular coordinates. Must be in
            the same reference frame and units as earth.
    earth -- The Earth's heliocentric rectangular coordinates. Must be in
             the same reference frame and units as body.

    Return 1: The body's geocentric longitude in radians.
    Return 2: The body's geocentric latitude in radians.
    Return 3: The body's geocentric distance in the units of body & earth.
    """

    longitude = c_double()
    latitude = c_double()
    radius = c_double()

    _libkepler.rectangular_to_spherical(byref(body), byref(earth),
                                        byref(longitude),
                                        byref(latitude),
                                        byref(radius))

    return longitude.value, latitude.value, radius.value

def spherical_to_rectangular(longitude, latitude, radius):
    """
    Convert the spherical coordinates of a celestial body to rectangular
    coordinates with the same origin and reference frame (xy plane).

    longitude -- The body's longitude in radians.
    latitude -- The body's latitude in radians.
    radius -- The body's distance from the central body.

    Return 1: The body's rectangular coordinates in the same units
    as radius.
    """

    rectangular = RectangularCoordinates()

    _libkepler.spherical_to_rectangular(longitude, latitude, radius,
                                        byref(rectangular))

    return rectangular

def phase_angle(body, earth):
    """
    Calculate the Sun-body-Earth phase angle.

    body -- The body's heliocentric rectangular coordinates. Must be in
            the same reference frame and units as ear.
    earth -- The Earth's heliocentric rectangular coordinates. Must be in
             the same reference frame and units as obj.

    Return 1: The Sun-body-Earth phase angle in radians.
    """

    return _libkepler.phase_angle(byref(body), byref(earth))

def equatorial_to_ecliptic(equatorial, obliquity):
    """
    Convert the geocentric equatorial coordinates of a celestial body to
    geocentric ecliptic coordinates.

    equatorial -- The body's geocentric equatorial coordinates.
    obliquity -- The obliquity of the ecliptic in radians.

    Return 1: The body's geocentric ecliptic coordinates.
    """

    ecliptic = EclipticCoordinates()

    _libkepler.equatorial_to_ecliptic(byref(equatorial), obliquity,
                                      byref(ecliptic))

    return ecliptic

def ecliptic_to_equatorial(ecliptic, obliquity):
    """
    Convert the geocentric ecliptic coordinates of a celestial body to
    geocentric equatorial coordinates. 

    ecliptic -- The body's geocentric ecliptic coordinates.
    obliquity -- The obliquity of the ecliptic in radians.

    Return 1: The body's geocentric equatorial coordinates.
    """

    equatorial = EquatorialCoordinates()

    _libkepler.ecliptic_to_equatorial(byref(ecliptic), obliquity,
                                      byref(equatorial))

    return equatorial

def equatorial_to_horizontal(hour_angle, declination, latitude):
    """
    Convert the geocentric equatorial coordinates of a celestial body to
    horizontal coordinates.

    hour_angle -- The body's local hour angle in radians.
    declination -- The body's declination in radians.
    latitude -- The observer's geographic latitude in radians.

    Return 1: The body's horizontal coordinates.
    """

    horizontal = HorizontalCoordinates()

    _libkepler.equatorial_to_horizontal(hour_angle, declination,
                                        latitude, byref(horizontal))

    return horizontal

def horizontal_to_equatorial(horizontal, latitude):
    """
    Convert the horizontal coordinates of a celestial body to geocentric
    equatorial coordinates.

    horizontal -- The body's horizontal coordinates.
    latitude -- The observer's geographic latitude in radians.

    Return 1: The body's local hour angle in radians.
    Return 2: The body's declination in radians.
    """

    hour_angle = c_double()
    declination = c_double()

    _libkepler.horizontal_to_equatorial(byref(horizontal), latitude,
                                        byref(hour_angle),
                                        byref(declination))

    return hour_angle.value, declination.value

def rotate_rectangular(rotation_matrix, rectangular):
    """
    Apply a rotation matrix to a body's rectangular coordinates.

    rotation_matrix -- The rotation matrix in the form
                       [[m11, m12, m13],[m21, m22, m23],[m31, m32, m33]]
    rectangular -- The rectangular coordinates to be rotated in-place.
    """

    mat = (c_double * 9)()
    for i in range(3):
        for j in range(3):
            mat[i * 3 + j] = rotation_matrix[i][j]

    _libkepler.rotate_rectangular(pointer(mat), byref(rectangular))

def rotate_equatorial(rotation_matrix, equatorial):
    """
    Apply a rotation matrix to a body's geocentric equatorial
    coordinates. This function can be used to apply the IAU2006/2000A
    precession/nutation matrices.

    rotation_matrix -- The rotation matrix in the form
                       [[m11, m12, m13],[m21, m22, m23],[m31, m32, m33]]
    equatorial -- The equatorial coordinates to be rotated in-place.
    """

    mat = (c_double * 9)()
    for i in range(3):
        for j in range(3):
            mat[i * 3 + j] = rotation_matrix[i][j]

    _libkepler.rotate_equatorial(pointer(mat), byref(equatorial))

def rotate_ecliptic_to_equator(obliquity, ecliptic):
    """
    Rotate rectangular coordinates from the ecliptic to the
    equatorial frame.

    obliquity -- The obliquity of the ecliptic in radians.
    ecliptic -- The coordinates to be rotated in-place.
    """

    _libkepler.rotate_ecliptic_to_equator(obliquity, byref(ecliptic))

def lightcor(id, dt, obj, ear = None):
    """
    Correct the coordinates of solar system bodies to account for the 
    finite speed of light.

    id  -- Object identifier for <obj>. MERCURY through NEPTUNE for
           the planets or NEPTUNE+1 for Pluto.
    dt  -- Dynamical time of coordinates.
    obj -- Coordinates of the body in AU. Corrected on exit.
    ear -- Coordinates of the Earth in AU. Corrected on exit if non-NULL.
    """

    if (ear is None):
        _libkepler.lightcor(id, byref(dt), byref(obj), None)
    else:
        _libkepler.lightcor(id, byref(dt), byref(obj), byref(ear))
    
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

_libkepler.lightcor.restype = None
_libkepler.lightcor.argtypes = [
    c_int,
    POINTER(JulianDate),
    POINTER(RectangularCoordinates),
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
    "rotate_ecliptic_to_equator",
    "lightcor"
]
