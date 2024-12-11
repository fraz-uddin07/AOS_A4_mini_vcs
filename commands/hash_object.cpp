#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <openssl/sha.h>
#include <zlib.h>
#include "../include/hash_object.hpp"
#include "../include/logger.hpp"

using namespace std;
namespace fs = std::filesystem;


HashObject::HashObject() {
    // Constructor
}

HashObject::~HashObject() {
    //Destructor
}

//Function Implementation

string HashObject::execute_hash(string& file_path, bool write_enabled) {
    ifstream infile_ptr(file_path, ios::binary);
    if (!infile_ptr) {
        Logger::print_failure_to_console("Failed to open file");
        return "";
    }

    //Calculate File Size
    infile_ptr.seekg(0, ios::end);
    streamsize file_size = infile_ptr.tellg();
    infile_ptr.seekg(0, ios::beg);

    //Header
    string header = "blob " + to_string(file_size) + '\0';

    //Calculate SHA1 Hash
    char buffer[8192];
    SHA_CTX sha_ctx;
    SHA1_Init(&sha_ctx);
    SHA1_Update(&sha_ctx, header.data(), header.size());

    while(infile_ptr.read(buffer, sizeof(buffer))) {
        SHA1_Update(&sha_ctx, buffer, sizeof(buffer));
    }

    if (infile_ptr.gcount() > 0) {
        SHA1_Update(&sha_ctx, buffer, infile_ptr.gcount());
    }

    unsigned char final_hash[SHA_DIGEST_LENGTH];
    SHA1_Final(final_hash, &sha_ctx);

    ostringstream final_result;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        final_result << hex << setw(2) << setfill('0') << (int)final_hash[i];
    }

    //Compress file using zlib compression algorithm
    if (write_enabled) {
        const size_t CHUNK_SIZE = 512 * 1024;   //512KB chunks
        infile_ptr.clear();
        infile_ptr.seekg(0);
        fs::create_directory(".git/objects/" + final_result.str().substr(0, 2));
        ofstream outfile_ptr(".git/objects/" + final_result.str().substr(0, 2) + "/" + final_result.str().substr(2), ios::binary);
        if (!outfile_ptr) {
            Logger::print_failure_to_console("Failed to create output file");
            return "";
        }

        z_stream deflateStream;
        deflateStream.zalloc = Z_NULL;
        deflateStream.zfree = Z_NULL;
        deflateStream.opaque = Z_NULL;
        deflateStream.avail_in = 0;
        deflateStream.next_in = Z_NULL;

        if (deflateInit(&deflateStream, Z_BEST_COMPRESSION) != Z_OK) {
            Logger::print_failure_to_console("Unable to compress requested file");
            return "";
        }

        string compressed_data;
        compressed_data.reserve(header.size() + file_size);

        //Compress Header
        deflateStream.next_in = reinterpret_cast<Bytef*>(header.data());
        deflateStream.avail_in = header.size();

        vector<char> inputBuffer(CHUNK_SIZE);
        vector<char> outputBuffer(CHUNK_SIZE);
        int last_chunk;
        // do {
        //     infile_ptr.read(inputBuffer.data(), CHUNK_SIZE);
        //     deflateStream.avail_in = infile_ptr.gcount();
        //     deflateStream.next_in = reinterpret_cast<Bytef*>(inputBuffer.data());

        //     last_chunk = infile_ptr.eof() ? Z_FINISH : Z_NO_FLUSH;

        //     do {
        //         deflateStream.avail_out = CHUNK_SIZE;
        //         deflateStream.next_out = reinterpret_cast<Bytef*>(outputBuffer.data());
        //         deflate(&deflateStream, last_chunk);
        //         size_t compressedSize = CHUNK_SIZE - deflateStream.avail_out;
        //         outfile_ptr.write(outputBuffer.data(), compressedSize);
        //     } while (deflateStream.avail_out == 0);


        // } while (last_chunk != Z_FINISH);
        // deflateEnd(&deflateStream);
        do {
            deflateStream.avail_out = CHUNK_SIZE;
            deflateStream.next_out = reinterpret_cast<Bytef*>(outputBuffer.data());
            deflate(&deflateStream, Z_NO_FLUSH);
            compressed_data.append(outputBuffer.data(), outputBuffer.size() - deflateStream.avail_out);
            outfile_ptr.write(outputBuffer.data(), CHUNK_SIZE - deflateStream.avail_out);
        } while (deflateStream.avail_out == 0);

        //Process Chunks
         while (infile_ptr) {
            infile_ptr.read(inputBuffer.data(), inputBuffer.size());
            streamsize bytesRead = infile_ptr.gcount();
            if (bytesRead > 0) {
                // Update SHA-1 hash
                // SHA1_Update(&sha_ctx, inputBuffer.data(), bytesRead);

                // Compress file data
                deflateStream.next_in = reinterpret_cast<Bytef *>(inputBuffer.data());
                deflateStream.avail_in = bytesRead;
                do {
                    deflateStream.avail_out = outputBuffer.size();
                    deflateStream.next_out = reinterpret_cast<Bytef *>(outputBuffer.data());
                    deflate(&deflateStream, Z_NO_FLUSH);
                    compressed_data.append(outputBuffer.data(), outputBuffer.size() - deflateStream.avail_out);
                    outfile_ptr.write(outputBuffer.data(), CHUNK_SIZE - deflateStream.avail_out);
                } while (deflateStream.avail_out == 0);
            }
        }

        //Finish Compression
        deflateStream.avail_in = 0;
        deflateStream.next_in = Z_NULL;
        do {
            deflateStream.avail_out = outputBuffer.size();
            deflateStream.next_out = reinterpret_cast<Bytef *>(outputBuffer.data());
            deflate(&deflateStream, Z_FINISH);
            compressed_data.append(outputBuffer.data(), outputBuffer.size() - deflateStream.avail_out);
            outfile_ptr.write(outputBuffer.data(), CHUNK_SIZE - deflateStream.avail_out);
        } while (deflateStream.avail_out == 0);

        deflateEnd(&deflateStream);

        Logger::print_info_to_console("File Compressed successfully");
        outfile_ptr.close();
    }
    infile_ptr.close();
    return final_result.str();
}


void HashObject::hash_handler (vector<string>& commands) {
    if (commands.size() != 3 && commands.size() != 4) {
        Logger::print_failure_to_console("Invalid arguments to hash-object command");
        return;
    }
    string file_path;
    bool write_enabled = false;
    if (commands.size() == 3) {
        file_path = commands[2];
    }
    if (commands.size() == 4) {
        file_path = commands[3];
        write_enabled = true;
    }

    string sha1_hash = HashObject::execute_hash(file_path, write_enabled);
    cout << sha1_hash << endl;

    return;
}

