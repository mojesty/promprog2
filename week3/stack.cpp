#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define Stackcheck() {if (!Stack_ok(This)) {\
	Stack_dump(This); \
	assert(!"Stack_ok failed"); \
}; }

#define NAME_OF_VAR(VAR) #VAR

const int ERR_CALLOC = -1;
const int ERR_COUNT = -2;
const int GIFT = 0xBADF00D;
/*
typedef struct Stack_t {
	double *data;
	int count;
	int size;
};*/



int Stack_ctor(Stack_t * This, int size) {
	This->data = (double*)calloc(size, sizeof(This->data[0]));
	This->count = 0;
	This->size = (size > 0 ? size : -1);
	if (!This->data) return ERR_CALLOC;
	return 0;
}

int Stack_dtor(Stack_t * This) {
	memset(This->data, GIFT, This->size);
	free(This->data);
	This->data = NULL;
	This->count = -1;
	This->size = -1;
	return 0;
}

bool Stack_ok(const Stack_t * This) {
	return This &&
		This->data &&
		This->count >= 0 &&
		This->size >= 0;
}

int Stack_dump(const Stack_t * This) {
	//Stackcheck();
	printf("Stack_t %s (%d) [0x%8x]\n", NAME_OF_VAR(This), Stack_ok(This), This);
	int i = 0;
	printf("  data[%3d] in memory [0x%8x]\n", This->count, This->data);
	for (i = 0; i < This->size; i++) {
		printf("    %c[%3d] = %5.1lf\n", i >= This->count ? '*' : ' ', i, This->data[i]/*, This->data[i]==GIFT? "POISON" : ""*/);
	}
	printf("  count = %3d\n", This->count);
	printf("End dump printing\n");
	return 0;
}

int Stack_push(Stack_t * This, double val) {
	Stackcheck();
	if (This->count  >= This->size) return ERR_COUNT;
	This->data[This->count++] = val;
	Stackcheck();
	return 0;
}

int Stack_pop(Stack_t * This, double * ptr) {
	Stackcheck();
	if (This->count == 0) return ERR_COUNT;
	This->count--;
	ptr[0] = This->data[This-> count];
	This->data[This->count] = GIFT;
	Stackcheck();
	return 0;
}


/*This function tests stack: creates a variable of type Stack_t, fills it until it is full, 
tests overfull protection, dumps it, empties it, tests overempty protection, dumps it again and finally destroies*/
bool Stack_test() {
	Stack_t stk = {};
	double a = 0;
	printf("----Starting testing of my stack realisation----\n");
	if (Stack_ctor(&stk, 3) == ERR_CALLOC) {
		printf("Stack test failed: unable to allocate memory\n");
		return ERR_CALLOC;
	}
	Stack_push(&stk, 1);
	Stack_push(&stk, 4);
	if (Stack_push(&stk, 2) == ERR_COUNT) printf("Stack_push does wrong");
	if (Stack_push(&stk, 2) != ERR_COUNT) printf("Stack_push does wrong!\n");
	Stack_dump(&stk);
	Stack_pop(&stk, &a);
	Stack_pop(&stk, &a);
	if (Stack_pop(&stk, &a) == ERR_COUNT) printf("Stack_pop does wrong!\n");
	if (Stack_pop(&stk, &a) != ERR_COUNT)  printf("Stack_pop does wrong!\n");
	Stack_dump(&stk);
	if (Stack_dtor(&stk) != 0) {
		printf("Stack test failed: stack destruction error\n");
		return ERR_CALLOC;
	}
	printf("----Finishing testing of my stack realisation----\n");
	return 0;
}