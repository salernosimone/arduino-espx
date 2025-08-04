#pragma once
#include <functional>
#include "./Str.h"


/**
 * Configurations for a thread
 */
typedef struct {
    String identifier;
    size_t stack;
    uint8_t priority;
    uint8_t core;
    void *userdata;
    TaskHandle_t *handler;
} ThreadxConfig;

// a configurator is a function that accepts a ThreadxConfig* pointer
using ThreadxConfigurator = std::function<void(ThreadxConfig*)>;

// a Task is a function that accepts void* (userdata) and returns void
using ThreadxTask = std::function<void(void *)>;

/**
 * Wrap a C-style function that gets passed to xTaskCreatePinnedToCore
 */
class ThreadxTaskWrapper {
public:
    String identifier;
    ThreadxTask task;
    void *userdata;

    /**
     * Constructor
     * @param id
     * @param t
     * @param p
     * @return
     */
    ThreadxTaskWrapper(const String &id, ThreadxTask t, void *p) :
        identifier(id),
        task(t),
        userdata(p) {}
};

/**
 * Allow the use of a std::function as a C-style function for xTaskCreatePinnedToCore
 */
extern "C" void callTask(void *arg) {
    ThreadxTaskWrapper *wrapper = static_cast<ThreadxTaskWrapper *>(arg);

    wrapper->task(wrapper->userdata);
    vTaskDelete(NULL);

    delete wrapper;
};

/**
 * Multithreading with a fluent interface
 */
class Threadx {
public:

    /**
     * Create thread with default configs
     * @param task
     */
    int operator()(ThreadxTask task) {
        defaultConfig();

        return handle(task);
    }

    /**
     * Create thread with custom configs
     * @param task
     * @return
     */
    template<typename... KWArgs>
    int operator()(ThreadxTask task, KWArgs... kwargs) {
        defaultConfig();

        return handle(task, kwargs...);
    }

    /**
     *
     * @param name
     */
    ThreadxConfigurator Name(const String& name) {
        return [name](ThreadxConfig* config) {
            config->identifier = name;
        };
    }

    /**
     *
     * @param size
     */
    ThreadxConfigurator Stack(const String& size) {
        return [size](ThreadxConfig* config) {
            config->stack = str(size).weight();
        };
    }

    /**
     *
     * @param core
     */
    ThreadxConfigurator Core(const uint8_t core) {
        return [core](ThreadxConfig* config) {
            config->core = core;
        };
    }

    /**
     *
     */
    ThreadxConfigurator NotImportant() {
        return [](ThreadxConfig* config) {
            config->priority = 0;
        };
    }

    /**
     *
     */
    ThreadxConfigurator Important() {
        return [](ThreadxConfig* config) {
            config->priority = configMAX_PRIORITIES / 2;
        };
    }

    /**
     *
     */
    ThreadxConfigurator Critical() {
        return [](ThreadxConfig* config) {
            config->priority = configMAX_PRIORITIES;
        };
    }

    /**
     *
     * @param p
     */
    ThreadxConfigurator Userdata(void *p) {
        return [p](ThreadxConfig* config) {
            config->userdata = p;
        };
    }

    /**
     *
     * @tparam T
     * @param p
     * @return
     */
    template<typename T>
    ThreadxConfigurator Userdata(T *p) {
        return [p](ThreadxConfig* config) {
            config->userdata = (void*) p;
        };
    }

    /**
     *
     * @param handler
     */
    ThreadxConfigurator Handler(TaskHandle_t& handler) {
        TaskHandle_t *h = &handler;

        return [h](ThreadxConfig* config) {
            config->handler = h;
        };
    }

protected:
    ThreadxConfig config;

    /**
     * Restore configs to default values
     */
    void defaultConfig() {
        config.identifier = Str::random();
        config.stack = 5000;
        config.core = 0;
        config.priority = 0;
        config.userdata = NULL;
        config.handler = NULL;
    }

    /**
     * No more kwargs to handle, create thread
     * @param task
     */
    int handle(ThreadxTask task) {
        // actually create thread
        ThreadxTaskWrapper *wrapper = new ThreadxTaskWrapper(
            config.identifier,
            task,
            config.userdata
        );

        ESP_LOGI("Threadx", "New thread: %s on core #%d", config.identifier.c_str(), config.core);

        int statusCode = xTaskCreatePinnedToCore(
                callTask,
                config.identifier.c_str(),
                config.stack,
                wrapper,
                config.priority,
                config.handler,
                config.core
        );

        return 0;

//        if (statusCode != pdPASS)
//            return status.failWithCode("Can't pin task", statusCode);
//
//        return status.succeed();
    }

    /**
     * Handle one kwarg at a time
     * @tparam T
     * @tparam KWArgs
     * @param task
     * @param arg
     * @param kwargs
     */
    template<typename T, typename... KWArgs>
    int handle(ThreadxTask task, T arg, KWArgs... kwargs) {
        arg(&config);

        return handle(task, kwargs...);
    }
};

static Threadx threadx;