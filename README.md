Introduction
------------

kepler contains a set of routines for performing astronomical
calculations. The library is implemented in ANSI C and the optional
pykepler package provides support for Python.

The intent is to supply the basic building blocks necessary for more
advanced applications. Consequently, no attempt has been made to
develop an user interface or to write programs for various use cases.

Prerequisites
-------------

Make sure your Unix installation has the GNU C compiler and build
tools installed. The following are the minimum version requirements.

1. GNU gcc 4.4.1
2. GNU Make 3.81
3. Python 2.6.5 (needed only if you use the Python interface)

kepler can also be used with Microsoft Windows provided you have MinGW
and MSYS installed. These tools may be downloaded from <http://www.mingw.org/>.

Installation
------------

Instructions for downloading the source archives and accessing the
development repository may be found at <https://github.com/Shiva-Iyer/kepler>.

Run 'make target={posix|windows}' in the kepler installation directory
to compile all the library files and example programs. In order to
call kepler's functions from your own applications, you will need to
create your own makefile along the lines of the one in the 'examples'
subdirectory.

Run 'make target={posix|windows} install' to deploy the shared library
that is created in the previous step. On most systems, you will need
to run this as root.  You will need to do this before you run the
example programs or use the pykepler Python package. On Windows,
please copy src/libkepler.dll manually to your application or Windows
system directory.

Run 'make target={posix|windows} clean' to remove all the files
created by running 'make'. This will not remove the shared libraries
or symbolic links in /usr/lib.

The 'pykepler' subdirectory contains the sources for the pykepler
Python package.  This package allows you to call kepler routines from
Python.

Documentation
-------------

The C library is documented in the file kepler.html under the 'doc'
subdirectory.

The 'pydoc' utility or the help() function in the interactive Python
interpreter may be used to view the documentation for the Python
interface.

Examples
--------

The following sample programs may be found in the 'examples'
subdirectory:

1. kepler_test.c calls most of the functions in kepler and displays
the results in a terminal window. To test the routines in mpc_file.c,
you must download the MPC Orbit Database files from <http://www.cfa.harvard.edu/iau/MPCORB.html>.

2. rise_set.c is a program that accepts command-line arguments and
displays the rise, transit and setting times for the Sun, Moon,
planets and Pluto.

3. planpos. py demonstrates the Python wrapper by calling routines in
the VSOP, ELP and Pluto modules to display the positions of the Sun,
Moon, planets and Pluto at the time specified on the command-line.

4. flightseg. py displays the great circle distance and bearing angles
for flight segments between the airports specified on the command
line.  The airport database from <http://openflights.org/data.html>
must be downloaded into the examples folder.

References
----------

kepler is targeted towards users with a sound grasp of positional
astronomy.  However, it has been thought fit to include references to
the relevant astronomical literature in the source code.

No work of this nature would be possible without the following
standard works on astronomical algorithms:

1. Celestial Mechanics - Forest Ray Moulton
2. Astronomical Algorithms - Jean Meeus
3. Explanatory supplement to the Astronomical Almanac - P. K. Seidelmann
