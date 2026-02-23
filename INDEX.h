#ifndef INDEX
#define INDEX

#define SUCCESS 0
#define FAILURE -1

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio_ext.h>

typedef struct file_name
{
    char file_name[20];
    struct file_name *link;
}file_list;

typedef struct Sub
{
    int word_count;
    char filename[20];
    struct Sub *next_sub;
}Sub_list;

typedef struct Main
{
    int file_count;
    char word[20];
    Sub_list *sub_link;
    struct Main *main_link;
}Main_list;

typedef struct hasttable
{
    int index;
    Main_list *next_main;
}Hash;


int read_validation(int argc, char *argv[], file_list **files);
int insert_at_last(file_list **files, char *filename);
int is_duplicate(file_list *files,char *filename);
void print_list(file_list *files);

int Create_database(Hash *h_table, file_list **files);
int get_index(char ch);
Main_list *create_mainnode(char *word, char *filename);
Sub_list *create_subnode(char *filename);


int Display(Hash *HT);
int Search_Data(Hash *HT);
int Save_Database(Hash *HT,file_list *files);
int write_database_to_file(char *filename, Hash *HT);

int Update_Database(Hash *HT,file_list **files);
int insert_at_first_sub(Sub_list **head, char *filename, int wordcount);
int removefiles(file_list **head, char *filename);

#endif