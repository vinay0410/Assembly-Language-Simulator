#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

int read_data(char* filename, char* str);
void add_data(char* filename);
void parse_name(char* str, char* outInst);
char* strupr(char* s);
int opcode(char* inst, char* out);
void binInst(char* str, char* out);
char* convert_binary(int n,  int no_of_digits);
void write_data(char* str, char* binCode, char* filename);
void create_label_table(int labels[]);

int main() {

	char filename[10] = "assembly.txt";
	char outputFile[15] = "machine.txt";
	char output[17];
	char str[30];
	char inst[10];
	int labels[20];
	create_label_table(labels);
	int i = 0, j = 0;
	FILE* fp = fopen("file.txt", "r");
	while(fgets(str, 30, fp)!= NULL) {
		
		if (labels[j] == i) {
			j++;
			i--;	
		} else {
		printf("%s\n", str);
		binInst(str, output);
		
		if (strlen(output) == 16) {
			printf("%s\n\n", output);
			write_data(str, output, outputFile);
		}
		}
		i++;
		
	}
	fclose(fp);
}

void binInst(char* str, char* out) {

	char inst[10];
	parse_name(str, inst);
	char *ch;
	char *ch2;
	int params = opcode(inst, out);
	if(params == -1) {
		printf("Invalid Instruction\n");
	} else {
		if (params == 0) {
			strcat(out, "00000000000");

		}

		else if (params == 1) {
                	ch = strchr(str, ' ');
			++ch;
			/*if ((strcmp(out, "00101") == 0) || (strcmp(out, "00110") == 0) || (strcmp(out, "00111") == 0) || (strcmp(out, "01000") == 0)) {
				if (*ch == '#') {
					++ch;
					strcat(out, "000");
                                	strcat(out, convert_binary(atoi(ch), 8));

				} else if (*ch == '$') {
					++ch;
					if (toupper(*ch) == 'L') {
						strcat(out, "001");
                                		++ch;
                                		strcat(out, convert_binary(atoi(ch), 8));	

					}

				} else {
					printf("Invalid Labeling Format\n");
			
				}


			}
			else {
			*/
                	if (*ch == '#') {
				strcat(out, "000");
				++ch;
                        	strcat(out, convert_binary(atoi(ch), 8));
                	} else if (toupper(*ch) == 'R') {
				strcat(out, "001");
				++ch;
                        	strcat(out, convert_binary(atoi(ch), 8));
                	} else if (*ch == '@') {
				if (toupper(*(++ch)) == 'R') {
					strcat(out, "010");
					++ch;
                        		strcat(out, convert_binary(atoi(ch), 8));
                		} else {
					printf("Invalid Instruction Format\n");
					printf("R expected after @\n");
				}
			} else {
				printf("Invalid Instruction Format\n");
			}
	//	}
        	} else if (params == 2) {
			if (strcmp(out, "01101") == 0) {
				// if cmp
				ch = strchr(str, ' ');
                        	++ch;
				if (*ch == '#') {
                                ch2 = strchr(ch, ' ');
                                ++ch2;
                                if (*ch2 == '#') {
					strcat(out, "000");
                                        ++ch;
                                        strcat(out, convert_binary(atoi(ch), 4));
                                        ++ch2;
                                        strcat(out, convert_binary(atoi(ch2), 4));
                                } else if (toupper(*ch2) == 'R') {
                                        strcat(out, "001");
                                        ++ch;
                                        strcat(out, convert_binary(atoi(ch), 4));
                                        ++ch2;
                                        strcat(out, convert_binary(atoi(ch2), 4));
                                } 
				} else if (toupper(*ch) == 'R') {
                                ch2 = strchr(ch, ' ');
                                ++ch2;
                                if (*ch2 == '#') {
					strcat(out, "010");
                                        ++ch;
                                        strcat(out, convert_binary(atoi(ch), 4));
                                        ++ch2;
                                        strcat(out, convert_binary(atoi(ch2), 4));

                                } else if (toupper(*ch2) == 'R') {
                                        strcat(out, "011");
                                        ++ch;
                                        strcat(out, convert_binary(atoi(ch), 4));
                                        ++ch2;
                                        strcat(out, convert_binary(atoi(ch2), 4));
                                }
				}
				
			} else {
                	ch = strchr(str, ' ');
			++ch;
                	if (*ch == '#') {
                        	ch2 = strchr(ch, ' ');
				++ch2;
                        	if (*ch2 == '#') {
                                	printf("Invalid Instruction Format\n");
                                	printf("Immediate addressing of both the parameters isn't allowed\n");
                        	} else if (toupper(*ch2) == 'R') {
					strcat(out, "000");
					++ch;
					strcat(out, convert_binary(atoi(ch), 4));
					++ch2;
					strcat(out, convert_binary(atoi(ch2), 4));
			        } else if (*ch2 == '@') {
					if (toupper(*(++ch2)) == 'R') {
						strcat(out, "001");
						++ch;
                        	                strcat(out, convert_binary(atoi(ch),  4));
						++ch2;
                                        	strcat(out, convert_binary(atoi(ch2), 4));
					} else {
						printf("Invalid Instruction Format\n");
						printf("R expected after @\n");
					}
				} else {
					printf("Invalid Instruction Format\n");
				}
        	        } else if (toupper(*ch) == 'R') {
				ch2 = strchr(ch, ' ');
				++ch2;
                	        if (*ch2 == '#') {
                                        printf("Invalid Instruction Format\n");
                                        printf("Second parameter cannot be immediately addressed\n");
                                } else if (toupper(*ch2) == 'R') {
                                        strcat(out, "010");
					++ch;
                                        strcat(out, convert_binary(atoi(ch), 4));
					++ch2; 
                                        strcat(out, convert_binary(atoi(ch2), 4));
                                } else if (*ch2 == '@') {
					if (toupper(*(++ch2)) == 'R') {
                     	                   	strcat(out, "011");
						++ch;
                                	        strcat(out, convert_binary(atoi(ch), 4));
						++ch2;
                                        	strcat(out, convert_binary(atoi(ch2), 4));
                                	} else {
						printf("Invalid Instruction Format\n");
	                                        printf("Second parameter cannot be immediately addressed\n");
					}
				} else {
					printf("Invalid Instruction Format\n");
				}


                	} else if (*ch == '@') {
				++ch;
				if (toupper(*ch) == 'R') {
					ch2 = strchr(ch, ' ');
					++ch2;
                                	if (*ch2 == '#') {
                                        	printf("Invalid Instruction Format\n");
                                        	printf("Second parameter cannot be immediately addressed\n");
                                	} else if (toupper(*ch2) == 'R') {
                                        	strcat(out, "100");
						++ch;
                                        	strcat(out, convert_binary(atoi(ch), 4));
						++ch2;
                                        	strcat(out, convert_binary(atoi(ch2), 4));
                                	} else if (*ch2 == '@') {
						if (toupper(*(++ch2)) == 'R') {
                                        		strcat(out, "101");
							++ch;
                                                        strcat(out, convert_binary(atoi(ch), 4));
							++ch2;
                                                        strcat(out, convert_binary(atoi(ch2), 4));
                                		} else {
							printf("Invalid Instruction Format\n R expected after @\n");
						}	
					} else {
						printf("Invalid Instruction Format\n");
					}
				}
				else {
					printf("Invalid Instruction Format\nR expected after @\n");
				}
                	} else {
				printf("Invalid Instruction Format\n");
			}
			}
		}
	}
}



int opcode(char* inst, char* out) {

	const char *names[15] = {"TKOFF", "LAND", "HLT", "GAHT","JNE","JMP",  "JGE", "JLE", "INC", "DEC", "MOV", "MVTL", "CMP", "ADD", "SUB"};

	int i;
	char* string;
	for (i=0; i<15; i++) {
		if (!strcmp(strupr(inst), names[i])) {
			break;	
		}	
	}

	if (i==15) {
		return -1;		//Invalid Instruction
	}

	int n = i + 1;
	string = convert_binary(n, 5);
	strcpy(out, string);
	
	if (i < 3) {
		return 0;
	} else if (i < 10) {
		return 1;
	} else {
		return 2;
	}
}


void parse_name(char* str, char* outInst) {
	char* ch;	
	
	ch = strchr(str, ' ');
	if (!ch) {
		ch = strchr(str, '\n');
	}

	strncpy(outInst, str, ch-str);
	outInst[ch-str] = '\0';
}


int read_data(char* filename, char* str) {
        FILE* fp = fopen(filename, "r");
        //char instr[30];
	if (1 == fscanf(fp, "%s", str)){
		return 1;
	} else {
		return 0;
	}
}

char* strupr(char* s) {
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }
    return s;
}

char* convert_binary(int n, int no_of_digits) {
    	char binary[no_of_digits];
        
	int i = no_of_digits - 1;
        while(i!=-1){
                binary[i--] = n%2 + 48;
                n = n/2;
        }
	char* output = malloc((no_of_digits+1)*sizeof(char));
        strcpy(output, binary);
	output[no_of_digits] = '\0';
	return output;
}

void write_data(char* str, char* binCode, char* filename) {
	FILE* fp = fopen(filename, "a");
	if (fp == NULL) {
		printf("Error\n");
	}
	char *pos;
	if ((pos=strchr(str, '\n')) != NULL) {
    		*pos = '\0';
	}

	fprintf(fp,"%s\n",binCode);


}

void create_label_table (int labels[])
{
	FILE* fp1 = fopen ("machine.txt", "r");
	FILE* fp2 = fopen ("labels.txt", "w");
	int i = 1;
	char temp[100];
	int j = 0;
	while (fgets(temp , 100 , fp1)!= NULL)
	{
		int len = strlen(temp);
		temp[len-1] = '\0';
		len--;

		if (temp[len-1] == ':')
		{
			temp[len-1] = '\0';
			fprintf(fp2, "%s %d\n", temp, i);
			i--;
			labels[j] = i;
			j++;
			labels[j] = 0;
		}

		i++;
	}

}
