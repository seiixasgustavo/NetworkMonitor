#ifndef FILE_DOWNLOADER_H
#define FILE_DOWNLOADER_H

#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>

namespace NetworkMonitor
{

    bool DownloadFile(
        const std::string &url,
        const std::filesystem::path &destination,
        const std::filesystem::path &caCertFile = {});

    nlohmann::json ParseJsonFile(
        const std::filesystem::path &source);
}

#endif
