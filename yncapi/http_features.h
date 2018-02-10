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

#ifndef HTTP_FEATURES_H
#define HTTP_FEATURES_H

#include "yncapi.h"
#include <SFML/Network.hpp>

#define DEFAULT_HTTP_MAJOR_VERSION      1
#define DEFAULT_HTTP_MINOR_VERSION      1
#define DEFAULT_CONTENT_TYPE            "text/xml; charset=\"utf-8\""

#define ERROR_NO_TARGET                 "200"
#define ERROR_HTTP(code)                code

namespace yncapi {
    extern sf::Http httpClient;
    
    /**
     * @brief Send request to httpClient
     * @param body Request body
     * @return Either a string containing the HTTP response body, or an error code such as code 400
     */
    string sendRequest(const string &body);
}

#endif
