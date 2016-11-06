#ifndef STACKH

#define STACKH

typedef struct Stack_double {
	double *data;
	double *peek;
	int size;
};

typedef struct Stack_int {
	int *data;
	int *peek;
	int size;
};

bool Stack_oki(const Stack_int * This); 
int Stack_pushi(Stack_int * This, int val); 
int Stack_popi(Stack_int * This, int * ptr); 
int Stack_ctori(Stack_int *This, int size); 
int Stack_dtori(Stack_int * This); 
int Stack_dumpi(const Stack_int * This); 

bool Stack_okd(const Stack_double * This);
int Stack_pushd(Stack_double * This, double val);
int Stack_popd(Stack_double * This, double * ptr);
int Stack_ctord(Stack_double *This, int size);
int Stack_dtord(Stack_double * This);
int Stack_dumpd(const Stack_double * This);

#endif