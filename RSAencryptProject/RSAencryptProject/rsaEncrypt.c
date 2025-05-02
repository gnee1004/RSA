#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#define KEY_LENGTH 2048
#define MAX_INPUT_LENGTH 5000 // 5000자로 변경
#define BASE_PATH "C:\\Users\\geunh\\source\\repos\\SharedFilesRSA\\"
#define HEADER_SIGNATURE "RSAENCv1"
#define HEADER_LENGTH 8
#define MAX_CHUNK_SIZE 245 // RSA 2048비트, PKCS1 패딩 기준 최대 245바이트

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

// 16진수로 데이터 출력
void print_hex(const unsigned char* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main() {
    EVP_PKEY* pkey = NULL;
    EVP_PKEY_CTX* ctx = NULL;
    BIO* pub_bio = NULL, * priv_bio = NULL, * enc_bio = NULL;
    unsigned char* encrypted = NULL;
    size_t encrypted_length;
    char plainText[MAX_INPUT_LENGTH];
    unsigned char chunk[MAX_CHUNK_SIZE];
    size_t chunk_size;

    // OpenSSL 초기화
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    // 사용자 입력 받기
    printf("암호화할 메시지를 입력하세요 (최대 %d자):\n", MAX_INPUT_LENGTH - 1);
    if (fgets(plainText, MAX_INPUT_LENGTH, stdin) == NULL) {
        fprintf(stderr, "입력 오류\n");
        exit(1);
    }
    size_t len = strlen(plainText);
    if (len > 0 && plainText[len - 1] == '\n') {
        plainText[len - 1] = '\0';
        len--;
    }
    if (len == 0) {
        fprintf(stderr, "입력된 메시지가 없습니다.\n");
        exit(1);
    }

    // RSA 키 생성
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0 || EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, KEY_LENGTH) <= 0) {
        handleErrors();
    }
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        handleErrors();
    }
    EVP_PKEY_CTX_free(ctx);

    // 공개키 저장
    char pub_path[] = BASE_PATH "public.pem";
    pub_bio = BIO_new_file(pub_path, "w");
    if (!pub_bio || !PEM_write_bio_PUBKEY(pub_bio, pkey)) {
        handleErrors();
    }
    BIO_free(pub_bio);

    // 개인키 저장
    char priv_path[] = BASE_PATH "private.pem";
    priv_bio = BIO_new_file(priv_path, "w");
    if (!priv_bio || !PEM_write_bio_PrivateKey(priv_bio, pkey, NULL, NULL, 0, NULL, NULL)) {
        handleErrors();
    }
    BIO_free(priv_bio);

    // 메시지 분할 및 암호화
    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx || EVP_PKEY_encrypt_init(ctx) <= 0 || EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
        handleErrors();
    }

    // 조각 개수 계산
    size_t num_chunks = (len + MAX_CHUNK_SIZE - 1) / MAX_CHUNK_SIZE;
    printf("Total number of chunks: %zu\n", num_chunks);

    // 암호화된 데이터를 파일에 저장
    char enc_path[] = BASE_PATH "encrypted.bin";
    enc_bio = BIO_new_file(enc_path, "wb");
    if (!enc_bio) {
        perror("Failed to open encrypted.bin");
        exit(1);
    }

    // 헤더 시그니처 기록
    BIO_write(enc_bio, HEADER_SIGNATURE, HEADER_LENGTH);
    // 조각 개수 기록 (4바이트)
    uint32_t num_chunks_32 = (uint32_t)num_chunks;
    BIO_write(enc_bio, &num_chunks_32, sizeof(uint32_t));

    // 각 조각 암호화 및 저장
    size_t offset = 0;
    for (size_t i = 0; i < num_chunks; i++) {
        chunk_size = (len - offset) > MAX_CHUNK_SIZE ? MAX_CHUNK_SIZE : (len - offset);
        memcpy(chunk, plainText + offset, chunk_size);

        // 암호화
        if (EVP_PKEY_encrypt(ctx, NULL, &encrypted_length, chunk, chunk_size) <= 0) {
            handleErrors();
        }
        encrypted = (unsigned char*)OPENSSL_malloc(encrypted_length);
        if (!encrypted) {
            handleErrors();
        }
        if (EVP_PKEY_encrypt(ctx, encrypted, &encrypted_length, chunk, chunk_size) <= 0) {
            handleErrors();
        }

        // 암호화된 데이터 출력 (16진수)
        printf("Chunk %zu (length: %zu):\n", i + 1, encrypted_length);
        print_hex(encrypted, encrypted_length);

        // 암호화된 데이터 파일에 저장
        BIO_write(enc_bio, encrypted, encrypted_length);
        OPENSSL_free(encrypted);
        encrypted = NULL;
        offset += chunk_size;
    }

    BIO_free(enc_bio);
    printf("Encrypted data with header saved to %s\n", enc_path);

    // 메모리 해제
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}