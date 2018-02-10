/* 
 * Copyright (C) 2018, Alexandre Courtois
 * 
 * This file is part of YNCAPI.
 *
 * YNCAPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YNCAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with YNCAPI.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
 
#include "tools.h"

const vector<string> yncapi::slice(const string& s, char c) {
    string buff{""};
    vector<string> v;

    for(auto n:s) {
        if(n != c) buff+=n;
        else
            if(n == c && buff != "") {
                v.push_back(buff);
                buff = "";
            }
    }

    if(buff != "") v.push_back(buff);

    return v;
}

void yncapi::replace(string& src, const string& toFind, const string& replaceBy) {
    string tmp;

    if(src.size() && toFind.size() && toFind.size() <= src.size()) {
        size_t pos = src.find(toFind);

        while(pos != string::npos) {
            src.replace(pos, toFind.length(), replaceBy);
            pos = src.find(toFind, pos + 1);
        }
    }
}
