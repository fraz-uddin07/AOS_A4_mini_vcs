
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <openssl/sha.h>
#include <sstream>
#include <cstring>
#include <zlib.h>
#include "../include/blob_creator.hpp"
#include "../include/logger.hpp"

using namespace std;
namespace fs = std::filesystem;

BlobCreator::BlobCreator () {
    //Constructor
}

BlobCreator::~BlobCreator () {
    //Destructor
}

string BlobCreator::blob_creator (const fs::path& file_path) {
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
    
    do {
        deflateStream.avail_out = CHUNK_SIZE;
        deflateStream.next_out = reinterpret_cast<Bytef*>(outputBuffer.data());
        deflate(&deflateStream, Z_NO_FLUSH);
        // compressed_data.append(outputBuffer.data(), outputBuffer.size() - deflateStream.avail_out);
        outfile_ptr.write(outputBuffer.data(), CHUNK_SIZE - deflateStream.avail_out);
    } while (deflateStream.avail_out == 0);

    //Process Chunks
        while (infile_ptr) {
        infile_ptr.read(inputBuffer.data(), inputBuffer.size());
        streamsize bytesRead = infile_ptr.gcount();
        if (bytesRead > 0) {
            // Compress file data
            deflateStream.next_in = reinterpret_cast<Bytef *>(inputBuffer.data());
            deflateStream.avail_in = bytesRead;
            do {
                deflateStream.avail_out = outputBuffer.size();
                deflateStream.next_out = reinterpret_cast<Bytef *>(outputBuffer.data());
                deflate(&deflateStream, Z_NO_FLUSH);
                // compressed_data.append(outputBuffer.data(), outputBuffer.size() - deflateStream.avail_out);
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
        // compressed_data.append(outputBuffer.data(), outputBuffer.size() - deflateStream.avail_out);
        outfile_ptr.write(outputBuffer.data(), CHUNK_SIZE - deflateStream.avail_out);
    } while (deflateStream.avail_out == 0);

    deflateEnd(&deflateStream);

    cout << file_path.filename() <<  " : ";
    Logger::print_success_to_console("File blob created");
    outfile_ptr.close();
    infile_ptr.close();
    return final_result.str();
}


string BlobCreator::calculate_sha1_hash(const string& input_str) {
    unsigned char final_hash[SHA_DIGEST_LENGTH];

    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, input_str.c_str(), input_str.size());
    SHA1_Final(final_hash, &ctx);

    ostringstream final_result;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        final_result << hex << setw(2) << setfill('0') << (int)final_hash[i];
    }
    return final_result.str();
}


string BlobCreator::calculate_sha1_hash_file(const string& file_path) {
    ifstream infile_ptr(file_path, ios::binary);
    if (!infile_ptr) {
        cout << file_path << " : ";
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

    return final_result.str();
}

string BlobCreator::create_tree_blob(string tree_data, const string& dir_name) {
    //Header info
    string header = "tree " + to_string(tree_data.size()) + '\0';
    tree_data = header + tree_data;

    unsigned char final_hash[SHA_DIGEST_LENGTH];
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, tree_data.c_str(), tree_data.size());
    SHA1_Final(final_hash, &ctx);

    ostringstream final_result;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        final_result << hex << setw(2) << setfill('0') << (int)final_hash[i];
    }

    //Compress and Write
    unsigned long compressed_size = compressBound(tree_data.size());
    string compressed_data(compressed_size, '\0');

    int result = compress(reinterpret_cast<Bytef*>(&compressed_data[0]), &compressed_size, reinterpret_cast<Bytef*>(tree_data.data()), tree_data.size());
    if (result != Z_OK) {
        cout << dir_name << " : ";
        Logger::print_failure_to_console("Unable to create Tree Blob");
        return "";
    }
    compressed_data.resize(compressed_size);
    fs::create_directory(".git/objects/" + final_result.str().substr(0, 2));
    ofstream outfile_ptr(".git/objects/" + final_result.str().substr(0, 2) + "/" + final_result.str().substr(2), ios::binary);
    if (!outfile_ptr) {
        cout << dir_name << " : ";
        Logger::print_failure_to_console("Failed to create output file");
        return "";
    }
    outfile_ptr.write(compressed_data.data(), compressed_data.size());
    cout << dir_name << " : ";
    Logger::print_success_to_console("Tree blob created");

    return final_result.str();
}


void BlobCreator::restore_file(string& file_sha, string& file_name) {
    string file_path = ".git/objects/" + file_sha.substr(0, 2) + "/" + file_sha.substr(2);
    ifstream infile_ptr(file_path, ios::binary);
    ofstream outfile_ptr(file_name, ios::out | ios::trunc);
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

                    // if (type == "-t") {
                    //     cout << object_type << endl;
                    //     inflateEnd(&inflateStream);
                    //     return;
                    // }
                    // else if (type == "-s") {
                    //     cout << file_size << endl;
                    //     inflateEnd(&inflateStream);
                    //     return;
                    // }

                    // Skip past the header content
                    decompressedBytes -= headerStream.tellg() + 1; // +1 for null character
                    memmove(outputBuffer.data(), outputBuffer.data() + headerStream.tellg() + 1, decompressedBytes);
                }

                // For `-p` flag, print the decompressed file content
                if (decompressedBytes > 0) {
                    // cout.write(outputBuffer.data(), decompressedBytes);
                    outfile_ptr.write(outputBuffer.data(), decompressedBytes);
                }
                else {
                    Logger::print_failure_to_console("Invalid argument to cat-file command");
                    return;
                }

            } while (inflateStream.avail_out == 0); // Continue if output buffer was filled
        }
    }
    inflateEnd(&inflateStream);
    outfile_ptr.close();
    return;
}