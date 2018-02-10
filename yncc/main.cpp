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

#include "yncc.h"

string getCmdLine() {
    string cmdLine;

    cout << "> ";
    getline(cin, cmdLine);

    return cmdLine;
}

int main(int argc, char **argv) {
    cout << "YNC Console " << YNCC_MAJOR_VERSION << "."
         << YNCC_MINOR_VERSION << "."
         << YNCC_PATCH_VERSION
         << YNCC_PRE_RELEASE   << " ("

         <<     "API "     << YNCAPI_MAJOR_VERSION << "."
         << YNCAPI_MINOR_VERSION << "."
         << YNCAPI_PATCH_VERSION
         << YNCAPI_PRE_RELEASE   << ")" << endl;

    yncapi::Device::initialize();

    while(yncc::parseCmdLine(getCmdLine()));

    cout << "Exiting..." << endl;

    return 0;
}
