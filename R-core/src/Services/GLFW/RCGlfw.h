#pragma once

#include "rcpch.h"
#include "Core/Service.h"

namespace RC {
    class RCGlfw : public Service {
       public:
        RCGlfw() : Service() {}
        ~RCGlfw() = default;

        virtual void Init() override;
        virtual void Run() override;
        virtual bool IsUniqueService() const override { return true; }

        static std::shared_ptr<RCGlfw> Create();

        void PrintOpenGLInfo();

       private:
        bool m_isPrinted = false;
    };
}  // namespace RC