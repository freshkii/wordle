#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define RED 		"\x1B[0;31m"
#define GREEN 	"\x1B[0;32m"
#define YELLOW 	"\x1B[0;33m"
#define BLUE 		"\x1B[0;34m"
#define RESET		"\x1B[0m"

#define WORD_LEN 5

#define green(X) GREEN X RESET

int main(int argc, char **argv) {
	// greeting
	printf(green("┌────────┐\n│") " Wordle " green("│\n└────────┘\n"));
	char filename[256];

	// input handling
	switch(argc) {
		case 1:
			#ifdef __linux__
				char *homedir = getenv("HOME");
				char *basedir = malloc( sizeof(char) *
						(strlen(homedir)+ strlen("/.local/share")) );
				strcpy(basedir, homedir);
				strcat(basedir, "/.local/share");
			#endif
			#ifdef __WIN32
				char *homedir = getenv("APPDATA");
				char *basedir = malloc( sizeof(char) *
						(strlen(homedir)+ strlen("/Library/Application Support/")) );
				strcpy(basedir, homedir);
				strcat(basedir, "/Library/Application Support/");
			#endif
			#ifdef __POMME__
				char *homedir = getenv("HOME");
			#endif

			if (!strcmp(homedir, "")) {
					fprintf(stderr, RED "error: couldn't get enviromnent variable\n" RESET);
					return 1;
			}

			strcpy(filename, basedir);
			strncat(filename, "/wordle/words.txt", strlen(filename));
			break;
		case 2:
			if (!strcmp(argv[1], "-f")) {
				printf("usage:\n- wordle \n- wordle -f\n- wordle -f [filename]\n");
				return 0;
			}

			printf("filename: ");
			scanf("%255s", filename);
			printf(BLUE "(tip : next time, type 'wordle [filename]')\n" RESET);
			break;
		case 3:
			if (!strcmp(argv[1], "-f")) {
				printf("usage:\n- wordle \n- wordle -f\n- wordle -f [filename]\n");
				return 0;
			}

			if (strlen(argv[1]) > 255) {
				printf("filename too long\n");
				return 0;
			}

			char cwd[256];
			if (getcwd(cwd,  sizeof(cwd)) == NULL){
				fprintf(stderr, RED "error: coudln't get working directory\n" RESET);
				return 1;
			}

			strcpy(filename, cwd);
			strncat(filename, argv[1], 255-strlen(filename));
			break;
	}
	
	// file streaming
	FILE* f = fopen(filename, "r");

	if (f == NULL) {
		fprintf(stderr, RED "error: couldn't open the file '%s'\n" RESET, filename);
		return 1;
	}

	// get amount of words
	fseek(f, 0L, SEEK_END);
	long file_size = ftell(f);
	long length = file_size / (WORD_LEN + 1);
	rewind(f);

	char words[length][WORD_LEN+1];

	for (int i = 0; i < length; i++) {
		char *line = NULL;
		size_t l = 0;
		if (getline(&line, &l, f) == -1) {
			fprintf(stderr, RED "error: a problem has occurred while reading the file\n" RESET);
			return 1;
		}
		for (int j = 0; j < WORD_LEN; j++)
			words[i][j] = line[j];
		words[i][WORD_LEN] = '\0';
	}

	fclose(f);

	// pick random word
	unsigned int seed = time(0);
	int n = rand_r(&seed) % length;

	char random_word[6];
	strncpy(random_word, words[n], WORD_LEN);

	char user_input[6] = "     ";
	int i = 0;
	for (; i < 5 && strcmp(user_input, random_word) != 0; i++) {
		char tmp[6];
		strncpy(tmp, random_word, WORD_LEN);
		printf("%i attempts left:", 5-i);
		scanf(" %5s", user_input);
		char ok = 1;
		for (int j = 0; j < 5; j++) {
			if (user_input[j] == '\0' || !isalpha(user_input[j])) {
				printf(RED "enter a valid 5-letter word\n" RESET);
				i--;
				ok = 0;
				break;
			}
		}

		if (ok == 0)
			continue;

		for (int i = 0; i < WORD_LEN; i++) {
			char c = user_input[i];
			if (c == '\0' || !isalpha(c)) {
			}
			for (int j = 0; j < WORD_LEN; j++) {
				if (c == tmp[j]) {
					tmp[j] = ' ';
					if (i == j)
						printf(GREEN "%c" RESET, c);
					else
						printf(YELLOW "%c" RESET, c);
					break;
				}
				else if (j == WORD_LEN-1){
					printf("%c", user_input[i]);
				}
			}
		}
		printf("\n");
	}

	if (strcmp(user_input, random_word) == 0)
		printf(green("Congratulations !") " You've won within " green("%i") " attempt%c.\n", i, (i == 1 ? '\0' : 's'));
	printf("The word was %s.\n", random_word);

	return 0;
}
