#ifndef STACKH

#define STACKH

typedef struct Stack_t {
	double *data;
	int count;
	int size;
};

bool Stack_OK(Stack_t * This);

int Stack_push(Stack_t * This, double val);
int Stack_pop(Stack_t * This, double * ptr);
int Stack_ctor(Stack_t *This, int size);
int Stack_dtor(Stack_t * This);
int Stack_dump(const Stack_t * This);

bool Stack_test();

#endif