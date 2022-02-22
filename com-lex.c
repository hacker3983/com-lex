#include <stdio.h>
#include <stdlib.h>

/*
 * When I'm gone
 * When I'm gone
 * You're gonna miss me when I'm gone
 * You're gonna miss me by my hair
 * You're gonna miss me everywhere, oh
 * You're gonna miss me when I'm gone
*/

void lexer_scan(const char* string) {
	size_t i = 0, line = 1;
	while(string[i]) {
		int error = 0;
		switch(string[i]) {
			case '#':
				printf("Single line comment found at line %ld with symbol \033[31m#\033[0m:", line);
				i++;
				while(string[i] != '\n') {
					if(string[i] == '\0') {
						break;
					}
					printf("%c", string[i]);
					i++;
				}
				printf("\n");
				break;
			case '/':
				i++;
				if(string[i] == '*') {
					i++;
					printf("Multiline comment found at line %ld with symbols \033[34m/*\033[0m:", line);
					while(1) {
						if(string[i] == '\0') {
							printf("Expected */ at line %ld at end of the comment\n",
								line);
							error = 1;
							break;
						}
						if(string[i] == '*') {
							if(string[i+1] == '/') {
								i++;
								break;
							}
						}
						if(string[i] == '\n') {
							line++;
						}
						printf("%c", string[i]);
						i++;
					}
					if(!error) {
						printf("\n");
					}
					break;
				} else if(string[i] != '/') {
					break;
				}
				i++;
				printf("Single line comment"
				      " found at line"
				      " %ld with symbol"
				      " \033[32m//\033[0m:", line);
				while(string[i] != '\n') {
					if(string[i] == '\0') {
						break;
					}
					printf("%c", string[i]);
					i++;
				}
				printf("\n");
				break;
			case '$':
				printf("Multiline comment found at line %ld with symbol \033[33m$\033[0m:", line);
				i++;
				while(1) {
					if(string[i] == '\0') {
						printf("Expected $ at line %ld to indicate the end of the comment\n",
							line);
						error = 1;
						break;
					}
					if(string[i] == '\n') {
						line++;
					}
					if(string[i] == '$') {
						break;
					}
					printf("%c", string[i]);
					i++;
				}
				if(!error) {
					printf("\n");
				}
				break;
		}
		if(string[i] == '\n') {
			line++;
		}
		i++;
	}
}

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s [FILE]...\n", argv[0]);
		fprintf(stderr, "No Input files.\n");
		return 1;
	}
	for(int i=1;i<argc;i++) {
		FILE* f = fopen(argv[i], "r");
		if(f == NULL) {
			fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[i]);
			continue;
		}
		printf("%s:\n", argv[i]);
		fseek(f, 0, SEEK_END);
		size_t size = ftell(f);
		rewind(f);
		char* source = calloc(size, sizeof(char*));
		fread(source, 1, size, f);
		lexer_scan(source);
		free(source);
		fclose(f);
	}
	return 0;
}
