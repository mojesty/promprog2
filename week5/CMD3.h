//Парсер команд для 30 версии процессора
//#define CMD( name , code, argc, hasmode , defaultmode)
CMD("push", 4, 1, 1, 4);
CMD("pop", 5, 1, 0, 3);
CMD("begin", 1, 0, 0, 0);
CMD("end", 2, 0, 0, 0);
CMD("out", 3, 0, 0, 0);
//арифметика
CMD("add", 6, 2, 0, 3);
CMD("sub", 7, 2, 0, 3);
CMD("mul", 8, 2, 0, 3);
CMD("div", 9, 2, 0, 3);
//переходы
CMD("jmp", 10, 1, 1, 6);
CMD("je", 11, 1, 1, 6);
CMD("jne", 12, 1, 1, 6);
CMD("jg", 14, 1, 1, 6);
CMD("jng", 18, 1, 1, 6);
CMD("call", 15, 1, 1, 6);
CMD("ret", 16, 0, 0, 0);
CMD("cmp", 17, 2, 0, 0);