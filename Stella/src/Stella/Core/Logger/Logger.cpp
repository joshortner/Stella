#include "Logger.h"

#include "Stella/Core/Strings/Strings.h"
#include "Stella/Core/Common/Common.h"
#include "Stella/Core/Assert/Assert.h"

#include <stdio.h>
#include <time.h>

#include <chrono>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace Stella
{

Logger * Logger::s_pLogger = nullptr;

void Logger::initialize()
{
    initialize("");
}

void Logger::initialize(std::string out_dir)
{
    STELLA_ASSERT((s_pLogger == nullptr), "Logger already initialized!");

    if (out_dir != "") {
        s_pLogger = new Logger(out_dir);
    }
    else {
        s_pLogger = new Logger();
    }
}

void Logger::destroy()
{
    STELLA_ASSERT((s_pLogger != nullptr), "Logger not initialized!");

    delete s_pLogger;
}

Logger::Logger() :
    m_pLogFile(nullptr)
{

}

Logger::Logger(std::string out_dir) :
    m_outDir(out_dir)
{
    if (!m_outDir.is_dir()) {
        m_outDir.create();
    }

    m_logFilePath = m_outDir.join_path(trim(get_time_stamp_str()) + ".log");
    m_pLogFile = fopen(m_logFilePath.c_str(), "w");

    if (m_pLogFile == nullptr) {
        printf("Unable to open log file: %s\n", m_logFilePath.c_str());
        printf("Redirecting output to the stdout\n");
    }
    else {
        printf("Log File Opened: %s\n", m_logFilePath.c_str());
    }
}

Logger::~Logger()
{
    if (m_pLogFile) {
        fclose(m_pLogFile);
        printf("Log File Closed: %s\n", m_logFilePath.c_str());
    }
}

void Logger::log(LevelFlag level, const char * fmt, ...)
{
    STELLA_ASSERT((s_pLogger != nullptr), "Logger not initialized!");

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");

    char output[2046] = { 0 };

    va_list args;
    va_start(args, fmt);
    vsnprintf(output, 2046, fmt, args);
    va_end(args);
    
    std::string output_string = (
        oss.str() + " " + 
        std::to_string(clock()) + " " + 
        level_to_string(level) + " " + 
        "[STELLA LOG] " +
        std::string(output) + "\n"
    );

    if (s_pLogger->m_pLogFile == nullptr) {
        printf("%s", output_string.c_str());
    }
    else {
        fputs(output_string.c_str(), s_pLogger->m_pLogFile);
    }
}

std::string Logger::get_time_stamp_str()
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string time_stamp(std::ctime(&end_time));
    std::replace(time_stamp.begin(), time_stamp.end(), ' ', '_');
    
    return time_stamp;
}

std::string Logger::level_to_string(LevelFlag level)
{
    switch (level)
    {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO"; 
        case WARN: return "WARN"; 
        case ERROR: return "ERROR";
        case FATAL: return "FATAL";
    }
    return "Unknow Level";
}

} // namespace Stella
