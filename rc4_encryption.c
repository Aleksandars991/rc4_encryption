#include <stdio.h>
#include <string.h>

int readFile(char fileName[]);
int writeFile(char fileName[], char value[]);

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        printf("You typed in the following argumens: \r\n");
        for (int i = 0; i < argc; i++) {
            printf("%s\r\n", argv[i]);
        }
    }

    char str[] = "A random input text string.\n";

    // TODO: input needs to be checked.
    readFile(argv[1]);
    writeFile(argv[2], str);
    
    return 0;
}

int readFile(char fileName[]) {
    printf("Filename is: %s\n", fileName);
    FILE *fp;
    // unsigned char buffer[4900] = "";
    int c;

    fp = fopen(fileName, "rb");
    printf("File successfully opened.\n");
    for(int i = 0; i < 4900; i++) {
        c = fgetc(fp);
        if (feof(fp)) {
            break;
        }
        printf("%d", c);
    }
    fclose(fp);

    return 0;
}

int writeFile(char fileName[], char value[]) {
    printf("value is: %s\n", value);
    FILE *fp;

    fp = fopen(fileName, "w");
    fwrite(value, 1, strlen(value), fp);

    fclose(fp);
    return 0;
}
