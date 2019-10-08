#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define KEYSTREAM_ARR_SIZE 256
#define MAX_FILE_SIZE 4900

int cipherText(char *fileIn, char *fileOut, uint8_t *S, uint8_t *out);
void initializeArrS(char password[], uint8_t *S);
int writeFile(char fileName[], char *value);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Instructions: arguments password, inputFile and outputFile must be provided.\r\n");
        return EXIT_FAILURE;
    }

    // Allocate memory for the 'ciphered' text.
    uint8_t *ciphered = malloc(sizeof(uint8_t) * MAX_FILE_SIZE);

    // Allocate memory for the keystream.
    uint8_t *keyStream = malloc(sizeof(uint8_t) * KEYSTREAM_ARR_SIZE);

    // Initialize the keystream array.
    initializeArrS(argv[1], keyStream);

    // Cipher the text of the provided file and write it to file provided in argv[3]
    cipherText(argv[2], argv[3], keyStream, ciphered);

    // Free memory again taken by ciphered and keyStream.
    free(keyStream);
    free(ciphered);

    return EXIT_SUCCESS;
}

/**
 * Use this function to initialize a keystream array.
 * The array is first filled with number 0 to 255 and
 * then swap the values in the array.
 */
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

/**
 * Use this function to cipher the provided text from the fileIn and save the output
 * in the provided fileOut.
 */
int cipherText(char *fileIn, char *fileOut, uint8_t *S, uint8_t *out) {
    printf("CIPHERING\r\n");
    FILE *fp;

    uint8_t c, encVal, K = 0, counter = 0, i = 0, j = 0, save = 0;

    fp = fopen(fileIn, "rb");
    while (1) {
        c = fgetc(fp);

        if (feof(fp)) {
//            out[counter] = K ^ '\0';
            break;
        }

        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        save = S[i];
        S[i] = S[j];
        S[j] = save;
        K = S[(S[i] + S[j]) % 256];

        encVal = K ^ c;
        out[counter++] = encVal;
    }
    fclose(fp);

    int successWrite = writeFile(fileOut, out);

    if (successWrite == EXIT_SUCCESS) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

/**
 * Use this function to write the provided value to the provided file.
 */
int writeFile(char fileName[], char *value) {
    printf("WRITING TO FILE: %s\r\n", fileName);
    FILE *fp;

    fp = fopen(fileName, "wb");
    fwrite(value, 1, strlen(value), fp);

    fclose(fp);
    return EXIT_SUCCESS;
}
