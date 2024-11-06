#include <iostream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <regex>
#include <sstream>
#include <stack>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <random>
#include <iomanip>
#include <map>
#include <filesystem>
#include <future>
#include <cerrno>
#include <cstdint> // For uint32_t
#include <streambuf>

#ifdef _WIN32
    #include <windows.h>
    #include <wininet.h>
    #include <conio.h>
#else
    #include <unistd.h>
#endif

using namespace std;

const std::string ASCII_GREEN = "\x1b[32m";
const std::string ASCII_BOLD = "\x1b[1m";
const std::string ASCII_RED = "\x1b[31m";
const std::string ASCII_WHITE = "\x1b[37m";
const std::string ASCII_RESET = "\x1b[0m" + ASCII_WHITE;

#ifdef _WIN32
bool fileExistsOnInternet(const std::string& url) {
    HINTERNET hInternet = InternetOpen("HTTP Checker", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hConnect = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return false;
    }

    DWORD statusCode = 0;
    DWORD length = sizeof(DWORD);
    HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &length, NULL);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return statusCode == 200;
}
#else
bool fileExistsOnInternet(const std::string& url) {
    std::string command = "wget --spider -q " + url;
    int result = system(command.c_str());
    return result == 0;
}
#endif

#ifdef _WIN32
std::string readFileFromInternet(const std::string& url) {
    HINTERNET hInternet = InternetOpen("File Reader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "Failed to open internet connection." << std::endl;
        return "";
    }

    HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        std::cerr << "Failed to open URL: " << url << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;
    std::stringstream fileContent;

    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        fileContent.write(buffer, bytesRead);
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return fileContent.str();
}
#else
std::string readFileFromInternet(const std::string& url) {
    std::string command = "wget -q -O - " + url;
    std::stringstream fileContent;
    FILE* fp = popen(command.c_str(), "r");
    if (fp) {
        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), fp)) {
            fileContent << buffer;
        }
        pclose(fp);
    } else {
        std::cerr << "Failed to execute command for URL: " << url << std::endl;
    }
    return fileContent.str();
}
#endif

void displayProgressBar(double percentage) {
    int barWidth = 35;
    std::cout << "[";
    int pos = static_cast<int>(barWidth * percentage);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(percentage * 100.0) << " %";
}

#ifdef _WIN32
bool downloadFileWithProgress(const std::string& url, const std::string& localFile) {
    HINTERNET hInternet = InternetOpen("File Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[8192];
    DWORD bytesRead;
    std::ofstream outputFile(localFile, std::ios::binary);

    if (!outputFile) {
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return false;
    }

    DWORD fileSize = 0;
    DWORD fileSizeLength = sizeof(fileSize);
    HttpQueryInfo(hUrl, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &fileSize, &fileSizeLength, NULL);

    double totalDownloaded = 0;
    auto startTime = std::chrono::steady_clock::now();

    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        totalDownloaded += bytesRead;
        outputFile.write(buffer, bytesRead);
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        double downloadSpeed = totalDownloaded / elapsedTime.count();

        double remainingTime = (fileSize - totalDownloaded) / downloadSpeed;
        std::cout << "\rDownloading " << url << " to " << localFile << " - "
                  << " Speed: " << downloadSpeed / (1024) << " KB/s"
                  << " Time left: " << int(remainingTime) << " sec "
                  << " Remaining: " << (fileSize - totalDownloaded) / (1024 * 1024) << " MB ";
        displayProgressBar(totalDownloaded / fileSize);
    }

    outputFile.close();
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    std::cout << "\nDownload completed successfully!" << std::endl;
    return true;
}
#endif
