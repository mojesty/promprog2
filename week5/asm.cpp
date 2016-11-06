#define _CRT_SECURE_NO_WARNINGS

#include "asm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mystring.h"

const int VERSION = 30;
const int IOERROR = -1;
const int VERERROR = -2;
const int TOKERROR = -3;
const int MAX_MODE = 7;
const int LABEL_LENGTH = 10;
const int NUM_LABELS = 20;

int filesize(FILE * fp){
	int res = 0;
	fseek(fp, 0L, SEEK_END);
	res = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return res;
}

int isnumber(char * str) {
	int i = 0;
	for (i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i])) return 0;
	}
	return 1;
}

unsigned char make_cmd(int cmd, int mode) {
	char res = cmd;
	res = res << 3;
	if (mode >= MAX_MODE) {
		printf("While assembling, invalid mode %d was found. Setting mode = MAX_MODE = %d\n", mode, MAX_MODE);
	}
	printf("New char from cmd %d and mode %d: int %d char %c\n", cmd, mode, res + mode,res + mode);
	return res + mode;
}

//args[i] = atoi(curr); \

//великий парсер все€ команд
#define CMD( name , code, argc, hasmode , defaultmode) {\
if (!strcmp((name), curr)) {\
	printf("---------\n---------name %s success scan\n", curr); \
if ((hasmode)) {\
	curr = mystrtok(buf, " \n\r"); \
	mode = atoi(curr); \
}\
else mode = defaultmode; \
	for (i = 0; i < argc; i++) {\
		curr = mystrtok(buf, " \n\r"); \
if (mode == 2 || mode == 6) {\
	printf("scanned string arg %d: %s\n", i, curr);\
if (curr[strlen(curr) - 1] != ':') printf("arg %s is not a label\n", curr);\
	curr[strlen(curr) - 1] = '\0';\
	mode == 6? argi[0] = label_search(curr, labels): argi[0] = peek;  \
	printf("Label addr found: %d\n", argi[0]);\
}\
else if (mode >= 4 && mode != 6) {\
sscanf(curr, "%lf", &args[i]); \
printf("scanned double arg %d: %lf \n", i, args[i]); \
}\
else if (mode <4 && mode != 2) {\
sscanf(curr, "%d", &argi[i]); \
printf("scanned int arg %d: %d \n", i, argi[i]); \
}\
	}\
	prog[peek] = make_cmd(code, mode); \
	peek++; \
		for (i = 0; i < argc; i++) {\
		if (mode >= 4 && mode !=6)	{\
			memcpy((void*)&prog[peek], (void*)&args[i], sizeof(double)); \
			printf("%lf\n", args[i]);\
			peek += sizeof(double); \
		}\
		else if (mode !=2 ) {\
			memcpy((void*)&prog[peek], (void*)&argi[i], sizeof(int)); \
			peek += sizeof(int); \
			}\
		}\
}\
}

struct labe {
	char name[LABEL_LENGTH];
	int addr;
};

int label_search(char * name, labe * data) {
	int i = 0;
	for (i = 0; i < NUM_LABELS; i++) {
		if (!strcmp(name, data[i].name)) return data[i].addr;
	}
	return -1;
}

int go_through(char * buf1, char * curr, char * buf, char * prog, char * save, labe * labels, FILE * in, FILE * out) {
	int  peek_label = 0, mode = 0, cmd = 0, i = 0, peek = 0;
	double args[4];
	int argi[4];
	sprintf(buf1, "EMELY%d", VERSION);
	curr = mystrtok(buf, " \n\r");
	if (strcmp(buf1, curr)) {
		printf("Source file has inappropiate version. Supported version is %2d\n", VERSION);
		return VERERROR;
	}
	curr = mystrtok(buf, " \n\r");
	while (curr) {
		if (curr[0] == ':') {
			//labels_preprocessor
			if (label_search(&curr[1], labels) >= 0) {	//проверить, что такой метки нщЄ не было объ€влено
				printf("Label %s already in use. Skipping current label..\n", &curr[1]);
			}
			else {
				strcpy(labels[peek_label].name, &curr[1]);	//скопировать им€ метки, не включа€ двоеточие
				//высчитать адрес следующей команды
				labels[peek_label++].addr = peek;				//и записать его в структуру
			}
		}
		else
		if (!isdigit(curr[0])) {
			//commands_preprocessor
			#include "CMD3.h"			
		}
		else {
			printf("Token %s cannot be parsed. Stop.\n", curr);
			fclose(in);
			fclose(out);
			return TOKERROR;
		}
		curr = mystrtok(buf, " \n\r");
	}
	return peek;
}

int assemble(char * source, char * bin) {
	FILE * in = fopen(source, "r");
	FILE * out = fopen(bin, "wb");
	if (!(in && out)) {
		printf("Unable to open either input or output file. Stop.");
		return IOERROR;
	}
	int size = filesize(in);
	char buf1[20];
	double args[4];
	labe * labels = (labe*)calloc(NUM_LABELS, sizeof(labe));
	char * curr = NULL, * buf = (char*)malloc(size * sizeof(char));	//буфер, куда складываетс€ текст программы на ассемблере
	char * prog = (char*)malloc(size * sizeof(double));				//массив с байт-кодом, который потом будет положен в выходной файл
	char * save = (char*)malloc(size * sizeof(char));				//буфер, в котором будет сохран€тьс€ текст программы, чтобы не считывать еЄ заново с файла
	int mode = 0, peek = 0, peek_label = 0;											//указатель на последнюю заполн€емую €чейку пам€ти в байт-коде
	int i = 0;
	//fgets(buf, size, in);
	for (i = 0; i < size; i++) buf[i] = fgetc(in);

	//version coincidence check
	memcpy(save, buf, size*sizeof(char));
	peek = go_through(buf1, curr, buf, prog, save, labels, in, out);
	memcpy(buf, save, size*sizeof(char));
	mystrtok(NULL, " ");
	peek = go_through(buf1, curr, buf, prog, save, labels, in, out);
	/*prog[peek++] = '\0';
	for (int i = 0; i < peek; i++) printf("%c", prog[i]);
	printf("\n");
	for (int i = 0; i < peek; i++) printf("%3d ", prog[i]);*/
	for (i = 0; i < peek; i++) {
		putc(prog[i], out);
	}
	fclose(in);
	fclose(out);
	free(buf);
	free(save);
	free(prog);
	free(labels);
	return 0;
}