#define _CRT_SECURE_NO_WARNINGS

#include "cpu.h"
#include "stack.h"
#include "asm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DEBUG

//const int GIFT = 0xBADF00D;
const int ERR_STK = -1;
const int SOMERROR = -1;
const int ERR_REGSIZE = -1;
const int GIFT = 0xBADF00D;
const int CALL_STACK_DEFAULT_SIZE = 30;

int Cpu_ctor(Cpu_t * This, int stack_size) {
	This->stk = (Stack_double*)calloc(1,sizeof(Stack_double));
	This->call = (Stack_int*)calloc(1,sizeof(Stack_int));
	if (Stack_ctord(This->stk, stack_size)) return ERR_STK;
	if (Stack_ctori(This->call, CALL_STACK_DEFAULT_SIZE)) return ERR_STK;
	This->flag = 0;
	int i = 0;
	for (i = 0; i < REG_SIZE; i++) This->reg[i] = 0;
	for (i = 0; i < MEM_SIZE; i++) This->mem[i] = 0;
	return 0;
}

int Cpu_dtor(Cpu_t * This) {
	Stack_dtord(This->stk);
	Stack_dtori(This->call);
	free(This->stk);
	free(This->call);
	int i = 0;
	for (i = 0; i < REG_SIZE; i++) This->reg[i] = GIFT;
	for (i = 0; i < MEM_SIZE; i++) This->mem[i] = GIFT;
	return 0;
}
/*
int Cpu_ok(Cpu_t * This) {
	return Stack_OK(This->stk);
}*/

int Cpu_dump(Cpu_t * This) {
#ifdef DEBUG
	printf("This is CPU_dump v0.1\n Stack_dump is called..\n");
	Stack_dumpd(This->stk);
	printf("Stack of function calls dump is called..\n");
	Stack_dumpi(This->call);
	printf("Registers:\n ax = %4d \n bx = %4d \n cx = %4d \n dx = %4d");
	printf("No memory dumping is done yet..");
#else
	printf("CPU crash:(\n");
#endif
	return 0;
}

//------------------------processor instructions---------------------------
//команда push в 20 версии
int Cpu_push20(Cpu_t * This,  double data) {
	if (Stack_pushd(This->stk, data)) return ERR_STK;
	return 0;
}

int Cpu_push(Cpu_t * This, int mode, double data) {
	int res = 0;
	if (mode == 5) res = Stack_pushd(This->stk, data);
	else if (mode == 4 && (int)data <= REG_SIZE) res = Stack_pushd(This->stk, This->reg[(int)data]);
	return res;
}


int Cpu_call(Cpu_t * This, int addr) {
	Stack_pushi(This->call, addr);
	return 0;
}

int Cpu_ret(Cpu_t * This, int * addr) {
	Stack_popi(This->call, addr);
	return 0;
}

int Cpu_pop(Cpu_t * This, int regno) {
	double tmp = 0;
	if (Stack_popd(This->stk, &tmp)) return ERR_STK;
	This->reg[regno] = tmp;
	return 0;
}

int Cpu_add2(Cpu_t * This) {
	double a1 = 0, a2 = 0;
	if (Stack_popd(This->stk, &a1)) return ERR_STK;
	if (Stack_popd(This->stk, &a2)) return ERR_STK;
	if (Stack_pushd(This->stk, (a1 + a2))) return ERR_STK;
	return 0;
}

int Cpu_sub2(Cpu_t * This) {
	double a1 = 0, a2 = 0;
	if (Stack_popd(This->stk, &a1)) return ERR_STK;
	if (Stack_popd(This->stk, &a2)) return ERR_STK;
	if (Stack_pushd(This->stk, (a1 - a2))) return ERR_STK;
	return 0;
}

int Cpu_mul2(Cpu_t * This) {
	double a1 = 0, a2 = 0;
	if (Stack_popd(This->stk, &a1)) return ERR_STK;
	if (Stack_popd(This->stk, &a2)) return ERR_STK;
	if (Stack_pushd(This->stk, (a1 * a2))) return ERR_STK;
	return 0;
}

int Cpu_div2(Cpu_t * This) {
	double a1 = 0, a2 = 0;
	if (Stack_popd(This->stk, &a1)) return ERR_STK;
	if (Stack_popd(This->stk, &a2)) return ERR_STK;
	if (Stack_pushd(This->stk, (a1 / a2))) return ERR_STK;
	return 0;
}

int Cpu_out(Cpu_t * This) {
	double tmp = 0;
	if (Stack_popd(This->stk, &tmp)) return ERR_STK;
	Stack_pushd(This->stk, tmp);
	printf("Value on the top of the stack: %4lf\n", tmp);
	return 0;
}

int Cpu_end(Cpu_t * This) {
	printf("CPU finishes execution...\n");
	return Cpu_dtor(This);
}

int Cpu_begin(Cpu_t * This) {
	printf("CPU begins execution...\n");
	if (!Cpu_ctor(This, 30)) return -1;
	return 0;
}
//----------------------------------v20-----------------------

void Cpu_jmp(Cpu_t * This, int addr, int mode) {
	if (mode == 1) This->peek = addr;
	else This->peek += addr;
}

void Cpu_call(Cpu_t * This, int addr, int mode) {
	Cpu_call(This, This->peek);
	if (mode == 1) This->peek = addr;
	else This->peek += addr;
}

void Cpu_ret(Cpu_t * This) {
	Cpu_ret(This, &This->peek);
}

/*
нумерация битов
76543210
Get: (INPUT >> N) & 1;

Set: INPUT |= 1 << N;

Unset: INPUT &= ~(1 << N);

Toggle: INPUT ^= 1 << N;*/

//В последний бит кладется 1, если равны, в предпоследний кладется 1, если значение reg1 больше, 0 -- если меньше
//временно неработоспособная версия cmp()
void Cpu_cmp2(Cpu_t * This, int reg1, int reg2) {
	This->reg[reg1] == This->reg[reg2] ? This->flag |= 1 << 0 : This->flag |= 0 << 0;
	if (This->reg[reg1] - This->reg[reg2] > 0) This->flag |= 1 << 1;
	if (This->reg[reg1] - This->reg[reg2] > 0) This->flag |= 0 << 1;
}

void Cpu_cmp(Cpu_t * This, int reg1, int reg2) {
	This->flag = 0;
	if (reg1 >= REG_SIZE || reg2 >= REG_SIZE) return;
	This->reg[reg1] == This->reg[reg2] ? This->flag += 1 : This->flag += 0;
	if (This->reg[reg1] - This->reg[reg2] > 0) This->flag += 2;
}

void Cpu_je(Cpu_t * This, int addr, int mode) {
	if ((This->flag % 2) == 0) {
		if (mode == 1) This->peek = addr;
		else This->peek += addr;
	}
}

void Cpu_jne(Cpu_t * This, int addr, int mode) {
	if ((This->flag % 2) != 0) {
		printf("CPU gonna jump to addr %d\n", addr);
		if (mode == 6) This->peek = addr;
		else This->peek += addr;
	}
}

void Cpu_jg(Cpu_t * This, int addr, int mode) {
	if ((This->flag >> 1) & 1 == 1) {
		if (mode == 1) This->peek = addr;
		else This->peek += addr;
	}
}

//--------------------------------v30------------------------

void Cpu_jng(Cpu_t * This, int addr, int mode) {
	if ((This->flag >> 1) & 1 == 0) {
		if (mode == 1) This->peek = addr;
		else This->peek += addr;
	}
}


int Cpu_add(Cpu_t * This, int reg1, int reg2) {
	double a1 = 0, a2 = 0;
	if (reg1 >= REG_SIZE || reg2 >= REG_SIZE) return ERR_REGSIZE;
	This->reg[0] = This->reg[reg1] + This->reg[reg2];
	return 0;
}

int Cpu_sub(Cpu_t * This, int reg1, int reg2) {
	double a1 = 0, a2 = 0;
	if (reg1 >= REG_SIZE || reg2 >= REG_SIZE) return ERR_REGSIZE;
	This->reg[0] = This->reg[reg1] - This->reg[reg2];
	return 0;
}

int Cpu_mul(Cpu_t * This, int reg1, int reg2) {
	double a1 = 0, a2 = 0;
	if (reg1 >= REG_SIZE || reg2 >= REG_SIZE) return ERR_REGSIZE;
	This->reg[0] = This->reg[reg1] * This->reg[reg2];
	return 0;
}

int Cpu_div(Cpu_t * This, int reg1, int reg2) {
	double a1 = 0, a2 = 0;
	if (reg1 >= REG_SIZE || reg2 >= REG_SIZE) return ERR_REGSIZE;
	This->reg[0] = This->reg[reg1] / This->reg[reg2];
	return 0;
}

//--------------------------------execute functions-----------
/*
int execute(Cpu_t * cpu,char * path) {
	FILE * f = fopen(path, "r");
	//Cpu_t * cpu = NULL;
	int cmd = 0, mode = 0, arg1 = 0, arg2 = 0;
	if (!f) {
		printf("Cannot open file %s \n", path);
		return SOMERROR;
	}
	fscanf(f, "%d", &cmd);
	if (cmd != 10) {
		printf("CPU cant execute program: program version  %d differs from supported version %d", cmd, 10);
		return SOMERROR;
	}
	//версия команд описана в файле v10.txt
	while (fscanf(f, "%d", &cmd) == 1) {
		switch (cmd){
		case 1: {
					Cpu_begin(cpu);
					break;
		}
		case 2: {
					Cpu_end(cpu);
					break;
		}
		case 3: {
					Cpu_out(cpu);
					break;
		}
		case 4: {
					fscanf(f, "%d", &arg1);
					Cpu_push20(cpu, arg1);
					break;
		}
		case 5: {
					fscanf(f, "%d", &arg1);
					Cpu_pop(cpu, arg1);
					break;
		}
		case 6: {
					Cpu_add(cpu);
					break;
		}
		case 7: {
					Cpu_sub(cpu);
					break;
		}
		case 8: {
					Cpu_mul(cpu);
					break;
		}
		case 9: {
					Cpu_div(cpu);
					break;
		}
		default: {
					 printf("Skipping unknown command number: %2d\n", cmd);
					 break;
		}
	}
	}
	
	fclose(f);
	return 0;
}
*/

int execute_bin(Cpu_t * cpu, char * path) {
	FILE * f = fopen(path, "rb");
	//Cpu_t * cpu = NULL;
	int cmd = 0, mode = 0, argi1 = 0, argi2 = 0, fsize = 0;
	double argd1 = 0, argd2 = 0;
	unsigned char c = 0;
	if (!f) {
		printf("Cannot open file %s \n", path);
		return SOMERROR;
	}
	fsize = filesize(f);
	char * buf = (char*)malloc(fsize);			//буфер, в котором хранится программа во время исполнения
	fread(buf, sizeof(char), fsize, f);
	fclose(f);
	//здесь нет системы проверки версий команд процессора;(
	c = buf[cpu->peek];
	while (cpu->peek < fsize) {
		mode = c & 7;
		cmd = c >> 3;
		cpu->peek++;
		switch (cmd){
		case 1: {
					Cpu_begin(cpu);
					break;
		}
		case 2: {
					Cpu_end(cpu);
					break;
		}
		case 3: {
					Cpu_out(cpu);
					break;
		}
		case 4: {
					//mode >= 4 ? fread(&argd1, sizeof(double), 1, f) : fread(&argi1, sizeof(int), 1, f);
					memcpy((void*)&argd1, (void*)&buf[cpu->peek], sizeof(double));
					printf("Arg %lf scanned\n", argd1);
					/*printf("Hex view of argd1:\n");
					for (int i = 0; i < sizeof(double); i++) {
						printf("%x ", (int)((char*)&argd1 + i));
					}
					printf("\n");*/
					cpu->peek += sizeof(double);
					Cpu_push(cpu, mode, argd1);
					break;
		}
		case 5: {
					//fread(&argi1, sizeof(int), 1, f);
					memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					Cpu_pop(cpu, argi1);
					break;
		}
		case 6: {
					memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					memcpy((void*)&argi2, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					Cpu_add(cpu, argi1, argi2);
					break;
		}
		case 7: {
					memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					memcpy((void*)&argi2, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					Cpu_sub(cpu, argi1, argi2);
					break;
		}
		case 8: {
					memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					memcpy((void*)&argi2, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					Cpu_mul(cpu, argi1, argi2);
					break;
		}
		case 9: {
					memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					memcpy((void*)&argi2, (void*)&buf[cpu->peek], sizeof(int));
					cpu->peek += sizeof(int);
					Cpu_div(cpu, argi1, argi2);
					break;
		}
		case 10: {
					 memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					 cpu->peek += sizeof(int);			
					 Cpu_jmp(cpu, argi1, mode);
					 break;
		}
		case 11: {
					 memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					 cpu->peek += sizeof(int);
					 Cpu_je(cpu, argi1, mode);
					 break;
		}		
		case 12: {
					 memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					 cpu->peek += sizeof(int);
					 Cpu_jne(cpu, argi1, mode);
					 break;
		}
		case 14: {
					 memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					 cpu->peek += sizeof(int);
					 Cpu_jg(cpu, argi1, mode);
					 break;
		}
		case 15: {
					 memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					 cpu->peek += sizeof(int);
					 Cpu_call(cpu, argi1, mode);
					 break;
		}
		case 16: {
					 Cpu_ret(cpu);
					 break;
		}
		case 17: {
					 memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					 cpu->peek += sizeof(int);
					 memcpy((void*)&argi2, (void*)&buf[cpu->peek], sizeof(int));
					 cpu->peek += sizeof(int);
					 Cpu_cmp(cpu, cpu->reg[argi1], cpu->reg[argi2]);
					 break;
		}
		case 18: {
					 memcpy((void*)&argi1, (void*)&buf[cpu->peek], sizeof(int));
					 cpu->peek += sizeof(int);
					 Cpu_jng(cpu, argi1, mode);
					 break;
		}
		default: {
					 printf("Unknown command number: %2d .\n CPU is in unsafe state. STOP.", cmd);
					 Cpu_end(cpu);
					 break;
		}
		}
		c = buf[cpu->peek];
	}
	free(buf);
}
