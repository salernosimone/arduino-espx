#pragma once


/**
 * URL handler
 */
class URL {
public:
    String protocol;
    String host;
    String uri;
    uint16_t port;

    /**
     * Constructor
     * @param url
     */
    URL(const String &url) {
        parse(url);
    }

    /**
     * Test if URL is valid
     * @return
     */
    operator bool() const {
        return port > 0;
    }

    /**
     *
     * @param url
     */
    void parse(const String &url) {
        int16_t idx = url.indexOf(':');

        // no protocol found
        if (idx < 0) {
            protocol = "";
            host = "";
            port = 0;
            uri = "";

            return;
        }

        protocol = url.substring(0, idx);
        String u = url.substring(idx + 3);

        // look for path (includes hash and querystring)
        idx = u.indexOf('/');

        if (idx < 0) {
            host = u;
            uri = "";
        } else {
            host = u.substring(0, idx);
            uri = u.substring(idx + 1);
        }

        findPort();
    }

protected:

    /**
     * Extract port from host
     */
    void findPort() {
        int16_t idx = host.indexOf(':');

        if (idx < 0)
            port = 80;
        else {
            port = host.substring(idx + 1).toInt();
            host = host.substring(0, idx);
        }
    }
};