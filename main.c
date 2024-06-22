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
        printf("5/ Question 5: Count the number of error messages. \n");
        printf("6/ Question 6: Count the number of error messages. \n");
        printf("7/ Exit \n");
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
                questFive();
                waitForUser();
                break;

            case 6:
                questSix();
                waitForUser();
                break;

            case 7:
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

void questOne() 
{
    char requiredChar[] = "\"cmd\":\"set\"";
    int nCount = countNews(fileStr, requiredChar);

    printf("The number of messages containing the keyword \"%s\": %d\n", requiredChar, nCount);
}

void questTwo() 
{
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

void questThree() 
{
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

void questFour() 
{
    int errorMess = countErrorMessages(FNAME);
    printf("errorMess: %i \n", errorMess);
}

void questFive() 
{
    thoiGianTreLonNhat(fileStr);
}

void questSix()
{
    thoiGianTreTrungBinh(fileStr);
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

void extractReqid(const char *logString, char *reqidValue) {
    char *reqidPos = strstr(logString, "\"reqid\":");
    if (reqidPos) {
        sscanf(reqidPos + 9, " \"%[^\"]\"", reqidValue); // Extract reqid value between quotes
    }
}

int countErrorMessages(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return -1;
    }

    char line1[1024];
    char line2[1024];
    char reqidValue1[10];
    char reqidValue2[10];
    int errorCount = 0;

    while (fgets(line1, sizeof(line1), file) && fgets(line2, sizeof(line2), file)) {
        extractReqid(line1, reqidValue1);
        extractReqid(line2, reqidValue2);
        
        if (strcmp(reqidValue1, reqidValue2) != 0) {
            errorCount++;
            //printf("errorCount: %i", errorCount);
        }
    }

    fclose(file);
    return errorCount;
}

int tinhTime(char *pos)
{
	int delay = 0;
	char hour[3], minute[3], second[7];
    hour[2] = '\0', minute[2] = '\0', second[6] = '\0';
    memcpy(hour, pos-2,2);
    
    memcpy(minute, pos+1,2);
    
    memcpy(second, pos+4,6);
   // printf("%s - %s - %s \n",hour,minute,second);

    // thoi gian delay
    delay = atoi(hour)*3600000+atoi(minute)*60000+atof(second)*1000;
    return delay;
}
//thoi gian tre lon nhat
void thoiGianTreLonNhat(char *str){
    int delay, delay2, maxDelay = 0;
    fileToStr(str);
    char *pos, *pos2;
    char *token = strtok(str,"\n"); // Chia chuoi str thanh cac token duoc phan biet khi xuong dong
    while(token != NULL){
        pos = strstr(token, ":"); // vi tri khop dau tien cua pos
        char *p = strstr(token, "reqid");
        char requid1[5];
        requid1[4] = '\0';
        memcpy(requid1, p+9,4);
        token = strtok(NULL, "\n");

        pos2 = strstr(token, ":"); // vi tri khop dau tien cua pos2
        char *p2 = strstr(token, "reqid");
        char requid2[5];
        requid2[4] = '\0';
        memcpy(requid2, p2+9,4);
        token = strtok(NULL, "\n");

        if(strcmp(requid1, requid2) == 0){
            // thoi gian delay1
            delay = tinhTime(pos); 
            // thoi gian delay2
            delay2 = tinhTime(pos2); 
            if((delay2 - delay) > maxDelay){
                maxDelay = delay2 - delay;
            }
        }
    }
    printf("\nDo tre lon nhat la: %d Millisecond", maxDelay);

}

void thoiGianTreTrungBinh(char *str)
{
    long delay, delay2, Xtb = 0;
    int count = 0;
    fileToStr(str);
    char *pos, *pos2;
    char *token = strtok(str,"\n"); // Chia chuoi str thanh cac token duoc phan biet khi xuong dong
    while(token != NULL){
        pos = strstr(token, ":");
        char *p = strstr(token, "reqid");
        char requid1[5];
        requid1[4] = '\0';
        memcpy(requid1, p+9, 4);
        char hour[3], minute[3], second[7];
        hour[2] = '\0', minute[2] = '\0', second[6] = '\0';
        token = strtok(NULL, "\n");

        pos2 = strstr(token, ":");
        char *p2 = strstr(token, "reqid");
        char requid2[5];
        requid2[4] = '\0';
        memcpy(requid2, p2+9, 4);
        char hour2[3], minute2[3], second2[7];
        hour2[2] = '\0', minute2[2] = '\0', second2[6] = '\0';

        token = strtok(NULL, "\n");

        if(strcmp(requid1, requid2) == 0){
            memcpy(hour, pos-3,2);
            memcpy(minute, pos+1,2);
            memcpy(second, pos+4,6);

            // thoi gian delay
            delay = atoi(hour)*3600000+atoi(minute)*60000+atof(second)*1000;

            memcpy(hour2, pos2-3,2);
            memcpy(minute2, pos2+1,2);
            memcpy(second2, pos2+4,6);

            // tinh do tre tb
            delay2 = atoi(hour2)*3600000+atoi(minute2)*60000+atof(second2)*1000;
            Xtb += delay2 - delay;
                count++;
        }
    }
    printf("\nDo tre trung binh la: %d Millisecond", Xtb/count);
}
