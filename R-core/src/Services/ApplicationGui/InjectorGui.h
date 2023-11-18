#include "rcpch.h"
#include "Core/Service.h"

#include "Services/ImGui/SkeletonGui.h"

namespace RC {

    class InjectorGui : public Service {
       public:
        InjectorGui();
        ~InjectorGui() = default;

        virtual void Init() override;
        virtual void OnGuiUpdate() override;
        virtual inline bool IsGuiService() const override { return true; }

        void DisplayNode(int Id);
        void DisplayNodeDependencyInfo(int Id);

        std::shared_ptr<SkeletonGui> m_guiService;
    };
}  // namespace RC