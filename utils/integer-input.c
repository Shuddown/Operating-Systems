// file: integer-input.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_DIGITS 12

/**
 * This function removes surplus characters from the input buffer.
 * Otherwise, if more than the permitted number of characters have been entered during the
 * call to fgets(), the surplus characters (after MAX_DIGITS chars) remain in the input buffer
 * and will be wrongly accepted as input on the next iteration of the loop.
 * */
static inline void ClearInputBuffer() 
{
	char c = 0;
	// Loop over input buffer and consume chars until buffer is empty
	while ((c = getchar()) != '\n' && c != EOF);
}

void getIntegerFromStdin(int *inputInteger)
{
	char *inputBuffer = malloc(sizeof(char) * MAX_DIGITS);
	memset(inputBuffer, 0, MAX_DIGITS);
	char *input = NULL;
	while (input == NULL) {
		// Note that fgets returns inputBuffer on success.
		// This becomes important when freeing - free either `input` or
		// `inputBuffer` to avoid an attempted double-free error.
		input = fgets(inputBuffer, MAX_DIGITS, stdin);
		
		// If fgets() receives less than MAX_DIGITS, the last char in the array is '\n'.
		// Therefore if the last char is not '\n', too many characters were entered.
		if (inputBuffer[strlen(inputBuffer) - 1] != '\n') {
			fprintf(stderr, "[ERROR]: Too many characters: max input is %d chars.\n", MAX_DIGITS);
			ClearInputBuffer();
			input = NULL;
			continue;
		}

		// Check that the input can be intepreted as an integer
		// Convert to integer using `strtol()`
		errno = 0;
		char *endptr = NULL;
		*inputInteger = strtol(input, &endptr, 10);
		
		// If an integer was not found, endptr remains set to input
		if (input == endptr) {
			// Remove trailing newline by adding NUL at the index of the
			// terminating '\n' character. See man strcspn - this function
			// gets the length of a prefix substring.
			input[strcspn(input, "\n")] = 0;
			printf("Invalid input: no integer found in %s.\n", input);
			input = NULL;
		}
		if (errno != 0) {
			fprintf(stderr, "[ERROR]: That doesn't look like an integer.\n");
			input = NULL;
		}
	}
	free(inputBuffer);
}
