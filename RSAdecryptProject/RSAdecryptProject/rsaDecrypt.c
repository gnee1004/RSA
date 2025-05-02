// decrypt.c: Decrypts RSA-encrypted data to recover the original message
// Supports up to 5000 characters (split into 245-byte chunks)
// Requires OpenSSL 3.0 or higher

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <windows.h> // For console settings

#define KEY_LENGTH 2048
#define HEADER_SIGNATURE "RSAENCv1"
#define HEADER_LENGTH 8

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

// Construct the file path using the user-specified directory
void get_file_path(char* base_path, char* buffer, size_t buffer_size, const char* filename) {
    snprintf(buffer, buffer_size, "%s\\%s", base_path, filename);
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // Set console to UTF-8 for potential non-ASCII output

    EVP_PKEY* pkey = NULL;
    EVP_PKEY_CTX* ctx = NULL;
    BIO* priv_bio = NULL, * enc_bio = NULL;
    unsigned char encrypted[KEY_LENGTH / 8] = { 0 };
    unsigned char* decrypted = NULL;
    char header[HEADER_LENGTH + 1] = { 0 };
    size_t encrypted_length, decrypted_length;
    char* final_message = NULL;
    size_t final_length = 0;
    char base_path[MAX_PATH];
    char priv_path[MAX_PATH], enc_path[MAX_PATH];

    // OpenSSL initialization
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    // Get the directory path from user
    printf("Enter the directory where files are located (e.g., C:\\Path\\To\\Directory):\n");
    if (fgets(base_path, MAX_PATH, stdin) == NULL) {
        fprintf(stderr, "Input error for directory path\n");
        exit(1);
    }
    size_t len = strlen(base_path);
    if (len > 0 && base_path[len - 1] == '\n') {
        base_path[len - 1] = '\0';
        len--;
    }
    if (len == 0) {
        fprintf(stderr, "No directory path entered.\n");
        exit(1);
    }

    // Generate file paths using user-specified directory
    get_file_path(base_path, priv_path, MAX_PATH, "private.pem");
    get_file_path(base_path, enc_path, MAX_PATH, "encrypted.bin");

    // Debug: Print file paths
    printf("Attempting to read private key from: %s\n", priv_path);
    printf("Attempting to read encrypted data from: %s\n", enc_path);

    // Read private key
    priv_bio = BIO_new_file(priv_path, "r");
    if (!priv_bio) {
        perror("Failed to open private.pem");
        exit(1);
    }
    pkey = PEM_read_bio_PrivateKey(priv_bio, NULL, NULL, NULL);
    if (!pkey) {
        handleErrors();
    }
    BIO_free(priv_bio);

    // Open encrypted data file
    enc_bio = BIO_new_file(enc_path, "rb");
    if (!enc_bio) {
        perror("Failed to open encrypted.bin");
        exit(1);
    }

    // Verify header signature
    if (BIO_read(enc_bio, header, HEADER_LENGTH) != HEADER_LENGTH) {
        fprintf(stderr, "Failed to read header signature\n");
        BIO_free(enc_bio);
        exit(1);
    }
    if (strncmp(header, HEADER_SIGNATURE, HEADER_LENGTH) != 0) {
        fprintf(stderr, "Invalid header signature\n");
        BIO_free(enc_bio);
        exit(1);
    }

    // Read number of chunks
    uint32_t num_chunks;
    if (BIO_read(enc_bio, &num_chunks, sizeof(uint32_t)) != sizeof(uint32_t)) {
        fprintf(stderr, "Failed to read number of chunks\n");
        BIO_free(enc_bio);
        exit(1);
    }
    printf("Total number of chunks: %u\n", num_chunks);

    // Decrypt
    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx || EVP_PKEY_decrypt_init(ctx) <= 0 || EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
        handleErrors();
    }

    // Decrypt each chunk and reassemble the message
    for (uint32_t i = 0; i < num_chunks; i++) {
        encrypted_length = BIO_read(enc_bio, encrypted, KEY_LENGTH / 8);
        if (encrypted_length != KEY_LENGTH / 8) {
            fprintf(stderr, "Failed to read encrypted chunk %u\n", i + 1);
            BIO_free(enc_bio);
            exit(1);
        }

        if (EVP_PKEY_decrypt(ctx, NULL, &decrypted_length, encrypted, encrypted_length) <= 0) {
            handleErrors();
        }
        decrypted = (unsigned char*)OPENSSL_malloc(decrypted_length);
        if (!decrypted) {
            handleErrors();
        }
        if (EVP_PKEY_decrypt(ctx, decrypted, &decrypted_length, encrypted, encrypted_length) <= 0) {
            handleErrors();
        }

        // Append decrypted chunk to final message
        final_message = (char*)realloc(final_message, final_length + decrypted_length + 1);
        memcpy(final_message + final_length, decrypted, decrypted_length);
        final_length += decrypted_length;
        final_message[final_length] = '\0'; // Null-terminate the string

        OPENSSL_free(decrypted);
        decrypted = NULL;
    }

    BIO_free(enc_bio);

    // Print the decrypted message
    printf("Decrypted message: %s\n", final_message);

    // Cleanup
    free(final_message);
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}