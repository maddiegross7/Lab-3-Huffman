#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024//im not sure about what size this needs to be

struct huff_node {
    struct huff_node *zero;
    struct huff_node *one;
    char *s_zero;
    char *s_one;
};

int main(int argc, char const *argv[])
{
    /* code */

    if(argc != 3){
        fprintf(stderr, "you do not have the correct number of arguments");
    }

    printf("1st file: %s\n2nd File: %s\n", argv[1], argv[2]);

    

    

    FILE *code = fopen(argv[1], "r");

    if(!code){
        fprintf(stderr, "Error opening file: %s", argv[1]);
    }

    fseek(code, 0, SEEK_END);
    
    long fileSize = ftell(code);

    char *buffer = malloc(fileSize + 1);

    rewind(code);

    size_t bytesRead = fread(buffer, 1, fileSize, code);

    fwrite(buffer, 1, bytesRead, stdout);

    char string[10001] = "";
    char bitSeq[10001] = "";

    for(size_t i = 0; i < bytesRead; i++){
        //printf("Byte %zu: %c (%d)\n", i, buffer[i], buffer[i]);
        int stringSizeCount = 0;
        while(buffer[i] != '\0'){
            string[stringSizeCount] = buffer[i];
            stringSizeCount++;
            i++;
        }
        string[stringSizeCount] = '\0';
        //printf("string?: %s\n", string);
        i++;

        int bitSeqSizeCount = 0;
        while(buffer[i] != '\0'){
            //printf("hello");
            bitSeq[bitSeqSizeCount] = buffer[i];
            bitSeqSizeCount++;
            i++;
        }
        bitSeq[bitSeqSizeCount] = '\0';
        printf("bitSeq?: %s\n", bitSeq);
    }


    return 0;
}
