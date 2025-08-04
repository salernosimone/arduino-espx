#pragma once
#include <WiFi.h>
#include "./Str.h"
#include "./OpStatus.h"

// keep track of WiFi connection failure reason
String wifixFailureReason("");

/**
 * Configure WiFi connection
 */
typedef struct {
    size_t timeout;
    uint8_t retries;
} WifixConfig;

// a configurator is a function that accepts a WifixConfig* pointer
using WifixConfigurator = std::function<void(WifixConfig*)>;


/**
 * Connect to WiFi with a fluent interface
 */
class Wifix : public HasOpStatus {
public:
    String ip;

    /**
     * Constructor
     */
    Wifix() : ip("") {

    }

    /**
     * Convert to bool
     * @return
     */
    explicit virtual operator bool() const {
        return isConnected();
    }

    /**
     * Try to connect
     * @param ssid
     * @param pass
     * @return
     */
    Wifix &operator()(const String &ssid, const String &pass) {
        defaultConfig();

        return connect(ssid.c_str(), pass.c_str());
    }

    /**
     * Try to connect
     * @param ssid
     * @param pass
     * @return
     */
    template<typename T, typename... KWArgs>
    Wifix &operator()(const String &ssid, const String &pass, T arg, KWArgs... kwargs) {
        handleConfig(arg, kwargs...);

        return connect(ssid.c_str(), pass.c_str());
    }

    /**
     * Configure timeout
     * @param timeout
     * @return
     */
    WifixConfigurator Timeout(const String& timeout) {
        return [timeout](WifixConfig *config) {
            config->timeout = str(timeout).millis();
        };
    }

    /**
     * Configure retries
     * @param retries
     * @return
     */
    WifixConfigurator Retry(uint8_t retries) {
        return [retries](WifixConfig *config) {
            config->retries = retries;
        };
    }

protected:
    WifixConfig config;

    /**
     *
     * @return
     */
    bool isConnected() const {
        return WiFi.status() == WL_CONNECTED;
    }

    /**
     *
     */
    void defaultConfig() {
        config.timeout = 8000;
        config.retries = 0;
    }

    /**
     * Stop condition
     */
    void handleConfig() {

    }

    /**
     * Handle one kwarg at a time
     * @tparam T
     * @tparam KWArgs
     * @param arg
     * @param kwargs
     */
    template<typename T, typename... KWArgs>
    void handleConfig(T arg, KWArgs... kwargs) {
        arg(&config);

        handleConfig(kwargs...);
    }



    /**
     * Try to connect to WiFi
     * @return
     */
    Wifix &connect(const char *ssid, const char *pass) {
        ip = "";
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        listenForWiFiEvents();

        for (uint8_t i = 0; i <= config.retries && !isConnected(); i++) {
            WiFi.begin(ssid, pass);
            const size_t timeout = config.timeout + millis() + 1;

            while (millis() < timeout && !isConnected())
                delay(100);
        }

        // if connection succeeded, get IP address as string
        if (isConnected()) {
            IPAddress addr = WiFi.localIP();
            ip = str(addr[0]).join(".", addr[1], addr[2], addr[3]).value();

            status.succeed();
            return *this;
        }

        status.fail(wifixFailureReason);
        return *this;
    }

    /**
     *
     */
    static void listenForWiFiEvents() {
        static bool registered = false;

        if (!registered) {
            registered = true;

            // get detailed error status
            WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
                switch (info.wifi_sta_disconnected.reason) {
                    case WIFI_REASON_NO_AP_FOUND:
                        wifixFailureReason = "AP not found";
                        break;
                    case WIFI_REASON_AUTH_FAIL:
                        wifixFailureReason = "Wrong WiFi password";
                        break;
                    case WIFI_REASON_HANDSHAKE_TIMEOUT:
                    case WIFI_REASON_BEACON_TIMEOUT:
                        wifixFailureReason = "Handshake/Beacon timeout";
                        break;
                    default:
                        wifixFailureReason = "Unknown WiFi connection error";
                }
            }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
        }
    }
};

static Wifix wifix;