//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char c;
	int n = 0;
	int cap = 256;
	char str[256]; 
	memset(str,0,256);
	char tmp ;

	while(read(fileno(stdin), &c, sizeof(char)) > 0){
		if((c != '\n') && (n < 254)){
			str[n] = c;
			++n;
		}else{
			if(c != '\n'){
				str[n] = c;
			}
			short len = strlen(str);
			for(int i = 0; i < len / 2; i++){
				tmp = str[len - i - 1];
				str[len - i - 1] = str[i];
				str[i] = tmp;
			}
			printf("%s",str);
			printf("\n");
			memset(str,0,256);
			n = 0;
		}
	}
}
