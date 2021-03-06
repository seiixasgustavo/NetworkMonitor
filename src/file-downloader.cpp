#include <NetworkMonitor/file-downloader.h>

#include <nlohmann/json.hpp>

#include <curl/curl.h>

#include <filesystem>
#include <fstream>
#include <stdio.h>
#include <string>

bool NetworkMonitor::DownloadFile(
    const std::string &url,
    const std::filesystem::path &destination,
    const std::filesystem::path &caCertFile)
{
    CURL *curl{curl_easy_init()};
    if (curl == nullptr)
    {
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, caCertFile.string().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    std::FILE *fp{fopen(destination.string().c_str(), "wb")};
    if (fp == nullptr)
    {
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    fclose(fp);

    return res == CURLE_OK;
}

nlohmann::json NetworkMonitor::ParseJsonFile(
    const std::filesystem::path &source)
{
    nlohmann::json parsed{};
    if (!std::filesystem::exists(source))
    {
        return parsed;
    }
    try
    {
        std::ifstream file{source};
        file >> parsed;
    }
    catch (...)
    {
        // Will return an empty object.
    }
    return parsed;
}
