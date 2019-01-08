//
//  FileHelper.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 30/12/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <functional>

namespace Mini {
    class FileHelper {
    public:
        static void ParseFile(const std::string& filename, const std::function<void(const std::string& line)>& lineRead);
        static bool FileExists(const std::string& path);
        static bool DeleteFile(const std::string& path);
        static std::string GetFileNameFromPath(const std::string& path);
        static std::string GetFolderPath(const std::string& path);
        static bool CreateFolder(const std::string& path);
        static void RecurseFolder(const std::string& path, const std::function<void(const std::string&)>& onFile, const std::string& extension = "");
        static void ParseBuffer(unsigned char* buffer, size_t size, const std::function<void(const std::string&)>& onLine);
    private:
        static void RecurseFolderInternal(const std::string& path, const std::function<void(const std::string&)>& onFile, const std::string& extension);
    };
}
