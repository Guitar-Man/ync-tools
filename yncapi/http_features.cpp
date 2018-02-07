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
