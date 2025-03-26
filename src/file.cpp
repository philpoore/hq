#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

// Read a file from filename or stdin to a string.
std::string read_input(const std::string &filename = "")
{
    std::ostringstream buffer;
    bool read_stdin = filename.empty() || filename == "-";

    if (!read_stdin)
    {
        std::ifstream file(filename);
        if (!file)
        {
            throw std::runtime_error("Could not open file: " + filename);
        }
        buffer << file.rdbuf();
    }
    else
    {
        // Read from stdin
        buffer << std::cin.rdbuf();
    }

    return buffer.str();
}