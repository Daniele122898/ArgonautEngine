//
// Created by Daniele on 07/09/2021.
//

#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Argonaut {

    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

    void Log::Init() {
        auto colorSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        colorSink->set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = std::make_shared<spdlog::logger>("ARGONAUT", colorSink);

        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);
    }
}