#pragma once

#include "Stella/Core/Common/Common.h"

#include <stdio.h>
#include <stdlib.h>

#define STELLA_ASSERT(condition, message, ...)              \
    do                                                      \
    {                                                       \
        if (!condition)                                     \
        {                                                   \
            printf("[STELLA ASSERT] %s:%d - " message "\n", \
               __FILENAME__,                                \
               __LINE__,                                    \
               ##__VA_ARGS__);                              \
            exit(0);                                        \
        }                                                   \
    } while(0);