#include"index.h"
int cflag;
int uflag;

int read_validation(int argc, char *argv[], file_list **files)
{
    //argc should be greater then 1
    if(argc < 2)
    {
        printf("⚠️  Argument Failed pass '.txt' file\n");
        return FAILURE;
    }

    for(int i=1;i < argc;i++)
    {
        //file extension must be ".txt"
        char *filename = argv[i];
        char *ext = strrchr(filename,'.');
            
        if(ext == NULL || strcmp(ext,".txt") != 0)
        {
            printf("⚠️  The file must contain '.txt' extension\n");
            continue;
        }

        //file should present in current woring directory
        FILE *fp = fopen(filename, "r");  

        if (fp == NULL)
        {
            printf("🚫  File does NOT exist in the current working directory.\n");
            continue;
        }

        //file should not be dublicate
        if(is_duplicate(*files,filename) == FAILURE)
        {
            printf("⚠️  Duplicate file is not allowed\n");
           continue;
        }

        //file should not be empty
        fseek(fp, 0, SEEK_END); //move to end of file

        long size = ftell(fp);   // tells how many bytes are in the file

        if (size == 0) 
        {
            printf("🚫  The file is EMPTY.\n");
            continue;
        }

        fclose(fp);

        //if file exists insert it in Single linked list(insert at last)
        insert_at_last(files,filename);
    }

   

    return SUCCESS;
}

int Create_database(Hash *h_table,file_list **files)
{

    if(cflag == 1)
    {
        printf("⚠️  Create DataBase is done already\n");
        return FAILURE;
    }
    //open file from linked list
    file_list *flist = *files;

    if(flist == NULL)
    {
        printf("⚠️  No files provided\n");
        return FAILURE;
    }

    char word[50];

    //Traverse each file
    while(flist != NULL)
    {
        FILE *fp = fopen(flist->file_name,"r");
        if(fp == NULL)
        {
            printf("⚠️  Cannot open %s\n", flist->file_name);
            flist = flist->link;
            continue;
        }

         // Read every word
        while (fscanf(fp, "%s", word) != EOF)
        {
            int index = get_index(word[0]);
            Main_list *main = h_table[index].next_main;
            Main_list *prev_main = NULL;

             // Traverse main list to check if word already exists
            int word_found = 0;

            while(main != NULL)
            {
                if (strcmp(main->word, word) == 0)
                {
                    word_found = 1;
                    break;
                }

                prev_main = main;
                main = main->main_link;
            }

            // CASE 3 & 4 : WORD NOT FOUND → Create NEW main + subnode

            if (!word_found)
            {
                Main_list *new_main = create_mainnode(word, flist->file_name);

                // If first node in this index
                new_main->file_count = 1; // first file
                new_main->sub_link->word_count = 1;

                if(prev_main == NULL)  // first node in index
                    h_table[index].next_main = new_main;
                else
                    prev_main->main_link = new_main;

            }
            else
            {

                // WORD FOUND → Check filename inside sublist
                Sub_list *sub = main->sub_link;
                Sub_list *prev_sub = NULL;

                int filename_found = 0;

                while (sub != NULL)
                {
                    if(strcmp(sub->filename, flist->file_name) == 0)
                    {
                        filename_found = 1;
                        break;
                    }

                    prev_sub = sub;
                    sub = sub->next_sub;
                }

                // CASE 1 → Word same, Filename same → increment word_count
                if (filename_found)
                {
                    sub->word_count++;
                }
                else
                {

                    // CASE 2 → Word same, Filename different
                    // Add a NEW subnode & increment file_count
                    Sub_list *new_sub = create_subnode(flist->file_name);
                    new_sub->word_count = 1;
                    if(prev_sub == NULL)
                    {
                        main->sub_link = new_sub;  // first sub node
                    }
                    else
                    {
                        prev_sub->next_sub = new_sub;
                    }
                    main->file_count++;
                }
            }
        }

        fclose(fp);
        flist = flist->link;
    }

    cflag = 1;
    return SUCCESS;
}

int Search_Data(Hash *HT)
{
    if(cflag == 0 && uflag == 0)
    {
        printf("⚠️  Database not yet created\n");
        return FAILURE;
    }
    char search_word[20];
    printf("Enter the word to search: ");
    scanf("%s",search_word);

    int i = get_index(search_word[0]);

    Main_list *main_temp = HT[i].next_main;

    while (main_temp != NULL)
    {
        if(strcmp(main_temp->word,search_word) == 0)
        {
            printf("\n ✔️  WORD FOUND\n\n");
            printf("Word : %s\nFile_count : %d\n",
                main_temp->word,
                main_temp->file_count);

            Sub_list *sub_temp = main_temp->sub_link;

            printf("------------------------------\n");
            printf("Filename        Word_Count\n");
            printf("------------------------------\n");
            while(sub_temp != NULL)
            {
                printf("%5s %10d\n",sub_temp->filename,sub_temp->word_count);
                sub_temp = sub_temp->next_sub;
            }
            printf("-------------------------------\n");
            return SUCCESS;
        }
    
        main_temp = main_temp->main_link;;
    }
    printf("⚠️  User entered word not found\n");
    return FAILURE;
}

int Display(Hash *HT)
{
    if(cflag == 0 && uflag == 0)
    {
        printf("⚠️  Database not yet created\n");
        return FAILURE;
    }

    printf("\n🖥️  DISPLAYING THE CONTENTS:\n");
    printf("--------------------------------------------------------------------\n");
    printf(" INDEX |      WORD    | FILE_COUNT |   FILE NAME  |   WORD_COUNT\n");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < 28; i++)
    {
        Main_list *main = HT[i].next_main;

        if (main == NULL)
            continue;   // Skip empty index

        // Traverse main list at this index
        while (main != NULL)
        {
            Sub_list *sub = main->sub_link;

            if(sub != NULL)
            {

                // For the first subnode – print index, word, file_count
                printf("%5d | %13s | %10d | %12s | %10d\n",
                    i,
                    main->word,
                    main->file_count,
                    sub->filename,
                    sub->word_count);

                sub = sub->next_sub;
            }

            // For remaining subnodes – index, word & file_count must be blank
            while (sub != NULL)
            {
                printf("      |               |            | %12s | %10d\n",
                    sub->filename,
                    sub->word_count);

                    sub = sub->next_sub;
            }

            main = main->main_link; // Move to next word
            printf("---------------------------------------------------------------------\n");
            
        }
    }
    return SUCCESS;
}

int Save_Database(Hash *HT,file_list *files)
{
    
    if(cflag == 0 && uflag == 0)
    {
        printf("🚫  Database not yet created\n");
        return FAILURE;
    }
    char fname[50];
    FILE *fp;
    
    while(1)
    {
        printf("Enter the file name to save: ");
        scanf("%s",fname);

        char *ext = strrchr(fname,'.');
                
        if(ext == NULL || strcmp(ext,".txt") != 0)
        {
            printf("⚠️  The file must contain '.txt' extension\n");
            continue;
        }
        
            
        fp = fopen(fname, "r"); // open for reading
        if(fp == NULL)
        {
            if (write_database_to_file(fname, HT) == SUCCESS)
                    return SUCCESS;
                else
                    return FAILURE;
        }
        else
        {
            fseek(fp, 0, SEEK_END);
            long size = ftell(fp);
            fclose(fp);
        
            char overwrite;
            if (size != 0)
            {
                // file is not empty 
                printf("⚠️  File exists but is not empty\n");
        
                printf("Do you want to overwrite ?(Y/N): ");
                scanf(" %c", &overwrite);

                if (overwrite == 'Y' || overwrite == 'y')
                {
                    if (write_database_to_file(fname, HT) == SUCCESS)
                        return SUCCESS;
                    else
                        return FAILURE;
                }
                else
                {
                    printf("🚫  Choose another filename.\n\n");
                    continue; //repeat step 1
                }
            }
            else
            {
                if (overwrite == 'Y' || overwrite == 'y')
                {
                    if (write_database_to_file(fname, HT) == SUCCESS)
                        return SUCCESS;
                    else
                        return FAILURE;
                }
            }
        }
    }
}

int Update_Database(Hash *HT,file_list **files)
{
    if(uflag == 1)
    {
        printf("⚠️  Update DataBase is done already\n");
        return FAILURE;
    }
    else if(cflag == 1)
    {
        printf("❌  Can't able to update database after creating database\n");
    }

    //checking hashtable empty or not
    for(int i = 0; i < 28; i++)
    {
        if(HT[i].next_main != NULL)
        {
            printf("❌  ERROR: Hashtable already contains data.\n");
            return FAILURE; 
        }
    }

    //if not empty update database

    char fname[30];
    FILE *fp;

ASK_FILE:
    printf("Enter the database file to UPDATE: ");
    scanf("%s", fname);

     //validating filename

    char *ext = strrchr(fname, '.');
    if(ext == NULL || strcmp(ext, ".txt") != 0)
    {
        printf("❌  ERROR: File must contain .txt extension\n");
        goto ASK_FILE;
    }

    fp = fopen(fname, "r");
    if(fp == NULL)
    {
        printf("❌  ERROR: File does NOT exist in directory.\n");
        goto ASK_FILE;
    }

 
    // Check START of file for '#'
    fseek(fp, 0, SEEK_SET);
    char start = fgetc(fp);

    if (start != '#')
    {
        printf("❌  ERROR: Backup file does NOT start with '#'\n");
        return FAILURE;
    }

    // Check END of file for '#'
    fseek(fp, -2, SEEK_END);
    char end = fgetc(fp);

     if (end != '#')
        {
            printf("❌  ERROR: Backup file does NOT end with '#'\n");
            return FAILURE;
        }

        int index=0;
    fseek(fp,0,SEEK_SET);
    while(fscanf(fp, "#%d;", &index) != EOF)
    {
        int  fcount;
        char word[20];

        //Read starting #
        fscanf(fp,"%[^;];%d;",word, &fcount);  
        
        //Create main node 
        Main_list *m = malloc(sizeof(Main_list));
        strcpy(m->word, word);
        m->file_count = fcount;
        m->sub_link = NULL;

        //Insert main node into hashtable
        m->main_link = HT[index].next_main;
        HT[index].next_main = m;

        for (int i = 0; i < fcount; i++)
        {
            char fname_temp[20];
            int wcount;

            fscanf(fp, "%[^;];%d;", fname_temp, &wcount);
            removefiles(files,fname_temp);
            // Create sub node
            Sub_list *s = malloc(sizeof(Sub_list));
            strcpy(s->filename, fname_temp);
            s->word_count = wcount;
            s->next_sub = NULL;

            // Insert subnode
            if (m->sub_link == NULL)
                m->sub_link = s;
            else
            {
                Sub_list *t = m->sub_link;
                while (t->next_sub)
                    t = t->next_sub;
                t->next_sub = s;
            }
        }
        fscanf(fp,"#\n");
    }
    
    fclose(fp);
    uflag = 1;
    return SUCCESS;  
}
