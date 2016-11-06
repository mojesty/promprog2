#include <stdio.h>
#include "cpu.h"
#include "asm.h"

int main() {

	assemble("rec_test.txt", "test30.out");

	Cpu_t cpu = {};
	execute_bin(&cpu, "test30.out");
	return 0;
}