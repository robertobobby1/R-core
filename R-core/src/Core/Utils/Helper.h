#pragma once 
#include "rcpch.h"

namespace RC {

    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }
}