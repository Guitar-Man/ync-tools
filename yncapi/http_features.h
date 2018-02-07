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
