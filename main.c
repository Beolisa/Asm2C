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
// void getDevice(char *tokens[]);
void questOne();
void questTwo();
void questThree();
// void questFour();

int main() {
    int choice = 0;

    // Save details into a string
    int status = fileToStr(fileStr);

    while (1) {

        printf("===MAIN MENU===\n");
        printf("1/ Question 1: Count the number of messages containing specific keywords.\n");
        printf("2/ Question 2: Find and store strings that match the substring.\n");
        printf("3/ Question 3: Calculate the number of switches that exchange information with the central controller during Log time.\n");
        printf("4/ Question 4: Count the number of error messages. \n");
        printf("5/ Exit \n");
        printf("Enter your choice here: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                questOne();
                waitForUser();

                break;
            case 2:
                questTwo();
                waitForUser();

                break;
            case 3:
                questThree();
                waitForUser();

                break;
            case 4:
                questFour();
                waitForUser();

                break;
            case 5:
                printf("Exiting the program...");
                exit(0);
            default:
                printf("Invalid choice, please try again. \n");
                waitForUser();
        }

        //system("cls");

    }
    
    return 0;
}

void questOne() {
    char requiredChar[] = "\"cmd\":\"set\"";
    int nCount = countNews(fileStr, requiredChar);

    printf("The number of messages containing the keyword \"%s\": %d\n", requiredChar, nCount);
}

void questTwo() {
    char *tokens[MAXLENGTH]; // Array to store tokens
    char requiredChar[] = "\"cmd\":\"set\"";

    //Tokenize the string into an array
    int leng = tokenizeStr(fileStr, tokens);

    FilterStr(tokens, requiredChar, leng);

    char networkID[10];

    printf("Enter the network ID: ");
    scanf("%s", &networkID);

    int filterLeng = 0;
    while (tokens[filterLeng] != NULL) {
        filterLeng++;
    }

    FilterStr(tokens, networkID, filterLeng);

    int i = 0;
    while (tokens[i] != NULL) {
        printf("Token %i: %s \n", i, tokens[i]);
        i++;
    }
}

void questThree() {
    char *tokens[MAXLENGTH];
    char requiredChar1[] = "\"cmd\":\"set\"";
    char requiredChar2[] = "\"type\":\"switch\"";

    int leng = tokenizeStr(fileStr, tokens);
    FilterStr(tokens, requiredChar1, leng); //1st filter

    printf("\n");

    int i = 0;
    while (tokens[i] != NULL) {
        i++;
    }

    getDevice(tokens, i);
}

void questFour() {
    char *tokens[MAXLENGTH];
    int leng = tokenizeStr(fileStr, tokens);

    int errorMess = countErrorMessages(fileStr, tokens);
    printf(errorMess);
}

void waitForUser() {
    printf("Press Enter to continue...");
    while (getchar() != '\n'); // Consume any lingering newline characters
    getchar(); // Wait for the user to press Enter
    
    system("cls"); // For Windows
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

void getDevice(char *tokens[], int size) {
    const char *pattern = "zwave-";
    char devices[10][5];  // Assuming there are only 10 unique devices with 4 characters each
    char endpoints[10][3]; // Assuming there are only 10 unique endpoints with 2 characters each
    int occurrences[10] = {0}; // To store the occurrences of each device
    int deviceIndex = 0;

    for (int i = 0; i < size; i++) {
        const char *foundStr = strstr(tokens[i], pattern);
        if (foundStr != NULL) {
            char extractedStr[5];
            char endpoint[3];
            
            strncpy(extractedStr, foundStr + strlen(pattern), 4);
            extractedStr[4] = '\0';
            
            strncpy(endpoint, foundStr + strlen(pattern) + 6, 2);
            endpoint[2] = '\0';

            // Check if the device is already in the devices array
            int existingDeviceIndex = -1;
            for (int j = 0; j < deviceIndex; j++) {
                if (strcmp(devices[j], extractedStr) == 0) {
                    existingDeviceIndex = j;
                    break;
                }
            }

            if (existingDeviceIndex == -1) {
                strcpy(devices[deviceIndex], extractedStr);
                strcpy(endpoints[deviceIndex], endpoint);
                occurrences[deviceIndex]++;
                deviceIndex++;
            } else {
                occurrences[existingDeviceIndex]++;
            }
        } else {
            printf("Pattern not found in input token: %s\n", tokens[i]);
        }
    }

    // Print the occurrences of each device with its corresponding endpoint
    for (int k = 0; k < deviceIndex; k++) {
        printf("Device %s with endpoint %s\n", devices[k], endpoints[k]);
    }
}

int countErrorMessages(char *fileStr, char *tokens[]) {
    int errorCount = 0;
    int i = 0;

    // Check for reqid mismatch in consecutive messages
    for (int j = 1; j < i; j += 2) {
        char *reqid1 = strstr(tokens[j-1], "reqid");
        char *reqid2 = strstr(tokens[j], "reqid");
        
        if (reqid1 && reqid2) {
            char value1[10], value2[10];
            sscanf(reqid1 + 6, "%s", value1); // Assuming reqid length won't exceed 10
            sscanf(reqid2 + 6, "%s", value2);
            
            if (strcmp(value1, value2) != 0) {
                errorCount++;
            }
        }
    }

    return errorCount;
}
