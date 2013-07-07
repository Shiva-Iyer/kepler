# makefile - makefile for the examples in the kepler library
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

CFLAGS = -I ../src -I . -D_GNU_SOURCE -c -pedantic -Wall 
LDFLAGS = -L../src/ -Wl,--no-as-needed -lm -lkepler

all: kepler_test rise_set

kepler_test.o: kepler_test.c
	$(CC) $(CFLAGS) -o $@ $<

rise_set.o: rise_set.c rise_set.h
	$(CC) $(CFLAGS) -o $@ $<

kepler_test: kepler_test.o
	$(CC) $(LDFLAGS) -o $@ $<

rise_set: rise_set.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	@$(RM) kepler_test.o kepler_test kepler_test.exe rise_set.o rise_set rise_set.exe

