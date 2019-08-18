#include "FileHelper.hpp"
#include <stdio.h>
#include <vector>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <streambuf>
#include <istream>

using namespace Mini;

void FileHelper::ParseFile(const std::string& filename, const std::function<void(const std::string& line)>& lineRead) {
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        while ( std::getline (file,line) ) {
            lineRead(line);
        }
        file.close();
    }
}

void FileHelper::RecurseFolder(const std::string& path, const std::function<void(const std::string&)>& onFile, const std::string& extension) {
    RecurseFolderInternal(path, onFile, extension);
}

void FileHelper::RecurseFolderInternal(const std::string& path, const std::function<void(const std::string&)>& onFile, const std::string& extension) {
    
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path.c_str());
    if (dp == NULL) {
        return;
    }

    while ((entry = readdir(dp))) {
        std::string filename(entry->d_name);
        if (entry->d_type == DT_DIR ) {
            if (filename!="." && filename!="..") {
                RecurseFolderInternal(path +"/"+ filename, onFile, extension);
            }
        } else {
            if (extension == "" || filename.find(extension)!=std::string::npos) {
                onFile(path+ "/"+ filename);
            }
        }
    }
    closedir(dp);
}


bool FileHelper::FileExists(const std::string &path) {
    std::ifstream f(path.c_str());
    return f.good();
}

bool FileHelper::DeleteFile(const std::string &path) {
    return remove( path.c_str() ) == 0;
}

bool FileHelper::RenameFile(const std::string &oldPath, const std::string &newPath) {
    return rename(oldPath.c_str(), newPath.c_str()) == 0;
}

void FileHelper::CreateTextFile(const std::string &path, const std::string &text) {
    std::ofstream out(path);
    out << text;
}

bool FileHelper::CreateFolder(const std::string &path) {
    return mkdir(path.c_str(), 0755) == 0;
}

std::string FileHelper::GetFileNameFromPath(const std::string &path) {
    auto pos = path.rfind("/");
    if (pos == std::string::npos) {
        return path;
    } else {
        return path.substr(pos+1, path.size()-pos-1);
    }
}

std::string FileHelper::GetFolderPath(const std::string &path) {
    auto pos = path.rfind("/");
    if (pos == std::string::npos) {
        return path;
    } else {
        return path.substr(0, pos);
    }
}

void FileHelper::ParseBuffer(unsigned char *unsignedBuffer, size_t size, const std::function<void (const std::string &)> &onLine) {
    struct membuf : std::streambuf {
        membuf(char* begin, char* end) {
            this->setg(begin, begin, end);
        }
    };

    char* buffer = reinterpret_cast<char*>(unsignedBuffer);
    membuf sbuf(buffer, buffer + size);
    std::istream in(&sbuf);
    std::string line;
    while (std::getline(in, line)) {
        onLine(line);
    }
}



