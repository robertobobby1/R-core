#include "rcpch.h"

#include "Core/Application.h"
#include "Core/Utils/Data.h"
#include "Core/Utils/Dispatch.h"
#include "Services/FileSystem/FileSystemGui.h"

#include "imgui.h"

namespace RC {

    // Object from different class running in different thread READ_ONLY!
    static std::shared_ptr<File> s_rootFile;
    static ImGuiTreeNodeFlags s_directoryFlags = ImGuiTreeNodeFlags_SpanAllColumns;
    static ImGuiTableFlags s_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH |
                                     ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
                                     ImGuiTableFlags_NoBordersInBody;
    static ImGuiTreeNodeFlags s_fileFlags = s_directoryFlags | ImGuiTreeNodeFlags_Leaf |
                                            ImGuiTreeNodeFlags_Bullet |
                                            ImGuiTreeNodeFlags_NoTreePushOnOpen;

    FileSystemGui::FileSystemGui() : Service() {
        this->m_dependencies.push_back(
            DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false));
        this->m_dependencies.push_back(
            DependencyDescriber("FILESYSTEM", std::make_shared<FileSystem>(), false));
        this->m_dockWindowName = "Filesystem";
    }

    void FileSystemGui::Init() {
        m_guiService = this->GetDep<SkeletonGui>("GUI");
        m_fileService = this->GetDep<FileSystem>("FILESYSTEM");
        m_fileService->AddDependencyCallback(RC_BIND_FN(FileSystemGui::OnDispatchable));

        // Added in init where the gui service is already initialized
        Application::GetApp().SetGuiRenderer(m_guiService);
        m_guiService->SetActiveDockWindow(m_dockWindowName, true);
    }

    void FileSystemGui::OnGuiUpdate() {
        if (!m_guiService->GetActiveDockWindow(m_dockWindowName)) return;

        ImGui::Begin("FileSystem");
        // Show loading while file system is not ready.
        if (!m_isFileSystemReady) {
            ImGui::Text("Loading %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
            ImGui::End();
            return;
        }

        ImGui::TextUnformatted("The root path computing is:");
        ImGui::SameLine();
        ImGui::TextUnformatted(s_rootFile->m_absPath.c_str());
        ImGui::Separator();

        static float smallColumnWidth = ImGui::CalcTextSize("A").x * 16.0f;
        if (ImGui::BeginTable("3ColumnTable", 3, s_flags)) {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, smallColumnWidth);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, smallColumnWidth);
            ImGui::TableHeadersRow();

            // recursively displays all files under rootFile
            DisplayFile(s_rootFile);
            ImGui::EndTable();
        }
        ImGui::End();
    }

    void FileSystemGui::DisplayFile(std::shared_ptr<File> file) {
        const char* fileName = file->m_fileName.c_str();
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        bool isDir = (file->m_fileType == DIR);
        // file name
        if (isDir) {
            bool open = ImGui::TreeNodeEx(fileName, s_directoryFlags);
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(File::GetFileTypeAsCharPointer(file->m_fileType));
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("--");

            if (open) {
                for (auto childFile : file->files) {
                    DisplayFile(childFile);
                }
                ImGui::TreePop();
            }
        } else {
            const char* sizeString = file->StringifyFileSize().c_str();
            bool open = ImGui::TreeNodeEx(fileName, s_fileFlags);
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(File::GetFileTypeAsCharPointer(file->m_fileType));
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(sizeString);
        }
    }

    void FileSystemGui::OnDispatchable(Dispatchable& dispatchable) {
        Dispatcher disp(dispatchable);
        disp.Dispatch<FileSystemData>([this](const FileSystemData& data) {
            s_rootFile = data.m_rootFile;
            this->m_isFileSystemReady = true;
        });
    }
}  // namespace RC
