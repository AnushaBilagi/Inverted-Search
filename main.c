/*
Name : Anusha
Date : 11-12-2025
Project : Inverted Search
*/

#include"index.h"

int main(int argc,char *argv[])
{
    int option, data, key;

    file_list *files = NULL;  //initialize the header to NULL
    read_validation(argc,argv,&files);
    print_list(files);
    if(files == NULL)
    {
        printf("\n ❌  Read & Validation failled\n");
        return 0;
    }
    else
    {
        printf("\nRead & Validation done Successfully...✅\n");
    }

    //creating hash table
    Hash h_table[28];

    for (int i = 0; i < 28; i++)
    {
        h_table[i].index = i;
        h_table[i].next_main = NULL;
    }


    while(1)
	{
        printf("1. Create DataBase\n2. Search DataBase\n3. Display DataBase\n4. Save DataBase\n5. Update DataBase\n6. Exit\n");
        option =0;
		/*ask user options*/
        printf("Enter the Option: ");
        __fpurge(stdin);
		scanf("%d", &option);
		
		switch (option)
		{
			case 1:
                if(Create_database(h_table,&files) == FAILURE)
                {
                    printf("❌  Creating of Data Base is failled\n");
                }
                else
                {
                    printf("Creating of Data Base is done Successfully...✅\n");
                }
                break;

            case 2:
                if(Search_Data(h_table) == SUCCESS)
                {
                    printf("Searching of word is done Successfully...✅\n");
                }
                else
                {
                    printf("❌  Searching of word is failled\n");
                }
                break;

            case 3:
                if(Display(h_table) == FAILURE)
                {
                    printf("❌  Displaying of DataBase is failled\n");
                }
                else
                {
                    printf("Displaying of DataBase is done successfully...✅\n");
                }
                break;

            case 4:
                if(Save_Database(h_table,files) == SUCCESS)
                {
                    printf("Saving of Database is done successfully...✅\n");
                }
                else
                {
                    printf("❌  Saving of Database is failled\n");
                }
                break; 

            case 5:
                if(Update_Database(h_table,&files) == SUCCESS)
                {
                    printf("Updating of Database done successfully...✅\n");
                }
                else
                {
                    printf("❌  Updating of Database is failled\n");
                }
                break;

            case 6:
                printf("ThankYou...😊\n");
                return SUCCESS;
                break;

            default:
                printf("⚠️  Enter proper choice !!\n");
				break;
        }
    
    }
    return 0;
}