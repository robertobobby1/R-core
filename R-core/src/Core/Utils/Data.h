#pragma once
#include "rcpch.h"

#include "Core/Utils/Dispatch.h"
#include "Core/Utils/File.h"

namespace RC {

    class OnWindowCloseEvent : public Dispatchable {
       public:
        OnWindowCloseEvent() = default;
        ~OnWindowCloseEvent() = default;

        virtual inline DispatchableType GetType() override {
            return DispatchableType::OnWindowCloseEventType;
        }
        static inline DispatchableType GetStaticType() {
            return DispatchableType::OnWindowCloseEventType;
        }
    };

    class ServerData : public Dispatchable {
       public:
        ServerData() = default;
        ~ServerData() = default;

        virtual inline DispatchableType GetType() override {
            return DispatchableType::ServerDataType;
        }
        static inline DispatchableType GetStaticType() { return DispatchableType::ServerDataType; }

        unsigned int m_maxPoolSize = 1;
        unsigned int m_port = 0;
        unsigned int m_activeConnections = 0;
        unsigned int m_totalConnections = 0;
        unsigned int m_backlog = 0;
    };

    class WindowData : public Dispatchable {
       public:
        const char* m_title;
        uint32_t m_width;
        uint32_t m_height;
        bool m_Vsync;
        // used to call functions
        std::function<void(Dispatchable&)> m_eventCallback;

        WindowData() = default;
        WindowData(uint32_t width, uint32_t height, const char* title, bool vsync)
            : m_height(height), m_width(width), m_Vsync(vsync), m_title(title) {}
        ~WindowData() = default;

        virtual inline DispatchableType GetType() override {
            return DispatchableType::WindowDataType;
        }
        static inline DispatchableType GetStaticType() { return DispatchableType::WindowDataType; }
    };

    class FileSystemData : public Dispatchable {
       public:
        std::shared_ptr<File> m_rootFile;

        FileSystemData(std::shared_ptr<File> rootFile) { m_rootFile = rootFile; }
        ~FileSystemData() = default;

        virtual inline DispatchableType GetType() override {
            return DispatchableType::FileSystemDataType;
        }
        static inline DispatchableType GetStaticType() {
            return DispatchableType::FileSystemDataType;
        }
    };
}  // namespace RC