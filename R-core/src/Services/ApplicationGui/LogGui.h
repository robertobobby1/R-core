#include "rcpch.h"

#include "Core/Service.h"
#include "Services/ImGui/SkeletonGui.h"

#include "imgui.h"

namespace RC {
    class LogGui : public Service {
       public:
        LogGui();
        ~LogGui() = default;

        virtual void Init() override;
        virtual void OnGuiUpdate() override;
        virtual inline bool IsGuiService() const override { return true; }

        void AddUnformattedLog(const spdlog::details::log_msg& msg);
        void AddLog(const char* start, size_t size);
        void Draw();

        std::shared_ptr<SkeletonGui> m_guiService;
        std::string m_dockWindowName;

        ImGuiTextBuffer m_buf;
        ImVector<int> m_lineOffsets;
        bool m_autoScroll;
    };
}  // namespace RC