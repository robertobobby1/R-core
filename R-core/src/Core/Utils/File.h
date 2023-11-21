#pragma once
#include "rcpch.h"

#include "Core/Core.h"

namespace RC {
    enum FILE_TYPE { NONE = 0,
                     DIR = 1,
                     FILE = 2 };
    static std::map<FILE_TYPE, const char*> s_map{
        {FILE_TYPE::NONE, "Other"}, {FILE_TYPE::FILE, "File"}, {FILE_TYPE::DIR, "Directory"}
    };

    // Defines not only single files but folders and it's children
    class File {
       public:
        File(FILE_TYPE fileType = FILE_TYPE::NONE, std::string fileName = "", std::string absPath = "", std::uintmax_t fileSize = 0)
            : m_fileType(fileType),
              m_fileName(fileName),
              m_absPath(absPath),
              m_fileSizeInBytes(fileSize) {}

        static std::shared_ptr<File> MakeFileObj(const char* fileName, const char* absolutePath, std::uintmax_t fileSize) {
            return std::make_shared<File>(FILE_TYPE::FILE, fileName, absolutePath, fileSize);
        }
        static std::shared_ptr<File> MakeDirectoryObj(const char* fileName, const char* absolutePath, std::uintmax_t fileSize) {
            return std::make_shared<File>(FILE_TYPE::DIR, fileName, absolutePath, fileSize);
        }

        static std::shared_ptr<File> MakeFileObj(
            const fs::directory_entry& directory
        ) {
            auto absolutePath = fs::absolute(directory.path());
            auto fileName = directory.path().filename();
            auto fileSize = directory.file_size();

            return std::make_shared<File>(FILE_TYPE::FILE, fileName, absolutePath, fileSize);
        }
        static std::shared_ptr<File> MakeDirectoryObj(
            const fs::directory_entry& directory
        ) {
            auto absolutePath = fs::absolute(directory.path());
            auto fileName = directory.path().filename();

            return std::make_shared<File>(FILE_TYPE::DIR, fileName, absolutePath);
        }

        static const char* GetFileTypeAsCharPointer(FILE_TYPE m_fileType) {
            return s_map[m_fileType];
        }

        std::string StringifyFileSize() {
            // if the file is a directory no size!
            if (m_fileType == DIR) {
                return std::string();
            }

            std::string res =
                fmt::format("{:.2f}KB ({})", RC_BTOKB(m_fileSizeInBytes), m_fileSizeInBytes);
            return res;
        }

        bool checkIfChanged(std::shared_ptr<File>& file) {
            return this->checkIfChanged(file.get());
        }
        bool checkIfChanged(File* file) {
            return this->m_fileName != file->m_fileName || this->m_fileType != file->m_fileType ||
                   this->files.size() != file->files.size();
        }

        void ToString() {
            auto fileTypeString = "";
            if (m_fileType == FILE_TYPE::DIR)
                fileTypeString = "Directory";
            else if (m_fileType == FILE_TYPE::FILE)
                fileTypeString = "File";
            else
                fileTypeString = "Other";

            RC_LOG_INFO("[{0}] with path [{1}] and filename [{2}]", fileTypeString, m_absPath, m_fileName);
            for (auto& file : files) {
                file->ToString();
            }
        }

        FILE_TYPE m_fileType;
        std::string m_absPath;
        std::string m_fileName;
        // size in Bytes
        std::uintmax_t m_fileSizeInBytes;
        std::vector<std::shared_ptr<File>> files;
    };
}  // namespace RC