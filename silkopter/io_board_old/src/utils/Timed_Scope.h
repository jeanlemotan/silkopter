#pragma once

#include "chrono.h"
#include "utils/Flash_String.h"

namespace util
{
    class Timed_Scope
    {
    public:
        Timed_Scope(util::Flash_String file, int line)
            : m_file(file)
            , m_line(line)
            , m_start(board::clock::now_us())
        {
        }

        ~Timed_Scope()
        {
            auto d = board::clock::now_us() - m_start;
            q::quick_logf(F_STR("TS: {}:{}: {}"), m_file, m_line, d);
        }

    private:
        util::Flash_String m_file;
        int m_line;
        chrono::time_us m_start;
    };
};


#define TIMED_BLOCK() util::Timed_Scope __ts__(F_STR(__FILE__), __LINE__);
