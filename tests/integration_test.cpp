#include <string>
#include <fstream>
#include "../Archiver.h"

TEST(Integration, test) {
    Archiver archiver;
    std::ofstream archive_ostream("test_archive");
    std::ofstream test_file("test.txt");
    std::ofstream second_test_file("test2.txt");
    test_file << "those deadlines are too crazy";
    second_test_file << "those deadlines are too crazy";
    test_file.close();
    second_test_file.close();
    BitOutput archive_writer(archive_ostream);
    archiver.CompressFile("test.txt", archive_writer, true);
    archive_writer.Close();
    archive_ostream.close();
    archiver.DecompressArchive("test_archive");
    std::ifstream first("test.txt");
    std::ifstream second("test2.txt");
    BitInput first_reader(first, false);
    BitInput second_reader(second, false);
    while (!second_reader.Eof()) {
        auto second_byte = second_reader.ReadBits(8);
        auto first_byte = first_reader.ReadBits(8);
        ASSERT_EQ(first_byte, second_byte);
        ASSERT_EQ(first_reader.Eof(), second_reader.Eof());
    }
    std::remove("test_archive");
    std::remove("test.txt");
    std::remove("test2.txt");
}