#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include "Archiver.h"

void PrintHelp () {
    std::cout << "Usage of archiver\n" <<
        "-h\tPrints this message :)\n" <<
        "-c archive_name file1 [file2 ...]\tCompresses files file1, ..., filen to archive archive_name\n" <<
        "-d archive_name\tDecompresses files from archive_name and put them into current directory\n" <<
        "\n\nMade by @baalaandaa in 2021";
}

int main(int argc, char* argv[]) {
    std::vector<std::string> arguments;
    for (int i = 1; i < argc; ++i) {
        arguments.emplace_back(argv[i]);
    }
    if(arguments.size() == 1 && arguments[0] == "-kek") {
        system("open tests/test.mp4");
        std::cout << "Nobody should see this message lol" << std::endl;
    } else if(arguments.size() >= 3 && arguments[0] == "-c") {
        const Archiver archiver;
        std::ofstream archive_ostream(arguments[1]);
        if (!archive_ostream.is_open()) {
           std::cout << "Can't open file " << arguments[1] << " for writing" << std::endl;
           return 1;
        }
        BitOutput archive_writer(archive_ostream);
        try {
            std::cout << "Opened archive" << std::endl;
            for(size_t file = 2; file < arguments.size(); ++file) {
                bool is_last_file = file + 1 == arguments.size();
                std::cout << "Processing file #" << file - 1 << ' ' << arguments[file] << std::endl;
                archiver.CompressFile(arguments[file], archive_writer, is_last_file);
            }
            archive_writer.Close();
            std::cout << "Archive generated: " << arguments[1] << std::endl;
        } catch (const std::exception& e) {
            archive_writer.Close();
            archive_ostream.close();
            std::remove(arguments[1].c_str());
            std::cout << e.what() << std::endl;
            return 0;
        }
    } else if(arguments.size() == 2 && arguments[0] == "-d") {
        const Archiver archiver;
        try {
            archiver.DecompressArchive(arguments[1]);
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return 0;
        }
    } else {
        PrintHelp();
    }
}
