/*
Darel Gomez
COP3502
March 13, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.0001
#define M_PI 3.14159265358979323846

typedef struct Group {
    int size;
    double radians;
    int groupNumber;
}Group;

typedef struct Result {
    int firstGroup;
    int secondGroup;
    double degrees;
}Result;

// Compare functions:

// Returns: 1 if d1 and d2 are same, else returns 0
int doubleCompare(double d1, double d2, double eps) {
    return (fabs(d1-d2) < eps);
}
// Returns: 1 if g1->radians < g2->radians, else returns 0
int groupCompare(Group g1, Group g2) {
    return (g1.radians < g2.radians);
}
// Returns: 1 if r1 goes before r2, else returns 0
int resultCompare(Result r1, Result r2) {
    if(r1.firstGroup == r2.firstGroup) return (r1.secondGroup <= r2.secondGroup);
    else return (r1.firstGroup <= r2.firstGroup);
}

// Swap functions: 
void intSwap(int* a, int* b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

// Merge functions:
void groupMerge(Group* groups, int left, int mid, int right);
void resultMerge(Result* results, int left, int mid, int right);

// Sort functions:
void groupSort(Group* groups, int left, int right);
void resultSort(Result* results, int left, int right);

// Other Functions:
Result* generateResults(Group* groups, int size);
double findMax(Result* results, int size);


// Print functions: (for debugging)
void printGroups(Group* groups, int size) {
    for(int i = 0; i < size; i++) {
        printf("Group %d: %d people at %.4lf radians\n", groups[i].groupNumber, groups[i].size, groups[i].radians/M_PI);
    }
}
void printResults(Result* results, int size) {
    for(int i = 0; i < size; i++) {
        printf("Groups %d and %d: %lf degrees between them.\n", results[i].firstGroup, results[i].secondGroup, results[i].degrees);
    }
}

int main() {
    int n, a; // number of groups and initial angle of projector
    scanf("%d %d", &n, &a);

    Group* groups = (Group*) malloc(sizeof(Group)*n);
    int x, y, s; // x,y coorindates and s number of people
    for(int i = 0; i < n; i++) {
        scanf("%d %d %d", &x, &y, &s);
        groups[i].size = s;
        groups[i].groupNumber = i;
        groups[i].radians = atan2(y, x);
        if(groups[i].radians < 0) groups[i].radians += 2*M_PI;
    }
    //printGroups(groups, n);

    //printf("Sorting...\n");
    groupSort(groups, 0, n-1);
    //printGroups(groups, n);

    Result* angles = generateResults(groups, n);
    //printResults(angles, n);

    //printf("Sorting...\n");
    resultSort(angles, 0, n-1);
    //printResults(angles, n);

    double max = findMax(angles, n);
    printf("Max projection degree without harming %.4lf\n", max);
    printf("Closest possible group pairs in order:\n");
    for(int i = 0; i < n; i++) {
        if(doubleCompare(max, angles[i].degrees, EPS)) {
            printf("%d %d\n", angles[i].firstGroup, angles[i].secondGroup);
        }
    }

    free(groups);
    free(angles);
    return 0;
}

void groupSort(Group* groups, int left, int right) {
    if(left >= right) return;

    int mid = left + (right-left)/2;
    groupSort(groups, left, mid);
    groupSort(groups, mid+1, right);
    groupMerge(groups, left, mid, right);
}

void groupMerge(Group* groups, int left, int mid, int right) {
    int leftbound = mid - left + 1;
    int rightbound = right - mid;

    Group* leftArr = malloc(sizeof(Group)*(leftbound));
    for(int i = 0; i < leftbound; i++) {
        leftArr[i] = groups[left+i];
    }
    Group* rightArr = malloc(sizeof(Group)*(rightbound));
    for(int i = 0; i < rightbound; i++) {
        rightArr[i] = groups[mid+1+i];
    }

    int leftIndex = 0;
    int rightIndex = 0;
    int index = left;
    while(leftIndex < leftbound && rightIndex < rightbound) {
        if(groupCompare(leftArr[leftIndex], rightArr[rightIndex])) {
            groups[index++] = leftArr[leftIndex++];
        }
        else {
            groups[index++] = rightArr[rightIndex++];
        }
    }
    while(leftIndex < leftbound) {
        groups[index++] = leftArr[leftIndex++];
    }
    while(rightIndex < rightbound) {
        groups[index++] = rightArr[rightIndex++];
    }

    free(leftArr);
    free(rightArr);
}

Result* generateResults(Group* groups, int size) {
    Result* array = malloc(sizeof(Result)*size);
    for(int i = 0; i < size-1; i++) {
        array[i].firstGroup = groups[i].groupNumber;
        array[i].secondGroup = groups[i+1].groupNumber;
        array[i].degrees = (fabs(groups[i+1].radians-groups[i].radians))*(180/M_PI);
        if(array[i].firstGroup > array[i].secondGroup) intSwap(&array[i].firstGroup, &array[i].secondGroup);
    }
    array[size-1].firstGroup = groups[0].groupNumber;
    array[size-1].secondGroup = groups[size-1].groupNumber;
    array[size-1].degrees = fabs((2*M_PI+groups[0].radians)-groups[size-1].radians)*(180/M_PI);
    if(array[size-1].firstGroup > array[size-1].secondGroup) intSwap(&array[size-1].firstGroup, &array[size-1].secondGroup);

    return array;
}

void resultSort(Result* results, int left, int right) {
    if(left >= right) return;

    int mid = left + (right-left)/2;
    resultSort(results, left, mid);
    resultSort(results, mid+1, right);
    resultMerge(results, left, mid, right);    
}

void resultMerge(Result* results, int left, int mid, int right) {
    int leftbound = mid - left + 1;
    int rightbound = right - mid;
    
    Result* leftArr = malloc(sizeof(Result)*(leftbound));
    for(int i = 0; i < leftbound; i++) {
        leftArr[i] = results[left+i];
    }
    Result* rightArr = malloc(sizeof(Result)*(rightbound));
    for(int i = 0; i < rightbound; i++) {
        rightArr[i] = results[mid+1+i];
    }

    int leftIndex = 0;
    int rightIndex = 0;
    int index = left;

    while(leftIndex < leftbound && rightIndex < rightbound) {
        if(resultCompare(leftArr[leftIndex], rightArr[rightIndex])) {
            results[index++] = leftArr[leftIndex++];
        }
        else {
            results[index++] = rightArr[rightIndex++];
        }
    }
    while(leftIndex < leftbound) {
        results[index++] = leftArr[leftIndex++];
    }
    while(rightIndex < rightbound) {
        results[index++] = rightArr[rightIndex++];
    }

    free(leftArr);
    free(rightArr);
}

double findMax(Result* results, int size) {
    double max = results[0].degrees;
    for(int i = 1; i < size; i++) {
        if(max < results[i].degrees) max = results[0].degrees;
    }
    return max;
}