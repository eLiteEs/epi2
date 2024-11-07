// epi² v_0.202
// @author Blas Fernández
// @date 31-07-24
// @brief Nice programming language

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
#include <windows.h>
#ifdef _WIN32
    #include <windows.h>
    #include <wininet.h>
    #include <conio.h>
#else
    #include <cstdlib>  // For system() call on Linux
#endif

#include "noerrorfile.cpp"
#include "license.cpp"
#include "downloadbar-utils.cpp"

using namespace std;
typedef string String;

// ASCII colors 
// Define ASCII escape sequences for text modifications
const std::string ASCII_UNDERLINE = "\x1b[4m";   // Underlined text
const std::string ASCII_REVERSE = "\x1b[7m";     // Reverse (swap foreground and background colors)

// Define colors (optional)
const std::string ASCII_BLACK = "\x1b[30m";
const std::string ASCII_YELLOW = "\x1b[33m";
const std::string ASCII_BLUE = "\x1b[34m";
const std::string ASCII_MAGENTA = "\x1b[35m";
const std::string ASCII_CYAN = "\x1b[36m";

// Additional text modifications (optional)
const std::string ASCII_DIM = "\x1b[2m";         // Dim text
const std::string ASCII_BLINK = "\x1b[5m";       // Blinking text
const std::string ASCII_HIDDEN = "\x1b[8m";      // Hidden text
 
// Background colors (optional)
const std::string ASCII_BG_BLACK = "\x1b[40m";
const std::string ASCII_BG_RED = "\x1b[41m";
const std::string ASCII_BG_GREEN = "\x1b[42m";
const std::string ASCII_BG_YELLOW = "\x1b[43m";
const std::string ASCII_BG_BLUE = "\x1b[44m";
const std::string ASCII_BG_MAGENTA = "\x1b[45m";
const std::string ASCII_BG_CYAN = "\x1b[46m";
const std::string ASCII_BG_WHITE = "\x1b[47m";

// Define bright color variables
const std::string BRIGHT_RED = "\033[91m";
const std::string BRIGHT_GREEN = "\033[92m";
const std::string BRIGHT_YELLOW = "\033[93m";
const std::string BRIGHT_BLUE = "\033[94m";
const std::string BRIGHT_MAGENTA = "\033[95m";
const std::string BRIGHT_CYAN = "\033[96m";
const std::string BRIGHT_WHITE = "\033[97m";
std::string RESET_COLOR = "\033[0m";

// Util Functions
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
    for(int i = 0; i < s.length(); i++) {
        if(s.at(i) == '\"' && !insideQoutes) {
            try {
                if(s.at(i - 1) != '\\') {
                    insideQoutes = true;
                }
            } catch(exception) {
                insideQoutes = true;
            }
        } else if(s.at(i) == '\"' && insideQoutes) {
            try {
                if(s.at(i - 1) != '\\') {
                    insideQoutes = false;
                }
            } catch(exception) {
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

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\"') {
            inQuotes = !inQuotes;
            temp += str[i];
        } else if (str[i] == c && !inQuotes) {
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
    for(int i = 0; i < s.length(); i++) {
        if(s.at(i) == '\"' && !insideQoutes) {
            try {
                if(s.at(i - 1) != '\\') {
                    insideQoutes = true;
                }
            } catch(exception) {
                insideQoutes = true;
            }
        } else if(s.at(i) == '\"' && insideQoutes) {
            try {
                if(s.at(i - 1) != '\\') {
                    insideQoutes = false;
                }
            } catch(exception) {
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


int hdasljdsaj = 0;
bool debug = false;


bool fullDebug = false;
ofstream fileObject("epi2.debug.log");

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
COORD getCursorPosition() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return csbi.dwCursorPosition;
    } else {
        // If it fails, return (0, 0) as a fallback
        COORD zero = {0, 0};
        return zero;
    }
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

// Variables for storing variables
vector<vector<String>> strings; // String user = "user"
vector<vector<String>> numbers; // print user
vector<vector<String>> bools;
vector<vector<String>> functions;
vector<vector<String>> objects;
vector<vector<String>> files;

// Variables for utils
bool writingFunction = false;
String functionToWrite = "";
bool writingTry = false;
bool writingCatch = false;
String tryCode = "";
String catchCode = "";

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool canBeOperation(String s) {
    for(int i = 0; i < s.length(); i++) {
        if(!isDigit(s.c_str()[i]) && s.c_str()[i] != '+' && s.c_str()[i] != '-' && s.c_str()[i] != '/' && s.c_str()[i] != '*') {
            return false;
        }
    }
    return true;
}
int calculate(const std::string& expression) {
    std::stack<int> numbers;
    std::stack<char> operators;

    std::istringstream iss(expression);

    char currentChar;
    while (iss >> currentChar) {
        if (isdigit(currentChar)) {
            iss.putback(currentChar);
            int number;
            iss >> number;
            numbers.push(number);
        } else if (currentChar == '(') {
            operators.push(currentChar);
        } else if (currentChar == ')') {
            while (!operators.empty() && operators.top() != '(') {
                int operand2 = numbers.top();
                numbers.pop();
                int operand1 = numbers.top();
                numbers.pop();
                char op = operators.top();
                operators.pop();

                if (op == '+') {
                    numbers.push(operand1 + operand2);
                } else if (op == '-') {
                    numbers.push(operand1 - operand2);
                } else if (op == '*') {
                    numbers.push(operand1 * operand2);
                } else if (op == '/') {
                    numbers.push(operand1 / operand2);
                }
            }
            operators.pop();  // Pop '('
        } else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') {
            while (!operators.empty() && (operators.top() == '*' || operators.top() == '/') && currentChar != '(') {
                int operand2 = numbers.top();
                numbers.pop();
                int operand1 = numbers.top();
                numbers.pop();
                char op = operators.top();
                operators.pop();

                if (op == '+') {
                    numbers.push(operand1 + operand2);
                } else if (op == '-') {
                    numbers.push(operand1 - operand2);
                } else if (op == '*') {
                    numbers.push(operand1 * operand2);
                } else if (op == '/') {
                    numbers.push(operand1 / operand2);
                }
            }
            operators.push(currentChar);
        }
    }

    while (!operators.empty()) {
        int operand2 = numbers.top();
        numbers.pop();
        int operand1 = numbers.top();
        numbers.pop();
        char op = operators.top();
        operators.pop();

        if (op == '+') {
            numbers.push(operand1 + operand2);
        } else if (op == '-') {
            numbers.push(operand1 - operand2);
        } else if (op == '*') {
            numbers.push(operand1 * operand2);
        } else if (op == '/') {
            numbers.push(operand1 / operand2);
        }
    }

    return numbers.top();
}

void throwError(String error, String message, String& exceptionN, int line, bool onFunction = false, String functionName = "", int& line2 = hdasljdsaj, bool isOnTry = false) {
    if(fullDebug) {
        String result = "";

        if(isOnTry) {
            result += "Handled exception at line: ";
            if(onFunction) {
                result += "#" + functionName + ":" + to_string(line2) + ":" + to_string(line);
            } else {
                result += to_string(line);
            }
            result += "; Caused by: " + error;
        } else {
            result += "Unhandled exception at line: ";
            if(onFunction) {
                result += "#" + functionName + ":" + to_string(line2) + ":" + to_string(line);
            } else {
                result += to_string(line);
            }
            result += "; Caused by: " + error;
        }
    
        fileObject << result + "\n";
    }
    
    exceptionN = error;

    if(debug) {
        if(!onFunction) {
            cout << "\t" << ASCII_BOLD << ASCII_UNDERLINE << ASCII_BLACK << ASCII_BG_GREEN << line << ASCII_RESET << ASCII_WHITE << ":\n";
        } else {
            cout << "\t" << ASCII_BOLD << ASCII_UNDERLINE << ASCII_BLACK << ASCII_BG_GREEN << line << " #" << functionName << ":" << line2 << ASCII_RESET << ASCII_WHITE << ":\n";
        }
        cout << ASCII_UNDERLINE << "\t\tStrings" << ASCII_RESET << ":\n" << ASCII_RESET << ASCII_WHITE;
        for(const auto& s : strings) {
            cout << ASCII_BOLD << s[0] << ":" << ASCII_RESET << ASCII_WHITE << s[1] << "\n";
        }
        cout << ASCII_UNDERLINE << "\t\tNumbers" << ASCII_RESET << ":\n" << ASCII_RESET << ASCII_WHITE;
        for(const auto& s : numbers) {
            cout << ASCII_BOLD << s[0] << ":" << ASCII_RESET << ASCII_WHITE << s[1] << "\n";
        }
        cout << ASCII_UNDERLINE << "\t\tBooleans" << ASCII_RESET << ":\n" << ASCII_RESET << ASCII_WHITE;
        for(const auto& s : bools) {
            cout << ASCII_BOLD << s[0] << ":" << ASCII_RESET << ASCII_WHITE << s[1] << "\n";
        }
        cout << ASCII_UNDERLINE << "\t\tFunctions" << ASCII_RESET << ":\n" << ASCII_RESET << ASCII_WHITE;
        for(const auto& s : functions) {
            cout << ASCII_BOLD << s[0] << ":" << ASCII_RESET << ASCII_WHITE << s[1] << "\n";
        }
        cout << ASCII_UNDERLINE << "\t\tFiles" << ASCII_RESET << ":\n" << ASCII_RESET << ASCII_WHITE;
        for(const auto& s : files) {
            cout << ASCII_BOLD << s[0] << ":" << ASCII_RESET << ASCII_WHITE << s[1] << "\n";
        }
    }

    if(!isOnTry) {
        if(!onFunction) {
            cout << ASCII_BOLD << BRIGHT_WHITE << "Error at line [" << line << "] " << ASCII_RESET << ASCII_BOLD << ASCII_RED << error << ASCII_RESET << " - " << ASCII_RED << message << "\n" << ASCII_RESET;
        } else {
            cout << ASCII_BOLD << BRIGHT_WHITE << "Error at line [" << line << "] while running #" << functionName << ":" << line2 << ASCII_RESET << ASCII_BOLD << ASCII_RED << " " << error << ASCII_RESET << " - " << ASCII_RED << message << "\n" << ASCII_RESET;
        }
    }
}

int runC(String& command, String& returnS, String& exceptionN, int& line, bool onFunction = false, String functionName = "", int line2 = 0, String runnedFunctionAs = "", bool onTry = false);

// Util Functions for compile
void translateString(String& s, int line, String& exceptionN, bool onFunction, String functionName, int line2, bool onTry) {
    removeSpacesOutsideQuotes(s);
    String result = "";

    cout << s << endl;

    if(canBeOperation(s)) {
        result = to_string(calculate(s));
        s = result;
        return;
    }

    if(isCharOutsideQoutes(s, '+')) {
        std::vector<std::string> parts = splitOutsideQuotes(s, '+');

        for (const auto &part : parts) {
            if (part.front() == '\"' && part.back() == '\"') {
                result += part.substr(1, part.length() - 2);
            } else if(s == "true") {
                result += "true";
            } else if(s == "false") {
                result += "false";
            } else {
                try {
                    int st = stoi(part);
                    result += part;
                } catch(invalid_argument) {
                    std::string call = part;
                    bool found = false;

                    if(call.substr(0,1) == "#") {
                        String functionName = call.substr(1);

                        int i = 0;
                        
                        for(const auto& vec : functions) {
                            if(functionName == vec[0]) {
                                for(String& s : splitString(vec[1], '\n')) {
                                    String exc = "", ret = "";
                                    runC(s, ret, exc, line, true, vec[0]);
                                    
                                    if(ret != "") {
                                        result += ret;
                                    }
                                }
                                found = true;
                                continue;
                            }
                            i++;
                        }

                        if(!found) {
                            String exceptionN = "";
                            throwError("epi2.error.function.unknownfunction", "That function doesn't exist.", exceptionN, line);   
                        }
                    }

                    for (const auto &arr : strings) {
                        if (call == arr[0]) {
                            result += arr[1];
                            found = true;
                            break;
                        }
                    }
                    for (const auto &arr : numbers) {
                        if (call == arr[0]) {
                            result += arr[1];
                            found = true;
                            break;
                        }
                    }
                    for (const auto &arr : bools) {
                        if (call == arr[0]) {
                            result += arr[1];
                            found = true;
                            break;
                        }
                    }
                    for (const auto &arr : objects) {
                        if (call == arr[0]) {
                            result += arr[1];
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        result += "undefined";
                    }
                }
            }
        }
    } else {
        if (s.front() == '\"' && s.back() == '\"') {
            result += s.substr(1, s.length() - 2);
        } else if(s == "true") {
            result += "true";
        } else if(s == "false") {
            result += "false";
        } else {
            try {
                int st = stoi(s);
                result += s;
            } catch(invalid_argument) {
                        cout << "a" << endl;

                if(s.front() == '-') {
                    try {
                        cout << "a" << endl;
                        int i = stoi(s.substr(1));

                        result += s;
                    } catch(invalid_argument) {}
                }

                std::string call = s;
                bool found = false;

                if(call.substr(0,1) == "#") {
                    String functionName = call.substr(1);

                    int i = 0;
                    
                    for(const auto& vec : functions) {
                        if(functionName == vec[0]) {
                            for(String& s : splitString(vec[1], '\n')) {
                                String exc = "", ret = "";
                                runC(s, ret, exc, line, true, vec[0]);
                                
                                if(ret != "") {
                                    result += ret;
                                }
                            }
                            found = true;

                            s = result;

                            return;
                        }
                        i++;
                    }
                    String exceptionN = "";
                    throwError("epi2.error.function.unknownfunction", "That function doesn't exist.", exceptionN, line);   
                }

                for (const auto &arr : strings) {
                    if (call == arr[0]) {
                        result += arr[1];
                        found = true;
                        break;
                    }
                }
                for (const auto &arr : numbers) {
                    if (call == arr[0]) {
                        result += arr[1];
                        found = true;
                        break;
                    }
                }
                for (const auto &arr : bools) {
                    if (call == arr[0]) {
                        result += arr[1];
                        found = true;
                        break;
                    }
                }
                for (const auto &arr : objects) {
                    if (call == arr[0]) {
                        result += arr[1];
                        found = true;
                        break;
                    }
                }

                for(const auto& v : files) {
                    try {
                        String fileVariableName = call.substr(0, call.find("."));

                        if(call == fileVariableName + ".read()") {
                            fstream f(v[1]);

                            String txt, line;
                            while(getline(f, line)) {
                                txt += line + "\n";
                            }

                            f.close();

                            result += txt;
                            found = true;
                        } else if(call.substr(0, fileVariableName.length() + 8) == fileVariableName + ".readli(") {
                            String args = call.substr(fileVariableName.length() + 8);
                            args.pop_back();

                            try {
                                int lineNumber = stoi(args); // This might throw invalid_argument

                                fstream f(v[1]);
                                String txtline, txtline2;
                                int i = 0;

                                while(getline(f, txtline)) {
                                    if(i == lineNumber) {
                                        txtline2 = txtline;
                                    }
                                    i++;
                                }

                                f.close();

                                if(txtline2.empty()) {
                                    throwError("epi2.error.syntax.outofbounds", "You passed the max number of a list.", exceptionN, line, onFunction, functionName, line2, onTry);
                                } else {
                                    result += txtline2;
                                    found = true;
                                }
                            } catch(invalid_argument e) {
                                throwError("epi2.error.syntax.notanumber", "The value that you entered isn't a number.", exceptionN, line, onFunction, functionName, line2, onTry);
                            }
                        }
                    } catch(exception e) {}
                }

                if (!found) {
                    result += "undefined";
                }
            }
        }
    }

    s = result;
}
bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
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
    for (int i = 0; i < n.length(); i++) {
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

// Included statements
bool dirGUIIncluded = false;

// Utils for reading files
bool exists(String filename) {
    fstream f(filename);
    bool r = f.good();
    f.close();
    return r;
}

void replaceOutsideQuotes2(std::string& input, const std::string& oldStr, const std::string& newStr) {
    bool inQuotes = false;
    std::string result;
    size_t i = 0;

    while (i < input.length()) {
        if (input[i] == '"' || input[i] == '\'') {
            inQuotes = !inQuotes;  // Toggle the inQuotes flag
            result += input[i];  // Add the quote character to the result
            ++i;
        } else if (!inQuotes && input.substr(i, oldStr.length()) == oldStr) {
            result += newStr;  // Replace the substring
            i += oldStr.length();  // Move the index forward by the length of oldStr
        } else {
            result += input[i];  // Add the current character to the result
            ++i;
        }
    }

    input = result;  // Assign the modified result back to the input string
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

String file = "";

void logThings(String command, int line, bool onFunction, String functionName, int line2, String runnedFunctionAs, bool onTry) {
    String result = "Running command: " + command + "; at line: ";
    if(onFunction) {
        result += "#" + functionName + ":" + to_string(line2) + ":" + to_string(line);

        result += "; Function runned as: " + runnedFunctionAs;
    } else {
        result += to_string(line);
    }

    result += "; On try: " + onTry;
    fileObject << result + "\n";
}

int runC(String& command, String& returnS, String& exceptionN, int& line, bool onFunction, String functionName, int line2, String runnedFunctionAs, bool onTry) {
    if(command.empty()) { return 0; }

    command = command.substr(0,command.find("\n"));

    if(fullDebug) {
        logThings(command, line, onFunction, functionName, line2, runnedFunctionAs, onTry);
    }

    if(writingFunction) {
        String s("    ");
        if(command.substr(0,s.length()) == s && functionToWrite != "") {
            command = command.substr(s.length());
            int i = 0;
            for(const auto& vec : functions) {
                if(vec[0] == functionToWrite) {
                    functions[i][1] += command + "\n";
                    return 0;
                }
                i++;
            }
            writingFunction = false;
            functionToWrite = "";
        } else {
            writingFunction = false;
            functionToWrite = "";
            runC(command, returnS, exceptionN, line);
        }
    } else if(writingTry) {
        String s("    ");
        if(command.substr(0,s.length()) == s) {
            command = command.substr(s.length());
            tryCode = command + "\n";
            return 0;
        } else {
            writingTry = false;
            if(command.substr(0, 5) == "catch") {
                catchCode = "";
                size_t start = 6;
                size_t end = command.find(":");
                std::string errorToCatch = command.substr(start, end - start);
                for(String l : splitOutsideQuotes(tryCode, '\n')) {
                    String excepN = "";
                    runC(l, returnS, excepN, line, onFunction, functionName, line2, runnedFunctionAs, true) ;
                    if(excepN == errorToCatch) {
                        writingCatch = true;
                        writingTry = false;
                        tryCode = "";
                        return 0;
                    } else if(excepN != "") {
                        runC(l, returnS, excepN, line, onFunction, functionName, line2, runnedFunctionAs) ;
                    }
                }
                return 0;
            } else {
                throwError("epi2.error.lang.nocatchaftertry", "You should put a catch statemet after a try statement.", exceptionN, line, onFunction, functionName, line2, onTry);
                return 1;
            }
        }
    } else if(writingCatch) {
        String s("    ");
        
        if(command.substr(0,s.length()) == s) {
            command = command.substr(s.length());
            
            catchCode += command +"\n";

            try {
                String nextLine = splitOutsideQuotes(file, '\n')[line];
            } catch(exception) {
                writingCatch = false;
                for(String s : splitOutsideQuotes(catchCode, '\n')) {
                    runC(s,returnS, exceptionN, line, onFunction);
                }
                catchCode = "";
                return 0;
            } 

            return 0;
        } else {
            writingCatch = false;
            for(String s : splitOutsideQuotes(catchCode, '\n')) {
                runC(s,returnS, exceptionN, line, onFunction);
            }
            catchCode = "";
            runC(command,returnS,exceptionN,line);
            return 0;
        }/* else {
            writingCatch = false;
            if(command.substr(0, 5) == "catch") {
                String errorToCatch = command.substr(5, command.find_last_of(":"));
                for(String l : splitOutsideQuotes(tryCode, '\n')) {
                    String excepN = "";
                    runC(l, returnS, excepN, line, onFunction, functionName, line2, runnedFunctionAs, true) ;
                    if(excepN == errorToCatch) {
                        writingCatch = true;
                        writingTry = false;
                        tryCode = "";
                        return 0;
                    } else if(excepN != "") {
                        runC(l, returnS, excepN, line, onFunction, functionName, line2, runnedFunctionAs) ;
                    }
                }
            } else {
                runC(command, returnS, exceptionN, line);
            }
        }*/
    } else {
        if(command.find('@') != -1) {
            command = splitOutsideQuotes(command, '@')[0];
        }
        try {
            while(findFirstIndexOutsideQuotes(command, "to_num(") != -1) {
                int pos = findFirstIndexOutsideQuotes(command, "to_num(");
                int end;

                String toReplace = "to_num(";
                String toCheck = command.substr(pos + 7, command.substr(pos + 7).find_first_of(")"));
                toReplace += toCheck + ")";

                translateString(toCheck, line, exceptionN, onFunction, functionName, line2, onTry);

                try {
                    int i = stoi(toCheck);
                    replaceOutsideQuotes2(command, toReplace, toCheck);
                } catch(invalid_argument) {
                    throwError("epi2.error.syntax.notanumber", "The value that you entered isn't a number.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.syntax.notanumber";
                    return 1;
                }
            } 
        } catch(out_of_range) {

        }

        if(onFunction) {
            if(countOccurencesOutsideQuotes(runnedFunctionAs, ' ') > 0) {
                int number = countOccurencesOutsideQuotes(runnedFunctionAs, ' ');

                std::stringstream ss;
                ss << number;
                std::string str = ss.str();
                replaceOutsideQuotes2(command, "argc", str);
            }
            while(findFirstIndexOutsideQuotes(command, "arg[") != -1) {
                int pos = findFirstIndexOutsideQuotes(command, "arg[");
                int end;

                String toReplace = "arg[";
                String toCheck = command.substr(pos + 4, command.substr(pos + 4).find_first_of("]"));
                toReplace += toCheck + "]";

                translateString(toCheck, line, exceptionN, onFunction, functionName, line2, onTry);

                try {
                    int i = stoi(toCheck) + 1;
                } catch(invalid_argument) {
                    throwError("epi2.error.syntax.notanumber", "The value that you entered isn't a number.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.syntax.notanumber";
                    return 1;
                }

                try {
                    String arg = splitOutsideQuotes(runnedFunctionAs.substr(runnedFunctionAs.find_first_of(" ") + 1), ',')[stoi(toCheck)];

                    replaceOutsideQuotes2(command, toReplace, arg);
                } catch(exception) {
                    throwError("epi2.error.syntax.outofbounds", "You passed the max number of a list.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.syntax.outofbounds";
                    return 1;    
                }
            } 
        }

        String desCommand;
        if(command.find(' ') != 0) {
            desCommand = command.substr(0, command.find(' '));
        } else {
            desCommand = command;
        }
        if(desCommand == "print") {
            // print function is for printing text into the screen
            // @example print "Hello, " + username + "!"
            // @since v_0.1
            String s = command.substr(desCommand.length() + 1);
            translateString(s, line, exceptionN, onFunction, functionName, line2, onTry);
            cout << s << "\n";
        } else if(desCommand == "printc") {
            // print function is for printing text into the screen but without new line
            // @example printc "Hello, " + username + "!"
            // @since v_0.1
            String s = command.substr(desCommand.length() + 1);
            translateString(s, line, exceptionN, onFunction, functionName, line2, onTry);
            cout << s;
        } else if(desCommand == "cmd") {
            // cmd function is for running a command line command
            // @example cmd "winver"
            // @since v_0.1
            String s = command.substr(desCommand.length() + 1);
            translateString(s, line, exceptionN, onFunction, functionName, line2, onTry);
            system(s.c_str());
        } else if(desCommand == "string") {
            // string keyword is for creating a variable for storing text
            // @example string username = "balas"
            // @since v_0.1
            String s = command.substr(desCommand.length() + 1);
            removeSpacesOutsideQuotes(s);
            if(s.find('=') != -1) {
                String varName = s.substr(0, s.find('='));
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        String varContent = s.substr(s.find('=') + 1);
                        translateString(varContent, line, exceptionN, onFunction, functionName, line2, onTry);
                        vector<String> v = {varName, varContent};
                        strings.push_back(v);
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            } else {
                String varName = s;
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        vector<String> v = {varName, "null"};
                        strings.push_back(v);
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            }
        } else if(desCommand == "number") {
            // number keyword is for creating a variable for storing a number
            // @example number age = 13
            // @since v_0.1
            String s = command.substr(desCommand.length() + 1);
            removeSpacesOutsideQuotes(s);
            if(s.find('=') != -1) {
                String varName = s.substr(0, s.find('='));
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        String varContent = s.substr(s.find('=') + 1);
                        translateString(varContent, line, exceptionN, onFunction, functionName, line2, onTry);
                        try {
                            int val = stoi(varContent);

                            vector<String> v = {varName, varContent};
                            numbers.push_back(v);
                        } catch(invalid_argument e) {
                            throwError("epi2.error.syntax.notanumber", "The value that you entered isn't a number.", exceptionN, line, onFunction, functionName, line2, onTry);
                            exceptionN = "epi2.error.syntax.notanumber";
                            return 1;
                        }
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            } else {
                String varName = s;
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        vector<String> v = {varName, "null"};
                        numbers.push_back(v);
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            }
        } else if(desCommand == "boolean") {
            // boolean keyword is for creating a variable for storing true or false
            // @example boolean man = true
            // @since v_0.1
            String s = command.substr(desCommand.length() + 1);
            removeSpacesOutsideQuotes(s);
            if(s.find('=') != -1) {
                String varName = s.substr(0, s.find('='));
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        String varContent = s.substr(s.find('=') + 1);
                        translateString(varContent, line, exceptionN, onFunction, functionName, line2, onTry);
                        if(varContent == "false" || varContent == "true") {
                            vector<String> v = {varName, varContent};
                            bools.push_back(v);
                        } else {
                            throwError("epi2.error.syntax.notaboolean", "The value that you entered isn't a boolean.", exceptionN, line, onFunction, functionName, line2, onTry);
                            exceptionN = "epi2.error.syntax.notaboolean";
                            return 1;
                        }
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            } else {
                String varName = s;
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        vector<String> v = {varName, "null"};
                        bools.push_back(v);
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            }
        } else if(desCommand == "function") {
            // function keyword is for creating a function for storing code
            // @example function helloWorld
            // @since v_0.1
            String s = command.substr(desCommand.length() + 1);
            removeSpacesOutsideQuotes(s);
            if(s.find(':') != -1) {
                String varName = s.substr(0, s.find(':'));
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        vector<String> v = {varName, ""};
                        functions.push_back(v);
                        functionToWrite = varName;
                        writingFunction = true;

                        int i = 0;
                        for(const auto& vec : functions) {
                            if(vec[0] == functionToWrite) {
                                functions[i][1] = "";
                                continue;
                            }
                            i++;
                        }
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            } else {
                String varName = s;
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        vector<String> v = {varName, ""};
                        functions.push_back(v);
                        return 0;
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            }
        } else if(desCommand.substr(0,1) == "$") {
            // $ keyword is for editing a variable
            // @example $hello "Hola"
            // @since v_0.1
            String varToWrite = desCommand.substr(1);

            int i = 0;
            for(vector<String> arr : strings) {
                if(varToWrite == arr[0]) {
                    String s = command.substr(desCommand.length() + 1);
                    translateString(s, line, exceptionN, onFunction, functionName, line2, onTry);
                    strings[i][1] = s;
                    return 0;
                }
                i++;
            }
            i = 0;
            for(vector<String> arr : numbers) {
                if(varToWrite == arr[0]) {
                    String s = command.substr(desCommand.length() + 1);
                    translateString(s, line, exceptionN, onFunction, functionName, line2, onTry);
                    try {
                        int n = stoi(s);
                        numbers[i][1] = s;
                        return 0;
                    } catch(invalid_argument) {
                        throwError("epi2.error.syntax.notanumber", "The value that you entered isn't a number.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.syntax.notanumber";
                        return 1;
                    }
                }
                i++;
            }
            for(vector<String> arr : bools) {
                if(varToWrite == arr[0]) {
                    String s = command.substr(desCommand.length() + 1);
                    translateString(s, line, exceptionN, onFunction, functionName, line2, onTry);
                    if(s == "true" || s == "false") {
                        bools[i][1] = s;
                    } else {
                        throwError("epi2.error.syntax.notaboolean", "The value that you entered isn't a boolean.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.syntax.notaboolean";
                        return 1;
                    }
                }
                i++;
            }
        } else if(command.length() >= 1 && command.substr(0,1) == "@") {
            return 0;
        } else if(desCommand.length() >= 1 && desCommand.substr(0,1) == "#") {
            int i = 0;
            line2 = 1;
            
            for(const auto& vec : functions) {
                if(desCommand == "#" + vec[0]) {
                    for(String& s : splitString(vec[1], '\n')) {
                        String exc = "", ret = "";
                        runC(s, ret, exc, line, true, vec[0], line2, command);
                        if(exc != "") {
                            exceptionN = exc;
                            return 1;
                        } else if(ret != "") {
                            returnS = ret;
                            return 0;
                        }
                        line2++;
                    }
                    return 0;
                }
                i++;
            }
            throwError("epi2.error.function.unknownfunction", "That function doesn't exist.", exceptionN, line);
            return 1;
        } else if(desCommand == "return") {
            if(command == "return") {
                return 0;
            } else {
                try {
                    String retText = command.substr(desCommand.length() + 1);
                    translateString(retText, line, exceptionN, onFunction, functionName, line2, onTry);
                    returnS = retText;
                    return 0;
                } catch(exception) {
                    return 0;
                }
            }
        } else if(desCommand == "in") {
            String s = command.substr(desCommand.length() + 1);
            int i = 0;
            for(const auto& v : strings) {
                if(v[0] == s) {
                    getline(cin, strings[i][1]);
                    return 0;
                }
                i++;
            }
            i = 0;
            for(const auto& v : numbers) {
                if(v[0] == s) {
                    String n;
                    getline(cin, n);
                    try {
                        int j = stoi(n);
                        numbers[i][1] = n;
                        return 0;
                    } catch(invalid_argument) {
                        throwError("epi2.error.syntax.notanumber", "The value that you entered isn't a number.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.syntax.notanumber";
                        return 1;
                    }
                    return 0;
                }
                i++;
            }
            throwError("epi2.error.variable.undefinedvariable", "That variable doesn't exist..", exceptionN, line, onFunction, functionName, line2, onTry);
            exceptionN = "epi2.error.variable.undefinedvariable";
            return 1;
        } else if(desCommand == "~include") {
            String s = command.substr(desCommand.length() + 1);
            if(s == "gui") {
                dirGUIIncluded = true;
            } else {
                if(exists(s)) {
                    fstream f(s);
                    String lineN, exc, ret;
                    while(getline(f, lineN)) {
                        runC(lineN, exc, ret, line);
                        if(exc != "") {
                            return 1;
                        }
                    }
                    f.good();
                } else {
                    cout << ASCII_BOLD << BRIGHT_WHITE << "[" << line << "] " << ASCII_RESET << ASCII_BOLD<< ASCII_RED << "epi2.error.lang.unknownlibrary" << ASCII_RESET << " - " << ASCII_RED << "That library doesn't exist or the file doesn't exist.\n" << ASCII_RESET;
                    exceptionN = "epi2.error.lang.unknownlibrary";
                    return 1;
                }
            }
        } else if(desCommand == "msgbox") {
            if(dirGUIIncluded) {
                if(command.find(",") != -1) {
                    String s = command.substr(desCommand.length() + 1);
                    String t = splitOutsideQuotes(s, ',')[1];
                    String m = splitOutsideQuotes(s, ',')[0];
                    translateString(t, line, exceptionN, onFunction, functionName, line2, onTry);
                    translateString(m, line, exceptionN, onFunction, functionName, line2, onTry);
                    showDialog(m, t);
                } else {
                    String s = command.substr(desCommand.length() + 1);
                    translateString(s, line, exceptionN, onFunction, functionName, line2, onTry);
                    showDialog(s, "epi2");
                }
            } else {
                cout << ASCII_BOLD << BRIGHT_WHITE << "[" << line << "] " << ASCII_RESET << ASCII_BOLD<< ASCII_RED << "epi2.error.lang.librarynotimported" << ASCII_RESET << " - " << ASCII_RED << "The library which contain the function isn't initialized.\n" << ASCII_RESET;
                exceptionN = "epi2.error.lang.librarynotimported";
                return 1;
            }
        } else if(desCommand == "inp") {
            String s = command.substr(desCommand.length() + 1);
            int i = 0;
            for(const auto& v : strings) {
                if(v[0] == s) {
                    strings[i][1] = takePasswdFromUser("");
                    return 0;
                }
                i++;
            }
            i = 0;
            for(const auto& v : numbers) {
                if(v[0] == s) {
                    String n = takePasswdFromUser("");
                    try {
                        int j = stoi(n);
                        numbers[i][1] = n;
                        return 0;
                    } catch(invalid_argument) {
                        throwError("epi2.error.syntax.notanumber", "The value that you entered isn't a number.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.syntax.notanumber";
                        return 1;
                    }
                    return 0;
                }
                i++;
            }
            throwError("epi2.error.variable.undefinedvariable", "That variable doesn't exist..", exceptionN, line, onFunction, functionName, line2, onTry);
            exceptionN = "epi2.error.variable.undefinedvariable";
            return 1;
        } else if(desCommand == "inpc") {
            String s = command.substr(desCommand.length() + 1);
            if(s.find(" ") == -1) {
                throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line, onFunction, functionName, line2, onTry);
                exceptionN = "epi2.error.lang.missingargs";
                return 1;
            }
            String vName = splitOutsideQuotes(s, ' ')[0];
            String text = splitOutsideQuotes(s, ' ')[1];
            translateString(text, line, exceptionN, onFunction, functionName, line2, onTry);
            int i = 0;
            for(const auto& v : strings) {
                if(v[0] == vName) {
                    strings[i][1] = takePasswdFromUser(text);
                    return 0;
                }
                i++;
            }
            i = 0;
            for(const auto& v : numbers) {
                if(v[0] == vName) {
                    String n = takePasswdFromUser(text);
                    try {
                        int j = stoi(n);
                        numbers[i][1] = n;
                        return 0;
                    } catch(invalid_argument) {
                        throwError("epi2.error.syntax.notanumber", "The value that you entered isn't a number.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.syntax.notanumber";
                        return 1;
                    }
                    return 0;
                }
                i++;
            }
            throwError("epi2.error.variable.undefinedvariable", "That variable doesn't exist..", exceptionN, line, onFunction, functionName, line2, onTry);
            exceptionN = "epi2.error.variable.undefinedvariable";
            return 1;
        } else if(command == "try:") {
            writingTry = true;
        } else if(desCommand == "File") {
            String s = command.substr(desCommand.length() + 1);
            removeSpacesOutsideQuotes(s);
            if(s.find('(') != -1) {
                String varName = s.substr(0, s.find('('));
                if(!isStringOn2DVector(strings, 0, varName) && !isStringOn2DVector(numbers, 0, varName) && !isStringOn2DVector(bools, 0, varName) && !isStringOn2DVector(objects, 0, varName) && !isStringOn2DVector(functions, 0, varName) && !isStringOn2DVector(files, 0, varName)) {
                    if(validateVariableName(varName)) {
                        try {
                            String varContent = s.substr(s.find('(') + 1);
                            varContent = varContent.substr(0, varContent.find(')'));
                            translateString(varContent, line, exceptionN, onFunction, functionName, line2, onTry);
                            vector<String> v = {varName, varContent};
                            files.push_back(v);
                        } catch(exception) {
                            throwError("epi2.error.file.definition.missingfilename", "You should define the filename on the declaration of that variable.", exceptionN, line);
                        }
                    } else {
                        throwError("epi2.error.variable.invalidname", "You can't use that as variable name, only use characters from a to z, from A to Z, and numbers from 0 to 9 and should start with a character.", exceptionN, line, onFunction, functionName, line2, onTry);
                        exceptionN = "epi2.error.variable.invalidname";
                        return 1;
                    }
                } else {
                    throwError("epi2.error.variable.redefinition", "You can't recreate a variable with the same name.", exceptionN, line, onFunction, functionName, line2, onTry);
                    exceptionN = "epi2.error.variable.redefinition";
                    return 1; 
                }
            } else {
                throwError("epi2.error.file.definition.missingfilename", "You should define the filename on the declaration of that variable.", exceptionN, line, onFunction);
            }
        } else if(desCommand == "return") {
            String rS = command.substr(desCommand.length() + 1);
            translateString(rS, line, exceptionN, onFunction, functionName, line2, onTry);
            returnS = rS;
        } else {
            for(const auto& v : files) {
                try {
                    if(command.substr(0,v[0].length()) == v[0]) {
                        if(command.find(' ') != -1) {
                            String function = command.substr(v[0].length() + 1);
                            function = function.substr(0, function.find(" "));
                            
                            if(function == "write") {
                                fstream f(v[1]);
                                String text = "";
                                if(f.good()) {
                                    String line;
                                    while(getline(f, line)) {
                                        text += line + "\n";
                                    }
                                }
                                
                                ofstream fa(v[1]);
                                String t;
                                try {
                                    t = command.substr(v[0].length() + 1 + function.length() + 1);
                                    translateString(t, line, exceptionN, onFunction, functionName, line2, onTry);
                                } catch(out_of_range) {
                                    throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line, onFunction, functionName, line2, onTry);
                                }
                                fa << text << t;
                                fa.close();
                                return 0;
                            } else if(function == "ovwrite") {
                                ofstream f(v[1]);
                                String t;
                                try {
                                    t = command.substr(v[0].length() + 1 + function.length() + 1);
                                    translateString(t, line, exceptionN, onFunction, functionName, line2, onTry);
                                } catch(out_of_range) {
                                    throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line, onFunction, functionName, line2, onTry);
                                }
                                f << t;
                                f.close();
                                return 0;
                            }
                        } else {
                            String function = command.substr(v[0].length() + 1);

                            if(function == "delete") {
                                remove(v[1].c_str());
                                return 0;
                            }
                        }
                    }
                } catch(exception) {}
            }

            throwError("epi2.lang.unexpected.notacommand", "I think the command is wrong.", exceptionN, line, onFunction, functionName, line2, onTry);
            exceptionN = "epi2.lang.unexpected.notacommand";
            return 1;
        }
    }

    return 0;
}

// Main
int main(int argc, char** argv) {
    if(isStrOnCharArr("--version", argv, argc)) {
        cout << "epi" << (char) 253 << " v_0.202\n";
        cout << "This program is open-source software licensed with GNU GPL v3.\n";
        cout << "Run \"epi --show-license\" for reading it.\n";
        cout << "You can modify this code BUT the name of the new program should contain the \"" << (char) 253 << "\" symbol.\n";
        return 0;
    }
    if(isStrOnCharArr("--show-license", argv, argc)) {
        cout << license << "\n";
        return 0;
    }
    if(isStrOnCharArr("--debug-v", argv, argc)) {
        debug = true;
    }
    if(argc != 1) {
        String s(argv[1]);

        if(s == "install") {
            if(argc != 2) {
                String libname(argv[2]);

                if(fileExistsOnInternet("https://elitees.github.io/epi2-libraries/" + libname + "/libInfo.inf")) {
                    if(fileExistsOnInternet("https://elitees.github.io/epi2-libraries/" + libname + "/libFiles.list")) {
                        vector<String> libFiles = splitString(readFileFromInternet("https://elitees.github.io/epi2-libraries/" + libname + "/libFiles.list"), '\n');

                        system("mkdir lib");

                        for(const auto& f : libFiles) {
                            downloadFileWithProgress("https://elitees.github.io/epi2-libraries/" + libname + "/" + f, "./lib/" + f);
                        }

                        return 0;
                    } else {
                        cout << ASCII_RED << ASCII_BOLD << "Error: " << ASCII_RESET << "Library is corrupt, uneable to locate files of the library..\n";
                        return -1;
                    }
                } else {
                    cout << ASCII_RED << ASCII_BOLD << "Error: " << ASCII_RESET << "Unknown library.\n";
                    return -1;
                }
            } else {
                cout << ASCII_RED << ASCII_BOLD << "Error: " << ASCII_RESET << "No lib was specified.\n";
                return -1;
            }
        }

        
        if(isStrOnCharArr("--f-debug", argv, argc)) {
            fullDebug = true;
            time_t rawtime;
            struct tm ltm;
            time(&rawtime);

            localtime_s(&ltm, &rawtime);
            std::ostringstream ss;
            ss << std::put_time(&ltm, "%d/%m/%Y %H:%M");

            fileObject << "-------------------\nCompilation Started at: " + ss.str() + " Compiling File: " + s + "\n-------------------\n";
        } else {
            fileObject.close();
            remove("epi2.debug.log");
        }
        if(exists(s)) {
            // Open script files
            fstream f(s);
            fstream f2(s);
            String line, exc, ret, line2;

            vector<String> fileLines;

            while(getline(f2, line2)) {
                file += line2 + "\n";
            }
            while(getline(f, line)) {
                fileLines.push_back(line);
            }

            int i = 1;
            for(const auto& s : fileLines) {
                String l = s;
                runC(l, ret, exc, i);
                if(!exc.empty()) {
                    return -1;
                }
                i++;
            }

            String excep, command = "#main";
            for(int i = 2; i < argc; i++) {
                command += " \"" + getIndexFromArgumentList(i, argv) + "\"";
            }
            runC(command, ret, excep, i, false);
            if(excep == "epi2.error.function.unknownfunction") {
                throwError("epi2.error.function.unknownfunction", "You must define a main method into the file if you want to compile it.", excep, i);
                if(isStrOnCharArr("--from-explorer", argv, argc)) {
                    system("pause");
                }
                return 1;
            } else if(excep != "") {
                cout << ASCII_RED << "Exception occured from main method.\n" << ASCII_RESET;
                return 1;
            }

            if(!ret.empty() && ret != "0") {
                return 1;
            }

            f.close();

            if(isStrOnCharArr("--from-explorer", argv, argc)) {
                system("pause");
            }

            cout << ASCII_RESET;
        } else {
            cout << ASCII_BOLD << BRIGHT_RED << "Fatal Error: The file you gave doesn't exist.\n" << ASCII_RESET;
        }
    } else {
        system("title epi2");
        cout << ASCII_BG_GREEN << ASCII_BLACK << ASCII_BOLD << " * " << ASCII_RESET << " epi" << (char) 253 << " v_0.202\nEnter any epi" << (char) 253 << " command or enter \"help\" for get a list of commands.\n";
        String command = "";
        while (command != "exit()") {
            cout << " " << ASCII_BOLD << BRIGHT_WHITE << ">> " << ASCII_RESET;
            getline(cin, command);
            if(command == "exit()") {} else if(command == "help") {
                cout << "List of commands:\n";
                cout << "- \"help\" Show this list.\n";
                cout << "- \"version\" Show the current epi" << (char) 253 << " version.\n";
                cout << "- \"exit()\" Exit\n";
                cout << "Go to " << ASCII_UNDERLINE << ASCII_BOLD << ASCII_CYAN << "https://e--epi.web.app/docs/" << ASCII_RESET << " for getting a full list of epi commands.\n";
            } else if(command == "version") {
                cout << "epi" << (char) 253 << " v_0.202\n";
                cout << "This program is open-source software licensed with GNU GPL v3.\n";
                cout << "Run \"epi --show-license\" for reading it.\n";
                cout << "You can modify this code BUT the name of the new program should contain the \"" << (char) 253 << "\" symbol.\n";
            } else {
                String returnS;
                String exceptionN;
                int l = 0;
                int result = runC(command, returnS, exceptionN, l);
                if(!returnS.empty()) {
                    cout << ASCII_BOLD << BRIGHT_WHITE << " << " << ASCII_RESET << returnS << "\n";
                }
            }
        }
        cout << ASCII_RESET;
        return 0;
    }
    cout << ASCII_RESET;
    return 0;
}
