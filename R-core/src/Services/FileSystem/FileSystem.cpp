#include "rcpch.h"

#include "Services/FileSystem/FileSystem.h"
#include "Core/Utils/Config.h"
#include "Core/Utils/Data.h"
#include <filesystem>

namespace RC {

    FileSystem::FileSystem() : Service() { m_actionQueue = std::make_shared<std::queue<Action>>(); }

    void FileSystem::Init() {}

    void FileSystem::Run() {
        m_rootFullPath = Config::Get("file_system_root_path");
        if (m_rootFullPath == "") {
            RC_LOG_ERROR("file_system_root_path config variable must be defined!");
            return;
        }

        HardReloadDirectoryInfo();
        while (m_isRunning) {
            // blocking until new action is called
            Action action = ConsumeFromActionQueue();
            // perform action
            switch (action.m_fileAction) {
                case HARD_RELOAD:
                    HardReloadDirectoryInfo();
                    break;
                case SOFT_RELOAD:
                    // to do! better performance, checking for changes
                    break;
                case NONE_ACTION:
                default:
                    RC_LOG_WARN("The given action is empty or unknown!");
                    break;
            }
        }
    }

    void FileSystem::HardReloadDirectoryInfo() {
        m_rootFile = FillDirectoryInfo(std::filesystem::directory_entry{m_rootFullPath});

        FileSystemData data{m_rootFile};
        Service::CallDepCallbacks(data);
    }

    std::shared_ptr<File> FileSystem::FillDirectoryInfo(
        const std::filesystem::directory_entry& directory
    ) {
        if (!directory.is_directory()) {
            RC_LOG_WARN("The given entry is not a directory!");
            return nullptr;
        }

        auto dir = File::MakeDirectoryObj(directory);
        for (auto const& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                dir->files.push_back(File::MakeFileObj(entry));
            } else if (entry.is_directory()) {
                dir->files.push_back(FillDirectoryInfo(entry));
            } else {
                RC_LOG_WARN("Strange file found!");
            }
        }
        return dir;
    }

    void FileSystem::AddActionToQueue(Action action) {
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_actionQueue->push(action);
        }
        m_queueCondition.notify_one();
    }

    Action FileSystem::ConsumeFromActionQueue() {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        // block the file system service here until a new action is necessary
        m_queueCondition.wait(lock);

        // return null action
        RC_IF((m_actionQueue->empty()), return Action());

        Action action = m_actionQueue->front();
        m_actionQueue->pop();
        return action;
    }
}  // namespace RC