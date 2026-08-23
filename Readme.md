# File Encryptor

A C++ command-line tool that recursively encrypts or decrypts every file in a given directory using a key stored in a local `.env` file.

## Overview

File Encryptor walks a target directory tree, and for every regular file it finds, applies a byte-wise transformation (encryption or decryption) based on an integer key. Tasks are queued and executed through a simple task-management layer (`ProcessManagement`), and each file is read/written in place through a shared `IO` abstraction.

> **Note:** The current cipher is a simple additive (Caesar-style) byte shift, not a cryptographically secure algorithm. It's suitable for learning/demo purposes, not for protecting sensitive data. See [Known Limitations](#known-limitations).

## Project Structure

```
file encryptor/
├── main.cpp                          # Entry point — walks directory, queues tasks
├── makefile                          # Build rules for both targets
├── .env                              # Stores the integer encryption key
├── src/
│   └── app/
│       ├── encryptDecrypt/
│       │   ├── Cryption.hpp/.cpp     # Core encrypt/decrypt byte transform
│       │   └── CryptionMain.cpp      # Standalone entry point for the cryption binary
│       ├── FileHandling/
│       │   ├── IO.hpp/.cpp           # fstream wrapper for opening files
│       │   └── ReadEnv.cpp           # Reads the key from .env
│       └── processes/
│           ├── Task.hpp              # Represents a single file + action
│           └── ProcessManagement.hpp/.cpp  # Queues and runs tasks sequentially
└── test/
    └── test1.txt                     # Sample file for manual testing
```

## Requirements

- A C++17-compatible compiler (`g++` recommended)
- `make`
- Linux/macOS (uses `std::filesystem`; paths are case-sensitive)

## Building

```bash
make
```

This produces two binaries:

- `encrypt_decrypt` — the main interactive tool, built from `main.cpp` and its dependencies
- `cryption` — a standalone binary that runs the encrypt/decrypt transform on a single task string, built from `CryptionMain.cpp`

To clean build artifacts:

```bash
make clean
```

## Usage

### Main tool

```bash
./encrypt_decrypt
```

You'll be prompted for:

1. The directory path to process
2. The action to perform (`ENCRYPT` or `DECRYPT`)

The tool then recursively walks the directory, queues every regular file it finds as a task, and processes the queue.

### Standalone cryption binary

```bash
./cryption "<file_path>,<ENCRYPT|DECRYPT>"
```

Runs the same byte transform directly against a single file, given a comma-separated task string.

### Setting the key

The encryption/decryption key is read from `.env` in the working directory as a plain integer:

```
5962
```

## How it works

1. `main.cpp` validates the target directory and iterates it with `std::filesystem::recursive_directory_iterator`.
2. Each regular file is opened via `IO`, wrapped in a `Task` (file stream + action + path), and pushed onto `ProcessManagement`'s internal queue.
3. `ProcessManagement::executeTasks()` pops tasks one at a time and calls `executeCryption()`.
4. `executeCryption()` reads the key from `.env` via `ReadEnv`, then shifts each byte of the file by that key (mod 256) for encryption, or the inverse for decryption — read and written in place, byte by byte.

## Known Limitations

- **Not cryptographically secure.** The cipher is a single-byte-key additive shift, not an authenticated modern algorithm (e.g. AES-GCM). Do not use this to protect sensitive files.
- **No integrity checking.** A corrupted or tampered file will "decrypt" into garbage silently rather than failing with an error.
- **Sequential processing only.** Despite the `ProcessManagement` name, files are processed one at a time, not in parallel.
- **Byte-by-byte file I/O**, which is slow on large files.
- **No CLI arguments** — the tool is interactive-only, which makes it hard to script.
- **No atomic writes** — if the process is interrupted mid-file, that file is left partially transformed with no automatic recovery.

## Running Tests

There is currently no automated test suite. `test/test1.txt` is provided as a sample file for manual encrypt/decrypt verification:

```bash
./encrypt_decrypt
# Enter the directory path: test/
# Enter the action: ENCRYPT
# ...then run again with DECRYPT and diff the result against a backup of the original
```
