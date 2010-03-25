# makefile - makefile for the kepler library
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

CC = gcc
CFLAGS = -D_GNU_SOURCE -c -ansi -pedantic -Wall -I ./
LDFLAGS = -lm
OBJS1 = julian_date.o delta-t.o vsop87_data.o vsop87.o fund_args.o elp82b_data.o \
	elp82b.o iau2006_precession.o iau2000a_data.o iau2000a_nutation.o \
	coordinates.o sidereal_time.o pluto.o orbital_elements.o mpc_file.o \
	aberration.o earth_figure.o parallax.o magnitude.o kepler_test.o
OBJS2 = julian_date.o vsop87_data.o vsop87.o fund_args.o elp82b_data.o \
	elp82b.o iau2000a_data.o iau2000a_nutation.o coordinates.o sidereal_time.o \
	pluto.o delta-t.o rise_set.o

all: kepler_test rise_set
 
julian_date.o: julian_date.c julian_date.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

delta-t.o: delta-t.c delta-t.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

vsop87_data.o: vsop87_data.c vsop87.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

vsop87.o: vsop87.c vsop87.h julian_date.h coordinates.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

fund_args.o: fund_args.c fund_args.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

elp82b_data.o: elp82b_data.c elp82b.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

elp82b.o: elp82b.c elp82b.h fund_args.h julian_date.h coordinates.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

iau2006_precession.o: iau2006_precession.c iau2006_precession.h \
	julian_date.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

iau2000a_data.o: iau2000a_data.c iau2000a_nutation.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

iau2000a_nutation.o: iau2000a_nutation.c iau2000a_nutation.h fund_args.h \
	julian_date.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

coordinates.o: coordinates.c coordinates.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

sidereal_time.o: sidereal_time.c sidereal_time.h iau2000a_nutation.h \
	fund_args.h julian_date.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

pluto.o: pluto.c pluto.h julian_date.h coordinates.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

orbital_elements.o: orbital_elements.c orbital_elements.h julian_date.h \
	coordinates.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

mpc_file.o: mpc_file.c mpc_file.h julian_date.h coordinates.h \
	orbital_elements.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

aberration.o: aberration.c aberration.h fund_args.h julian_date.h coordinates.h
	$(CC) $(CFLAGS) -o $@ $<

earth_figure.o: earth_figure.c earth_figure.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

parallax.o: parallax.c parallax.h earth_figure.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

magnitude.o: magnitude.c magnitude.h coordinates.h
	$(CC) $(CFLAGS) -o $@ $<

kepler_test.o: kepler_test.c julian_date.h delta-t.h vsop87.h elp82b.h \
	iau2006_precession.h iau2000a_nutation.h coordinates.h sidereal_time.h \
	pluto.h mpc_file.h aberration.h parallax.h kepler.h
	$(CC) $(CFLAGS) -o $@ $<

rise_set.o: rise_set.c rise_set.h julian_date.h vsop87.h elp82b.h \
	iau2000a_nutation.h coordinates.h sidereal_time.h pluto.h kepler.h \
	delta-t.h
	$(CC) $(CFLAGS) -o $@ $<

kepler_test: $(OBJS1)
	$(CC) -o $@ $(LDFLAGS) $(OBJS1)

rise_set: $(OBJS2)
	$(CC) -o $@ $(LDFLAGS) $(OBJS2)

.PHONY: clean
clean:
	rm -f $(OBJS1) $(OBJS2) kepler_test rise_set
