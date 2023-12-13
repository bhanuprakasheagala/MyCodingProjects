#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <unordered_map>

struct Counts {
    int lines;
    int words;
    int bytes;
    int chars;
    int maxLineLength;

    Counts() : lines(0), words(0), bytes(0), maxLineLength(0) {}
};

void countFile(std::istream& input, Counts& counts) {
    std::string line;
    while (std::getline(input, line)) {
        counts.lines++;
        counts.bytes += line.size() + 1;  // +1 for newline character
        counts.chars += line.size() + 1;  // +1 for newline character

        if (line.size() > counts.maxLineLength) {
            counts.maxLineLength = line.size();
        }

        std::istringstream iss(line);
        std::string word;

        while (iss >> word) {
            counts.words++;
        }
    }
}

void printCounts(const Counts& counts, bool printLines, bool printWords, bool printChars, bool printBytes,
                 bool printMaxLineLength, const std::string& fileName) {
    if (printLines) std::cout << std::setw(8) << counts.lines;
    if (printWords) std::cout << std::setw(8) << counts.words;
    if (printChars) std::cout << std::setw(16) << counts.chars;
    if (printBytes) std::cout << std::setw(8) << counts.bytes;
    if (printMaxLineLength) std::cout << std::setw(8) << counts.maxLineLength;

    if (!fileName.empty()) {
        std::cout << ' ' << fileName;
    }
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    bool countBytes = false;
    bool countChars = false;
    bool countLines = false;
    bool countMaxLineLength = false;
    bool countWords = false;

    std::unordered_map<std::string, bool*> optionFlags = {
        {"-c", &countBytes},
        {"--bytes", &countBytes},
        {"-m", &countChars},
        {"--chars", &countChars},
        {"-l", &countLines},
        {"--lines", &countLines},
        {"-L", &countMaxLineLength},
        {"--max-line-length", &countMaxLineLength},
        {"-w", &countWords},
        {"--words", &countWords},
    };

    std::vector<std::string> files;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (optionFlags.find(arg) != optionFlags.end()) {
            // Handle options
            *optionFlags[arg] = true;
        } else if (arg == "--files0-from") {
            // Handle --files0-from
            if (++i < argc) {
                std::string fileFrom = argv[i];
                std::ifstream fileNames(fileFrom);

                if (!fileNames.is_open()) {
                    std::cerr << "Error: Unable to open file " << fileFrom << '\n';
                    return 1;
                }

                std::string fileName;
                while (std::getline(fileNames, fileName, '\0')) {
                    files.push_back(fileName);
                }
            } else {
                std::cerr << "Error: --files0-from requires a file argument\n";
                return 1;
            }
        } else {
            // Assume it's a file name
            files.push_back(arg);
        }
    }

    if (files.empty() || (files.size() == 1 && files[0] == "-")) {
        // Read from standard input
        Counts totalCounts;
        countFile(std::cin, totalCounts);
        printCounts(totalCounts, countLines, countWords, countChars, countBytes, countMaxLineLength, "");
        std::cerr << "\nFinished reading from Standard input\n";
    } else {
        // Read from files
        Counts totalCounts;
        for (const auto& file : files) {
            Counts counts;

            if (file == "-") {
                countFile(std::cin, counts);
            } else {
                std::ifstream inputFile(file, std::ios::binary);
                if (inputFile.is_open()) {
                    countFile(inputFile, counts);
                } else {
                    std::cerr << "Error: Unable to open file " << file << '\n';
                    return 1;
                }
            }

            printCounts(counts, countLines, countWords, countChars, countBytes, countMaxLineLength, file);

            totalCounts.lines += counts.lines;
            totalCounts.words += counts.words;
            totalCounts.bytes += counts.bytes;
            totalCounts.chars += counts.chars;

            if (counts.maxLineLength > totalCounts.maxLineLength) {
                totalCounts.maxLineLength = counts.maxLineLength;
            }
        }

        if (files.size() > 1) {
            printCounts(totalCounts, countLines, countWords, countChars, countBytes, countMaxLineLength, "total");
        }
    }

    return 0;
}