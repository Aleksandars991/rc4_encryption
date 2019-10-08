#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define KEYSTREAM_ARR_SIZE 256
#define MAX_FILE_SIZE 20000

void cipherText(char *fileIn, char *fileOut, uint8_t *S, uint8_t *out);
void getDecryptedValue(uint8_t encrypted[], uint8_t S[], uint8_t *outDec);
void initializeArrS(char password[], uint8_t *S);
int writeFile(char fileName[], char *value);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Instructions: arguments password, inputFile and outputFile must be provided.\r\n");
        return EXIT_FAILURE;
    }
    uint8_t *ciphered = malloc(sizeof(uint8_t) * MAX_FILE_SIZE);
    uint8_t *keyStreamEnc = malloc(sizeof(uint8_t) * KEYSTREAM_ARR_SIZE);
    initializeArrS(argv[1], keyStreamEnc);
    cipherText(argv[2], argv[3], keyStreamEnc, ciphered);

    free(keyStreamEnc);
    free(ciphered);

    return EXIT_SUCCESS;
}

void initializeArrS(char password[], uint8_t *S) {
    // Initialize array S with values 0 to 255.
    for (int i = 0; i < KEYSTREAM_ARR_SIZE; i++) {
        S[i] = i;
    }

    int j = 0;
    int save = 0;

    // Swap values in array S.
    for (int i = 0; i < KEYSTREAM_ARR_SIZE; i++) {
        j = (j + S[i] + password[i % strlen(password)]) % 256;
        save = S[i];
        S[i] = S[j];
        S[j] = save;
    }
}

void cipherText(char *fileIn, char *fileOut, uint8_t *S, uint8_t *out) {
    printf("ENCRYPTING\r\n");
    FILE *fp;

    uint8_t c, encVal, K = 0, counter = 0, i = 0, j = 0, save = 0;

    fp = fopen(fileIn, "rb");
    while (1) {
        c = fgetc(fp);

        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        save = S[i];
        S[i] = S[j];
        S[j] = save;
        K = S[(S[i] + S[j]) % 256];

        if (feof(fp)) {
            out[counter] = K ^ '\0';
            break;
        }

        encVal = K ^ c;
        out[counter++] = encVal;
    }
    fclose(fp);

    writeFile(fileOut, out);
}

int writeFile(char fileName[], char *value) {
    FILE *fp;

    fp = fopen(fileName, "wb");
    fwrite(value, 1, strlen(value), fp);

    fclose(fp);
    return EXIT_SUCCESS;
}
