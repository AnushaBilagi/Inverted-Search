#include"index.h"

int insert_at_last(file_list **files, char *filename)
{
    file_list *new = malloc(sizeof(file_list));
    if(new == NULL)
    {
        return FAILURE;
    }
    
    strcpy(new->file_name,filename);
    new->link = NULL;
    
    if(*files == NULL)
    {
        *files = new;
        return SUCCESS;
    }

    file_list *temp = *files;
    while(temp->link != NULL)
    {
        temp = temp->link;
    }
        
    temp->link = new;
        
    return SUCCESS;
}

int is_duplicate(file_list *files,char *filename)
{
	file_list *temp = files;
	while(temp != NULL)
	{
	    if(strcmp(filename,temp->file_name) == 0)
	    {
	        return FAILURE;
	    }
	    temp = temp->link;
	}
    return SUCCESS;
}

void print_list(file_list *files)
{
	if (files == NULL)
	{
		printf("No Valid Files\n");
	}
    
    while (files != NULL)		
	{
	    printf("%s      ", files->file_name);
		files = files->link;
	}
    // printf("NULL\n");
    
}

int get_index(char ch)
{
    //Check uppercase A–Z 
    if (ch >= 'A' && ch <= 'Z')
        return ch - 'A';

    //Check lowercase a–z
    if (ch >= 'a' && ch <= 'z')
        return ch - 'a';

    //Check digits 0–9
    if (ch >= '0' && ch <= '9')
        return 26;

    //special character
    return 27;
}

Main_list *create_mainnode(char *word, char *filename)
{
    Main_list *new = malloc(sizeof(Main_list));
    strcpy(new->word, word);
    new->file_count = 1;

    new->sub_link = create_subnode(filename);
    new->main_link = NULL;

    return new;
}

Sub_list *create_subnode(char *filename)
{
    Sub_list *new = malloc(sizeof(Sub_list));
    new->word_count = 1;
    strcpy(new->filename, filename);
    new->next_sub = NULL;
    return new;
}

int write_database_to_file(char *filename, Hash *HT)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("❌  ERROR: Could not open file %s for writing.\n", filename);
        return FAILURE;
    }

    // Write HT contents in format:
    // #index;word;f_count;f_name;w_count#
    for (int i = 0; i < 28; i++)
    {
        Main_list *main_temp = HT[i].next_main;

        while (main_temp != NULL)
        {
            Sub_list *sub_temp = main_temp->sub_link;

            // Start writing the line
            fprintf(fp, "#%d;%s;%d;", 
                    i, 
                    main_temp->word, 
                    main_temp->file_count);

            // Add all files and word counts in same line
            while (sub_temp != NULL)
            {
                fprintf(fp, "%s;%d;", 
                        sub_temp->filename,
                        sub_temp->word_count);

                sub_temp = sub_temp->next_sub;
            }

            // Close the line
            fprintf(fp, "#\n");

            main_temp = main_temp->main_link;
        }
    }

    fclose(fp);
    return SUCCESS;
}

int insert_at_first_sub(Sub_list **head, char *filename, int wordcount)
{
    Sub_list *new = malloc(sizeof(Sub_list));
    if(new == NULL)
        return FAILURE;

    strcpy(new->filename, filename);
    new->word_count = wordcount;

    new->next_sub = *head;   // insert at first
    *head = new;

    return SUCCESS;
}

int removefiles(file_list **head, char *filename)
{
    if (*head == NULL)
        return FAILURE;
        
    file_list *temp = *head;
    file_list *prev = NULL;
    
    while(temp != NULL)
    {
        if(strcmp(temp->file_name,filename) == 0)
        {
            if(*head == temp)
            {
                *head = temp->link;
            }
            else
            prev->link = temp->link;
            free(temp);
        }
        prev = temp;
        temp = temp->link;
    }
}
