#ifndef HTTP_FEATURES_H
#define HTTP_FEATURES_H

#include "yncapi.h"
#include <SFML/Network.hpp>

#define DEFAULT_URI                     "/YamahaRemoteControl/ctrl"
#define DEFAULT_HTTP_MAJOR_VERSION      1
#define DEFAULT_HTTP_MINOR_VERSION      1
#define DEFAULT_CONTENT_TYPE            "text/xml; charset=\"utf-8\""

#define ERROR_NO_TARGET                 "200"
#define ERROR_HTTP(code)                code

using namespace sf;

namespace yncapi {
    Http httpClient;

    string sendRequest(const string &body) {
        if(Device::isAssigned() && deviceProbe(Device::id)) {
            Http::Request request;
            Http::Response response;

            request.setMethod(Http::Request::Method::Post);
            request.setUri(DEFAULT_URI);
            request.setHttpVersion(DEFAULT_HTTP_MAJOR_VERSION, DEFAULT_HTTP_MINOR_VERSION);
            request.setField("content-type", DEFAULT_CONTENT_TYPE);
            request.setField("content-length", to_string(body.size()));
            request.setField("host", Device::id + ":80");
            request.setBody(body);

            response = httpClient.sendRequest(request);

            if(response.getStatus() == Http::Response::Ok)
                return response.getBody();
            else {
                return ERROR_HTTP(to_string(response.getStatus()));
            }
        }

        return ERROR_NO_TARGET;
    }
}

#endif
