//
//  File.cpp
//  Android
//
//  Created by Jeppe Nielsen on 15/07/15.
//
//

#include "File.hpp"
#import <Foundation/Foundation.h>
#include <stdlib.h>
#include "FileArchive.hpp"

using namespace Mini;

File::File() : size(0), data(0), userData(0) {}
File::File(std::string path) { Load(path); }
File::~File() { delete[] data; data = 0; size = 0; }

bool File::Load(std::string filename) {
    
    if (fileArchive) {
        return fileArchive->TryLoadData(filename, [this] (void* d, size_t s) {
            this->data = (unsigned char*)d;
            this->size = s;
        });
    }

    NSString* file = [NSString stringWithUTF8String:filename.c_str()];
    NSString* path = [[NSBundle mainBundle] pathForResource:file ofType:nil];
    if (!path) {
        path = file;
    }
    NSData *nsData = [NSData dataWithContentsOfFile:path];
    if (!nsData) return false;
    //[nsData retain];
    NSUInteger size = [nsData length] / sizeof(unsigned char);
    data = (unsigned char*) [nsData bytes];
    this->size = size;
    return true;
}

std::string File::GetFullPath(std::string filename) {
    NSString* file = [NSString stringWithUTF8String:filename.c_str()];
    NSString* path = [[NSBundle mainBundle] pathForResource:file ofType:nil];
    return [path UTF8String];
}

FileArchive* File::fileArchive = 0;

void File::SetArchive(Mini::FileArchive &archive) {
    fileArchive = &archive;
}

void File::RemoveArchive() {
    fileArchive = 0;
}
