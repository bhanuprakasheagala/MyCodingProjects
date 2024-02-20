#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>

int countBytes(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    if(!file) {
        std::cerr << "Error: Cannot open file  " << filename << '\n';
        return -1;
    }
    file.seekg(0, std::ios::end);
    int byteCount = file.tellg();
    file.close();

    return byteCount;
}

int countLines(std::string filename) {
    std::ifstream file(filename);
    if(!file) {
        std::cerr << "Error: Cannot open file  " << filename << '\n';
        return -1;
    }
    int lineCount = 0;
    std::string line;
    while(std::getline(file, line)) {
        lineCount++;
    }
    file.close();

    return lineCount;
}

int countWords(std::string filename) {
    std::ifstream file(filename);
    if(!file) {
        std::cerr << "Error: Cannot open file  " << filename << '\n';
        return -1;
    }
    int wordCount = 0;
    std::string word;
    while(file >> word) {
        wordCount++;
    }
    file.close();

    return wordCount;
}

int countCharacters(std::string filename) {
    std::ifstream file(filename);
    if(!file) {
        std::cerr << "Error: Cannot open file  " << filename << '\n';
        return -1;
    }
    int charCount = 0;
    char c;
    while(file.get(c)) {
        charCount++;
    }
    file.close();

    return charCount;
}

int main(int argc, char* argv[])
{
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-c | -l | -w | -m] filename \n";
        return 1;
    }

    std::string option = argv[1];
    std::string filename;

    // Check if reading from standard input
    if(argc == 2) {
        filename = "/dev/stdin";
    }
    else {
        filename = argv[2];
    }

    int count;
    if(option == "-c") {
        count = countBytes(filename);
        std::cout << count << " " << filename << '\n';
    }
    else if(option == "-l") {
        count = countLines(filename);
        std::cout << count << " " << filename << '\n';
    }
    else if(option == "-w") {
        count = countWords(filename);
        std::cout << count << " " << filename << '\n';
    }
    else if(option == "-c") {
        count = countCharacters(filename);
        std::cout << count << " " << filename << '\n';
    }
    else {
        // Default case: equivalent to -l, -c, -w
        int lineCount = countLines(filename);
        int wordCount = countWords(filename);
        int byteCount = countBytes(filename);
        std::cout << lineCount << " " << wordCount << " " << byteCount << " " << filename << '\n';
    }

    return 0;
}