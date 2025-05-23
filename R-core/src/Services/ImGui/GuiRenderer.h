#pragma once

#include "Core/Service.h"
#include "Services/Window/Window.h"

namespace RC {

    class GuiRenderer : public Service {
       public:
        /*
         * For more information on how this works check imgui implementation
         * Specifically, example_glfw_opengl3 due to it being multiplatform
         */
        GuiRenderer();
        ~GuiRenderer();

        virtual void Init() override;
        virtual bool IsUniqueService() const override { return true; }

        void Shutdown();
        virtual void Begin();
        virtual void End();

        void OnDispatchable(Dispatchable& dispatchable);

       public:
        std::shared_ptr<Window> m_windowService;

       protected:
        bool m_isWindowRunning = true;
        bool m_isInitialized = false;
    };
}  // namespace RC