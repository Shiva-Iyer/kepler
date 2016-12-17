# planpos.py - Display geocentric coordinates of the Sun, Moon, planets.
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

try:
    import sys
    from os import path
    sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))

    import time
    import traceback
    from pykepler import julian_date as J
    from pykepler import delta_t
    from pykepler.constants import SolarSystemPlanets as P
    from pykepler.constants import Conversion
    from pykepler.constants import DegMinSec
    from pykepler import vsop87 as V
    from pykepler import elp82b as E
    from pykepler import pluto
    from pykepler import coordinates as R
    from math import fabs

    # Handle command-line parameters
    if (len(sys.argv) > 1 and sys.argv[1].lower() in ["--h", "--help"]):
        print(sys.argv[0] + " [datetime]")
        print("where datetime is a UTC date/time in the form "
              "yyyy-mm-dd HH:MM:SS")
        print("Defaults to current date/time if not specified.")
        print("Displays true RA, dec., distance of Sun, Moon, planets.")
        sys.exit(0)

    if (len(sys.argv) == 2):
        tm = time.strptime(sys.argv[1], "%Y-%m-%d")
    elif (len(sys.argv) == 3):
        tm = time.strptime(sys.argv[1] + " " + sys.argv[2],
                           "%Y-%m-%d %H:%M:%S")
    else:
        tm = time.gmtime()

    # Convert date to JDE form and add Delta-T to obtain Dynamical Time
    jd = J.calendar_to_julian_date(tm[0], tm[1], tm[2])[1]
    dt,dtc = delta_t(tm[0], tm[1])[1:3]
    jd.date2 += tm[3]/24.0 + tm[4]/1440.0 + (tm[5]+dt+dtc)/86400.0

    pn = (["Sun", "Moon", "Mercury", "Venus", "Mars",
           "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto"])
    ra = [0.0]*len(pn)
    dec = [0.0]*len(pn)
    dist = [0.0]*len(pn)

    # Compute the position of the Moon
    zer = R.RectangularCoordinates(0, 0, 0)
    moo = E.elp82b_coordinates(jd)
    E.elp82b_ecliptic_to_equator(moo)
    ra[1],dec[1],dist[1] = R.rectangular_to_spherical(moo, zer)

    # Compute the positions of the Sun, planets and Pluto
    j = 2
    for i in ([P.EARTH, P.MERCURY, P.VENUS] +
              list(range(P.MARS, P.NEPTUNE+2))):
        if (i < P.NEPTUNE + 1):
            pla = V.vsop87_coordinates(i, jd)[1]
            V.vsop87_ecliptic_to_equator(pla)
        else:
            pla = pluto.pluto_coordinates(jd)[1]

        if (i == P.EARTH):
            ear = pla
            ra[0],dec[0],dist[0] = R.rectangular_to_spherical(zer, ear)
        else:
            ra[j],dec[j],dist[j] = R.rectangular_to_spherical(pla, ear)
            j += 1

    # Display the results
    print("At " + time.strftime("%Y-%m-%d %H:%M:%S", tm) + " UTC" +  (
        " (%16.8f DT):" % (jd.date1+jd.date2)))
    print("Planet: RA (HH:MM:SS), Dec. (dd:mm:ss), Distance\n")
    for i,r,d,s in zip(range(10), ra, dec, dist):
        rc = DegMinSec(r*Conversion.RAD_TO_HRS)
        dc = DegMinSec(d*Conversion.RAD_TO_DEG)
        print("%7s: %02d:%02d:%05.2f, %3d:%02d:%04.1f, %12.5f %2s" % (
            pn[i], rc.degrees, rc.minutes, rc.seconds,
            dc.degrees, fabs(dc.minutes), fabs(dc.seconds), s,
            "KM" if i == 1 else "AU"))
except SystemExit:
    pass
except:
    traceback.print_exc()
