/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 5
 * QUICK SORT ON LINKED LIST
 * Last edited on 7 September 2023
 * PURPOSE: To read a text file containing integers into a linked list and apply quicksort on them
 * INPUT: Takes arguments from the command line in the form of a text file containing integers
 * OUTPUT: Prints the initial linked list given through the command line and the sorted linked list
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

struct Node { //defining struct node
    int data;
    struct Node* next;
};
struct Node* getTail(struct Node* head);
struct Node* createNode(int data) {//function that creates a new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));//memory allocation
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertAtEnd(struct Node** head, int data) {//function to insert a node at the end of the linked list
    struct Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printList(struct Node* head) {//function to perform the printlist operation
    struct Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

struct Node* partition(struct Node* head, struct Node* end, struct Node** newHead, struct Node** newEnd) {//quicksort partition function
    struct Node* pivot = end;
    struct Node* prev = NULL, *cur = head, *tail = pivot;

    while (cur != pivot) {
        if (cur->data < pivot->data) {
            if (*newHead == NULL)
                *newHead = cur;
            prev = cur;
            cur = cur->next;
        } else {
            if (prev)
                prev->next = cur->next;
            struct Node* temp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = temp;
        }
    }

    if (*newHead == NULL)
        *newHead = pivot;

    *newEnd = tail;

    return pivot;
}

struct Node* quickSort(struct Node* head, struct Node* end) {//recursive quicksort function
    if (!head || head == end)
        return head;

    struct Node* newHead = NULL, *newEnd = NULL;

    struct Node* pivot = partition(head, end, &newHead, &newEnd);

    if (newHead != pivot) {
        struct Node* tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = NULL;

        newHead = quickSort(newHead, tmp);

        tmp = getTail(newHead);
        tmp->next = pivot;
    }

    pivot->next = quickSort(pivot->next, newEnd);

    return newHead;
}

struct Node* getTail(struct Node* head) {
    while (head != NULL && head->next != NULL) {
        head = head->next;
    }
    return head;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    struct Node* head = NULL;
    int num;

    while (fscanf(inputFile, "%d", &num) == 1) {//taking input and creating the linked list
        insertAtEnd(&head, num);
    }

    fclose(inputFile);

    struct Node* tail = getTail(head);
    head = quickSort(head, tail);

    printList(head);

    // Free the memory used by the linked list
    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
