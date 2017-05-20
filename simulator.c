#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void readMachine(FILE* fp);
void sub (int* g_register1, int* g_register2);
void add (int* g_register1, int* g_register2);
void comparison (int* status_flag, int value1, int value2);
void jump_less_equal (int* status_flag, int line);
void jump_greater_equal (int* status_flag, int line);
void jump (int line);
void jump_not_equal (int* status_flag, int line);
void landing (int* altitude);
void move_register (int* g_register, int value);
void move_location (int* x_pos, int* y_pos, int value1, int value2);
void gain_height (int* altitude, int value);
void take_off (int* altitude);
int read_registers(void);
void comparision (int* status_flag, int value1, int value2);
int str2dec(char *str, int len);
int label2line(int labelno);
void write_registers(void);

int programCounter = 1;
int reg[30];
int *pint;
char space[2] = " ";

int main() {
	FILE* fp = fopen("machine.txt", "r");
	if (fp == NULL) {
		printf("File not found\n");
		return 0;
	}
	if (read_registers()) {
		printf("Registers Read\n");
	} else {
		printf("Registers file not found\n");
	}
	readMachine(fp);
	write_registers();
}

int str2dec(char *str, int len) {
	int i, base = 1, dec = 0;
	for (i=len - 1; i>=0; i--) {
		dec += (str[i]-48)*base;
		base *= 2;
	}
	return dec;
}

void readMachine(FILE* fp) {
	char inst[20]; 
	char read[30];
	char buff[6];
	printf("hi");
	char op[6];
	char* p;
	int dec, val, temp;
	FILE* fop = fopen("opcodes.txt", "r");
	printf("no\n");
	while(fscanf(fp, "%s", inst) == 1) {
		 strncpy(op, inst, 5);
		 op[5] = '\0';
		 fseek(fp, programCounter*17, SEEK_SET);
		 fseek(fop, 0, SEEK_SET);
		 while (fgets(read, 100, fop) != NULL) {
			
			read[strlen(read) - 1] = '\0';
			strncpy(buff, read, 5);
			buff[5] = '\0';
			if (strcmp(buff, op) == 0) {
				break;
			}
		
		}  
		p = strtok(read, space);		
		p = strtok(NULL, space);
		if (strcmp(p, "TKOFF") == 0) {
			printf("took off\n");
			pint = reg;
			take_off(pint);
		} else if (strcmp(p, "LAND") == 0) {
			printf("landed\n");
			pint = reg;
			landing(reg);
		} else if (strcmp(p, "GAHT") == 0) {
			pint = reg;
			strncpy(op, inst+5,3);
			val = str2dec(op, 3);
			printf("gh\n");
			if (val == 0) {
				gain_height(pint, str2dec(inst+8, 8));
				
			} else if (val == 1) {
				gain_height(pint, reg[str2dec(inst+8, 8) + 6]);

			} else if (val == 2) {
				//indirect addressing not supported

			}				

		} else if (strcmp(p, "MVTL") == 0) {
			pint = reg + 1;
			strncpy(op, inst + 5 ,3);
                        val = str2dec(op, 3);
			printf("mov\n");
                        if (val == 0) {
                                //immediate not allowed
                        } else if (val == 1) {
                                //indirect not supported

                        } else if (val == 2) {
                                move_location(pint, pint+1, reg[str2dec(inst + 8, 4)+6], reg[str2dec(inst + 12, 4) + 6]);

                        } else if (val == 3) {
                                //indirect
                        }


		} else if (strcmp(p, "MOV") == 0) {
                        strncpy(op, inst+5 ,3);
                        val = str2dec(op, 3);
			printf("movr\n");
			if (val == 0) {
				move_register(reg + str2dec(inst+12, 4), str2dec(inst+8, 4));	
			
			} else if (val == 1) {
				move_register(reg+str2dec(inst+12, 4),reg[str2dec(inst+8, 4)+6]);
		
			}
		} else if (strcmp(p, "JNE") == 0) {
			pint = reg + 5;
                        strncpy(op,inst + 5 ,3);
                        val = str2dec(op, 3);
                        if (val == 0) {
                                jump_not_equal(pint, str2dec(inst+8, 8));

                        } else if (val == 1) {
                                //jump_not_equal(pint, reg[str2dec(inst+8, 8)+6]);
				// control flow using labeling
				jump_not_equal(pint, label2line(str2dec(inst+8, 8)));		

                        } else if (val == 2) {
                                //indirect addressing not supported

                        }			

		} else if (strcmp(p, "JMP") == 0) {
			pint = reg + 5;
                        strncpy(op, inst+5 ,3);
                        val = str2dec(op, 3);
			printf("jmp\n");
                        if (val == 0) {
                                jump(str2dec(inst+8, 8));

                        } else if (val == 1) {
                                jump(label2line(str2dec(inst+8, 8)));

                        } else if (val == 2) {
                                //indirect addressing not supported

                        }

		} else if (strcmp(p, "JGE") == 0) {
			pint = reg + 5;
                        strncpy(op, inst+5 ,3);
                        val = str2dec(op, 3);
                        if (val == 0) {
				jump_greater_equal(pint, str2dec(inst+8, 8));
                        } else if (val == 1) {
                                jump_greater_equal(pint, label2line(str2dec(inst+8, 8)));

                        } else if (val == 2) {
                                //indirect addressing not supported
                        }
                } else if (strcmp(p, "JLE") == 0) {
			pint = reg + 5;
                        strncpy(op, inst+5 ,3);
                        val = str2dec(op, 3);
                        if (val == 0) {
                                jump_less_equal(pint, str2dec(inst+8, 8));
                        } else if (val == 1) {
                                jump_less_equal(pint, label2line(str2dec(inst+8, 8)));

                        } else if (val == 2) {
                                //indirect addressing not supported
                        }

                } else if (strcmp(p, "ADD") == 0) {
                        strncpy(op, inst+5,3);
                        val = str2dec(op, 3);
			if (val == 0) {
				temp = str2dec(inst+8, 8);
				add(&temp, reg + str2dec(inst+8, 8)+6);			
			} else if (val == 1) {
				//indirect not supported

			} else if (val == 2) {
				add(reg + str2dec(inst + 8, 8)+6, reg + str2dec(inst + 8, 8)+6);

			} else if (val == 3) {
				//indirect
			}


                } else if (strcmp(p, "SUB") == 0) {
                        strncpy(op, inst+5,3);
                        val = str2dec(op, 3);
                        if (val == 0) {
                                temp = str2dec(inst+8, 8);
                                sub(&temp, reg + str2dec(inst+8, 8)+6);
                        } else if (val == 1) {
                                //indirect not supported

                        } else if (val == 2) {
                                sub(reg + str2dec(inst + 8, 8)+6, reg + str2dec(inst + 8, 8)+6);

                        } else if (val == 3) {
                                //indirect
                        }

                } else if (strcmp(p, "INC") == 0) {
			strncpy(op, inst+5,3);
                        val = str2dec(op, 3);
			temp = 1;
                        if (val == 0) {
				//immediate invalid
                        } else if (val == 1) {
                                add(&temp, reg + str2dec(inst + 8, 8)+6);

                        } else if (val == 2) {
				//indirect
			}

                } else if (strcmp(p, "DEC") == 0) {
			strncpy(op, inst+5 ,3);
                        val = str2dec(op, 3);
                        temp = 1;
                        if (val == 0) {
                                //immediate invalid
                        } else if (val == 1) {
                                sub(&temp , reg + str2dec(inst + 8, 8)+6);

                        } else if (val == 2) {
                                //indirect
                        }

                } else if (strcmp(p, "CMP") == 0) {
			pint = reg + 5;
			strncpy(op, inst+5 ,3);	
			val = str2dec(op, 3);
			
			if (val == 0) {
				comparision(pint, str2dec(inst+8, 4), str2dec(inst+12, 4));

			} else if (val == 1) {
				comparision(pint, str2dec(inst+8, 4), reg[str2dec(inst+12, 4)+6]);

			} else if (val == 2) {
				comparision(pint, reg[str2dec(inst+8, 4)+6], str2dec(inst+12, 4));
			} else if (val == 3) {
				comparision(pint, reg[str2dec(inst+8, 4)+6], reg[str2dec(inst+12, 4)+6]);
			}

		}
		//write_registers();
		reg[3] = programCounter;	
	}
}

void take_off (int* altitude)
{
	*altitude = 1;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "AT LINE : %d\nAltitude register changes to 1\n\n", programCounter);
	fclose (fp);

	programCounter++;
}

void gain_height (int* altitude, int value)
{
	*altitude += value;

	FILE* fp = fopen ("status.txt", "a");
	fprintf(fp, "AT LINE : %d\nAltitude register changes to %d\n\n", programCounter, value);
	fclose (fp);

	programCounter++;
}

void move_location (int* x_pos, int* y_pos, int value1, int value2)
{
	*x_pos = value1;
	*y_pos = value2;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "AT LINE : %d \n x_pos register <- %d \n y_pos register <- %d\n\n", programCounter, value1, value2);
	fclose (fp);

	programCounter++;
}

void move_register (int* g_register, int value)
{
	*g_register = value;

	 FILE* fp = fopen ("status.txt", "a");
	 fprintf (fp, "AT LINE : %d \n value of general register changes to %d\n\n", programCounter, value);
	 fclose (fp);

	 programCounter++;
}

void landing (int* altitude)
{
	*altitude = 0;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "AT LINE : %d \n value of altitude register changes to 0\n\n", programCounter);
	fclose (fp);

	programCounter++;
}

void jump_not_equal (int* status_flag, int line)
{
	if (*status_flag == 0 || *status_flag == 4)
		programCounter = line;
	else
		programCounter += 1;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "programCounter jumps to %d\n\n", programCounter);
	fclose (fp);

}

void jump (int line)
{
	programCounter = line;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "programCounter jumps to %d\n\n", programCounter);
	fclose (fp); 
}

void jump_greater_equal (int* status_flag, int line)
{
	if (*status_flag == 0 || *status_flag == 2)
		programCounter = line;
	else
		programCounter += 1;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "programCounter jumps to %d\n\n", programCounter);
	fclose (fp);

	//programCounter++; 
}

void jump_less_equal (int* status_flag, int line)
{
	if (*status_flag == 2 || *status_flag == 4)
		programCounter = line;
	else
		programCounter += 1;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "programCounter jumps to %d\n\n", programCounter);
	fclose (fp); 
}

void comparision (int* status_flag, int value1, int value2)
{
	if (value1 < value2)
		*status_flag = 0;
	else if (value1 > value2)
		*status_flag = 4;
	else
		*status_flag = 2;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "AT LINE : %d \n status flag register changes to %d\n\n", programCounter, *status_flag);
	fclose (fp);

	programCounter++;
}

void add (int* g_register1, int* g_register2)
{
	*g_register2 += *g_register1;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "AT LINE : %d \n value genral register changes to %d\n\n", programCounter, *g_register2);
	fprintf (fp, "ALU Signal: ADD\n\n");
	fclose (fp);

	programCounter++;
}

void sub (int* g_register1, int* g_register2)
{
	*g_register2 -= *g_register1;

	FILE* fp = fopen ("status.txt", "a");
	fprintf (fp, "AT LINE : %d \n value genral register changes to %d\n\n", programCounter, *g_register2);
	fprintf (fp, "ALU Signal: SUB\n\n");
	fclose (fp);	
	
	programCounter++;
}
int read_registers(void) {

FILE * fp;
int i = 0;
   char buff[20];
   fp = fopen ("registers.txt", "r");

   if (fp == NULL)
    {
    printf("can not open file \n");
    return 0;
    }
   
   rewind(fp);

while(fscanf(fp,"%s %d",buff,reg + i) == 2) {
    i++;
    }

   fclose(fp);
   return 1;

}

int label2line(int labelno) {
	char str[10];
	FILE* fp = fopen("label.txt", "r");
	int i = 1;
	while (fscanf(fp, "%s", str) != EOF) {
		if (labelno == i) {
			return i;
		}
		i++;


	}
	return 0;
}

void write_registers(void) {
	FILE* fp = fopen("registers.txt", "w");
	fprintf(fp, "AC %d\n", reg[0]);
	fprintf(fp, "XC %d\n", reg[1]);
	fprintf(fp, "YC %d\n", reg[2]);
	fprintf(fp, "PC %d\n", reg[3]);
	fprintf(fp, "SP %d\n", reg[4]);
	fprintf(fp, "SF %d\n", reg[5]);
	int i;
	for (i = 6; i< 22; i++) {
		fprintf(fp, "R%02d %d\n", i-6, reg[i]);


	}
}
