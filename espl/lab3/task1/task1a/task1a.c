#include <stdio.h>

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
    while (curr != null)
    {
        fprintf(output, "%ld %c %c", curr->diff_data->offset, curr->diff_data->orig_value, curr->diff_data->new_value)
        curr = curr->next;
    }
}
/* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE.
Each item followed by a newline character. */

node* list_append(node* diff_list, diff* data)
{
    node* curr = diff_list;
    if (curr == NULL)
    {
        return new {data, NULL};
    }

    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = new {data, NULL};

    return diff_list;
}
/* Add a new node with the given data to the list,
   and return a pointer to the list (i.e., the first node in the list).
   If the list is null - create a new entry and return a pointer to the entry.*/

int main(int argc, char **argv)
{
    node lst1;
    lst1 = list_append(lst1, new {&{1, 'a', 'b'}, NULL})
    lst1 = list_append(lst1, new {&{2, 'a', 'a'}, NULL})
    list_print(lst1, stdout);

    return 0;
}