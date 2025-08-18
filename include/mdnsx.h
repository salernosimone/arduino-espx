#pragma once
#include <ESPmDNS.h>

/**
 * MDNS utilities
 */
class Mdnsx {
public:

    /**
     * Set hostname
     * @param hostname
     */
    void operator()(const char *hostname) const {
        MDNS.begin(hostname);
    }
};

static Mdnsx mdnsx;