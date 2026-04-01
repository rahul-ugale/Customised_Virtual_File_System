// CVFS Project 
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h> 

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros
//
////////////////////////////////////////////////////////////////////////////////////////////////

//Maximum file size that we allow in project---
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0
#define REGULARFILE 1
#define SPECIALFILE 2

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros For Error Handling
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name : BootBlock
//  Description    : Holds the information to boot the OS
//
////////////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name : SuperBlock
//  Description    : Holds the information about the file system
//
////////////////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name : Inode
//  Description    : Holds the information about the file
//
////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack (1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;           // 200pages book
    int ActualFileSize;     // used pages 70 from 200 book pages
    int FileType;
    int ReferenceCount;
    int Permission;         // for file permission 
    char *Buffer;           // file data
    struct Inode *next;     //  Self Refrential pointer
};

typedef struct Inode INODE;
typedef struct Inode* PINODE;
typedef struct Inode** PPINODE;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name : FileTable
//  Description    : Holds the information about opened file
//
////////////////////////////////////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;     // from which offset to read
    int WriteOffset;    // from which offset to write
    int Mode;           // access file using given mode
    PINODE ptrinode;
};
typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name : UAREA
//  Description    : Holds the information about process
//
////////////////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Global variables or objects used in the project
//
////////////////////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : InitialiseUAREA
//  Description    : it is used to initialise UAREA member
//  Author         : Rahul Balasaheb Ugale  
//  Date           : 13/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName,"Myexe");

    int i = 0;
    for(i = 0; i < MAXOPENFILES; i ++)
    {
        uareaobj.UFDT[i] = NULL;       // initialise pointer to NULL
    }
    
    printf("MINI CVFS : UAREA  gets initialised successfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : InitialiseSuperBlock
//  Description    : to initialise Super block members
//  Author         : Rahul Balasaheb Ugale   
//  Date           : 13/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("MINI CVFS : Super block gets initialised successfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : CreateDILB
//  Description    : It is used to create linkedlist of inodes
//  Author         : Rahul Balasaheb Ugale  
//  Date           : 13/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount =  
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)        // LL is empty
        {
           head = newn;
           temp = head;  
        }
        else                    // LL contains atleast one node
        {
            temp->next = newn;
            temp = temp->next;
        }

    }

    printf("MINI CVFS : DILB created successfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : StartAuxillaryDataInitilaisation
//  Description    : it is used to call all such functions which are used to initialise
//                   auxillary data   
//  Author         : Rahul Balasaheb Ugale   
//  Date           : 13/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitilaisation()
{
    strcpy(bootobj.Information,"Booting process of Marvellous CVFS is done");
    printf("\n");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("MINI CVFS : Auxillary data initilaised successfully\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : DisplayHelp
//  Description    : it is used to display the help page
//                   auxillary data   
//  Author         : Rahul Balasaheb Ugale   
//  Date           : 14/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("--------------------------------------------------------------------------------\n"); 
    printf("------------------------------MINI CVFS Help Page-------------------------------\n");
    printf("--------------------------------------------------------------------------------\n"); 

    printf("man    : It is used to display manual page\n");
    printf("clear  : It is used to clear the terminal\n");
    printf("creat  : It is used to create new file\n");
    printf("write  : It is used to write data into file\n");
    printf("read   : It is used to read the data from file\n");
    printf("stat   : It is used to display statistical information\n");
    printf("unlink :It is used to delete the file\n");
    printf("exit   : It is used to terminate Marvellous CVFS\n");

    printf("--------------------------------------------------------------------------------\n"); 
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : ManPageDisplay
//  Description    : it is used to display man page
//                   auxillary data   
//  Author         : Rahul Balasaheb Ugale   
//  Date           : 14/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp("ls",Name) == 0)
    {
        printf("About  : It is used to list the names of all files\n");
        printf("Usage  : ls\n");
    }
    else if (strcmp("man",Name) == 0)
    {
        printf("About  : It is used to display manual page\n");
        printf("Usage  : man command_name\n");
        printf("command_name : It is the name of the command\n"); 
    }
    else if(strcmp("exit",Name) == 0)
    {
        printf("About  : It is used to terminate the shell\n");
        printf("Usage  : exit\n");   
    }
    else if(strcmp("clear",Name) == 0)
    {
        printf("About  : It is used to clear the shell\n");
        printf("Usage  : clear\n");   
    }
    else
    {
        printf("No manual entry for %s\n",Name);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : IsFileExist
//  Description    : It is used to check whether file is already exist or not
//  Input          : It accepts file name                     
//  Output         : It return file true or false  
//  Author         : Rahul Balasaheb Ugale     
//  Date           : 16/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char *name   //File name
               )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name,temp-> FileName)== 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    } 
    return bFlag;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : CreateFile
//  Description    : It is used to create new regular file
//  Input          : It accepts file name and permissions                    
//  Output         : It return file descriptor   
//  Author         : Rahul Balasaheb Ugale      
//  Date           : 16/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(
                    char *name,         //Name of new file
                    int permission      //Permission for that file
              )       
{
    PINODE temp = head;
    int i = 0;
    printf("Total number of INODES remaning : %d\n",superobj.FreeInodes);

    //if name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    //if the permission value is wrong
    //permission -> 1 -> read
    //permission -> 2 -> write
    //permission -> 3 -> read + write
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    //if the inodes are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    //if file is already present
    if(IsFileExist(name)== true)
    {
        return ERR_FILE_ALREADY_EXIST; 
    }

    // Search empty Inode

    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }
        temp = temp->next;
    }
    
    if(temp == NULL)
    {
        printf("There is no inode\n");
        return ERR_NO_INODES;
    }

    // Search for empty UFDT entry

    // Note : 0 1 2 are rakhiv
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    //UFDT is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for file table

    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise file table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    //Connect file table with INODE
    uareaobj.UFDT[i]->ptrinode = temp;

    //Initialise elements of INODE
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXOPENFILES;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;
    
    //Allocate memory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer =(char*)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i;   //File Descriptor

}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : LsFile
//  Description    : It is used to list all files
//  Input          : Nothing                    
//  Output         : Nothing  
//  Author         : Rahul Balasaheb Ugale  
//  Date           : 16/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("--------------------------------------------------------------------------------\n"); 
    printf("--------------------------MINI CVFS Files Information---------------------------\n");
    printf("--------------------------------------------------------------------------------\n"); 

    while(temp != NULL)
    {   
        if(temp-> FileType != 0)
        {
            printf("%d\t%s\t%d\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        temp = temp->next;
    }

    printf("--------------------------------------------------------------------------------\n"); 
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : Unlink
//  Description    : It is used to delete the file
//  Input          : File name                    
//  Output         : Nothing  
//  Author         : Rahul Balasaheb Ugale  
//  Date           : 22/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

int UnlinFile(
                char *name
             )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Travel the UFDT
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                // Deallocate memory of Buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                // Recent all values of inode
                // Dont deallocate memory of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName, '\0', sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                // Deallocate memory of filetable 

                free(uareaobj.UFDT[i]);

                //Set NULL to UDFT
                uareaobj.UFDT[i] = NULL;

                // Increment free inodes count
                superobj.FreeInodes++;

                break;  // Imp


            }// End of if
        }// End of if
    }//End of for

    return EXECUTE_SUCCESS;


}// End of Function

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : WriteFile()
//  Description    : It is used to write the data into the file
//  Input          : File descriptor
//                   Address of Buffer which contains data
//                   Size of data that we want to write                    
//  Output         : Number of bytes successfully written  
//  Author         : Rahul Balasaheb Ugale  
//  Date           : 22/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

int WriteFile(
                int fd,
                char *data,
                int size
            )
{
    printf("File Descriptor : %d\n",fd);
    printf("Data that we want to write : %s\n", data);
    printf("Number of bytes that we want to write : %d\n",size);

    // Invalid fd
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Fd points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // There is no permission write
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]-> WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]-> WriteOffset, data,size);

    // Update the writeoffset
    uareaobj.UFDT[fd]->WriteOffset =  uareaobj.UFDT[fd]->WriteOffset + size;

    // Update the actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  : ReadFile()
//  Description    : It is used to read the data from the file
//  Input          : File descriptor
//                   Address of empty Buffer which contains data
//                   Size of data that we want to read                    
//  Output         : Number of bytes successfully read  
//  Author         : Rahul Balasaheb Ugale   
//  Date           : 22/01/2026
//
////////////////////////////////////////////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size  
            )
{   
    // Invalid fd
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd]== NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Filter for permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset < size))
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the data

    strncpy(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]-> ReadOffset, size);

    // Update the read offset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Entry Point Function Of The Project
//
////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{   
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    char InputBuffer[MAXFILESIZE] = {'\0'};
    char *EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;    //seperate for create file function

    StartAuxillaryDataInitilaisation();

    printf("--------------------------------------------------------------------------------\n"); 
    printf("-------------------------MINI CVFS started successfully-------------------------\n");
    printf("--------------------------------------------------------------------------------\n"); 


    //Infinite listening shell
    while(1)
    {
        fflush(stdin);

        strcpy(str,"");

        printf("\nMarvellous CVFS : > ");
        fgets(str,sizeof(str),stdin);

        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3],Command[4]);

        fflush(stdin);

        if(iCount == 1)
        {
            // Marvellous CVFS : > exit
            if(strcmp("exit",Command[0]) == 0) //comparing exit with the given string by user
            {
                printf("Thank You For Using MINI CVFS\n");

                printf("Deallocating all the allocated resources\n");

                break;
            }
            else if(strcmp("ls",Command[0]) == 0)
            {
                LsFile();
            }
            else if(strcmp("help",Command[0]) == 0)
            {
                DisplayHelp();
            }
            else if(strcmp ("clear",Command[0]) == 0)
            {
                    #ifdef _WIN32
                        system("cls");
                    #else
                        system("clear");
                    #endif
            }
            
        } //End of else if 1       
        else if (iCount == 2)
        {
            if(strcmp("man",Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }

            //Marvellous CVFS : > unlinll Demo.txt
            if(strcmp("unlink",Command[0]) == 0)
            {
                iRet = UnlinFile(Command[1]);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to delete as there is no such file\n");

                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets successfully deleted\n");
                }
            }

            // Marvellous CVFS : > write 2
            else if(strcmp("write",Command[0])== 0)
            {
                printf("Enter the data that you want to write :\n");
                fgets(InputBuffer,MAXFILESIZE,stdin);

                iRet = WriteFile(atoi(Command[1]),InputBuffer, strlen(InputBuffer)-1);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters\n");
                }
                else if(iRet ==  ERR_FILE_NOT_EXIST)
                {
                    printf("Error : There is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to write as there is no space left\n");
                }
                else
                {
                    printf("%d bytes successfully written\n",iRet);
                }
            }
            else 
            {
                printf("There is no such command\n");
            }
        }//End of else if 2
        else if (iCount == 3)
        {   
            if(strcmp("creat",Command[0]) == 0)
            {
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create the file as parameters are invalid\n");
                    printf("Please refer man page\n");
                }

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create  file as there are no INODE\n");
                    
                }

                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file because the file is already present\n");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file \n");
                    printf("Max opended files limit reached\n");
                }
              
                else
                {

                }
                printf("File gets successfully created with FD %d\n",iRet);
                if(strcmp ("read",Command[0])== 0)
                {
                    EmptyBuffer = (char*)malloc(sizeof(atoi(Command[2])));

                    iRet = ReadFile(atoi(Command[1]), EmptyBuffer, atoi(Command[2]));

                    if(iRet == ERR_INVALID_PARAMETER)
                    {
                        printf("Error : Invalid paramrter\n");
                    }
                    
                    else if(iRet == ERR_FILE_NOT_EXIST)
                    {
                        printf("Error : file no exits\n");
                    }

                    else if(iRet = ERR_PERMISSION_DENIED)
                    {
                        printf("Error : Permission denied\n ");
                    }
                    else if(ERR_INSUFFICIENT_DATA)
                    {
                        printf("Error : Insufficient data\n");
                    }
                    else
                    {
                        printf("Read operation is successfull\n");
                        printf("Data from file is %s\n",EmptyBuffer);

                        free(EmptyBuffer);
                    }

                }
                else
                {
                    printf("There is no such command \n");
                }
            }
        }//End of else if 3
        else if (iCount == 4)
        {

        }//End of else if 4
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get information \n");
        }//En of else

    }//End of while


    return 0;
}//End of main
