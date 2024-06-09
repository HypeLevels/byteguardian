#pragma once
#include <cstdio>
#include <ctime>
#include <windows.h>
#include <string>

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void Error(const char* file, const char* message) {
        printf("\033[1;31m[ERROR][%s][%s]: \033[0m%s\n", getTimestamp().c_str(), getRelativePath(file).c_str(), message);
    }

    void Success(const char* file, const char* message) {
        printf("\033[1;32m[SUCCESS][%s][%s]: \033[0m%s\n", getTimestamp().c_str(), getRelativePath(file).c_str(), message);
    }

    void Warning(const char* file, const char* message) {
        printf("\033[1;33m[WARNING][%s][%s]: \033[0m%s\n", getTimestamp().c_str(), getRelativePath(file).c_str(), message);
    }

    void Info(const char* file, const char* message) {
        printf("\033[1;34m[INFO][%s][%s]: \033[0m%s\n", getTimestamp().c_str(), getRelativePath(file).c_str(), message);
    }

private:
    Logger() {
        static bool consoleCreated = false;
        if (!consoleCreated) {
            AllocConsole();
            freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

            // Enable ANSI escape code support
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            GetConsoleMode(hOut, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);

            consoleCreated = true;
        }
    }

    static std::string getRelativePath(const std::string& path) {
        std::string marker = "byteguardian\\";
        size_t pos = path.rfind(marker);
        if (pos != std::string::npos) {
            return path.substr(pos + marker.length());
        }
        return path;
    }

    static std::string getTimestamp() {
        struct tm timeInfo;
        std::time_t now = std::time(nullptr);
        char buf[100] = { 0 };
        localtime_s(&timeInfo, &now);
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);
        return buf;
    }

    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;
};
