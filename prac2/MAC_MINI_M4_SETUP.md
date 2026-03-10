# Mac Mini (M4) Setup Guide

This guide explains how to build and run this project on a Mac mini with an M4 chip.

## 1. Install Apple command line tools

```bash
xcode-select --install
```

Verify:

```bash
xcode-select -p
```

## 2. Install Homebrew

If Homebrew is not installed:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Verify:

```bash
brew --version
```

## 3. Install project dependencies

```bash
brew install glew glfw glm
```

Verify the headers exist:

```bash
ls /opt/homebrew/opt/glew/include/GL/glew.h
ls /opt/homebrew/opt/glfw/include/GLFW/glfw3.h
```

## 4. Open project folder

```bash
cd "/Users/marcelstoltz/Library/Mobile Documents/com~apple~CloudDocs/CS/2026/UNI/S1/Graphics/practicals/COS344 Docker/workspace/prac2"
```

## 5. Build

```bash
make clean
make
```

## 6. Run

```bash
./main
```

Or:

```bash
make run
```

## 7. VS Code include errors (if you still see them)

If `GL/glew.h` is still flagged in the editor:

1. Run `clangd: Restart language server` from Command Palette.
2. If needed, run `Developer: Reload Window`.

This project already has `compile_flags.txt`, which clangd uses for include paths.

## 8. Quick diagnostics

```bash
brew --prefix
brew --prefix glew
brew --prefix glfw
clang++ -std=c++17 -I/opt/homebrew/include -I/opt/homebrew/opt/glew/include -I/opt/homebrew/opt/glfw/include -fsyntax-only main.cpp
```

## Notes

- M4 is Apple Silicon, so Homebrew paths are under `/opt/homebrew`.
- You do not need to restart macOS after installing dependencies.
