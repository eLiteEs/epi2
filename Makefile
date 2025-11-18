CXX = g++
CXXFLAGS = -std=c++17
LIBS = -lncurses -lcurl -lwininet
TARGET = target/epi2

ifeq ($(OS),Windows_NT)
    LIBS += -lwininet
endif

SOURCES = src/downloadbar-utils.cpp \
          src/epi.cpp \
          src/license.cpp \
          src/noerrorfile.cpp \
		  src/color.cpp \
          third-parties/linenoise-ng/linenoise.cpp \
          third-parties/linenoise-ng/ConvertUTF.cpp \
          third-parties/linenoise-ng/wcwidth.cpp \

build: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p target
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

run: build
	@$(TARGET)

debug: clean 
debug: ++FLAGS += -g -O0
debug: build
	gdb -tui ./$(TARGET)

test: $(SOURCES)
	$(CXX) $(CXXFLAGS) -fsyntax-only $(SOURCES) $(LIBS)

full-test: $(SOURCES)
	@for f in $(SOURCES); do \
		clang-tidy $$f -- -std=c++17 || exit 1; \
	done

install: build
	@printf "\033[36m→ Creating installation folder\033[0m\n"
	mkdir -p "$(HOME)/epi2"

	@printf "\033[36m→ Copying files\033[0m\n"
	cp target/epi2 "$(HOME)/epi2"
	cp LICENSE "$(HOME)/epi2"
	cp uninstall.sh "$(HOME)/epi2"

	@printf "\033[36m→ Adding epi2 to PATH\033[0m\n"
	@if ! grep -q '$(HOME)/epi2' ~/.bashrc; then \
		echo 'export PATH=$$PATH:$(HOME)/epi2' >> ~/.bashrc; \
	fi

	@printf "\033[33m! For adding epi2 to PATH you should run \"source ~/.bashrc\" manually\033[0m\n"
	@printf "\033[32m✓ Installation completed\033[0m\n"

clean:
	rm -rf target
	mkdir -p target
