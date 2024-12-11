# AOS Assignment 4 - Mini Version Control System (VCS)

## Overview

This project implements a mini version control system (VCS) that mimics basic functionalities of Git. The system allows users to add files, commit changes, and checkout specific commits, helping to understand the core concepts of version control systems and how they manage file changes over time.

## Requirements

### 1. Initialize Repository

- **Command**: `./mygit init`
- **Purpose**: Initializes a new repository by creating a hidden directory called `.mygit`.
- **Implementation**:
  - Create a `.mygit` directory.
  - Set up necessary structures to store objects, metadata, and references.

### 2. Hash Object

- **Command**: `./mygit hash-object [-w] <file>`
- **Purpose**: Computes the SHA-1 hash of a file, compresses it, and stores it as a blob object in the repository if `-w` is used.
- **Implementation**:
  - If `-w` is specified, write the file as a blob object to the repository.
  - Print the computed SHA-1 hash.

### 3. Cat-File

- **Command**: `./mygit cat-file <flag> <file_sha>`
- **Purpose**: Displays the content or metadata of a file stored as an object using its SHA-1 hash.
- **Implementation**:
  - Implement flags: `-p` (print content), `-s` (display size in bytes), `-t` (show type- either "tree" or "blob").

### 4. Write Tree

- **Command**: `./mygit write-tree`
- **Purpose**: Writes the current directory structure to a tree object.
- **Implementation**:
  - Capture the current directory's state and create a tree object.

### 5. List Tree (ls-tree)

- **Command**: `./mygit ls-tree [--name-only] <tree_sha>`
- **Purpose**: Lists the contents of a tree object using its SHA-1 hash.
- **Implementation**:
  - If `--name-only` is provided, show only names.
  - Without flags, display detailed information about each entry.

### 6. Add Files

- **Command**: `./mygit add .` or `./mygit add <file1> <file2> ... <fileN>`
- **Purpose**: Adds files or directories to the staging area for the next commit.
- **Implementation**:
  - Stage all files or specific files while skipping the `.mygit` directory.

### 7. Commit Changes

- **Command**: `./mygit commit -m "Commit message"` or `./mygit commit`
- **Purpose**: Creates a commit object representing a snapshot of the staged changes.
- **Implementation**:
  - Store a reference to the tree object.
  - Update HEAD to point to the new commit and display the commit SHA.

### 8. Log Command

- **Command**: `./mygit log`
- **Purpose**: Displays the commit history from the latest to the oldest.
- **Implementation**:
  - Read commit SHAs from a log file and display metadata for each commit.

### 9. Checkout Command

- **Command**: `./mygit checkout <commit_sha>`
- **Purpose**: Restores the project state to that of a specific commit.
- **Implementation**:
  - Extract tree and blob objects from the specified commit SHA.

## Approach

1. **Setup**: Use C++ with the filesystem library to manage directories and files.
2. **SHA-1 Hashing**: Implement SHA-1 hashing using the OpenSSL library.
3. **Compression**: Use zlib for compressing and decompressing file content.
4. **File Handling**: Manage file I/O for reading and writing to the repository.
5. **Data Structures**: Utilize C++ STL data structures for managing in-memory objects.
6. **Error Handling**: Implement robust error handling for invalid inputs and operations.

## Running the Program

To get started with the mini VCS:

1. Run the `Makefile` provided in the root directory to build the executable `mygit` by typing :-
    ```
    make
    ```
- This will compile all the source files and generate an executable.
2. Execute the VCS system by typing :- 
    ```
    ./mygit <command> <flags>
    ```

## Conclusion

This project provides a foundational understanding of how version control systems operate, focusing on file management, hashing, and commit history tracking. It serves as a stepping stone for deeper learning in software development and version control practices.
