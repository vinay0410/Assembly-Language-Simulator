#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct LINKED
{
	char part[30];
	struct LINKED* next;
};
typedef struct LINKED LINKED;

void insert_instructions (char file_name[]);
void display_instructions (char file_name[]);
int read_instructions (char file_name[]);

void partition (LINKED* division, char text[]);
void display_linked_list (LINKED* division);

void opcode_check (LINKED* division);

int check_min_dist (char file_name[], char instruction_code[]);
int edit_distance (int len_1, int len_2, int matrix[][30], char str1[], char str2[]);
int diff (int len_1, int len_2, char str1[], char str2[]);
void initialize_matrix (int len_1, int len_2, int matrix[][30], char str1[], char str2p[]);
void display_matrix (int len_1, int len_2, int matrix[][30]);
int min (int a, int b);	

int check_integer (char test[30], int high);
int check_register (char test[30]);
int count_nodes (LINKED* temp);
int check_indirect (char test[30]);




int main ()
{
	char file_name[30] = "instructions24";
	//LINKED* division = (LINKED*)malloc(sizeof(LINKED));

	insert_instructions (file_name);
	display_instructions (file_name);
	read_instructions (file_name);
	//char text[30] = "@@11";
	//partition (division, text);
	//display_linked_list (division);
	//int total = count_nodes (division);

	//int p = check_indirect (text);
	//printf("%d\n", total);

	/*char test[30] = "#321a31";
	int p = check_integer (test, 124);*/

	//printf("%d\n", p);
	
	/*LINKED* pt = division;

	while (pt!=NULL) {
		printf("%s	", pt->part);
		pt = pt->next;
	}*/
	
}

void insert_instructions (char file_name[])
{
	int i, N;
	printf ("how many instructions you wish to insert\n");
	scanf ("%d", &N);
	getchar ();

	char code[30];
	FILE* instructions = fopen (file_name, "a");

	while (N--)
	{
		//scanf ("%s", code);
		fgets (code, 30, stdin);

		for (i=0;i<30;i++)
		{
			if (code[i] == '\n')
				code[i] = '\0';
		}
		fwrite (code, 30*sizeof(char), 1, instructions);
	}

	fclose (instructions);

}

void display_instructions (char file_name[])
{
	FILE* instructions = fopen (file_name, "r");
	char code[30];
	int i = 0;

	printf("\n");

	while (fread (code, 30*sizeof(char), 1, instructions) != 0)
	{
		printf ("%s\t\t\t%d\n", code, ++i);
		/*LINKED* division = (LINKED*)malloc(sizeof(LINKED));

		partition (division, code);
		display_linked_list (division);*/
	}

	printf ("\n");
	fclose (instructions);
}

void partition (LINKED* division, char temp[])
{
	int i = 0;
	int j = 0;
	
	//LINKED* head;
	//if(division == NULL)
	//	division = (LINKED*)malloc(sizeof(LINKED));
	while (i < strlen(temp))
	{
		j = 0;
		while (i < strlen(temp) && temp[i] != ' ')
		{
			division->part[j] = temp[i];

			//printf ("%c", division->part[j]);

			i++;
			j++;
		}
		division->part[j] = '\0';
		//printf("\n");
		while (i < strlen(temp) && temp[i] == ' ')
			i++;
		if(i<strlen(temp))
		{
			division->next = (LINKED*)malloc(sizeof(LINKED));
			division = division->next;
		}
	}
	division = NULL;
	//return head;
}

void display_linked_list (LINKED* division)
{
	int i = 0;
	while (division != NULL)
	{
		printf("%s %d\n", division->part, i++);

		division = division->next;
	}
}

int read_instructions (char file_name[])
{
	FILE* instructions = fopen (file_name, "r");
	char file1[30] = "opcodes";
	char code[30];
	int line = 1;
	int nodes;

	while (fread (code, 30*sizeof(char), 1, instructions) != 0)
	{
		//printf ("%s %d\n", code, (int)strlen (code));
		LINKED* division = (LINKED*)malloc(sizeof(LINKED));

		partition (division, code);
		//printf("partition done\n");

		nodes = count_nodes (division);
		//printf("nodes counted\n");

		if (check_min_dist (file1, division->part) == 1)
		{
			printf ("\n error at :: [%d : 1]\n", line);
			//return 0;
		}
		else if (strcmp ("TKOFF", division->part) == 0)
		{
			if (nodes > 1)
				
				printf ("too many arguements for take off \n error at :: [%d : 2] \n", line);
			
		}
		else if (strcmp ("LAND", division->part) == 0)
		{
			if (nodes > 1)
				printf ("too many arguements for landing\n error at :: [%d : 2] \n", line);
		}
		else if (strcmp ("OBAV", division->part) == 0)
		{
			if (nodes > 1)
				printf ("no arguement needed for obsacle avoidance\n error at :: [%d : 2] \n", line);
		}
		else if (strcmp ("SPER", division->part) == 0)
		{
			if (nodes > 1)
				printf ("no arguement needed for searching a person\n error at :: [%d : 2] \n", line);
		}
		else if (strcmp ("HLT", division->part) == 0)
		{
			if (nodes > 1)
				printf ("no arguement needed for stoping the machine\n error at :: [%d : 2] \n", line);
		}
		else if (strcmp ("JNE", division->part) == 0)
		{
			if (nodes < 2)
				printf ("too less arguements for jumping\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf(" too many arguements for jumping\n error at :: [%d : 3] \n", line);
			else if (check_integer(division->next->part, 255) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
		}
		else if (strcmp ("JMP", division->part) == 0)
		{
			if (nodes < 2)
				printf ("too less arguements for jumping\n error at :: [%d : 0]\n", line);
			else if (nodes > 3)
				printf(" too many arguements for jumping\n error at :: [%d : 3] \n", line);
			else if (check_integer(division->next->part, 255) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
		}
		else if (strcmp ("FLWT", division->part) == 0)
		{
			if (nodes < 2)
				printf ("too less arguements for flying till a time\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf(" too many arguements for flying till a time\n error at :: [%d : 3] \n", line);
			else if (check_integer(division->next->part, 255) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
		}
		else if (strcmp ("JGE", division->part) == 0)
		{
			if (nodes < 2)
				printf ("too less arguements for jumping\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf(" too many arguements for jumping\n error at :: [%d : 3] \n", line);
			else if (check_integer(division->next->part, 255) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
		}
		else if (strcmp ("JLE", division->part) == 0)
		{
			if (nodes < 2)
				printf ("too less arguements for jumping\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf(" too many arguements for jumping\n error at :: [%d : 3] \n", line);
			else if (check_integer(division->next->part, 255) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
		}
		else if (strcmp ("GAHT", division->part) == 0)
		{
			if (nodes < 2)
				printf ("too less arguements to gain height\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf(" too many arguements to gain height\n error at :: [%d : 3] \n", line);
			else if (check_integer(division->next->part, 255) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
		}
		else if (strcmp ("MSF", division->part) == 0)
		{
			if (nodes < 2)
				printf ("too less arguements for MSF at line **%d**\n", line);
			else if (nodes > 3)
				printf(" too many arguements for MSF at \n [%d : 3] \n", line);
			else if (check_integer(division->next->part, 255) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement at \n [%d : 2]\n", line);				
		}
		else if (strcmp ("ROT", division->part) == 0)
		{
			if (nodes < 2)
				printf ("too less arguements for rotation\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf(" too many arguements for rotation\n error at :: [%d : 3] \n", line);
			else if (check_integer(division->next->part, 256) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
		}
		else if (strcmp ("SUB", division->part) == 0)
		{
			if (nodes < 3)
				printf("too less arguements for subtraction\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf("too many arguements for subtraction\n error at :: [%d : 4]\n", line);
			else if (check_integer(division->next->part, 15) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
			else if (check_register(division->next->next->part) == 0 && check_indirect(division->next->next->part) == 0) 
				printf ("no access is available to this kind of register or this register doesn't exist \n error at :: \n [%d : 3]\n", line);				
		}
		else if (strcmp ("ADD", division->part) == 0)
		{
			if (nodes < 3)
				printf("too less arguements for addition\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf("too many arguements for addition\n error at :: [%d : 4]\n", line);
			else if (check_integer(division->next->part, 15) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
			else if (check_register(division->next->next->part) == 0 && check_indirect (division->next->next->part) == 0) 
				printf ("no access is available to this kind of register or this register doesn't exist\n error at :: [%d : 3]\n", line);				
		}
		else if (strcmp ("MOV", division->part) == 0)
		{
			if (nodes < 3)
				printf("too less arguements for tranfering\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf("too many arguements for tranfering\n error at :: [%d : 4]\n", line);
			else if (check_integer(division->next->part, 15) == 0 && check_register(division->next->part) == 0 && check_indirect(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
			else if (check_register(division->next->next->part) == 0 && check_indirect(division->next->next->part) == 0) 
				printf ("no access is available to this kind of register or this register doesn't exist\n error at :: [%d : 3]\n", line);				
		}
		else if (strcmp ("MVTL", division->part) == 0)
		{
			if (nodes < 3)
				printf("too less arguements for movement\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf("too many arguements for movement\n error at :: [%d : 4]\n", line);
			else if (check_indirect (division->next->part) == 0 && check_register(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
			else if (check_indirect (division->next->next->part) == 0 && check_register(division->next->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement at :: [%d : 3]\n", line);				
		}
		else if (strcmp ("CMP", division->part) == 0)
		{
			if (nodes < 3)
				printf("too less arguements for comparison\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf("too many arguements for comparison\n error at :: [%d : 4]\n", line);
			else if (check_indirect (division->next->part) == 0 && check_register(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
			else if (check_indirect (division->next->next->part) == 0 && check_register(division->next->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 3]\n", line);				
		}
		else if (strcmp ("GUL", division->part) == 0)
		{
			if (nodes < 3)
				printf("too less arguements for guidance\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf("too many arguements for guidance\n error at :: [%d : 4]\n", line);
			else if (check_indirect (division->next->part) == 0 && check_register(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
			else if (check_indirect (division->next->next->part) == 0 && check_register(division->next->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 3]\n", line);				
		}
		else if (strcmp ("SPTM", division->part) == 0)
		{
			if (nodes < 3)
				printf("too less arguements for sports mode\n error at :: [%d : 1]\n", line);
			else if (nodes > 3)
				printf("too many arguements for sports mode\n error at :: [%d : 4]\n", line);
			else if (check_indirect (division->next->part) == 0 && check_register(division->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 2]\n", line);				
			else if (check_indirect (division->next->next->part) == 0 && check_register(division->next->next->part) == 0) 
				printf ("wrong type of arguement or overflow of arguement\n error at :: [%d : 3]\n", line);				
		}


		//display_linked_list (division);
		line++;
	}

	fclose (instructions);
}

int edit_distance(int len_1, int len_2, int matrix[][30], char str1[], char str2[])
{
    if (matrix[len_1][len_2] != -1)
        return matrix[len_1][len_2];
    else
    {
        int temp1,temp2,temp3,temp4,temp5;

        /*temp1 = 100;
        if (str1[len_1] == str2[len_2])
        	temp1 = edit_distance (len_1-1, len_2-1, matrix, str1, str2) + 1;*/
        temp1 = edit_distance(len_1-1, len_2-1, matrix, str1, str2) + diff(len_1, len_2, str1, str2);
        temp2 = edit_distance(len_1-1, len_2, matrix, str1, str2) + 1;
        temp3 = edit_distance(len_1, len_2-1, matrix, str1, str2) + 1;
        temp4 = min(temp1, temp2);
        temp5 = min(temp2, temp3);

        matrix[len_1][len_2] = min (temp5, temp4);

        return matrix[len_1][len_2];
    }
}

int diff(int len_1, int len_2, char str1[], char str2[])
{
    if (str1[len_1] == str2[len_2])
        return 0;
    else
        return 1;
}

void initialize_matrix (int len_1, int len_2, int matrix[][30], char str1[], char str2[])
{
    int i,j;

    for (i=0;i<len_1;i++)
    {
        for (j=0;j<len_2;j++)
        {
            if (i==0)
                matrix[i][j] = j+1;
            else if (j==0)
                matrix[i][j] = i+1;
            else
                matrix[i][j] = -1;
        }
    }

    for (i=0;i<len_1;i++)
    {
        if (str1[i] == str2[0])
        {
            matrix[i][0]--;
            break;
        }
    }
    i++;
    while (i < len_1)
    {
        matrix[i][0]--;
        i++;
    }

    for (i=0;i<len_2;i++)
    {
        if (str2[i] == str1[0])
        {
            if (i != 0)
                matrix[0][i]--;
            break;
        }
    }
    i++;
    while (i < len_2)
    {
        matrix[0][i]--;
        i++;
    }
}

int check_min_dist (char file_name[], char instruction_code[])
{
	//printf("%s\n", player_name);

	FILE* opcodes = fopen (file_name, "r");
	char code[30];

	int min_distance=100;
	int matrix[strlen (instruction_code)][30];

	char mod_instruction[30]; 

	while (fread (code, 30*sizeof(char), 1, opcodes) != 0)
	{
		initialize_matrix (strlen (instruction_code), strlen (code), matrix, instruction_code, code);

		//int temp_dist = edit_distance (strlen(player_name)-1, strlen(player.name)-1, matrix, player_name, player.name);
		//printf("%d %s\n",temp_dist, player.name);

		if (min_distance > edit_distance (strlen(instruction_code)-1, strlen(code)-1, matrix, instruction_code, code)) 
		{
			min_distance = edit_distance (strlen(instruction_code)-1, strlen(code)-1, matrix, instruction_code, code);
			strcpy (mod_instruction, code);
		}

		//display_matrix (strlen (player_name), strlen (player.name), matrix);

		//break;
		//printf ("%s %d %s %d\n", instruction_code, (int)strlen(instruction_code), code, (int)strlen(code));
	}
	fclose (opcodes);

	if (min_distance == 0)
	{
		//printf("fine\n");
		return 0;
	}
	else
	{
		printf("do you mean %s", mod_instruction);
		return 1;
	}
}

void display_matrix (int len_1, int len_2, int matrix[][30])
{
    int i,j;

    for (i=0;i<len_1;i++)
    {
        for (j=0;j<len_2;j++)
            printf ("%d ", matrix[i][j]); 
        printf("\n");
    }
}

int min (int a, int b)
{
	if (a<=b)
		return a;
	else
		return b;
}

int check_integer (char test[30], int high)
{
	if (test[0] != '#')
		return 0;
	else
	{
		int i, last = strlen(test)-1;

		//printf("here it was\n");

		for (i=last;i>0;i--)
		{
			//printf("did it last\n");
			if ((int)test[i] < 48 || (int)test[i] > 57)
				return 0;
		}


		int j = 0;
		int sum = 0;
		for (i=last;i>0;i--)
		{
			sum += ((int)test[i]-48)*pow(10, j);
			j++;
		}

		//printf ("%d\n", sum);

		if (sum <= high)
			return 1;
		else
			return 0;
	}
}

int check_register (char test[30])
{
	if (test[0] != 'R')
		return 0;
	else
	{
		int i, last = strlen(test)-1;

		//printf("here it was\n");

		for (i=last;i>0;i--)
		{
			//printf("did it last\n");
			if ((int)test[i] < 48 || (int)test[i] > 57)
				return 0;
		}


		int j = 0;
		int sum = 0;
		for (i=last;i>0;i--)
		{
			sum += ((int)test[i]-48)*pow(10, j);
			j++;
		}

		//printf ("%d\n", sum);

		if (sum <= 15)
			return 1;
		else
			return 0;	
	}
}

int count_nodes (LINKED* temp)
{
	int i = 0;

	while (temp != NULL)
	{
		temp = temp->next;
		i++;
		//printf("%s %d\n",temp->part, i);
	}

	return i;
}

int check_indirect (char test[30])
{
	if (test[0] != '@' || test[1] != 'R')
		return 0;
	else
	{
		int i, last = strlen(test)-1;

		//printf("here it was\n");

		for (i=last;i>1;i--)
		{
			//printf("did it last\n");
			if ((int)test[i] < 48 || (int)test[i] > 57)
				return 0;
		}


		int j = 0;
		int sum = 0;
		for (i=last;i>1;i--)
		{
			sum += ((int)test[i]-48)*pow(10, j);
			j++;
		}

		//printf ("%d\n", sum);

		if (sum <= 15)
			return 1;
		else
			return 0;	
	}
}
