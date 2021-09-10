//
// Created by Daniele on 07/09/2021.
//

#ifndef ENGINE_LOG_H
#define ENGINE_LOG_H

#include <spdlog/spdlog.h>

namespace Argonaut {
    class Log {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };
}

// Core log macros
#define AG_CORE_TRACE(...)    ::Argonaut::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AG_CORE_INFO(...)     ::Argonaut::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AG_CORE_WARN(...)     ::Argonaut::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AG_CORE_ERROR(...)    ::Argonaut::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AG_CORE_CRITICAL(...) ::Argonaut::Log::GetCoreLogger()->critical(__VA_ARGS__)


#endif //ENGINE_LOG_H
