// File containing util functions

#include <iostream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <regex>
#include <stack>
#include <chrono>
#include <thread>
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
    #include <cstdlib>  // For system() call on Linux
    #include <ncurses.h>
#endif

using namespace std;
typedef string String;

// Util Functions
#ifdef _WIN32
#else
struct COORD {
    int X;
    int Y;
};
#endif
bool isStrOnCharArr(const std::string& str, char** arr, int argc) {
    for (int i = 0; i < argc; ++i) {
        if (str == arr[i]) {
            return true;
        }
    }
    return false;
}
int indexOfStrOnCharArr(const std::string& str, char** arr, int argc) {
    for (int i = 0; i < argc; ++i) {
        if (str == arr[i]) {
            return i;
        }
    }
    return 0;
}
bool isCharOutsideQoutes(String s, char c) {
    bool insideQoutes = false;
    for(long long unsigned int i = 0; i < s.length(); i++) {
        if(s.at(i) == '\"' && !insideQoutes) {
            try {
                if(s.at(i - 1) != '\\') {
                    insideQoutes = true;
                }
            } catch(exception&) {
                insideQoutes = true;
            }
        } else if(s.at(i) == '\"' && insideQoutes) {
            try {
                if(s.at(i - 1) != '\\') {
                    insideQoutes = false;
                }
            } catch(exception&) {
                insideQoutes = false;
            }
        }
        if(s.at(i) == c && !insideQoutes) {
            return true;
        }
    }
    return false;
}
std::string replaceOutsideQuotes(const std::string &input, const std::string &toReplace, const std::string &replaceWith) {
    std::string result;
    bool insideQuotes = false;

    size_t len = toReplace.length();
    for (size_t i = 0; i < input.length(); ) {
        if (input[i] == '\"') {
            insideQuotes = !insideQuotes;
            result += input[i];
            ++i;
        } else if (!insideQuotes && input.substr(i, len) == toReplace) {
            result += replaceWith;
            i += len; // Move past the replaced substring
        } else {
            result += input[i];
            ++i;
        }
    }

    return result;
}

void removeSpacesOutsideQuotes(std::string &s) {
    s = replaceOutsideQuotes(s, " ", "");
}
std::vector<std::string> splitOutsideQuotes(const std::string& str, char c) {
    std::vector<std::string> result;
    std::string temp;
    bool inQuotes = false;
    int parenthesesDepth = 0;

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\"') {
            inQuotes = !inQuotes;
            temp += str[i];
        } else if(str[i] == '(' && !inQuotes) {
            parenthesesDepth++;
            temp += "(";
        } else if(str[i] == ')' && !inQuotes && parenthesesDepth != 0) {
            parenthesesDepth--;
            temp += ")";
        } else if (str[i] == c && !inQuotes && parenthesesDepth == 0) {
            if (!temp.empty()) {
                result.push_back(temp);
                temp.clear();
            }
        } else {
            temp += str[i];
        }
    }

    if (!temp.empty()) {
        result.push_back(temp);
    }

    return result;
}
int countOccurencesOutsideQuotes(String s, char c) {
    bool insideQoutes = false;
    int r = 0;
    for(long long unsigned int i = 0; i < s.length(); i++) {
        if(s.at(i) == '\"' && !insideQoutes) {
            try {
                if(s.at(i - 1) != '\\') {
                    insideQoutes = true;
                }
            } catch(exception&) {
                insideQoutes = true;
            }
        } else if(s.at(i) == '\"' && insideQoutes) {
            try {
                if(s.at(i - 1) != '\\') {
                    insideQoutes = false;
                }
            } catch(exception&) {
                insideQoutes = false;
            }
        }
        if(s.at(i) == c && !insideQoutes) {
            r++;
        }
    }
    return r;
}
bool isStringOn2DVector(vector<vector<String>> v, int i, String s) {
    for(const auto& arr : v) {
        if(arr[i] == s) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::istringstream stream(input);
    std::string token;
    
    while (std::getline(stream, token, delimiter)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }

    return result;
}

void gotoxy(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}
enum IN {
  
    // 13 is ASCII for carriage
    // return
    IN_BACK = 8,
    IN_RET = 13
  
};
string takePasswdFromUser(String t)
{
    // Stores the password
    string passwd = "";
    char ch_ipt;
  
    // Until condition is true
    while (true) {
  
        ch_ipt = getch();
  
        // if the ch_ipt
        if (ch_ipt == IN::IN_RET) {
            cout << endl;
            return passwd;
        }
        else if (ch_ipt == IN::IN_BACK
                 && passwd.length() != 0) {
            passwd.pop_back();
  
            // Cout statement is very
            // important as it will erase
            // previously printed character
            cout << "\b \b";
  
            continue;
        }
  
        // Without using this, program
        // will crash as \b can't be
        // print in beginning of line
        else if (ch_ipt == IN::IN_BACK
                 && passwd.length() == 0) {
            continue;
        }
  
        passwd.push_back(ch_ipt);
        cout << t;
    }
}
String getIndexFromArgumentList(int ind, char** arr) {
    String s(arr[ind]);
    return s;
}
std::vector<std::string> splitStringUsingString(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        std::string token = input.substr(start, end - start);
        if (!token.empty()) {
            result.push_back(token);
        }

        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    std::string lastToken = input.substr(start);
    if (!lastToken.empty()) {
        result.push_back(lastToken);
    }

    return result;
}
std::string encryptText(const std::string& plainText, const std::string& key) {
    std::string encryptedText = plainText;

    for (size_t i = 0; i < plainText.size(); ++i) {
        encryptedText[i] ^= key[i % key.size()];
    }

    return encryptedText;
}
std::string replaceNthOccurrence(const std::string& original, const std::string& toFind, const std::string& replacement, size_t occurrence) {
    size_t index = 0;
    size_t count = 0;

    while ((index = original.find(toFind, index)) != std::string::npos) {
        count++;
        if (count == occurrence) {
            return original.substr(0, index) + replacement + original.substr(index + toFind.size());
        }
        index += toFind.size();
    }

    throw out_of_range("");
}
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

int findFirstIndexOutsideQuotes(const std::string& input, const std::string& target) {
    bool inQuotes = false;
    size_t inputLength = input.length();
    size_t targetLength = target.length();

    for (size_t i = 0; i <= inputLength - targetLength; ++i) {
        if (input[i] == '"' || input[i] == '\'') {
            inQuotes = !inQuotes;  // Toggle the inQuotes flag
        }

        if (!inQuotes && input.substr(i, targetLength) == target) {
            return i;  // Found the target outside quotes, return the index
        }
    }

    return -1;  // Target not found outside quotes
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isValidCharacter(char c) {
    return isAlpha(c) || isDigit(c);
}
bool isReservedKeyword(const string& n) {
    return n == "null" || n == "undefined" || n == "true" || n == "false" || n == "cmd" || n == "function" || n == "string" || n == "object" || n == "number" || n == "boolean" || n == "print" || n == "printc" || n == "in" || n == "inp" || n == "~include" || n == "msgbox";
}
bool validateVariableName(const string& n) {
    if (n.length() == 0 || n.length() > 256) {
        return false;
    }
    if (isDigit(n[0])) { // variable name should not start with a digit
        return false;
    }
    if (isReservedKeyword(n)) { // variable name should not be a reserved keyword
        return false;
    }

    bool hasAlpha = false;
    for (long long unsigned int i = 0; i < n.length(); i++) {
        if (!isValidCharacter(n[i])) {
            return false;
        }
        if (isAlpha(n[i])) {
            hasAlpha = true;
        }
    }

    if (!hasAlpha) { // variable name must contain at least one alphabetic character
        return false;
    }

    return true;
}
