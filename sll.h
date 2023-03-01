#ifndef SLL_H_
#define SLL_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void *next;
    int data;
} Node;

Node *head = NULL;

Node *addNode(int data);
int removeNode(int data);
Node *insertNode(int data, int position);
void printList();
void printMenu();

#endif // SLL_H_

#ifndef SLL_IMPLEMENTATION
#define SLL_IMPLEMENTATION

// Implementation

Node *head = NULL;

// add a node to the list
Node *addNode(int data)
{
    Node *new = NULL;
    // two cases:

    // if the list is empty.
    if (head == NULL)
    {
        new = malloc(sizeof(Node));
        if (new == NULL)
            return NULL;
        
        new->data = data;
        head = new;
        new->next = NULL;
    } else {
        new = malloc(sizeof(Node));
        if (new == NULL)
            return NULL;
        
        new->data = data;
        new->next = head;
        head = new;
    }

    return new;
}

// remove a node from the list
int removeNode(int data)
{
    Node *current = head;
    Node *prev = head;
    while (current != NULL)
    {
        if (current->data == data)
        {
            // if current node is the list head
            if (current == head)
            {
                head = current->next;
            } else {
                prev->next = current->next;
                free(current);
                current = NULL;
            }

            return 1;
        }
        prev = current;
        current = current->next;
    }

    return 0;
}

// insert a node into a position in the list
Node *insertNode(int data, int position)
{
    Node *current = head;
    while (current != NULL && position != 0)
    {
        position--;
    }

    if (position != 0)
    {
        printf("Requested position too far into list\n");
        return NULL;
    }

    Node *new = malloc(sizeof(Node));
    if (new == NULL)
        return NULL;

    new->data = data;
    new->next = current->next;
    current->next = new;

    return new;
}

// print operation
void printList()
{
    Node *current = head;
    while (current != NULL)
    {
        printf("%d->", current->data);
        current = current->next;
    }

    printf("\n");
    return;
}

void printMenu()
{
    printf("You have the following options:\n");
    printf("\t1. Add a node to the list.\n");
    printf("\t2. Remove a node from the list.\n");
    printf("\t3. Insert a node to the list.\n");
    printf("\t4. Print your list\n");
    printf("\t5. Quit.\n");
    return;
}

#endif