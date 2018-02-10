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

#include "http_features.h"
#include <SFML/Network.hpp>

sf::Http yncapi::httpClient;

string yncapi::sendRequest(const string &body) {    
    if(Device::isAssigned() && deviceProbe(Device::id)) {
        sf::Http::Request request;
        sf::Http::Response response;

        request.setMethod(sf::Http::Request::Method::Post);
        request.setUri(Device::id.CtrlURL);
        request.setHttpVersion(DEFAULT_HTTP_MAJOR_VERSION, DEFAULT_HTTP_MINOR_VERSION);
        request.setField("content-type", DEFAULT_CONTENT_TYPE);
        request.setField("content-length", to_string(body.size()));
        request.setField("host", Device::id.DeviceIP + ":80");
        request.setBody(body);

        response = httpClient.sendRequest(request);

        if(response.getStatus() == sf::Http::Response::Ok)
            return response.getBody();
        else {
            return ERROR_HTTP(to_string(response.getStatus()));
        }
    }

    return ERROR_NO_TARGET;
}
