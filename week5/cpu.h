#ifndef CPUH

#define CPUH

#include "stack.h"

const int MEM_SIZE = 1000;
const int REG_SIZE = 4;

typedef struct Cpu_t{
	Stack_double *stk;
	Stack_int *call;
	int reg[REG_SIZE];
	int mem[MEM_SIZE];
	int peek;
	char flag;
	char OF : 1;
};

//в заголовочном файле поддерживается наиболее актуальная версия команд процессора -- констанста VERSION в файле asm.h

int cpu_ctor(Cpu_t * This, int stack_size);
int cpu_dtor(Cpu_t * This);
int cpu_dump(Cpu_t * This);
int cpu_ok(Cpu_t * This);

int Cpu_begin(Cpu_t * This);
int Cpu_end(Cpu_t * This);
int Cpu_out(Cpu_t * This);
int Cpu_div(Cpu_t * This, int reg1, int reg2);
int Cpu_mul(Cpu_t * This, int reg1, int reg2);
int Cpu_sub(Cpu_t * This, int reg1, int reg2);
int Cpu_add(Cpu_t * This, int reg1, int reg2);
int Cpu_pop(Cpu_t * This, int regno);
int Cpu_push(Cpu_t * This, int mode, double data);

int execute(Cpu_t * This, char * path);
int execute_bin(Cpu_t * cpu, char * path);

#endif