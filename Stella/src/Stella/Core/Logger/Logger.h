#pragma once

#include "Stella/Core/Common/Common.h"
#include "Stella/Platform/Path/Path.h"

#include <stdint.h>
#include <stdlib.h>

#include <string>

#define STELLA_ENGINE_LOG(fmt, ...)
#define STELLA_LOG(lvl, fmt, ...) Logger::log(lvl, "%s " fmt, __FILENAME__, ##__VA_ARGS__)

namespace Stella
{   

class Logger
{

public:
typedef uint32_t LevelFlag;
enum LevelFlag_
{
    DEBUG = 0x1 << 0,
    INFO  = 0x1 << 1,
    WARN  = 0x1 << 2,
    ERROR = 0x1 << 3,
    FATAL = 0x1 << 4
};

public:

    Logger();
    Logger(std::string out_dir);
    ~Logger();

    static void log(LevelFlag level, const char * fmt, ...);

    static void initialize();
    static void initialize(std::string out_dir);
    static void destroy();

private:

    static std::string get_time_stamp_str();
    static std::string level_to_string(LevelFlag level);

    Path m_outDir;
    Path m_logFilePath;
    FILE * m_pLogFile;

private:

    static Logger * s_pLogger;

};

} // namespace Stella