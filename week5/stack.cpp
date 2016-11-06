#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DEBUG

#define Stackchecki() {if (!Stack_oki(This)) {\
	Stack_dumpi(This); \
	assert(!"Stack_ok failed"); \
}; }

#define Stackcheckd() {if (!Stack_okd(This)) {\
	Stack_dumpd(This); \
	assert(!"Stack_ok failed"); \
}; }

#define NAME_OF_VAR(VAR) #VAR



const int ERR_CALLOC = -1;
const int ERR_COUNT = -2;
const int GIFT = 0xBADF00D;
const double REALLOC_MULTIPLIER = 1.5;
const double FREE_SPACE_COEF = 0.8;

//#define Stack_template(intype) {\
typedef struct Stack_t {\
	intype *data; \
	intype *peek; \
	int size; \
}; \
int Stack_ctor(Stack_t * This, int size) {\
		This->data = (intype*)calloc(size, sizeof(intype)); \
		This->peek = This->data; \
		This->size = (size > 0 ? size : -1); \
	if (!This->data) return ERR_CALLOC; \
		return 0; \
}\
\
int Stack_dtor(Stack_t * This) {\
		memset(This->data, GIFT, This->size); \
		free(This->data); \
		This->data = NULL; \
		This->peek = NULL; \
		This->size = -1; \
		return 0; \
}\
\
bool Stack_ok(const Stack_t * This) {\
		return This &&\
		This->data &&\
		This->peek &&\
		This->size >= 0; \
}\
\
int Stack_dump(const Stack_t * This) {\
		printf("Stack_t %s (%d) [0x%8x]\n", NAME_OF_VAR(This), Stack_ok(This), This); \
		int i = 0; \
		int count = (This->peek - This->data) / sizeof(This->data[0]); \
		printf("  data[%3d] in memory [0x%8x]\n", This->size, This->data); \
	for (i = 0; i < This->size; i++) {\
			printf("    %c[%3d] = %5.1lf\n", i >= count ? '*' : ' ', i, This->data[i]); \
	}\
	printf("  count = %3d\n", count); \
	printf("End dump printing\n"); \
	return 0; \
}\
\
int Stack_push(Stack_t * This, intype val) {\
		Stackcheck(); \
		int count = (This->peek - This->data) / sizeof(This->data[0]); \
	if (count >= This->size) {\
			intype * ptr = (intype*)realloc(This->data, sizeof(This->data[0]) * (int)(This->size * REALLOC_MULTIPLIER)); \
		if (!ptr) return ERR_CALLOC; \
			This->data = ptr; \
			This->size = (int)(This->size * REALLOC_MULTIPLIER); \
	}\
	This->data[count++] = val; \
	This->peek += sizeof(This->data[0]); \
	Stackcheck(); \
	return 0; \
}\
\
int Stack_pop(Stack_t * This, intype * ptr) {\
		Stackcheck(); \
		int count = (This->peek - This->data) / sizeof(This->data[0]); \
	if (count == 0) return ERR_COUNT; \
		*ptr = This->data[count - 1]; \
		This->peek -= sizeof(This->data[0]); \
		This->data[count - 1] = GIFT; \
	if ((intype)This->size / (intype)count >= FREE_SPACE_COEF) {\
			double * ptr = (intype*)realloc(This->data, sizeof(This->data[0]) * (int)(This->size *  FREE_SPACE_COEF + 1)); \
		if (!ptr) return ERR_CALLOC; \
			This->data = ptr; \
			This->size = (int)(This->size * FREE_SPACE_COEF + 1); \
	}\
	Stackcheck(); \
	return 0; \
}\
}
/*bool Stack_OK(const Stack_t * This); \
\
int Stack_push(Stack_t * This, intype val); \
int Stack_pop(Stack_t * This, intype * ptr); \
int Stack_ctor(Stack_t *This, int size); \
int Stack_dtor(Stack_t * This); \
int Stack_dump(const Stack_t * This); \
\
bool Stack_test(); */

int Stack_ctord(Stack_double * This, int size) {
	This->data = (double*)calloc(size, sizeof(double));
	This->peek = This->data;
	This->size = (size > 0 ? size : -1);
	if (!This->data) return ERR_CALLOC;
	return 0;
}

int Stack_dtord(Stack_double * This) {
	memset(This->data, GIFT, This->size);
	free(This->data);
	This->data = NULL;
	This->peek = NULL;
	This->size = -1;
	return 0;
}

bool Stack_okd(const Stack_double * This) {
	return This &&
		This->data &&
		This->peek &&
		This->size >= 0;
}

int Stack_dumpd(const Stack_double * This) {
	printf("Stack_double %s (%d) [0x%8x]\n", NAME_OF_VAR(This), Stack_okd(This), This);
	int i = 0;
	int count = (This->peek - This->data) / sizeof(This->data[0]);
	printf("  data[%3d] in memory [0x%8x]\n", This->size, This->data);
	for (i = 0; i < This->size; i++) {
		printf("    %c[%3d] = %5.1lf\n", i >= count ? '*' : ' ', i, This->data[i]);
	}
	printf("  count = %3d\n", count);
	printf("End dump printing\n");
	return 0;
}

int Stack_pushd(Stack_double * This, double val) {
	Stackcheckd();
	int count = (This->peek - This->data) / sizeof(This->data[0]);
	if (count >= This->size) {
		double * ptr = (double*)realloc(This->data, sizeof(This->data[0]) * (int)(This->size * REALLOC_MULTIPLIER));
		if (!ptr) return ERR_CALLOC;
		This->data = ptr;
		This->size = (int)(This->size * REALLOC_MULTIPLIER);
	}
	This->data[count++] = val;
	This->peek += sizeof(This->data[0]);
	Stackcheckd();
	return 0;
}

int Stack_popd(Stack_double * This, double * ptr) {
	Stackcheckd();
	int count = (This->peek - This->data) / sizeof(This->data[0]);
	if (count == 0) return ERR_COUNT;
	*ptr = This->data[count - 1];
	This->peek -= sizeof(This->data[0]);
	This->data[count - 1] = GIFT;
	if ((int)This->size / (int)count >= FREE_SPACE_COEF) {
		double * ptr2 = (double*)realloc(This->data, sizeof(This->data[0]) * (int)(This->size *  FREE_SPACE_COEF + 1));
		if (!ptr2) return ERR_CALLOC;
		This->data = ptr2;
		This->size = (int)(This->size * FREE_SPACE_COEF + 1);
	}
	Stackcheckd();
	return 0;
}



int Stack_ctori(Stack_int * This, int size) {
	This->data = (int*)calloc(size, sizeof(int));
	This->peek = This->data;
	This->size = (size > 0 ? size : -1);
	if (!This->data) return ERR_CALLOC;
	return 0;
}

int Stack_dtori(Stack_int * This) {
	memset(This->data, GIFT, This->size);
	free(This->data);
	This->data = NULL;
	This->peek = NULL;
	This->size = -1;
	return 0;
}

bool Stack_oki(const Stack_int * This) {
	return This &&
		This->data &&
		This->peek &&
		This->size >= 0;
}

int Stack_dumpi(const Stack_int * This) {
	printf("Stack_int %s (%d) [0x%8x]\n", NAME_OF_VAR(This), Stack_oki(This), This);
	int i = 0;
	int count = (This->peek - This->data) / sizeof(This->data[0]);
	printf("  data[%3d] in memory [0x%8x]\n", This->size, This->data);
	for (i = 0; i < This->size; i++) {
		printf("    %c[%3d] = %5.1lf\n", i >= count ? '*' : ' ', i, This->data[i]);
	}
	printf("  count = %3d\n", count);
	printf("End dump printing\n");
	return 0;
}

int Stack_pushi(Stack_int * This, int val) {
	Stackchecki();
	int count = (This->peek - This->data) / sizeof(This->data[0]);
	if (count >= This->size) {
		int * ptr = (int*)realloc(This->data, sizeof(This->data[0]) * (int)(This->size * REALLOC_MULTIPLIER));
		if (!ptr) return ERR_CALLOC;
		This->data = ptr;
		This->size = (int)(This->size * REALLOC_MULTIPLIER);
	}
	This->data[count++] = val;
	This->peek += sizeof(This->data[0]);
	Stackchecki();
	return 0;
}

int Stack_popi(Stack_int * This, int * ptr) {
	Stackchecki();
	int count = (This->peek - This->data) / sizeof(This->data[0]);
	if (count == 0) return ERR_COUNT;
	*ptr = This->data[count - 1];
	This->peek -= sizeof(This->data[0]);
	This->data[count - 1] = GIFT;
	if ((int)This->size / (int)count >= FREE_SPACE_COEF) {
		int * ptr2 = (int*)realloc(This->data, sizeof(This->data[0]) * (int)(This->size *  FREE_SPACE_COEF + 1));
		if (!ptr2) return ERR_CALLOC;
		This->data = ptr2;
		This->size = (int)(This->size * FREE_SPACE_COEF + 1);
	}
	Stackchecki();
	return 0;
}


/*This function tests stack: creates a variable of type Stack_t, fills it until it is full, 
tests overfull protection, dumps it, empties it, tests overempty protection, dumps it again and finally destroies*/
//bool Stack_test() {
//	Stack_t stk = {};
//	double a = 0;
//	printf("----Starting testing of my stack realisation----\n");
//	if (Stack_ctor(&stk, 2) == ERR_CALLOC) {
//		printf("Stack test failed: unable to allocate memory\n");
//		return ERR_CALLOC;
//	}
//	Stack_push(&stk, 1);
//	Stack_push(&stk, 2);
//	if (Stack_push(&stk, 3) == ERR_COUNT) {}//printf("Stack_push does wrong");
//	if (Stack_push(&stk, 4) != ERR_COUNT) {}//printf("Stack_push does wrong!\n");
//	Stack_dump(&stk);
//	Stack_pop(&stk, &a);
//	printf("%lf\n", a);
//	Stack_pop(&stk, &a);
//	printf("%lf\n", a);
//	if (Stack_pop(&stk, &a) == ERR_COUNT) {}//printf("Stack_pop does wrong!\n");
//	printf("%lf\n", a);
//	if (Stack_pop(&stk, &a) != ERR_COUNT)  {}//printf("Stack_pop does wrong!\n");
//	printf("%lf\n", a);
//	Stack_dump(&stk);
//	if (Stack_dtor(&stk) != 0) {
//		printf("Stack test failed: stack destruction error\n");
//		return ERR_CALLOC;
//	}
//	printf("----Finishing testing of my stack realisation----\n");
//	return 0;
//}