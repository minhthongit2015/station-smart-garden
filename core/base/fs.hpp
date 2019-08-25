
#pragma once
#ifndef SMART_GARDEN_FILESYSTEM_H
#define SMART_GARDEN_FILESYSTEM_H

#include "./utils.hpp"
#include <FS.h>

class FileSystem {
  private:
    void _listingFiles(Dir &root);
  public:
    FS &fs;
    FileSystem():fs(SPIFFS) { }
    void setup();
    void listingFiles();
    void showInfo();
    bool format() {
      logz("File System", "Format Disk");
      return fs.format();
    }
};

typedef FileSystem *pFileSystem;

void FileSystem::setup() {
  logStart("File System");
  if (!fs.begin()) {
    error("File System", "Failed to mount file system");
  }
}

void FileSystem::listingFiles() {
  logz("File System", "Listing files");
  Dir root = fs.openDir("/");
  _listingFiles(root);
}

void FileSystem::_listingFiles(Dir &root) {
  while (root.next()) {
    if (root.isFile()) {
      prf("> [File System] Root: %s\r\n", root.fileName().c_str());
      File file = root.openFile("r");
      prf("> [File System] %15s / %15s (%d Bytes)\r\n", file.name(), file.fullName(), file.size());
    } else {
      prf("> [File System] Dir: %s\r\n", root.fileName().c_str());
      _listingFiles(root);
    }
  }
}

void FileSystem::showInfo() {
  logz("File System", "Disk Info");
  FSInfo fs_info;
  fs.info(fs_info);
  prf("> [File System] Usage: %d / %d (%d%%)\r\n",
    fs_info.usedBytes, fs_info.totalBytes,
    fs_info.usedBytes / fs_info.totalBytes);
  prf("> [File System] Block Size: %d / Page Size: %d\r\n",
    fs_info.blockSize, fs_info.pageSize);
  prf("> [File System] Max Open Files: %d / Max Path Length: %d\r\n",
    fs_info.maxOpenFiles, fs_info.maxPathLength);
}

#endif
