#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct huff_node {
    struct huff_node *zero;
    struct huff_node *one;
    char *s_zero;
    char *s_one;
} Huff_node;

Huff_node* createHuffNode(){
    Huff_node *newNode = malloc(sizeof(Huff_node));

    newNode->one = NULL;
    newNode->zero = NULL;
    newNode->s_one = NULL;
    newNode->s_zero = NULL;

    return newNode;
}


int main(int argc, char const *argv[])
{
    if(argc != 3){
        fprintf(stderr, "you do not have the correct number of arguments");
    }

    FILE *code = fopen(argv[1], "r");

    if(!code){
        fprintf(stderr, "Error opening file: %s", argv[1]);
    }

    fseek(code, 0, SEEK_END);
    long fileSize = ftell(code);
    char *buffer = malloc(fileSize + 1);

    rewind(code);

    size_t bytesRead = fread(buffer, 1, fileSize, code);

    //maybe error check if file is big enough

    char string[10001] = "";

    Huff_node *root = createHuffNode();

    Huff_node *current = root;

    for(size_t i = 0; i < bytesRead; i++){
        current = root;
        int stringSizeCount = 0;
        while(buffer[i] != '\0'){
            string[stringSizeCount] = buffer[i];
            stringSizeCount++;
            i++;
        }
        string[stringSizeCount] = '\0';
        i++;

        int bitSeqSizeCount = 0;
        while(buffer[i] != '\0'){
            bitSeqSizeCount++;
            if(buffer[i] == '1'){
                if(buffer[i+1] == '\0'){
                    current->s_one = strdup(string);
                }else if(current->one == NULL){
                    current->one = createHuffNode();
                }else{
                    current = current->one;
                }
            }else if (buffer[i] == '0'){
                if(buffer[i+1] == '\0'){
                    current->s_zero = strdup(string);
                }else if(current->zero == NULL){
                    current->zero = createHuffNode();
                }else {
                    current = current->zero;
                }
            }
            i++;
        }
    }

    free(buffer);


    FILE *encoded = fopen(argv[2], "r");

    if(!code){
        fprintf(stderr, "Error opening file: %s", argv[2]);
    }

    fseek(encoded, 0, SEEK_END);
    fileSize = ftell(encoded);
    char *diffBuffer = malloc(fileSize + 1);

    rewind(encoded);

    bytesRead = fread(diffBuffer, 1, fileSize, encoded);

    for(size_t i = 0; i < bytesRead; i++){
        
    }



    return 0;
}
