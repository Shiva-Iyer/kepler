# flightseg.py - Display great circle flight segment details
# Copyright (C) 2017 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
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

import sys
from os import path

d = path.dirname(path.abspath(__file__))
sys.path.append(path.dirname(d))

from pykepler import constants as C
from pykepler import earth_figure as E

f = path.join(d, "airports.dat")
if (not path.isfile(f)):
    print("Please download airports.dat from "
          "http://openflights.org/data.html into examples folder")
    sys.exit(1)

with open(f, "r") as f:
    A = [l.replace("\"", "").split(",") for l in f]

I = [i for s in sys.argv[1:] for i,e in enumerate(A)
     if e[4].lower() == s.lower()]
if (len(I) < 2):
    print("Please specify 2 or more 3-letter IATA airport codes "
          "on the command-line")
    sys.exit(2)

for i in range(0, len(I)-1):
    dis,inb,fib = E.earth_gcdist(
        float(A[I[i]][7])*C.Conversion.DEG_TO_RAD,
        float(A[I[i]][6])*C.Conversion.DEG_TO_RAD,
        float(A[I[i+1]][7])*C.Conversion.DEG_TO_RAD,
        float(A[I[i+1]][6])*C.Conversion.DEG_TO_RAD)

    print("%3s (%-30s) %11s, %10s ->" % (
        A[I[i]][4], A[I[i]][1], A[I[i]][7], A[I[i]][6]))
    print("%3s (%-30s) %11s, %10s\n" % (
        A[I[i+1]][4], A[I[i+1]][1], A[I[i+1]][7], A[I[i+1]][6]))
    print("%14s, %9s, %s" % (
        "GC distance", "Initial", "Final bearing"))
    print(u"%11.5f km, %8.4f\xB0, %8.4f\xB0\n" % (
        dis/1000, inb*C.Conversion.RAD_TO_DEG,
        fib*C.Conversion.RAD_TO_DEG))
