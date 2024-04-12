#include <stdio.h>
#include <stdlib.h> // for malloc and free

#define HASH_SIZE 10 // Define hash table size

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Node for linked list to handle collisions
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// HashType
struct HashType
{
    struct Node* head; // Head of linked list at each index
};

// Compute the hash function
int hash(int x)
{
    const float A = 0.6180339887; // A constant between 0 and 1
    float fracPart = x * A - (int)(x * A); // Fractional part of the result
    return (int)(fracPart * HASH_SIZE); // Map the fractional part to the table size
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
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

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
    struct Node* current;

    printf("\nRecords in Hash Table:\n");
    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        if (pHashArray[i].head != NULL)
        {
            current = pHashArray[i].head;
            while (current != NULL)
            {
                printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
                current = current->next;
            }
        }
        printf("\n");
    }
}

// Insert a record into the hash table
void insertRecord(struct HashType *pHashArray, struct RecordType record)
{
    int index = hash(record.id);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    newNode->data = record;
    newNode->next = pHashArray[index].head;
    pHashArray[index].head = newNode;
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int i;

    recordSz = parseData("input_lab_9.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create hash table
    struct HashType hashTable[HASH_SIZE];
    for (i = 0; i < HASH_SIZE; ++i)
    {
        hashTable[i].head = NULL; // Initialize all heads to NULL
    }

    // Insert records into hash table
    for (i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, pRecords[i]);
    }

    // Display records in hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    free(pRecords); // Free allocated memory

    return 0;
}