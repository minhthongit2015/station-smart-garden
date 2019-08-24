
#pragma once
#ifndef SMART_GARDEN_FILESYSTEM_H
#define SMART_GARDEN_FILESYSTEM_H

#include "./utils.hpp"
#include <FS.h>

class FileSystem {
  public:
    FS &fs;
    FileSystem():fs(SPIFFS) { }
    void setup();
};

void FileSystem::setup() {
  fs.begin();
}

#endif
