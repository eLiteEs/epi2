#include <windows.h>
#include <iostream>
#include <string>

void showDialog(std::string message, std::string title) {
    MessageBox(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
}