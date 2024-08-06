#pragma once
#include "rcpch.h"

#include "Services/ImGui/GuiRenderer.h"

namespace RC {
    class SkeletonGui : public GuiRenderer {
       public:
        SkeletonGui() : GuiRenderer() {}
        ~SkeletonGui() = default;

        virtual bool IsUniqueService() const override { return true; }
        virtual void Begin() override;
        virtual void End() override;

        void SetActiveDockWindow(std::string windowName, bool isOpen) {
            s_dockWindowsOpen[windowName] = isOpen;
        }
        bool GetActiveDockWindow(std::string windowName) { return s_dockWindowsOpen[windowName]; }

        static std::map<std::string, bool> s_dockWindowsOpen;
    };
}  // namespace RC