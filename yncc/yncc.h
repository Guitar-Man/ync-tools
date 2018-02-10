/* 
 * Copyright (C) 2018, Alexandre Courtois
 * 
 * This file is part of YNCC.
 *
 * YNCC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YNCC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with YNCC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef YNCC_H
#define YNCC_H

#include "../yncapi/yncapi.h"
#include <string>

#define _YNCC_MAJOR_VERSION  0
#define _YNCC_MINOR_VERSION  24
#define _YNCC_PATCH_VERSION  1
#define _YNCC_PRE_RELEASE    ""

static const unsigned int YNCC_MAJOR_VERSION = _YNCC_MAJOR_VERSION;
static const unsigned int YNCC_MINOR_VERSION = _YNCC_MINOR_VERSION;
static const unsigned int YNCC_PATCH_VERSION = _YNCC_PATCH_VERSION;
static const char*        YNCC_PRE_RELEASE   = _YNCC_PRE_RELEASE;

namespace yncc {
    void handle(yncapi::TReturnCode code);
    bool parseCmdLine(const std::string &cmdline);
}

#endif
