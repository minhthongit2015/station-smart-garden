
#pragma once
#ifndef BEYOND_GARDEN_FILESYSTEM_H
#define BEYOND_GARDEN_FILESYSTEM_H

#include <FS.h>
#include "./utils.hpp"

// const char FILE_SYSTEM[] = "File System";
#define FILE_SYSTEM "File System"


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
  logStart(FILE_SYSTEM);
  if (!begin()) {
    error(FILE_SYSTEM, "Failed to mount file system");
  }
}

void FileSystem::listingFiles() {
  logBlock(FILE_SYSTEM, "Listing files");
  Dir root = openDir("/");
  _listingFiles(root);
}

void FileSystem::_listingFiles(Dir &root) {
  while (root.next()) {
    if (root.isFile()) {
      File file = root.openFile("r");
      logf(FILE_SYSTEM, "\"%s\" (%d Bytes)\r\n", file.fullName(), file.size());
    } else {
      logf(FILE_SYSTEM, "Dir: \"%s\"\r\n", root.fileName().c_str());
      _listingFiles(root);
    }
  }
}

void FileSystem::showInfo() {
  log(FILE_SYSTEM, "Disk Info");
  FSInfo fs_info;
  info(fs_info);
  logf(FILE_SYSTEM, "Usage: %d / %d (%d%%)\r\n",
    fs_info.usedBytes, fs_info.totalBytes,
    fs_info.usedBytes / fs_info.totalBytes);
  logf(FILE_SYSTEM, "Block Size: %d / Page Size: %d\r\n",
    fs_info.blockSize, fs_info.pageSize);
  logf(FILE_SYSTEM, "Max Open Files: %d / Max Path Length: %d\r\n",
    fs_info.maxOpenFiles, fs_info.maxPathLength);
}

#endif
