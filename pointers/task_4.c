#include<stdlib.h>
#include<stdio.h>

#define BUF_SIZE 100

char *find(char *s1, char *s2)
{
	
	char *ans = NULL;

	for(size_t i = 0; s1[i] != '\0'; i++) {

		int flag = 1;
		size_t j = 0;
		for(;s1[i + j] != '\0' &&  s2[j] != '\0'; j++) {
			if(s1[i + j] != s2[j])
				flag = 0;
	 	}

		if(flag == 1 && s2[j] == '\0') {
			ans = s1 + i;
			return ans;
		}

	}
	
	return NULL;

}


int main()
{
	char s1[BUF_SIZE], s2[BUF_SIZE];
	char buf[BUF_SIZE];
	do {
		printf("Input two strings: ");
		fgets(buf, BUF_SIZE, stdin);

	} while (sscanf(buf, "%50s %50s", s1, s2) != 2);
	
	char *ans = find(s1, s2);
	printf("index of the first occurence is: %d", ans - s1);
	
	return 0;
}
