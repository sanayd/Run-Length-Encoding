#include <iostream>
#include <fstream>
#include <cstring>

void compress(const char* file) {
    
    FILE* uncompressed = fopen(file, "r");
    if (!uncompressed) {
        std::cerr << "Error: Could not open file for reading\n";
        return;
    }

    
    std::ofstream outfile("compressed.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open output file\n";
        fclose(uncompressed);
        return;
    }

    outfile << file << '\n';

    char buf[100];
    
    while (fgets(buf, sizeof(buf), uncompressed)) {
        char cur = buf[0];
        int count = 1;
        for (int i = 1; buf[i] != '\0'; i++) {
            if (buf[i] == cur) {
                count++;
            } else {
                outfile << cur << count;
                cur = buf[i];
                count = 1;
            }
        }
        outfile << '\n';
    }

    
    outfile.close();
    fclose(uncompressed);

    if (remove(file) != 0) {
        std::cerr << "Error: Could not remove original file \n";
    }
}

// Function to decompress the file
void decompress(const char* file) {
    // Open the compressed file
    std::ifstream infile(file);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file for reading\n";
        return;
    }

    // Read the original file name from the compressed file
    std::string originalFileName;
    std::getline(infile, originalFileName);

    // Open the output file with the original file name
    std::ofstream outfile(originalFileName);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open output file\n";
        infile.close();
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        size_t i = 0;
        while (i < line.length()) {
            char ch = line[i++];
            std::string countStr;
            while (i < line.length() && std::isdigit(line[i])) {
                countStr += line[i++];
            }
            int count = std::stoi(countStr);
            for (int j = 0; j < count; j++) {
                outfile << ch;
            }
        }
        outfile << '\n';
    }

    // Close the files
    infile.close();
    outfile.close();

    // Remove the compressed file
    if (remove(file) != 0) {
        std::cerr << "Error: Could not remove compressed file\n";
    }
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <mode> <file>\n";
        std::cerr << "mode: 'c' for compress, 'd' for decompress\n";
        return 1;
    }

    char *mode = argv[1];
    char *file = argv[2];

    if (*mode == 'c') {
        compress(file);
    } else if (*mode == 'd') {
        decompress(file);
    } else {
        std::cerr << "Invalid mode. Use 'c' for compress and 'd' for decompress\n";
        return 1;
    }

    return 0;
}
