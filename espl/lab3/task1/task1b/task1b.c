#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

int PrintHex(char buffer[], size_t length){
    int i = 0;
    while (i < (length - 1))
    {
        printf("%02hhX ", buffer[i]);
        i++;
    }
    printf("%02hhX\n", buffer[length - 1]);
    return 0;
}

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
        fprintf(output, "%ld %02X %02X \n", curr->diff_data->offset, curr->diff_data->orig_value, curr->diff_data->new_value);
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
    diff * newDiff = create_new_diff(diff_list->diff_data->offset,
                                        diff_list->diff_data->orig_value,
                                        diff_list->diff_data->new_value);
    node* newList = create_new_node(newDiff, list_append(diff_list->next, data));
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
        free(head->diff_data);
        free(head);
    }
}

FILE * file1;
FILE * file2;
char* path1;
char* path2;

long getSize()
{
    file1 = fopen(path1, "rb");
    file2 = fopen(path2, "rb");
    fseek(file1,0, SEEK_END);
    long size1 = ftell(file1);
    rewind(file1);

    fseek(file2,0, SEEK_END);
    long size2 = ftell(file2);
    rewind(file2);

    return size1 > size2 ? size2 : size1;
}

node* buildDiffList()
{
    long size = getSize(file1, file2);
    node* diffList=NULL;
    long i=0;
    char buffer[1];
    while(i<size)
    {
        unsigned char c1;
        unsigned char c2;
        fread(buffer, 1,1, file1);
        c1 = (unsigned char)*buffer;
        fread(buffer, 1,1, file2);
        c2 = (unsigned char)*buffer;
        if (c1 != c2)
        {
            diffList = list_append(diffList, create_new_diff(i, c1, c2));
        }
        i++;
    }
    return diffList;
}


int main(int argc, char **argv)
{
    path1 = argv[1];
    path2 = argv[2];
    node *diffList = buildDiffList();
    list_print(diffList, stdout);
    free(diffList);
    fclose(file1);
    fclose(file2);
    return 0;
}