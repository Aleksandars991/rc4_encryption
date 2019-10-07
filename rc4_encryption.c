#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define KEYSTREAM_ARR_SIZE 256
#define MAX_FILE_SIZE 4900

uint8_t * getEncryptedValue(char fileName[], uint8_t S[]);
uint8_t * getDecryptedValue(uint8_t encrypted[], uint8_t S[]);
void initializeArrS(char password[], uint8_t *S);

int main(int argc, char *argv[]) {
    uint8_t *out1 = malloc(sizeof(uint8_t) * KEYSTREAM_ARR_SIZE);
    initializeArrS(argv[2], out1);
    uint8_t *output = getEncryptedValue(argv[1], out1);

    free(out1);

    printf("ENCRYPTED FILE: \r\n");
    for (unsigned int i = 0; i < 4900; i++) {
        printf("%c", output[i]);
    }
    printf("\r\n");

    uint8_t *out2 = malloc(sizeof(uint8_t) * KEYSTREAM_ARR_SIZE);
    initializeArrS(argv[2], out2);
    uint8_t *decOutput = getDecryptedValue(output, out2);

    free(out2);

    printf("DECRYPTED FILE: \r\n");
    for (unsigned int i = 0; i < 20; i++) {
        printf("%c", decOutput[i]);
    }
    printf("\r\n");
    
    return 0;
}

void initializeArrS(char password[], uint8_t *S) {
    // uint8_t S[KEYSTREAM_ARR_SIZE];

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

uint8_t * getEncryptedValue(char fileName[], uint8_t S[]) {
    printf("KEYSTREAM ARRAY: \n");
    for (int i = 0; i < KEYSTREAM_ARR_SIZE; i++) {
        printf("%d\t", S[i]);
    }
    printf("\r\n");
    printf("ENCRYPTING\r\n");
    FILE *fp;

    uint8_t c, encrypted, K = 0, counter = 0, i = 0, j = 0, save = 0;

    uint8_t arrEnc[MAX_FILE_SIZE];
    uint8_t *output = malloc(sizeof(uint8_t) * MAX_FILE_SIZE);
    output = arrEnc;

    fp = fopen(fileName, "rb");
    while (1) {
        c = fgetc(fp);
        if (feof(fp)) {
            break;
        }

        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        save = S[i];
        S[i] = S[j];
        S[j] = save;
        K = S[(S[i] + S[j]) % 256];

        encrypted = K ^ c;
        printf("Original value is: %c :: %d\t", c, c);
        printf("Key is: %d\n", K);
        arrEnc[++counter] = encrypted;
    }
    fclose(fp);

    return output;
}

uint8_t * getDecryptedValue(uint8_t encrypted[], uint8_t S[]) {
    printf("KEYSTREAM ARRAY: \n");
    for (int i = 0; i < KEYSTREAM_ARR_SIZE; i++) {
        printf("%d\t", S[i]);
    }
    printf("\r\n");
    printf("DECRYPTING\r\n");

    uint8_t decrypted = 0, i = 0, j = 0, save = 0, K = 0;

    uint8_t arrDec[20];
    uint8_t *output = malloc(sizeof(uint8_t) * 20);
    output = arrDec;

    for (int l = 1; l < 20; l++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        save = S[i];
        S[i] = S[j];
        S[j] = save;
        K = S[(S[i] + S[j]) % 256];

        decrypted = encrypted[l] ^ K;
        printf("Original value is: %c :: %d\t", decrypted, decrypted);
        printf("Key is: %d\n", K);
        arrDec[l] = decrypted;
    }

    return output;
}

//int writeFile(char fileName[], char value[]) {
//    printf("value is: %s\n", value);
//    FILE *fp;
//
//    fp = fopen(fileName, "wb");
//    fwrite(value, 1, strlen(value), fp);
//
//    fclose(fp);
//    return 0;
//}
