RSA Encryption/Decryption Project
Overview
This project is a simple RSA encryption and decryption tool implemented in C using the OpenSSL library. It can handle messages up to 5000 characters, splitting them into 245-byte chunks for encryption and decryption.

encrypt.c: Encrypts a user-entered message using RSA and saves the encrypted data, public key, and private key to a user-specified directory. Encrypted data is also displayed in hexadecimal format.
decrypt.c: Decrypts the encrypted data using the private key and outputs the original message.

Features

Processes messages up to 5000 characters, split into 245-byte chunks.
Dynamically saves files (public.pem, private.pem, encrypted.bin) to a directory specified by the user at runtime.
Supports non-ASCII characters (e.g., Korean) with UTF-8 encoding.
Outputs encrypted data in hexadecimal format for debugging purposes.

Requirements

OpenSSL 3.0 or higher: Required for RSA encryption and decryption.
Windows Environment: Code is modified to avoid Windows-specific functions, though console settings are Windows-based.
C Compiler: Visual Studio or a C compiler supporting OpenSSL (e.g., MinGW).

Setup
1. Install OpenSSL

Download and install OpenSSL for Windows (e.g., from Win64 OpenSSL).
Install to C:\OpenSSL-Win64 (modify settings if the path differs).
For Visual Studio:
Include Directories: Add C:\OpenSSL-Win64\include (Project → Properties → C/C++ → General → Additional Include Directories).
Library Directories: Add C:\OpenSSL-Win64\lib (Project → Properties → Linker → General → Additional Library Directories).
Additional Dependencies: Add libcrypto.lib (Project → Properties → Linker → Input → Additional Dependencies).
Copy libcrypto-*.dll (e.g., libcrypto-3-x64.dll) to the executable directory (e.g., C:\Path\To\Your\Project\x64\Debug).


Set the platform to x64.

2. Compile the Code

Clone or download this repository.
Compile encrypt.c and decrypt.c:
Using Visual Studio: Add both files to the project and build.
Using a command-line compiler (e.g., MinGW):gcc -o encrypt.exe encrypt.c -I"C:\OpenSSL-Win64\include" -L"C:\OpenSSL-Win64\lib" -lcrypto
gcc -o decrypt.exe decrypt.c -I"C:\OpenSSL-Win64\include" -L"C:\OpenSSL-Win64\lib" -lcrypto




Verify that encrypt.exe and decrypt.exe are generated in the project directory.

Usage
1. Run encrypt.exe

Run encrypt.exe from the command prompt (CMD):encrypt.exe


Enter the directory to save files when prompted:Enter the directory to save files (e.g., C:\Path\To\Directory):

Example input: C:\Users\YourUsername\Documents\RSAFiles
Enter the message to encrypt (max 4999 characters):Enter the message to encrypt (max 4999 characters):

Example input: hello world
The program performs the following:
Generates and saves the public key and private key (public.pem, private.pem).
Encrypts the message and saves it to encrypted.bin.
Displays the encrypted data in hexadecimal format.



Example Output:
Enter the directory to save files (e.g., C:\Path\To\Directory):
C:\Users\YourUsername\Documents\RSAFiles
Enter the message to encrypt (max 4999 characters):
hello world
Saving public key to: C:\Users\YourUsername\Documents\RSAFiles\public.pem
Saving private key to: C:\Users\YourUsername\Documents\RSAFiles\private.pem
Saving encrypted data to: C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin
Total number of chunks: 1
Chunk 1 (length: 256):
84 5D 80 98 4B A3 7D 46 62 B7 A2 33 AE 42 A7 15 ...
Encrypted data with header saved to C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin

2. Run decrypt.exe

Run decrypt.exe from the command prompt (CMD):decrypt.exe


Enter the directory where the files are located:Enter the directory where files are located (e.g., C:\Path\To\Directory):

Example input: C:\Users\YourUsername\Documents\RSAFiles
The program performs the following:
Reads the private key (private.pem) and encrypted data (encrypted.bin).
Decrypts the data and outputs the original message.



Example Output:
Enter the directory where files are located (e.g., C:\Path\To\Directory):
C:\Users\YourUsername\Documents\RSAFiles
Attempting to read private key from: C:\Users\YourUsername\Documents\RSAFiles\private.pem
Attempting to read encrypted data from: C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin
Total number of chunks: 1
Decrypted message: hello world

Notes
Directory Specification

Both encrypt.exe and decrypt.exe prompt the user to specify the directory for saving or reading files. Ensure:
The same directory is used for both programs.
The specified directory has read/write permissions.
Use backslashes in the path (e.g., C:\Path\To\Directory) for Windows compatibility.



Korean/Non-ASCII Support

This program supports non-ASCII characters (e.g., Korean) with UTF-8 encoding.
If the decrypted message appears garbled (e.g., ????) in CMD:
Run the following command before executing the program:chcp 65001


Or modify CMD settings:
Right-click the CMD window → "Properties" → "Options" tab → Set "Current Code Page" to "UTF-8 (65001)".


Alternatively, redirect output to a file:decrypt.exe > output.txt





Troubleshooting

"Failed to open private.pem" or "Failed to open encrypted.bin" Error:
Verify that the directory entered in decrypt.exe matches the one used in encrypt.exe.
Check that private.pem and encrypted.bin exist in the specified directory.
Ensure the directory has read/write permissions.


OpenSSL Errors:
Confirm that OpenSSL is correctly installed and linked.
Verify that libcrypto-*.dll is in the executable directory or system PATH.



License
This project is distributed under the MIT License. See the LICENSE file for details.
Contribution
Contributions are welcome! Please submit pull requests or open issues for suggestions, bug reports, or improvements.
