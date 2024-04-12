//Aidan Huggins
//Dr. Torosdagli
//COP3502
//Lab Assignment 9

#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10

struct RecordType {
    int id;
    char name;
    int order;
};

struct Node {
    struct RecordType data;
    struct Node* next;
};

struct HashType {
    struct Node* head;
};

int hash(int x) {
    return x % HASH_SIZE;
}

int parseData(char* inputFileName, struct RecordType** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL) {
            printf("Out of memory!\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void displayRecordsInHash(struct HashType* pHashArray, int hashSz) {
    int i;
    struct Node* current;

    printf("\nRecords in Hash Table:\n");
    for (i = 0; i < hashSz; ++i) {
        printf("Index %d -> ", i);
        if (pHashArray[i].head != NULL) {
            current = pHashArray[i].head;
            while (current != NULL) {
                printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
                current = current->next;
            }
        }
        printf("\n");
    }
}

void insertRecord(struct HashType* pHashArray, struct RecordType record) {
    int index = hash(record.id);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Out of memory!\n");
        exit(-1);
    }
    newNode->data = record;
    newNode->next = pHashArray[index].head;
    pHashArray[index].head = newNode;
}

int main(void) {
    struct RecordType* pRecords;
    int recordSz = 0;
    int i;

    recordSz = parseData("input_lab_9.txt", &pRecords);
    printRecords(pRecords, recordSz);

    struct HashType hashTable[HASH_SIZE];
    for (i = 0; i < HASH_SIZE; ++i) {
        hashTable[i].head = NULL;
    }

    for (i = 0; i < recordSz; ++i) {
        insertRecord(hashTable, pRecords[i]);
    }

    displayRecordsInHash(hashTable, HASH_SIZE);

    free(pRecords);

    return 0;
}
