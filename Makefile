# makefile - base makefile for the kepler library
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

ifndef target
 $(error Please specify target OS on the command line. Syntax: make target={posix|windows})
endif

ifeq ($(target),posix)

 export LIB_DEST_PATH = /usr/lib
 export LIB = libkepler.so
 export LIB_SONAME = $(LIB).1
 export LIB_DEST = $(LIB_SONAME).0.0

 CP = cp
 LN = ln -sf

else

 ifeq ($(target),windows)
  export LIB = libkepler.dll
  export LIB_SONAME = $(LIB).1
 else
  $(error Invalid target OS specified. Must be posix or windows)
 endif

endif

export CC = gcc

SUBDIRS = src examples

all: $(SUBDIRS)

.PHONY: $(SUBDIRS) install clean

$(SUBDIRS):
	@$(MAKE) -C $@

examples: src

install: $(SUBDIRS)
ifeq ($(target),posix)
	$(CP) src/$(LIB) $(LIB_DEST_PATH)/$(LIB_DEST)
	$(LN) $(LIB_DEST_PATH)/$(LIB_DEST) $(LIB_DEST_PATH)/$(LIB_SONAME)
else
	$(warning Please manually copy $(LIB) to your application or Windows system directory)
endif

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
