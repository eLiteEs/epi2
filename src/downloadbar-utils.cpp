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
#include <windows.h>
#include <conio.h>
#ifdef _WIN32
    #include <windows.h>
    #include <wininet.h>
    #pragma comment(lib, "wininet.lib")  // Link WinINet library
#else
    #include <cstdlib>  // For system() call on Linux
#endif

using namespace std;
const std::string ASCII_GREEN = "\x1b[32m";
const std::string ASCII_BOLD = "\x1b[1m";        // Bold text
const std::string ASCII_RED = "\x1b[31m";
const std::string ASCII_WHITE = "\x1b[37m";
const std::string ASCII_RESET = "\x1b[0m" + ASCII_WHITE;   

// Functions for installing libraries
bool fileExistsOnInternet(const std::string& url) {
#ifdef _WIN32
    // Windows-specific code using WinINet API
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

#else
    // Linux-specific code using system call
    std::string command = "wget --spider -q " + url;
    int result = system(command.c_str());

    return result == 0;  // If wget returns 0, it means the file exists
#endif
}
std::string readFileFromInternet(const std::string& url) {
    // Initialize an internet session
    HINTERNET hInternet = InternetOpen("File Reader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "Failed to open internet connection." << std::endl;
        return "";
    }

    // Open the URL
    HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        std::cerr << "Failed to open URL: " << url << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    // Buffer for reading data
    char buffer[4096];
    DWORD bytesRead;
    std::stringstream fileContent;

    // Read data from the URL
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        fileContent.write(buffer, bytesRead);
    }

    // Clean up
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return fileContent.str();  // Return the full content of the file as a string
}

void displayProgressBar(double percentage) {
    int barWidth = 35;  // Width of the progress bar
    std::cout << "[";
    int pos = static_cast<int>(barWidth * percentage);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(percentage * 100.0) << " %";
}

// Function to download the file and display progress in one line
bool downloadFileWithProgress(const std::string& url, const std::string& localFile) {
    HINTERNET hInternet = InternetOpen("File Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "Failed to open internet connection." << std::endl;
        return false;
    }

    HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        std::cerr << "Failed to open URL: " << url << std::endl;
        InternetCloseHandle(hInternet);
        return false;
    }

    // Buffer and progress tracking variables
    char buffer[8192];
    DWORD bytesRead;
    std::ofstream outputFile(localFile, std::ios::binary);

    if (!outputFile) {
        std::cerr << "Failed to create file: " << localFile << std::endl;
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return false;
    }

    // Get file size (if available from server)
    DWORD fileSize = 0;
    DWORD fileSizeLength = sizeof(fileSize);
    HttpQueryInfo(hUrl, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &fileSize, &fileSizeLength, NULL);

    double totalDownloaded = 0;
    double lastDownloaded = 0;
    double downloadSpeed = 0;
    double lastDownloadSpeed = 0;
    auto startTime = std::chrono::steady_clock::now();
    bool speedIncreased = false;

    // Read the file in chunks and track progress
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        totalDownloaded += bytesRead;
        outputFile.write(buffer, bytesRead);

        // Calculate time and speed
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        downloadSpeed = totalDownloaded / elapsedTime.count();  // Bytes per second

        // Check if speed increased or decreased
        speedIncreased = downloadSpeed >= lastDownloadSpeed;

        // Estimate remaining time
        double remainingTime = (fileSize - totalDownloaded) / downloadSpeed;

        // Display progress in one line
        std::cout << "\r";  // Carriage return to overwrite the line


        // Print progress line
        std::cout << "Downloading " << url << " to " << localFile << ":"
                  << " Size: " << fileSize / (1024 * 1024) << " MB"
                  << " D. Speed: ";
        if(speedIncreased) {
            cout << ASCII_GREEN << ASCII_BOLD;
        } else {
            cout << ASCII_RED << ASCII_BOLD;
        }
        cout << std::fixed << std::setprecision(2) << downloadSpeed / (1024) << " KB/s";
        cout << ASCII_RESET
                  << " Time left: " << int(remainingTime) << " sec"
                  << " Remaining: " << (fileSize - totalDownloaded) / (1024 * 1024) << " MB ";
        displayProgressBar(totalDownloaded / fileSize);  // Display progress bar


        lastDownloadSpeed = downloadSpeed;

        // Simulate real-time progress update
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    outputFile.close();
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    std::cout << "\nDownload completed successfully!" << std::endl;
    return true;
}
