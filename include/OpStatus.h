#pragma once

/**
 * Operation result status
 * (either success or error)
 */
class OpStatus {
public:
    String reason;

    /**
     * Constructor
     */
    OpStatus() : reason("") {

    }

    /**
     * Convert to boolean
     * @return
     */
    operator bool() const {
        return succeeded();
    }

    /**
     * Set fail reason
     * @param reason
     * @return
     */
    OpStatus &fail(const String& reason) {
        this->reason = reason;

        if (failed())
            ESP_LOGE("espx", "%s", reason.c_str());

        return *this;
    }

    /**
     * Set fail reason with error code
     * @param message
     * @param errorCode
     * @return
     */
    OpStatus &failWithCode(const String &message, const int errorCode) {
        return fail(message + " (error code " + errorCode + ")");
    }

    /**
     * Mark status as success
     * @return
     */
    inline OpStatus &succeed() {
        return fail("");
    }

    /**
     *
     * @return
     */
    inline bool failed() const {
        return reason != "";
    }

    /**
     *
     * @return
     */
    inline bool succeeded() const {
        return !failed();
    }
};


/**
 * Parent class for those who work with OpStatus
 */
class HasOpStatus {
public:
    OpStatus status;

    /**
     * Convert to bool
     * @return
     */
    explicit virtual operator bool() const {
        return !failed();
    }

    /**
     *
     * @return
     */
    inline bool failed() const {
        return status.failed();
    }

    /**
     *
     * @return
     */
    inline String& failure() {
        return status.reason;
    }

    /**
     * Enter an endless loop on error
     */
    void raise() {
        while (failed()) {
            Serial.println(status.reason);
            delay(1000);
        }
    }
};