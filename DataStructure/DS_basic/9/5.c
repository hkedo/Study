#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG
#define MAX 50
#define NAME_SIZE 64
#define PHONE_SIZE 20

enum MENU{ADD, DELETE, FIND, PRINT, QUIT};

typedef struct Phone
{
	char name[NAME_SIZE];
	char phone[PHONE_SIZE];
}Phone;


int put_data(char*, char*);
void write_data(FILE *fp);
void read_data(FILE *fp);
void add();
void delete(char *);
int find(char *);
int choice_function(char *);
void print();

static int phone_size=0;
static Phone phone[MAX]; //배열선언

int main(int argc, char *argv[])
{
	FILE *fp;
	int ch;
	char choice[10];
	char find_name[NAME_SIZE];
	int find_index;
	
	if(argc<2)
	{
		fprintf(stderr,"%s filename \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if((fp = fopen(argv[1], "a+"))==NULL)
	{
		fprintf(stderr, "file not found\n");
		exit(EXIT_FAILURE);
	}

	read_data(fp);

	while(1)
	{
		printf("add, delete, find, print, quit ? ");
		fgets(choice,10,stdin);
		ch = choice_function(choice);
		
		if(ch == -1)
		{
			printf("sorry.. rechoice please! \n");
			continue;
		}

		switch(ch)
		{
			case ADD:
				add();
				break;
			case DELETE:
				printf("name : ");
				fgets(find_name,NAME_SIZE, stdin);
				find_name[strlen(find_name)-1]='\0';
				delete(find_name);
				break;
			case FIND:
				printf("name : ");
				fgets(find_name,NAME_SIZE, stdin);
				find_name[strlen(find_name)-1]='\0';
				find_index=find(find_name);

				if(find_index == -1)
				{
					printf("not found\n");
				}
				else
				{
					printf("------------find !!---------\n");
					printf("name : %s \n", phone[find_index].name);
					printf("phone number  : %s \n", phone[find_index].phone);
				}
				
				break;
			case PRINT:
				print();
				break;
			case QUIT:
				write_data(fp);
				fclose(fp);
				return 0;
		}
	}
	return 0;
}

int choice_function(char *choice)
{
	int result;
	
	if(!strcmp("add\n",choice))
		result=0;
	else if(!strcmp("delete\n", choice))
		result=1;
	else if(!strcmp("find\n", choice))
		result=2;
	else if(!strcmp("print\n", choice))
		result=3;
	else if(!strcmp("quit\n", choice))
		result=4;
	else
		result=-1;

	return result;
}
void read_data(FILE *fp)
{
	
	int i=0;
	char name[NAME_SIZE];
	char phone[PHONE_SIZE];

	fseek(fp,0L,SEEK_SET);
	
	while(fscanf(fp, "%s %s", name, phone) >= 2)
	{
#ifdef DEBUG
		while(name[i])
		{
			printf("[DEBUG] name : 0x%X \n", name[i++]);
		}
#endif
		put_data(name, phone);
	}

	printf("file read complete ! \n");
}

int put_data(char * n, char* p)
{
	Phone * ph = &(phone[phone_size]);

	if(phone_size > MAX-1)
	{
	#ifdef DEBUG
		printf(" [DEBUG]put data if 문 걸림 \n");
	#endif
		printf("sorry.. full!!  \n");
		return -1;
	}

	
	#ifdef DEBUG
		printf(" [DEBUG]put data return 0\n");
	#endif

	strcpy(ph->name, n);
	strcpy(ph->phone, p);

	phone_size++;

	return 0;
}

void write_data(FILE *fp)
{
	int i;
	fseek(fp, 0L, SEEK_SET);
	for(i=0;i<phone_size;i++)
	{
		fprintf(fp,"%s %s\n",phone[i].name, phone[i].phone);
	}

	printf("file write complete ! \n");
}

void add() //phone size 는 등록된것보다 +1
{
	char reply;
	char name[NAME_SIZE];
	char phone[PHONE_SIZE];

	while(1)  //phone_size 증가 
	{
		printf("name : ");
		fgets(name,NAME_SIZE,stdin);
		printf("phone : ");
		fgets(phone,PHONE_SIZE,stdin);
		
		name[strlen(name)-1]='\0';
		phone[strlen(phone)-1]='\0';

	#ifdef DEBUG
		printf(" [DEBUG]put data 호출전\n");
	#endif
	#ifdef DEBUG
		printf(" [DEBUG]put data 호출후\n");
	#endif
		if(put_data(name, phone) == 0)
		{
			printf("add complete ! \n");

			printf("[%d/%d]add continue? (y/n) ",phone_size, MAX);
			reply=getchar();
			getchar();
		
			if((reply != 'y'))
				break;
		}
		else
			break;
	}

}

void delete(char* name)
{
	int i;

	int delete_index = find(name);

	if(delete_index != -1)
	{
		for(i=delete_index ; i<phone_size; i++)
			phone[i] = phone[i+1];
		phone_size--;
		printf("delete complete !!\n");
	}
	else
		printf("fail not found !! \n");

	return;
}

int find(char *name)  //-1 반환시 못찾음
{
	int i;
	int index = -1;

	for(i=0; i<phone_size; i++)
	{
		if(!strcmp(phone[i].name, name))
		{		
			index=i;
		}
	}

	return index;  // override name ->  find last index
}

void print()
{
	int i;
	for(i=0; i<phone_size; i++)
	{
			printf("------------[%d]--------------\n", i+1);
			printf("name : %s \n", phone[i].name);
			printf("phone number  : %s \n", phone[i].phone);
	}
}
