#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h> // For Windows MessageBox
#else
    #include <cstdlib>   // For Linux Zenity
#endif

void showDialog(const std::string& message, const std::string& title) {
#ifdef _WIN32
    MessageBox(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
#else
    std::string command = "zenity --info --text=\"" + message + "\" --title=\"" + title + "\"";
    std::system(command.c_str());
#endif
}