#include "rcpch.h"
#include "Core/Service.h"

#include "Services/ImGui/SkeletonGui.h"

namespace RC {
    class ApplicationGui : public Service {
       public:
        ApplicationGui();
        ~ApplicationGui() = default;

        virtual void Init() override;
        virtual void OnGuiUpdate() override;
        virtual inline bool IsGuiService() const override { return true; }

        std::shared_ptr<SkeletonGui> m_guiService;
    };
}  // namespace RC