#include "lineeditor.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

class MultilineEditor::Impl {
private:
    std::vector<std::string> history;
    size_t history_pos = 0;
    std::string buffer;
    size_t cursor_pos = 0;
    
    // Store configuration locally in Impl
    size_t max_history;
    bool multiline;
    
    struct Terminal {
#ifdef _WIN32
        HANDLE hStdin;
        HANDLE hStdout;
        DWORD dwSaveOldMode;
#else
        termios oldt;
#endif
        bool raw_mode = false;
        
        bool EnableRawMode();
        void DisableRawMode();
        int GetWindowWidth();
        int ReadKey();
    } terminal;

public:
    Impl(size_t max_hist, bool multi_mode);
    ~Impl();

    bool Edit(std::string& input, const std::string& prompt);
    void AddHistory(const std::string& line);
    void SaveHistory(const std::string& filename);
    void LoadHistory(const std::string& filename);

    // Configuration setters
    void SetMaxHistory(size_t max) { max_history = max; }
    void SetMultiLineMode(bool enable) { multiline = enable; }

private:
    void RefreshDisplay(const std::string& prompt);
    void InsertChar(char c);
    void DeleteChar();
    void MoveHome();
    void MoveEnd();
    void MoveLeft();
    void MoveRight();
    void NewLine();
    void HistoryPrev();
    void HistoryNext();
    void ClearScreen();
    std::vector<std::string> GetLines();
    size_t GetCursorLine();
};

// Terminal setup
bool MultilineEditor::Impl::Terminal::EnableRawMode() {
#ifdef _WIN32
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleMode(hStdin, &dwSaveOldMode)) return false;
    
    DWORD mode = dwSaveOldMode;
    mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hStdin, mode)) return false;
#else
    if (!isatty(STDIN_FILENO)) return false;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return false;
    
    termios raw = oldt;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) return false;
#endif
    raw_mode = true;
    return true;
}

void MultilineEditor::Impl::Terminal::DisableRawMode() {
    if (!raw_mode) return;
#ifdef _WIN32
    SetConsoleMode(hStdin, dwSaveOldMode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
#endif
    raw_mode = false;
}

int MultilineEditor::Impl::Terminal::GetWindowWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdout, &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return 80;
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
        return ws.ws_col;
    return 80;
#endif
}

int MultilineEditor::Impl::Terminal::ReadKey() {
#ifdef _WIN32
    INPUT_RECORD rec;
    DWORD count;
    while (ReadConsoleInput(hStdin, &rec, 1, &count)) {
        if (rec.EventType != KEY_EVENT || !rec.Event.KeyEvent.bKeyDown) continue;
        
        auto k = rec.Event.KeyEvent;
        if (k.uChar.UnicodeChar) return k.uChar.UnicodeChar;
        
        switch (k.wVirtualKeyCode) {
            case VK_UP: return 0x10FF51;
            case VK_DOWN: return 0x10FF50;
            case VK_LEFT: return 0x10FF52;
            case VK_RIGHT: return 0x10FF53;
            case VK_HOME: return 0x10FF48;
            case VK_END: return 0x10FF4B;
            case VK_DELETE: return 0x10FF56;
        }
    }
    return 0;
#else
    char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return 0;
    
    if (c == '\x1B') {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1B';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1B';
        
        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return 0x10FF51;
                case 'B': return 0x10FF50;
                case 'C': return 0x10FF53;
                case 'D': return 0x10FF52;
                case 'H': return 0x10FF48;
                case 'F': return 0x10FF4B;
                case '3':
                    if (read(STDIN_FILENO, &c, 1) == 1 && c == '~') 
                        return 0x10FF56;
                    break;
            }
        }
    }
    return c;
#endif
}

// Implementación principal corregida
MultilineEditor::MultilineEditor() : pimpl(std::make_unique<Impl>(max_history, multiline)) {}
MultilineEditor::~MultilineEditor() = default;

MultilineEditor::Impl::Impl(size_t max_hist, bool multi_mode) 
    : max_history(max_hist), multiline(multi_mode) {
    terminal.EnableRawMode();
}

MultilineEditor::Impl::~Impl() {
    terminal.DisableRawMode();
}

bool MultilineEditor::Edit(std::string& input, const std::string& prompt) {
    return pimpl->Edit(input, prompt);
}

void MultilineEditor::AddHistory(const std::string& line) {
    pimpl->AddHistory(line);
}

void MultilineEditor::SaveHistory(const std::string& filename) {
    pimpl->SaveHistory(filename);
}

void MultilineEditor::LoadHistory(const std::string& filename) {
    pimpl->LoadHistory(filename);
}

bool MultilineEditor::Impl::Edit(std::string& input, const std::string& prompt) {
    buffer = input;
    cursor_pos = buffer.length();
    history_pos = history.size();
    
    std::cout << prompt;
    RefreshDisplay(prompt);
    
    while (true) {
        int key = terminal.ReadKey();
        
        if (key == 0) continue;
        
        switch (key) {
            case '\r': case '\n': 
                NewLine();
                // CORRECCIÓN: Usar la variable local 'multiline' en lugar de la de la clase externa
                if (!multiline || (multiline && key == '\n' && GetCursorLine() == GetLines().size() - 1)) {
                    input = buffer;
                    std::cout << "\r\n";
                    return true;
                }
                break;
            case 3:  // Ctrl-C
                std::cout << "^C\r\n";
                input = buffer;
                return false;
            case 4:  // Ctrl-D
                if (buffer.empty()) {
                    std::cout << "^D\r\n";
                    return false;
                }
                break;
            case 8: case 127:
                if (cursor_pos > 0) {
                    buffer.erase(--cursor_pos, 1);
                    RefreshDisplay(prompt);
                }
                break;
            case 0x10FF56:
                if (cursor_pos < buffer.length()) {
                    buffer.erase(cursor_pos, 1);
                    RefreshDisplay(prompt);
                }
                break;
            case 0x10FF51:
                HistoryPrev();
                RefreshDisplay(prompt);
                break;
            case 0x10FF50:
                HistoryNext();
                RefreshDisplay(prompt);
                break;
            case 0x10FF52:
                MoveLeft();
                break;
            case 0x10FF53:
                MoveRight();
                break;
            case 0x10FF48:
                MoveHome();
                break;
            case 0x10FF4B:
                MoveEnd();
                break;
            case 12:
                ClearScreen();
                RefreshDisplay(prompt);
                break;
            default:
                if (key >= 32 && key <= 126) {
                    InsertChar(static_cast<char>(key));
                    RefreshDisplay(prompt);
                }
                break;
        }
    }
}

void MultilineEditor::Impl::RefreshDisplay(const std::string& prompt) {
    auto lines = GetLines();
    size_t current_line = GetCursorLine();
    
    std::cout << "\r\x1B[0K";
    std::cout << prompt << lines[0];
    
    for (size_t i = 1; i < lines.size(); ++i) {
        std::cout << "\r\n\x1B[0K" << lines[i];
    }
    
    size_t line_start = 0;
    for (size_t i = 0; i < current_line; ++i) {
        line_start += lines[i].length() + 1;
    }
    size_t col = cursor_pos - line_start;
    
    if (current_line > 0) {
        std::cout << "\x1B[" << current_line << "A";
    }
    std::cout << "\r\x1B[" << (prompt.length() + col) << "C";
    std::cout.flush();
}

std::vector<std::string> MultilineEditor::Impl::GetLines() {
    std::vector<std::string> lines;
    size_t start = 0;
    int width = terminal.GetWindowWidth() - 2;
    
    while (start < buffer.length()) {
        size_t end = start + width;
        if (end >= buffer.length()) {
            lines.push_back(buffer.substr(start));
            break;
        }
        
        size_t break_pos = buffer.rfind(' ', end);
        if (break_pos == std::string::npos || break_pos < start) {
            break_pos = end;
        }
        
        lines.push_back(buffer.substr(start, break_pos - start));
        start = break_pos + 1;
    }
    
    if (lines.empty()) lines.push_back("");
    return lines;
}

size_t MultilineEditor::Impl::GetCursorLine() {
    auto lines = GetLines();
    size_t pos = 0;
    
    for (size_t i = 0; i < lines.size(); ++i) {
        if (cursor_pos >= pos && cursor_pos <= pos + lines[i].length()) {
            return i;
        }
        pos += lines[i].length() + 1;
    }
    return lines.size() - 1;
}

void MultilineEditor::Impl::InsertChar(char c) {
    buffer.insert(cursor_pos++, 1, c);
}

void MultilineEditor::Impl::MoveLeft() {
    if (cursor_pos > 0) {
        cursor_pos--;
        RefreshDisplay("> ");
    }
}

void MultilineEditor::Impl::MoveRight() {
    if (cursor_pos < buffer.length()) {
        cursor_pos++;
        RefreshDisplay("> ");
    }
}

void MultilineEditor::Impl::MoveHome() {
    auto lines = GetLines();
    size_t current_line = GetCursorLine();
    size_t line_start = 0;
    
    for (size_t i = 0; i < current_line; ++i) {
        line_start += lines[i].length() + 1;
    }
    cursor_pos = line_start;
    RefreshDisplay("> ");
}

void MultilineEditor::Impl::MoveEnd() {
    auto lines = GetLines();
    size_t current_line = GetCursorLine();
    size_t line_start = 0;
    
    for (size_t i = 0; i <= current_line; ++i) {
        if (i == current_line) {
            cursor_pos = line_start + lines[i].length();
            break;
        }
        line_start += lines[i].length() + 1;
    }
    RefreshDisplay("> ");
}

// CORRECCIÓN: Usar la variable local 'multiline'
void MultilineEditor::Impl::NewLine() {
    if (multiline) {
        InsertChar('\n');
    }
}

void MultilineEditor::Impl::HistoryPrev() {
    if (!history.empty() && history_pos > 0) {
        if (history_pos == history.size()) {
            history.push_back(buffer);
        }
        buffer = history[--history_pos];
        cursor_pos = buffer.length();
    }
}

void MultilineEditor::Impl::HistoryNext() {
    if (!history.empty() && history_pos < history.size() - 1) {
        buffer = history[++history_pos];
        cursor_pos = buffer.length();
    }
}

void MultilineEditor::Impl::ClearScreen() {
    std::cout << "\x1B[2J\x1B[H";
}

void MultilineEditor::Impl::AddHistory(const std::string& line) {
    if (!line.empty()) {
        history.push_back(line);
        if (history.size() > max_history) {
            history.erase(history.begin());
        }
        history_pos = history.size();
    }
}

void MultilineEditor::Impl::SaveHistory(const std::string& filename) {
    // TODO: Implementar
}

void MultilineEditor::Impl::LoadHistory(const std::string& filename) {
    // TODO: Implementar
}