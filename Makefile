# makefile - base makefile for the kepler library
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

export CC = gcc

export LIB_SONAME = libkepler.so.1
export LIB = libkepler.so
export LIB_DEST_PATH = /usr/lib
export LIB_DEST = libkepler.so.1.0.0

CP = cp
LN = ln -sf

SUBDIRS = src examples

all: $(SUBDIRS)

.PHONY: $(SUBDIRS) install clean

$(SUBDIRS):
	@$(MAKE) -C $@

examples: src

install: $(SUBDIRS)
	$(CP) src/$(LIB) $(LIB_DEST_PATH)/$(LIB_DEST)
	$(LN) $(LIB_DEST_PATH)/$(LIB_DEST) $(LIB_DEST_PATH)/$(LIB_SONAME)

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
