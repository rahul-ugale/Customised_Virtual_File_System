////////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//
//  User Define Macros
//
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :  BootBlock
//  Description    :  Holds the information to boot the OS
//
////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :  SuperBlock
//  Description    :  Holds the information about the file system
//
////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :  Inode
//  Description    :  Holds the information about file
//
////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode *PINODE;
typedef struct Inode **PPINODE;

////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :  FileTable
//  Description    :  Holds the information about opened file
//
////////////////////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable *PFILETABLE;

////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :  UAREA
//  Description    :  Holds the information about Process file
//
////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

////////////////////////////////////////////////////////////////////////////////
//
//  Global variables or objects used in the project
//
////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  :  InitialiseUAREA
//  Description    :  It is used to initialise UAREA members
//  Author         :  Rahul Balasaheb Ugale
//  Date           :  13/01/2026
//
////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe");

    int i = 0;

    for (i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVFS : UAREA gets initialise succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  :  InitialiseSuperBlock
//  Description    :  It is used to initialise Super block members
//  Author         :  Rahul Balasaheb Ugale
//  Date           :  13/01/2026
//
////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super block gets initialise succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  :  CreateDILB
//  Description    :  It is used to create LinkedList of inodes
//  Author         :  Rahul Balasaheb Ugale
//  Date           :  13/01/2026
//
////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for (i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName, "\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if (temp == NULL) // LL is empty
        {
            head = newn;
            temp = head;
        }
        else // LL contains atlest 1 node
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("Marvellous CVFS : DILB created succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function Name  :  StartAuxillaryDataInitialisation
//  Description    :  It is used to call all such function which are used to
//                    initialise auxillary data
//  Author         :  Rahul Balasaheb Ugale
//  Date           :  13/01/2026
//
////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialisation()
{
    strcpy(bootobj.Information, "Booting process of Marvellous CVFS is Done");

    printf("%s\n", bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("Marvellous CVFS : Auxillary data initialise succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////
//
//  Entry Point Function of The Project
//
////////////////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[4][20];
    int iCount = 0;

    StartAuxillaryDataInitialisation();

    printf("-------------------------------------------------------------\n");
    printf("---------- Marvellous CVFS Started Succesfully --------------\n");
    printf("-------------------------------------------------------------\n");

    while (1)
    {
        fflush(stdin);

        strcpy(str, "");

        printf("\n Marvellous CVFS : > ");
        fgets(str, sizeof(str), stdin); // scanf("%s",str);

        
    }

    return 0;
}