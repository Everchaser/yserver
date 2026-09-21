#include "EndpointConfig.h"
#include <stdexcept>
EndpointConfig::EndpointConfig(std::string host,
int port) :
host_(host),
port_(port) {

    if (port_ < 1 || port_ > 65535)
    throw (std::invalid_argument("Invalid port"));

}

const std::string& EndpointConfig:: host() const {
    return host_; }

int EndpointConfig:: port() const {
    return port_; }
