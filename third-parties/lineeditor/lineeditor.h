#ifndef MULTILINE_EDITOR_H
#define MULTILINE_EDITOR_H

#include <string>
#include <vector>
#include <memory>

class MultilineEditor {
public:
    MultilineEditor();
    ~MultilineEditor();

    // Main editing function
    bool Edit(std::string& input, const std::string& prompt = "> ");

    // History management
    void AddHistory(const std::string& line);
    void SaveHistory(const std::string& filename);
    void LoadHistory(const std::string& filename);

    // Configuration
    void SetMaxHistory(size_t max) { max_history = max; }
    void SetMultiLineMode(bool enable) { multiline = enable; }

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
    
    size_t max_history = 1000;
    bool multiline = true;
};

#endif