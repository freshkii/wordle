#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define RED "\x1B[0;31m"
#define GREEN "\x1B[0;32m"
#define YELLOW "\x1B[0;33m"
#define BLUE "\x1B[0;34m"
#define RESET "\x1B[0m"

#define green(X) GREEN X RESET

int main(int argc, char **argv) {
	// greeting
	printf(green("┌────────┐\n│") " Wordle " green("│\n└────────┘\n"));
	char filename[256];
	const int word_len = 5;

	// input handling
	if (argc == 2) {
		if (strlen(argv[1]) <= 255) {
			strcpy(filename, argv[1]);
		}
		else {
			fprintf(stderr, RED "error: filename too long\n" RESET);
			return 1;
		}
	}
	else if (argc > 2) {
		printf("usage: wordle [filename] or wordle\n");
	}
	else {
		printf("filename: ");
		scanf("%255s", filename);
		printf(BLUE "(tip : next time, type 'wordle [filename]')\n" RESET);
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
	long length = file_size / (word_len + 1);
	rewind(f);

	char words[length][word_len+1];

	for (int i = 0; i < length; i++) {
		char *line = NULL;
		size_t l = 0;
		if (getline(&line, &l, f) == -1) {
			fprintf(stderr, RED "error: a problem has occurred while reading the file\n" RESET);
			return 1;
		}
		for (int j = 0; j < word_len; j++)
			words[i][j] = line[j];
		words[i][word_len] = '\0';
	}

	fclose(f);

	// pick random word
	unsigned int seed = time(0);
	int n = rand_r(&seed) % length;

	char random_word[6];
	strncpy(random_word, words[n], word_len);

	char user_input[6] = "     ";
	int i = 0;
	for (; i < 5 && strcmp(user_input, random_word) != 0; i++) {
		char tmp[6];
		strncpy(tmp, random_word, word_len);
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

		for (int i = 0; i < word_len; i++) {
			char c = user_input[i];
			if (c == '\0' || !isalpha(c)) {
			}
			for (int j = 0; j < word_len; j++) {
				if (c == tmp[j]) {
					tmp[j] = ' ';
					if (i == j)
						printf(GREEN "%c" RESET, c);
					else
						printf(YELLOW "%c" RESET, c);
					break;
				}
				else if (j == word_len-1){
					printf("%c", user_input[i]);
				}
			}
		}
		printf("\n");
	}
	i++;

	if (strcmp(user_input, random_word) == 0)
		printf(green("Congratulations !") " You've won within" green("%i") "attempt%c.\n", i, (i == 1 ? '\0' : 's'));
	printf("The word was %s.\n", random_word);

	return 0;
}
