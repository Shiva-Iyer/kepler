/*
 * moonphase.h - Declarations for computing the phases of the Moon
 * Copyright (C) 2016 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MOONPHASE_H_
#define _MOONPHASE_H_

#include <julian_date.h>

enum MoonPhases {
  FirstQuarter,
  FullMoon,
  LastQuarter,
  NewMoon
};

double moonlit(struct julian_date *dt);

double moonphase(struct julian_date *dt, enum MoonPhases phase);

#endif
