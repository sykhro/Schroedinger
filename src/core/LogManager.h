/*
 * Schroedinger - Scienza (c) 2019
 * Licensed under the LGPL 2.1; see the included LICENSE for details
 */

#ifndef LOG_MGR_H_
#define LOG_MGR_H_

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <filesystem>

#ifndef NDEBUG
#    define S_TRACE(...) LogManager::getInstance().Trace(__VA_ARGS__);
#    define S_DEBUG(...) LogManager::getInstance().Debug(__VA_ARGS__);
#else
#    define S_TRACE(...)
#    define S_DEBUG(...)
#endif
#define S_CRITICAL(...) LogManager::getInstance().Critical(__VA_ARGS__);
#define S_ERROR(...) LogManager::getInstance().Error(__VA_ARGS__);
#define S_WARN(...) LogManager::getInstance().Warn(__VA_ARGS__);
#define S_INFO(...) LogManager::getInstance().Info(__VA_ARGS__);
#define S_WF(x) LogManager::getInstance().DumpWavefunction((x));

template <typename T, typename = std::enable_if<std::is_arithmetic<T>::value>>
std::string vectorToString(const std::vector<T> &values) {
    fmt::memory_buffer writer;

    for (auto val : values) {
        format_to(writer, "{} \n", val);
    }
    return to_string(writer);
}

enum Sink {
    FILE_SINK = 0,
    CONSOLE_SINK,
    UI_SINK, /* To be implemented */

    SINKS_NO /* MUST be last */
};

class LogManager {
  public:
    static LogManager &getInstance() {
        static LogManager mainlogger;
        return mainlogger;
    };

    LogManager(const LogManager &) = delete;
    LogManager(LogManager &&)      = delete;
    LogManager &operator=(const LogManager &) = delete;
    LogManager &operator=(LogManager &&) = delete;

    void Init(bool wfdump = false) {
        RegisterLoggers();
        this->wf_dump = wfdump;

        if(wfdump){
            std::uintmax_t n = std::filesystem::remove_all(this->wf_path);
            std::filesystem::create_directory(this->wf_path);
        }
    }

    void SetLogLevel(spdlog::level::level_enum log_level, Sink logger) {
        loggers.at(logger)->set_level(log_level);
    }

	void EnableWavefunctionDump(bool val) { wf_dump = val; }
    bool WavefunctionDumpEnabled() const { return wf_dump; }
    void SetWavefunctionDumpPath(std::string val) { wf_path = val; }

    spdlog::level::level_enum GetLogLevel(Sink logger) const { return loggers.at(logger)->level(); }

    template <typename... Args>
    void Trace(const char *fmt, const Args &... args) {
        for (auto logger : loggers) {
            if (logger) logger->trace(fmt, args...);
        }
    }

    template <typename... Args>
    void Debug(const char *fmt, const Args &... args) {
        for (auto logger : loggers) {
            if (logger) logger->debug(fmt, args...);
        }
    }

    template <typename... Args>
    void Info(const char *fmt, const Args &... args) {
        for (auto logger : loggers) {
            if (logger) logger->info(fmt, args...);
        }
    }

    template <typename... Args>
    void Warn(const char *fmt, const Args &... args) {
        for (auto logger : loggers) {
            if (logger) logger->warn(fmt, args...);
        }
    }

    template <typename... Args>
    void Error(const char *fmt, const Args &... args) {
        for (auto logger : loggers) {
            if (logger) logger->error(fmt, args...);
        }
    }

    template <typename... Args>
    void Critical(const char *fmt, const Args &... args) {
        for (auto logger : loggers) {
            if (logger) logger->critical(fmt, args...);
        }
    }

    void DumpWavefunction(const std::vector<double> &wf,
                          std::optional<std::string> name = std::nullopt) {
        if (this->wf_dump) {
            static int wf_id = 0;
            std::ofstream wf_out(this->wf_path.string() + name.value_or("wf_" + std::to_string(wf_id++)) + ".dat");
            if (wf_out) {
                wf_out << vectorToString<double>(wf);
            }
        }
    }

  private:
    std::array<std::shared_ptr<spdlog::logger>, Sink::SINKS_NO> loggers;
    std::shared_ptr<spdlog::logger> log_console, log_file;

    size_t const maxsize      = 4194304;  // 4MB
    size_t const maxfiles     = 4;
    std::string const logpath = "./schroedinger.log";

    std::filesystem::path wf_path = "./wavefunctions/";
    
    bool wf_dump        = false;

    void RegisterLoggers() {
        /* Console is for the important stuff */
        log_console = spdlog::stdout_color_mt("console");
        log_console->set_level(spdlog::level::warn);

        /* File is for debugging so let's get everything in there */
        log_file = spdlog::rotating_logger_mt("file", logpath, maxsize, maxfiles);
        log_file->set_level(spdlog::level::trace);

        loggers.at(Sink::CONSOLE_SINK) = log_console;
        loggers.at(Sink::FILE_SINK)    = log_file;
    }

    LogManager()  = default;
    ~LogManager() = default;
};

#endif