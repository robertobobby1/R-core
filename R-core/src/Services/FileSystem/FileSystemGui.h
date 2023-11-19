#pragma once

#include "Core/Service.h"
#include "Services/ImGui/SkeletonGui.h"
#include "Services/FileSystem/FileSystem.h"

namespace RC {

    class FileSystemGui : public Service {
       public:
        FileSystemGui();
        ~FileSystemGui() = default;

        virtual void Init() override;
        virtual void OnGuiUpdate() override;
        virtual inline bool IsGuiService() const override { return true; }
        void DisplayFile(std::shared_ptr<File> file);

        void OnDispatchable(Dispatchable& dispatchable);

        std::string m_dockWindowName;
        std::shared_ptr<SkeletonGui> m_guiService;
        std::shared_ptr<FileSystem> m_fileService;

        bool m_isFileSystemReady = false;
    };
}  // namespace RC