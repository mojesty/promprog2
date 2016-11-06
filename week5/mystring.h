#include <stdio.h>
#include <stdlib.h>

int mystrlen(const char * str) {
	int len = 0;
	int c = str[0];
	while (c != '\0') {
		len++;
		c = str[len];
	}
	return len;
}

char * mystrncpy(char * destination, const char * source, int num) {
	int i = 0, fin = -1;
	for (i = 0; i < num; i++) {
		if (source[i] == '\0') fin = i;
		i>fin ? destination[i] = source[i] : destination[i] = '\0';
	}
	return destination;
}

char * mystrncat(char * destination, const char * source, int num){
	int i = 0, len = mystrlen(destination);
	for (i = 0; i < num; i++) {
		destination[len + i] = source[i];
		if (source[i] == '\0') break;
	}
	destination[len + i] = '\0';
	return destination;
}

void prefix(const char* str, int * prefix){
	int i = 0, k = 0;
	for (i = 1; i < mystrlen(str); i++) {
		k = prefix[i - 1];
		while (k>0 && str[k] != str[i]) {
			k = prefix[k - 1];
		}
		if (str[k] == str[i])	k++;
		prefix[i] = k;
	}
	return;
}

char * strstr(char * str1, char * str2){
	int index = -1, k = 0, i = 0;
	int * f = (int*)calloc(mystrlen(str1), sizeof(char));
	prefix(str1, f);
	for (i = 0; i < mystrlen(str1); i++) {
		while (k > 0 && str1[k] != str2[i])
			k = f[k - 1];
		if (str1[k] == str2[i]) k++;
		if (k == mystrlen(str1)) {
			index = i - mystrlen(str1) + 1;
			break;
		}
	}
	free(f);
	if (index >= 0) return &(str2[index]);
	return NULL;
}

char * mystrtok(char * str, const char * delimiters){
	static int pos = 0;
	static int length = mystrlen(str)+1;
	if (str == NULL) {
		//length = 0;
		pos = 0;
		return NULL;
	}
	int i = 0, j = 0, curr = 0, start = 0, found = 0;	//флаг start поднимается, когда мы достигли начала токена, found служит для того, чтобы начало токена отследить
	for (i = pos; i < length; i++) {
		found = i;

		for (j = 0; j < mystrlen(delimiters)+1; j++) {
			if (str[i] == delimiters[j]) {
				found++;

				if (start){
					str[i] = '\0';
					curr = pos;
					pos = i + 1;
					return &(str[curr]);
				}

			}
		}

		if (found <= i) {
			start = 1;
		}
		if (str[i] == '\0' && start) return &(str[curr]);
		if (!start) pos++;
	}
	return NULL;
}
