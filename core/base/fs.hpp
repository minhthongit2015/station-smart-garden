
#pragma once
#ifndef BEYOND_GARDEN_FILESYSTEM_H
#define BEYOND_GARDEN_FILESYSTEM_H

#include <FS.h>
#include "./utils.hpp"

class FileSystem : public FS {
  private:
    void _listingFiles(Dir &root);
  public:
    FileSystem(): FS(SPIFFS) { }
    void setup();
    void listingFiles();
    void showInfo();
} fsz;

typedef FileSystem* pFileSystem;

extern FileSystem fsz;

void FileSystem::setup() {
  logStart("File System");
  if (!begin()) {
    error("File System", "Failed to mount file system");
  }
}

void FileSystem::listingFiles() {
  logBlock("File System", "Listing files");
  Dir root = openDir("/");
  _listingFiles(root);
}

void FileSystem::_listingFiles(Dir &root) {
  while (root.next()) {
    if (root.isFile()) {
      File file = root.openFile("r");
      prf("> [File System] \"%s\" (%d Bytes)\r\n", file.fullName(), file.size());
    } else {
      prf("> [File System] Dir: \"%s\"\r\n", root.fileName().c_str());
      _listingFiles(root);
    }
  }
}

void FileSystem::showInfo() {
  log("File System", "Disk Info");
  FSInfo fs_info;
  info(fs_info);
  prf("> [File System] Usage: %d / %d (%d%%)\r\n",
    fs_info.usedBytes, fs_info.totalBytes,
    fs_info.usedBytes / fs_info.totalBytes);
  prf("> [File System] Block Size: %d / Page Size: %d\r\n",
    fs_info.blockSize, fs_info.pageSize);
  prf("> [File System] Max Open Files: %d / Max Path Length: %d\r\n",
    fs_info.maxOpenFiles, fs_info.maxPathLength);
}

#endif
