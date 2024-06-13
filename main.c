#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNAME "log.txt"
#define MAXLENGTH 5000

char fileStr[MAXLENGTH];

// Prototypes
int fileToStr(char* str);
int countNews(char mainStr[], char subStr[]);
int tokenizeStr(char mainStr[], char *tokens[]);
void FilterStr(char *tokens[], char subStr[], int leng);
void getDevice(char *tokens[]);

int main() {
    // Save details into a string
    int status = fileToStr(fileStr);

    // Question 1: Count the number of messages containing specific keywords
    char requiredChar[] = "\"cmd\":\"set\"";
    int nCount = countNews(fileStr, requiredChar);
    //printf("The number of messages containing the keyword \"%s\": %d\n", requiredChar, nCount);

    // Question 2: Find and store strings that match the substring
    char *tokens[MAXLENGTH]; // Array to store tokens

    //Tokenize the string into an array
    // int leng = tokenizeStr(fileStr, tokens);

    // FilterStr(tokens, requiredChar, leng);

    // char networkID[10];
    // printf("Enter the network ID: ");
    // scanf("%s", &networkID);

    // int filterLeng = 0;
    // while (tokens[filterLeng] != NULL) {
    //     filterLeng++;
    // }

    // FilterStr(tokens, networkID, filterLeng);

    int i = 0;
    // while (tokens[i] != NULL) {
    //     printf("Token %i: %s \n", i, tokens[i]);
    //     i++;
    // }

    //Question 3:
    char *tokens2[MAXLENGTH];
    //int leng2 = tokenizeStr(fileStr, tokens);

    //FilterStr(tokens2, requiredChar, leng2);

    getDevice(tokens2);

    return 0;
}

int fileToStr(char* str) {
    FILE* fp = fopen(FNAME, "r");
    if (fp == NULL) {
        printf("File does not exist\n");
        return -1;
    }

    int status = fread(str, sizeof(char), MAXLENGTH, fp);
    fclose(fp);
    
    return status;
}

int countNews(char mainStr[], char subStr[]) {
    int count = 0;
    char *pCount = strstr(mainStr, subStr);

    while (pCount != NULL) {
        count++;
        pCount = strstr(pCount + strlen(subStr), subStr);
    }

    return count;
}

int tokenizeStr(char mainStr[], char *tokens[]) { //Tokenizing the string to arrays
    char *token = strtok(mainStr, "\n");
    int i = 0;

    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, "\n");
    }

    return i;
}

void FilterStr(char *tokens[], char subStr[], int leng) {
    int i = 0;
    char *tempArr[MAXLENGTH]; // Array of pointers to store filtered tokens
    int tempIndex = 0; // Index for tempArr

    while (i < leng) {
        char *ptrStr = strstr(tokens[i], subStr);
        if (ptrStr != NULL) {
            tempArr[tempIndex] = ptrStr; // Copy the pointer to tempArr
            tempIndex++;
        }
        i++; // Move to the next token
    }

    //Print or process the filtered tokens
    // for (int j = 0; j < tempIndex; j++) {
    //     printf("Filtered Token %d: %s\n", j, tempArr[j]);
    // }

    for (int i = 0; i < tempIndex; i++) {
        strcpy(tokens[i], tempArr[i]);
    }

    for (int i = tempIndex; i < leng; i++) {
        tokens[i] = NULL; //Fill the rest with NULL  
    }
}

void getDevice(char *tokens[]) {
    const char *pattern = "zwave-";
    int i = 0;

    char *deviceName[10]; // Assume there are only 10 different devices
    char *endpoints[10];   // Assume there are only 10 different endpoints

    int tempArr1 = 0; 
    int tempArr2 = 0;

    while (tokens[i] != NULL) {
        const char *foundStr = strstr(tokens[i], pattern);

        if (foundStr != NULL) {
            char extractedStr[5];  // Assuming 'ffa2' has 4 characters
            char endpointStr[3];   // For 2 characters after extractedStr and null terminator

            // Copy characters after 'zwave-' into extractedStr
            strncpy(extractedStr, foundStr + strlen(pattern), 4);
            extractedStr[4] = '\0';  // Null terminate the extracted string

            // Copy the next 2 characters after the extracted string
            strncpy(endpointStr, foundStr + strlen(pattern) + 4, 2);
            endpointStr[2] = '\0';  // Null terminate the endpoint string

            // Check if the last extractedStr is different, then update tempArr1
            if (tempArr1 == 0 || strcmp(deviceName[tempArr1 - 1], extractedStr) != 0) {
                deviceName[tempArr1] = strdup(extractedStr);
                tempArr1++;
            }

            // Check if the last endpoint is different, then update tempArr2
            if (tempArr2 == 0 || strcmp(endpoints[tempArr2 - 1], endpointStr) != 0) {
                endpoints[tempArr2] = strdup(endpointStr);
                tempArr2++;
            }

            printf("Extracted substring: %s\n", extractedStr);
            printf("Characters after extracted string: %s\n", endpointStr);
        } else {
            printf("Pattern not found in the input string.\n");
        }

        i++;
    }

    // Free dynamically allocated memory
    for (i = 0; i < tempArr1; i++) {
        free(deviceName[i]);
    }

    for (i = 0; i < tempArr2; i++) {
        free(endpoints[i]);
    }
}