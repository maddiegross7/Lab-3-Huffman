//Lab 3: Huffman Decoding
//Madelyn Gross

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//./bin/huff_dec data/celebrate-code.txt data/celebrate-encoded.txt  

typedef struct huff_node {
    struct huff_node *zero;
    struct huff_node *one;
    char *s_zero;
    char *s_one;
} Huff_node;

//just making an initialization function so that I do not have to keep initializing
Huff_node* createHuffNode(){
    Huff_node *newNode = malloc(sizeof(Huff_node));

    newNode->one = NULL;
    newNode->zero = NULL;
    newNode->s_one = NULL;
    newNode->s_zero = NULL;

    return newNode;
}

//doing the math to get a single bit from a byte
int getBitFromByte(int pos, unsigned char character){
    int bit = ((character >> pos) & 1);
    return bit;
}

//this is freeing all of the memory within the tree recursively if they exist
void freeHuffNode(Huff_node *huffNode){
    if(huffNode == NULL){
        return;
    }

    freeHuffNode(huffNode->one);
    freeHuffNode(huffNode->zero);

    if(huffNode->s_one){
        free(huffNode->s_one);
    }
    if(huffNode->s_zero){
        free(huffNode->s_zero);
    }
    
    free(huffNode);
}

int main(int argc, char const *argv[])
{
    //ensure the arguments are as expected
    if(argc != 3){
        fprintf(stderr, "you do not have the correct number of arguments");
        exit(1);
    }
    //open file and confirming that it has been opened correctly, opeing as a normal text file with the -r
    FILE *code = fopen(argv[1], "r");

    if(!code){
        fprintf(stderr, "Error opening file: %s", argv[1]);
        exit(1);
    }

    //looking for the end of the file to determine the size we need to allocate for the buffer
    fseek(code, 0, SEEK_END);
    long fileSize = ftell(code);
    char *buffer = malloc(fileSize + 1);
    //we have read it once so now we are at the end of the file this will bring us back to the begining
    rewind(code);

    size_t bytesRead = fread(buffer, 1, fileSize, code);
    //closing the file
    fclose(code);
    //the string that can be read in can be at most this long so allocating that much memory
    char string[10001] = "";

    Huff_node *root = createHuffNode();
    //beginning of our tree!!
    Huff_node *current = root;

    for(size_t i = 0; i < bytesRead; i++){
        current = root;
        //I will read until the first null terminator and add those characters to the end of my string variable
        //I am moving through the buffer with each iteration
        int stringSizeCount = 0;
        while(buffer[i] != '\0'){
            string[stringSizeCount] = buffer[i];
            stringSizeCount++;
            i++;
        }
        string[stringSizeCount] = '\0';
        i++;
        //This is for the binary code I am again going until the next null terminator, I am treating them as a pair
        //and I am also adding to my huff trie
        while(buffer[i] != '\0'){
            if(buffer[i] == '1'){
                //if the next character is a null terminator instead of creating a new node I will set this 
                //nodes string for the corresponding number to the string that I read in earlier
                if(buffer[i+1] == '\0'){
                    current->s_one = strdup(string);
                }else if(current->one == NULL){
                    current->one = createHuffNode();
                    current = current->one;
                }else{
                    current = current->one;
                }
            }else if (buffer[i] == '0'){
                if(buffer[i+1] == '\0'){
                    current->s_zero = strdup(string);
                }else if(current->zero == NULL){
                    current->zero = createHuffNode();
                    current = current->zero;
                }else {
                    current = current->zero;
                }
            }
           i++; 
        }
    }
    //freeing the first buffer
    free(buffer);

    //opeing the second file
    FILE *encoded = fopen(argv[2], "rb");

    if(!encoded){
        fprintf(stderr, "Error opening file: %s", argv[2]);
        exit(1);
    }

    fseek(encoded, 0, SEEK_END);
    fileSize = ftell(encoded);
    //I want to check to make sure that this file is at least big enough to have the data we need in it
    if(fileSize < 4){
        fprintf(stderr, "Error: file is not the correct size.\n");
        fclose(encoded);
        exit(1);
    }

    //I need to figure out how many bits I can expect to read from the file
    long bitCount;
    fseek(encoded, -4, SEEK_END);
    fread(&bitCount, sizeof(long), 1, encoded);


    if(bitCount > (fileSize*8 + 32)){
        fprintf(stderr, "Error: Total bits = %lu, but file's size is %lu\n", bitCount, fileSize);
        exit(1);
    }
    //I could have used the same buffer but I wanted to separate just for myself to ensure I treat them correctly
    unsigned char *diffBuffer = malloc(fileSize + 1);

    rewind(encoded);

    bytesRead = fread(diffBuffer, 1, fileSize, encoded);
    diffBuffer[bytesRead] = '\0';

    fclose(encoded);

    current = root;
    int bitIndex = 0;
    //here i am just navigating through the tree as I read in bytes and then moving through each bit within
    //that byte if I get to a word I immediately print it out
    for(long i = 0; i < fileSize; i++){
        for(int j = 0; j < 8; j++){
            if(bitIndex >= bitCount){
                break;
            }
            int singleBit = getBitFromByte(j, diffBuffer[i]);
            //if current is NULL that means that at some point we received a bit sequence that we did not have set 
            //up so when this happens we print out an error and exit the program
            if(current == NULL ){
                fprintf(stderr, "Unrecognized bits\n");
                exit(1);
            }
            //just moving through the tree some more
            if(singleBit == 1){
                if(current->s_one != NULL){
                    printf("%s", current->s_one);
                    current = root;
                }else{
                    current = current->one;
                }
            }else if(singleBit == 0){
                if(current->s_zero != NULL){
                    printf("%s", current->s_zero);
                    current = root;
                }else{
                    current = current->zero;
                }
            }
            bitIndex++;
        }
    }
    //freeing all the nodes and the buffer that we allocated
    freeHuffNode(root);
    free(diffBuffer);

    return 0;
}
