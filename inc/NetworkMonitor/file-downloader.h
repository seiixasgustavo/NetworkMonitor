#ifndef FILE_DOWNLOADER_H
#define FILE_DOWNLOADER_H

#include <filesystem>
#include <string>

namespace NetworkMonitor {

bool DownloadFile(const std::string &url,
                  const std::filesystem::path &destination,
                  const std::filesystem::path &caCertFile = {});
}

#endif
