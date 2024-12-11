
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <zlib.h>
#include <cstring>
#include "../include/cat_file.hpp"
#include "../include/logger.hpp"

using namespace std;
namespace fs = std::filesystem;

CatFile::CatFile () {
    //Constructor
}

CatFile::~CatFile () {
    //Destructor
}

//Function Implementation
void CatFile::decompress_file(string& file_path, string& type) {
    ifstream infile_ptr(file_path, ios::binary);
    if (!infile_ptr) {
        Logger::print_failure_to_console("Unable to open file for decompression");
        return;
    }

    z_stream inflateStream;
    inflateStream.zalloc = Z_NULL;
    inflateStream.zfree = Z_NULL;
    inflateStream.opaque = Z_NULL;
    inflateStream.avail_in = 0;
    inflateStream.next_in = Z_NULL;
    string decompressed_data;

    if (inflateInit(&inflateStream) != Z_OK) {
        Logger::print_failure_to_console("Failed to initialize zlib");
        return;
    }

    const size_t CHUNK_SIZE = 512 * 1024; // 512KB chunk size
    vector<char>inputBuffer(CHUNK_SIZE);
    vector<char>outputBuffer(CHUNK_SIZE);
    bool is_header_processed = false;
    string object_type;
    int file_size = 0;

    while (infile_ptr) {
            infile_ptr.read(inputBuffer.data(), inputBuffer.size());
            streamsize bytesRead = infile_ptr.gcount();

            if (bytesRead > 0) {
                // Set up input buffer for zlib
                inflateStream.avail_in = bytesRead;
                inflateStream.next_in = reinterpret_cast<Bytef *>(inputBuffer.data());

                do {
                    inflateStream.avail_out = sizeof(outputBuffer);
                    inflateStream.next_out = reinterpret_cast<Bytef *>(outputBuffer.data());

                    int ret = inflate(&inflateStream, Z_NO_FLUSH);

                    if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                        Logger::print_failure_to_console("Decompression failed");
                        inflateEnd(&inflateStream);
                        return;
                    }

                    // Calculate the decompressed size
                    streamsize decompressedBytes = sizeof(outputBuffer) - inflateStream.avail_out;

                    // Process the header (object type and size) only once
                    if (!is_header_processed) {
                        istringstream headerStream(string(outputBuffer.data(), decompressedBytes));
                        headerStream >> object_type >> file_size;
                        is_header_processed = true;

                        if (type == "-t") {
                            cout << object_type << endl;
                            inflateEnd(&inflateStream);
                            return;
                        }
                        else if (type == "-s") {
                            cout << file_size << endl;
                            inflateEnd(&inflateStream);
                            return;
                        }

                        // Skip past the header content
                        decompressedBytes -= headerStream.tellg() + 1; // +1 for null character
                        memmove(outputBuffer.data(), outputBuffer.data() + headerStream.tellg() + 1, decompressedBytes);
                    }

                    // For `-p` flag, print the decompressed file content
                    if (type == "-p" && decompressedBytes > 0) {
                        cout.write(outputBuffer.data(), decompressedBytes);
                    }
                    else {
                        Logger::print_failure_to_console("Invalid argument to cat-file command");
                        return;
                    }

                } while (inflateStream.avail_out == 0); // Continue if output buffer was filled
            }
        }
        inflateEnd(&inflateStream);
        return;
}


void CatFile::cat_file_handler (vector<string>& commands) {
    if (commands.size() != 4) {
        Logger::print_failure_to_console("Invalid Arguments to cat-file command");
        return;
    }
    int action_type = 0;
    string sha1_hash = commands[3];
    string file_path = ".git/objects/" + sha1_hash.substr(0, 2) + "/" + sha1_hash.substr(2);
    if (!fs::exists(file_path)) {
        Logger::print_failure_to_console("No such object exists");
        return;
    }
    CatFile::decompress_file(file_path, commands[2]);

    return;
}