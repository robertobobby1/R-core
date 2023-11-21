#pragma once
#include "rcpch.h"

#include "Core/Service.h"
#include "Core/Utils/File.h"

namespace RC {

    enum FILE_ACTION {
        NONE_ACTION = 0,
        SOFT_RELOAD = 1,
        HARD_RELOAD = 2,
    };

    struct Action {
        FILE_ACTION m_fileAction;
        // will receive paths in the future to delete files or create new...
        Action(FILE_ACTION fileAction = FILE_ACTION::NONE_ACTION) : m_fileAction(fileAction) {}
    };

    class FileSystem : public Service {
       public:
        FileSystem();
        ~FileSystem() = default;

        virtual void Init() override;
        virtual void Run() override;

        void AddActionToQueue(Action action);

       private:
        Action ConsumeFromActionQueue();
        std::shared_ptr<File> FillDirectoryInfo(const fs::directory_entry& directory);
        void HardReloadDirectoryInfo();

        std::shared_ptr<File> m_rootFile;
        std::string m_rootFullPath;

        std::mutex m_queueMutex;
        std::condition_variable m_queueCondition;
        std::shared_ptr<std::queue<Action>> m_actionQueue;

        bool m_isRunning = true;
    };
}  // namespace RC