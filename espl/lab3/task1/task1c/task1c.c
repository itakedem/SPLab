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

FILE* out;
FILE * file1;
FILE * file2;
char* path1;
char* path2;
int limit = -1;

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
    while (limit!=0  && curr != NULL)
    {
        fprintf(output, "byte %ld %02X %02X \n", curr->diff_data->offset, curr->diff_data->orig_value, curr->diff_data->new_value);
        curr = curr->next;
        limit--;
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
    node* curr = diff_list;
    while(curr->next !=NULL)
    {
        curr = curr->next;
    }
    curr->next = create_new_node(data, NULL);

    return diff_list;
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

void resolver(int argc, char* task, char ** argv)
{
    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-o") == 0)
            out = fopen(argv[++i], "w");
        
        else if (strcmp(argv[i], "-t") == 0)
            *task = 'b';
        
        else if (strcmp(argv[i], "-k") == 0)
        {
            *task = 'c';
            limit =atoi(argv[++i]);
        }
        else
        {
            if (path1 == NULL)
            {
                path1 = argv[i];
            }
            else
            {
                path2 = argv[i];
            }
        }
        i++;
    }
}
int listSize(node* list){
    int size=0;
    node* curr=list;
    while(curr!=NULL)
    {
        curr=curr->next;
        size++;
    }
    return size;
}

void closeFiles()
{
    fclose(file1);
    fclose(file2);
    if (out != stdout)
    {
        fclose(out);
    }
}

int main(int argc, char **argv)
{
    out = stdout;
    path1 = NULL;
    path2 = NULL;
    char task = 'a';
    resolver(argc, &task, argv);
    node *diffList = buildDiffList();
    if(task == 'b')
    {
        fprintf(out,"%d\n", listSize(diffList));
    }
    else
    {
        list_print(diffList, out);
    }
    list_free(diffList);
    closeFiles();
    return 0;
}