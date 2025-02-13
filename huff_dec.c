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

int getBitFromByte(int pos, unsigned char character){
    int bit = ((character >> pos) & 1);
    printf("bit: %i\n", bit);
    return bit;
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
                    //printf("current->s_one: %s\n", current->s_one);
                }else if(current->one == NULL){
                    current->one = createHuffNode();
                }else{
                    current = current->one;
                }
            }else if (buffer[i] == '0'){
                if(buffer[i+1] == '\0'){
                    current->s_zero = strdup(string);
                    //printf("current->s_zero: %s\n", current->s_zero);
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


    FILE *encoded = fopen(argv[2], "rb");

    if(!code){
        fprintf(stderr, "Error opening file: %s", argv[2]);
    }

    fseek(encoded, 0, SEEK_END);
    fileSize = ftell(encoded);
    unsigned char *diffBuffer = malloc(fileSize + 1);

    rewind(encoded);

    bytesRead = fread(diffBuffer, 1, fileSize, encoded);
    diffBuffer[bytesRead] = '\0';

    current = root;
    //printf("bytes read for 2nd file: %zu\n", bytesRead);

    for(size_t i = 0; i < bytesRead; i++){
        //printf("hey we are in for loop\n");
        printf("byte: %d\n", (unsigned char)diffBuffer[i]);
        for(int j = 0; j < 8; j++){
            int singleBit = getBitFromByte(j, buffer[i]);
            if(singleBit == 1){
                //printf("are we in the 1 if?\n");
                if(current->s_one != NULL){
                    printf("%s", current->s_one);
                    current = root;
                }else{
                    current = current->one;
                }
            }else if(singleBit == 0){
                //printf("are we in the 0 if?\n");
                if(current->s_zero != NULL){
                    printf("%s", current->s_zero);
                    current = root;
                }else{
                    current = current->zero;
                }
            }
        }
    }
    return 0;
}
