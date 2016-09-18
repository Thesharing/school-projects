//
// Created by Hongzhilong on 16/1/8.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *frame;
int *pageReference;
int frameNum;
int referenceNum = 10;

void FIFO(int referenceCount);

void LRU(int referenceCount);

int *generateRandomPageReference(int num);

int isExist(int num, int *array, int arrayNum);

int findMax(int *array, int arrayNum);

int *generateRandomPageReferenceWithNoRepeat(int num);

int main(void) {
    printf("Input the total number of frames:");
    scanf("%d", &frameNum);
    while (frameNum > 7 && frameNum <= 0) {
        printf("Wrong Input,retry please:");
        scanf("%d", &frameNum);
    }
    frame = (int *) malloc(sizeof(int) * frameNum);
    for (int i = 0; i < frameNum; i++) {
        frame[i] = -1;
    }

    int referenceCount = 1;
    while (referenceCount > 0) {
        printf("Input the count of reference (Negative Number to Quit):");
        scanf("%d", &referenceCount);
        pageReference = generateRandomPageReference(referenceCount);
        printf("FIFO\n");
        FIFO(referenceCount);
        for (int i = 0; i < frameNum; i++) {
            frame[i] = -1;
        }
        printf("\n\nLRU\n");
        LRU(referenceCount);
        printf("\n\n");
    }
    free(frame);
    free(pageReference);
    return 0;
}

void FIFO(int referenceCount) {
    int line = 0;
    int nowPtr = 0;
    int faultCount = 0;
    for (int i = 0; i < referenceCount; i++) {
        if (isExist(pageReference[i], frame, frameNum) < 0) {
            faultCount++;
            frame[nowPtr] = pageReference[i];
            nowPtr = (nowPtr + 1) % frameNum;
        }

        printf("%d\t\t", ++line);
        for (int i = 0; i < frameNum; i++) {
            printf("%d\t", frame[i]);
        }
        printf("\tNow Referring:%d\n", pageReference[i]);
    }
    printf("Total Faults: %d", faultCount);
}

void LRU(int referenceCount) {
    int line = 0;
    int faultCount = 0;
    int *referTimes = (int *) malloc(sizeof(int) * frameNum);
    for (int i = 0; i < frameNum; i++) {
        referTimes[i] = 0;
    }

    for (int i = 0; i < referenceCount; i++) {
        int pos = isExist(pageReference[i], frame, frameNum);
        if (pos < 0) {
            faultCount++;
            pos = findMax(referTimes, frameNum);
            frame[pos] = pageReference[i];
        }
        referTimes[pos] = -1;
        for (int i = 0; i < frameNum; i++) {
            referTimes[i]++;
        }

        printf("%d\t\t", ++line);
        for (int i = 0; i < frameNum; i++) {
            printf("%d\t", frame[i]);
        }
        printf("\tNow Referring:%d\n", pageReference[i]);
    }
    printf("Total Faults: %d", faultCount);
    free(referTimes);
}


int findMax(int *array, int arrayNum) {
    int max = INT_MIN;
    int maxPos = 0;
    for (int i = 0; i < arrayNum; i++) {
        if (max < array[i]) {
            max = array[i];
            maxPos=i;
        }
    }
    return maxPos;
}

int isExist(int num, int *array, int arrayNum) {
    for (int i = 0; i < arrayNum; i++) {
        if (array[i] == num) {
            return i;
        }
    }
    return -1;
}

int *generateRandomPageReference(int num) {
    int *array = (int *) malloc(sizeof(int) * num);
    srand((unsigned int) time(NULL));
    for (int i = 0; i < num; i++) {
        array[i] = rand() % referenceNum;
    }
    return array;
}


int *generateRandomPageReferenceWithNoRepeat(int num) {
    int *startArray = (int *) malloc(sizeof(int) * num);
    for (int i = 0; i < num; i++) {
        startArray[i] = i + 1;
    }
    int *resultArray = (int *) malloc(sizeof(int) * num);
    srand((unsigned int) time(NULL));
    for (int i = 0; i < num; i++) {
        int seed = rand() % (num - i);
        resultArray[i] = startArray[seed];
        startArray[seed] = startArray[num - i - 1];
    }
    free(startArray);
    return resultArray;
}

