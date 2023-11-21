#pragma once
#include "rcpch.h"

namespace RC {

    enum INTERCEPTOR_TYPE {
        REQUEST,
        RESPONSE
    };

    class Middleware {
       public:
        virtual void Run(const Req &req, const Res &res){};
        virtual inline const char *GetChildClassName() const { return typeid(*(this)).name(); }
    };

    class Interceptor {
       public:
        Interceptor(INTERCEPTOR_TYPE type) : m_type(type) {}
        virtual void Run(const Req &req, const Res &res){};
        virtual inline const char *GetChildClassName() const { return typeid(*(this)).name(); }

        INTERCEPTOR_TYPE m_type;
    };
}  // namespace RC