```
# RSA Encrypt/Decrypt Project

## Overview
This project provides a simple RSA encryption and decryption tool implemented in C using the OpenSSL library. It supports encrypting and decrypting messages up to 5000 characters by splitting them into 245-byte chunks.

- `encrypt.c`: Encrypts a user-provided message using RSA and saves the encrypted data, public key, and private key to a user-specified directory. The encrypted data is also displayed in hexadecimal format.
- `decrypt.c`: Decrypts the encrypted data using the private key and displays the original message.

## Features
- Supports messages up to 5000 characters by splitting them into 245-byte chunks.
- Dynamically saves files (`public.pem`, `private.pem`, `encrypted.bin`) to a directory specified by the user at runtime.
- Handles non-ASCII characters (e.g., Korean) with UTF-8 encoding.
- Displays encrypted data in hexadecimal format for debugging purposes.

## Requirements
- **OpenSSL 3.0 or higher**: Required for RSA encryption and decryption.
- **Windows environment**: The code uses Windows-specific functions (`GetModuleFileNameA` is replaced with user input, but console settings are Windows-based).
- **C Compiler**: Visual Studio or any C compiler that supports OpenSSL (e.g., MinGW).

## Setup

### 1. Install OpenSSL
- Download and install OpenSSL for Windows (e.g., from [Win64 OpenSSL](https://slproweb.com/products/Win32OpenSSL.html)).
- Install it to `C:\OpenSSL-Win64` (or adjust paths as needed).
- If using Visual Studio:
  - **Include directories**: Add `C:\OpenSSL-Win64\include` (Project → Properties → C/C++ → General → Additional Include Directories).
  - **Library directories**: Add `C:\OpenSSL-Win64\lib` (Project → Properties → Linker → General → Additional Library Directories).
  - **Additional dependencies**: Add `libcrypto.lib` (Project → Properties → Linker → Input → Additional Dependencies).
  - Copy `libcrypto-*.dll` (e.g., `libcrypto-3-x64.dll`) to your executable directory (e.g., `C:\Path\To\Your\Project\x64\Debug`).
- Set the platform to `x64`.

### 2. Compile the Code
- Clone or download this repository.
- Compile `encrypt.c` and `decrypt.c` separately:
  - Using Visual Studio: Add both files to your project and build.
  - Using a command-line compiler (e.g., MinGW):
    ```
    gcc -o encrypt.exe encrypt.c -I"C:\OpenSSL-Win64\include" -L"C:\OpenSSL-Win64\lib" -lcrypto
    gcc -o decrypt.exe decrypt.c -I"C:\OpenSSL-Win64\include" -L"C:\OpenSSL-Win64\lib" -lcrypto
    ```
- Ensure `encrypt.exe` and `decrypt.exe` are generated in your project directory.

## Usage

### 1. Run `encrypt.exe`
- Execute `encrypt.exe` in a command prompt (CMD):
  ```
  encrypt.exe
  ```
- You will be prompted to enter a directory to save the files (`public.pem`, `private.pem`, `encrypted.bin`):
  ```
  Enter the directory to save files (e.g., C:\Path\To\Directory):
  ```
  Example input: `C:\Users\YourUsername\Documents\RSAFiles`
- Enter the message to encrypt (up to 5000 characters):
  ```
  Enter the message to encrypt (max 4999 characters):
  ```
  Example input: `Hello World`
- The program will:
  - Generate and save the public and private keys (`public.pem`, `private.pem`).
  - Encrypt the message and save it to `encrypted.bin`.
  - Display the encrypted data in hexadecimal format.

**Example Output**:
```
Enter the directory to save files (e.g., C:\Path\To\Directory):
C:\Users\YourUsername\Documents\RSAFiles
Enter the message to encrypt (max 4999 characters):
Hello World
Saving public key to: C:\Users\YourUsername\Documents\RSAFiles\public.pem
Saving private key to: C:\Users\YourUsername\Documents\RSAFiles\private.pem
Saving encrypted data to: C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin
Total number of chunks: 1
Chunk 1 (length: 256):
3A 5F 8C 2D ... (256-byte hexadecimal data)
Encrypted data with header saved to C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin
```

### 2. Run `decrypt.exe`
- Execute `decrypt.exe` in a command prompt (CMD):
  ```
  decrypt.exe
  ```
- Enter the same directory where the files were saved during encryption:
  ```
  Enter the directory where files are located (e.g., C:\Path\To\Directory):
  ```
  Example input: `C:\Users\YourUsername\Documents\RSAFiles`
- The program will:
  - Read the private key (`private.pem`) and encrypted data (`encrypted.bin`).
  - Decrypt the data and display the original message.

**Example Output**:
```
Enter the directory where files are located (e.g., C:\Path\To\Directory):
C:\Users\YourUsername\Documents\RSAFiles
Attempting to read private key from: C:\Users\YourUsername\Documents\RSAFiles\private.pem
Attempting to read encrypted data from: C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin
Total number of chunks: 1
Decrypted message: Hello World
```

## Notes

### Directory Specification
- Both `encrypt.exe` and `decrypt.exe` prompt for a directory to save or read files. Ensure you:
  - Use the same directory for both programs.
  - Have read/write permissions for the specified directory.
  - Enter the path with backslashes (e.g., `C:\Path\To\Directory`), as the program runs on Windows.

### Non-ASCII Character Support (e.g., Korean)
- The program supports non-ASCII characters (e.g., Korean) using UTF-8 encoding.
- If the decrypted message contains non-ASCII characters and appears garbled in CMD (e.g., `????`):
  - Run the following command in CMD before executing the programs:
    ```
    chcp 65001
    ```
  - Alternatively, modify CMD settings:
    - Right-click CMD → "Defaults" → "Options" → Set "Current code page" to "UTF-8 (65001)".
  - If the issue persists, consider redirecting the output to a file:
    ```
    decrypt.exe > output.txt
    ```

### Troubleshooting
- **"Failed to open private.pem" or "Failed to open encrypted.bin"**:
  - Ensure the directory entered in `decrypt.exe` matches the one used in `encrypt.exe`.
  - Verify that `private.pem` and `encrypted.bin` exist in the specified directory.
  - Check for read/write permissions in the directory.
- **OpenSSL Errors**:
  - Ensure OpenSSL is correctly installed and linked.
  - Verify that `libcrypto-*.dll` is in the executable directory or system PATH.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for suggestions, bug reports, or improvements.
```

