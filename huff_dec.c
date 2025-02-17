#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//./bin/huff_dec data/celebrate-code.txt data/celebrate-encoded.txt  

typedef struct huff_node {
    struct huff_node *zero;
    struct huff_node *one;
    char *s_zero;
    char *s_one;
    char *bitSequence0;
    char *bitSequence1;
} Huff_node;

Huff_node* createHuffNode(){
    Huff_node *newNode = malloc(sizeof(Huff_node));

    newNode->one = NULL;
    newNode->zero = NULL;
    newNode->s_one = NULL;
    newNode->s_zero = NULL;
    newNode->bitSequence0 = NULL;
    newNode->bitSequence1 = NULL;

    return newNode;
}

int getBitFromByte(int pos, unsigned char character){
    int bit = ((character >> pos) & 1);
    //printf("bit: %i\n", bit);
    return bit;
}


// Queue structure for BFS
typedef struct queue_node {
    Huff_node *node;
    struct queue_node *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

void enqueue(Queue *q, Huff_node *node) {
    QueueNode *newNode = malloc(sizeof(QueueNode));
    newNode->node = node;
    newNode->next = NULL;
    if (q->rear) {
        q->rear->next = newNode;
    } else {
        q->front = newNode;
    }
    q->rear = newNode;
}

Huff_node *dequeue(Queue *q) {
    if (!q->front) return NULL;
    QueueNode *temp = q->front;
    Huff_node *node = temp->node;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    return node;
}

int isQueueEmpty(Queue *q) {
    return q->front == NULL;
}

void bfsPrint(Huff_node *root) {
    if (!root) return;
    Queue q = {NULL, NULL};
    enqueue(&q, root);
    while (!isQueueEmpty(&q)) {
        Huff_node *current = dequeue(&q);
        printf("Node at %p\n", (void*)current);
        if (current->s_zero) printf("  s_zero: %s, bitSequence:%s\n", current->s_zero, current->bitSequence0);
        if (current->s_one) printf("  s_one: %s, bitSequence:%s\n", current->s_one, current->bitSequence1);
        if (current->zero) {
            printf("  Has zero child\n");
            enqueue(&q, current->zero);
        }
        if (current->one) {
            printf("  Has one child\n");
            enqueue(&q, current->one);
        }
    }
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
    char bitSeq[10001] = "";

    Huff_node *root = createHuffNode();

    Huff_node *current = root;

    for(size_t i = 0; i < bytesRead; i++){
        current = root;
        
        int stringSizeCount = 0;
        while(buffer[i] != '\0'){
            //printf("buffer[i] %s\n", buffer[i]);
            string[stringSizeCount] = buffer[i];
            stringSizeCount++;
            i++;
        }
        string[stringSizeCount] = '\0';
        i++;

        int bitSeqSizeCount = 0;
        while(buffer[i] != '\0'){
            //printf("buffer[i] %c\n", buffer[i]);
            bitSeq[bitSeqSizeCount] = buffer[i];
            bitSeqSizeCount++;
            bitSeq[bitSeqSizeCount] = '\0';
            if(buffer[i] == '1'){
                // printf("buffer value is 1\n");
                // if(buffer[i+1]){
                //     printf("buffer i + 1: %c\n", buffer[i+1]);
                // }
                if(buffer[i+1] == '\0'){
                    current->s_one = strdup(string);
                    //printf("current->s_one: %s\n", current->s_one);
                    current->bitSequence1 = strdup(bitSeq);
                    //printf("current->bitSeqence: %s\n", current->bitSequence1);
                }else if(current->one == NULL){
                    //printf("We are creating a one node\n");
                    current->one = createHuffNode();
                    current = current->one;
                }else{
                    current = current->one;
                }
            }else if (buffer[i] == '0'){
                // printf("buffer value is 0\n");
                // if(buffer[i+1]){
                //     printf("buffer i + 1: %c\n", buffer[i+1]);
                // }
                if(buffer[i+1] == '\0'){
                    current->s_zero = strdup(string);
                    //printf("current->s_zero: %s\n", current->s_zero);
                    current->bitSequence0 = strdup(bitSeq);
                    //printf("current->bitSeqence: %s\n", current->bitSequence0);
                }else if(current->zero == NULL){
                    //printf("we are creating a 0 node\n");
                    current->zero = createHuffNode();
                    current = current->zero;
                }else {
                    current = current->zero;
                }
            }
           i++; 
        }
    }

    //bfsPrint(root);
    //printf("hello");
    free(buffer);

    
    FILE *encoded = fopen(argv[2], "rb");

    if(!code){
        fprintf(stderr, "Error opening file: %s", argv[2]);
    }

    fseek(encoded, 0, SEEK_END);
    fileSize = ftell(encoded);

    if(fileSize < 4){
        fprintf(stderr, "File is too small");
        fclose(encoded);
        exit(1);
    }

    long bitCount;
    fseek(encoded, -4, SEEK_END);
    fread(&bitCount, sizeof(long), 1, encoded);

    //printf("bit count: %lu\n", bitCount);
    // printf("file size: %lu\n", fileSize);

    unsigned char *diffBuffer = malloc(fileSize + 1);

    rewind(encoded);

    bytesRead = fread(diffBuffer, 1, fileSize, encoded);
    diffBuffer[bytesRead] = '\0';

    current = root;
    //printf("bytes read for 2nd file: %zu\n", bytesRead);
    int bitIndex = 0;
    for(long i = 0; i < fileSize; i++){
        //printf("hey we are in for loop\n");
        //printf("byte: %d\n", (unsigned char)diffBuffer[i]);
        for(int j = 0; j < 8; j++){
            if(bitIndex > bitCount){
                break;
            }
            int singleBit = getBitFromByte(j, diffBuffer[i]);
            if(current->one == NULL && current->zero == NULL && current->s_one == NULL && current->s_zero == NULL){
                printf("Unrecognized bits\n");
            }
            //printf("bit: %i\n", singleBit);
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
            bitIndex++;
        }
    }
    //printf("\n");
    return 0;
}
