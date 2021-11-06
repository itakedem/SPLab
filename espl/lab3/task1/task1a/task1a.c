#include <stdio.h>
#include <stdlib.h>


typedef struct diff {
    long offset; /* offset of the difference in file starting from zero*/
    unsigned char orig_value;     /* value of the byte in ORIG */
    unsigned char new_value;     /* value of the byte in NEW */
} diff;

typedef struct node node;

struct node {
    diff* diff_data; /* pointer to a struct containing the offset and the value of the bytes in each of the files*/
    node *next;
};

void list_free(node* head);

diff* create_new_diff(long newOffset, unsigned char newOrigValue, unsigned char newNewValue)
{
    diff* newDiff = (diff*)calloc (1, sizeof(diff));
    newDiff->offset = newOffset;
    newDiff->orig_value = newOrigValue;
    newDiff->new_value = newNewValue;
    return newDiff;
}

node* create_new_node(diff* newDiff, node* newNext)
{
    node* newNode = (node*)calloc (1, sizeof(node));
    newNode->diff_data = newDiff;
    newNode->next = newNext;
    return newNode;
}

void list_print(node* diff_list,FILE* output)
{
    node* curr = diff_list;
    while (curr != NULL)
    {
        fprintf(output, "%ld %c %c \n", curr->diff_data->offset, curr->diff_data->orig_value, curr->diff_data->new_value);
        curr = curr->next;
    }
}


/* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE.
Each item followed by a newline character. */

node* list_append(node* diff_list, diff* data)
{
    if (diff_list == NULL)
    {
        return create_new_node(data, NULL);
    }

    node* newList = create_new_node(diff_list->diff_data, list_append(diff_list->next, data));
    free(diff_list);

    return newList;
}
/* Add a new node with the given data to the list,
   and return a pointer to the list (i.e., the first node in the list).
   If the list is null - create a new entry and return a pointer to the entry.*/

    void list_free(node* head)
    {
        if (head != NULL)
        {
            list_free(head->next);
            free(head);
        }
    }


    int main(int argc, char **argv)
    {
        diff *diff1 = create_new_diff(1, 'a', 'b');
        diff* diff2 = create_new_diff(2, 'a', 'a');
        node* lst1 = list_append(NULL, diff1);
        lst1 = list_append(lst1, diff2);
        list_print(lst1, stdout);

        list_free(lst1);
        free(diff1);
        free(diff2);
        return 0;
    }