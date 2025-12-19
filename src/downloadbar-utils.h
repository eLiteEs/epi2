#ifndef DOWNLOADBAR_H
#define DOWNLOADBAR_H

#include <string>

extern bool fileExistsOnInternet(const std::string& url);
extern std::string readFileFromInternet(const std::string& url);
extern void displayProgressBar(double percentage);
extern void downloadFileWithProgress(const std::string& url, const std::string& localFile);

#endif
