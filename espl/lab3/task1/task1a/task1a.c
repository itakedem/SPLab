#include <stdio.h>
#include <stdlib.h>

typedef struct diff {
    long offset; /* offset of the difference in file starting from zero*/
    unsigned char orig_value;     /* value of the byte in ORIG */
    unsigned char new_value;     /* value of the byte in NEW */
} diff;

typedef struct node node;

struct node {
    diff *diff_data; /* pointer to a struct containing the offset and the value of the bytes in each of the files*/
    node *next;
};

void list_print(node *diff_list,FILE* output)
{
    node* curr = diff_list;
    while (curr != NULL)
    {
        fprintf(output, "%ld %c %c", curr->diff_data->offset, curr->diff_data->orig_value, curr->diff_data->new_value);
        curr = curr->next;
    }
}
/* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE.
Each item followed by a newline character. */

node* list_append(node* diff_list, diff* data)
{
    node* curr = diff_list;
    node* newNode = (node*)calloc (sizeof(node), 1);
    if (curr == NULL)
    {
        *newNode = (node){data, NULL};
        return newNode;
    }

    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    *newNode = (node){data, NULL};
    curr->next = newNode;

    return diff_list;
}
/* Add a new node with the given data to the list,
   and return a pointer to the list (i.e., the first node in the list).
   If the list is null - create a new entry and return a pointer to the entry.*/


void list_free(node* node)
{
    if (node != null)
    {
        node* next = (node*)calloc (sizeof(node), 1);
        node* next = node->next;
        free(node);
        list_free(next);
        free(next);
    }
}
int main(int argc, char **argv)
{
    diff diff1 = {1, 'a', 'b'};
    diff diff2 = {2, 'a', 'a'};
    node* lst1 = (node*)calloc (sizeof(node), 1);
    lst1 = list_append(lst1, &diff1);
    lst1 = list_append(lst1, &diff2);
    list_print(lst1, stdout);

    list_free(lst1);
    return 0;
}