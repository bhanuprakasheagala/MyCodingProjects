/*
wc - print newline, word, and byte counts for each file

Print newline, word, and byte counts for each FILE, and a total line if more than one FILE is specified.
With no FILE, or when FILE is -, read standard input.

-c, --bytes: print the byte counts
-m, --chars: print the character counts
-l, --lines: print the newline counts
--files0-from=F: read input from the files specified by NUL-terminated names in file F;
                 If F is - then read names from standard input
-L, --max-line-length: print the length of the longest line
-w, --words: print the word counts

*/

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <iomanip>

struct Counts {

    int lines;
    int words;
    int bytes;
    int chars;
    int maxLineLength;

    Counts() : lines(0), words(0), bytes(0), maxLineLength(0) {}

};

void countFile(const std::string& fileName, Counts& counts){
    std::ifstream file(fileName, std::ios::binary);

    if(!file.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << std::endl;
        return;
    }

    std::string line;
    while(std::getline(file, line)) {
        counts.lines++;
        //counts.words += std::count_if(line.begin(), line.end(), ::isspace);
        counts.bytes += line.size() + 1;  // +1 for newline character
        counts.chars += line.size() + 1;  // +1 for newline character
        if(line.size() > counts.maxLineLength) {
            counts.maxLineLength = line.size();
        }

        std::istringstream iss(line);
        std::string word;

        while(iss >> word) {
            counts.words++;
        }
    }

    file.close();
}

void printCounts(const std::string& fileName, const Counts& counts){
    std::cout << std::setw(8) << counts.lines << std::setw(8) << counts.words << std::setw(8) << counts.bytes
    <<std::setw(8) << counts.chars << std::setw(8) << counts.maxLineLength;
    if(!fileName.empty()) {
        std::cout << " " << fileName;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {

    /*if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " [OPTIONS] [FILE...]" << std::endl;
        return 1;
    }*/

    bool countBytes = false;
    bool countChars = false;
    bool countLines = false;
    bool countMaxLineLength = false;
    bool countWords = false;

    std::vector<std::string> files;
    if(argc > 1){
        for(int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if(arg == "-c" || arg == "--bytes") {
                countBytes = true;
            }
            else if(arg == "-m" || arg == "--chars") {
                countChars = true;
            }
            else if(arg == "-l" || arg == "--lines") {
                countLines = true;
            }
            else if(arg == "--files0-from") {
                // Read input from the files specified by NUL-terminated names in file F
                // If F is `-` then read names from standard input

                if(i+1 < argc) {
                    std::string fileFrom = argv[++i];
                    std::ifstream fileNames(fileFrom);

                    if(!fileNames.is_open()) {
                        std::cerr << "Error: Unable to open file " << fileFrom << std::endl;
                        return 1;
                    }

                    std::string fileName;
                    while(std::getline(fileNames, fileName, '\0')) {
                        files.push_back(fileName);
                    }

                    fileNames.close();
                }
                else {
                    std::cerr << "Error: --files0-from requires a file argument" << std::endl;
                    return 1;
                }
            }
            else if(arg == "-L" || arg == "--max-line-length") {
                countMaxLineLength = true;
            }
            else if(arg == "-w" || arg == "--words") {
                countWords = true;
            }
            else {
                // Assume it's a file name
                files.push_back(arg);
            }
        }

        if(!countBytes && !countChars && !countLines && !countMaxLineLength && !countWords) {
            // If no count option is provided, default to counting lines, words, and bytes
            countLines = countBytes = countWords = true;
        }

        if(files.empty()) {
            // Read from standard input
            Counts totalCounts;
            countFile("-", totalCounts);
            printCounts("", totalCounts);
        }
        else {
            // Read from files
            Counts totalCounts;
            for(const auto& file : files) {
                Counts counts;
                
                countFile(file, counts);
                printCounts(file, counts);
                
                totalCounts.lines += counts.lines;
                totalCounts.words += counts.words;
                totalCounts.bytes += counts.bytes;
                totalCounts.chars += counts.chars;

                if (counts.maxLineLength > totalCounts.maxLineLength) {
                    totalCounts.maxLineLength = counts.maxLineLength;
                }
            }
            if(files.size() > 1) {
                printCounts("total", totalCounts);
            }
        }
    }
    else {
        // Read from standard input
        Counts totalCounts;
        std::string line;
        while(std::getline(std::cin, line)) {
            totalCounts.lines++;
            //totalCounts.words += std::count_if(line.begin(), line.end(), ::isspace);
            totalCounts.bytes += line.size() + 1; //+1 for newline character

            if(line.size() > totalCounts.maxLineLength) {
                totalCounts.maxLineLength = line.size();
            }

            std::istringstream iss(line);
            std::string word;

            while(iss >> word) {
                totalCounts.words++;
            }
        }
        printCounts("", totalCounts);
    }
    return 0;
}