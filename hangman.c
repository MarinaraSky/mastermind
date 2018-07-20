#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 		35 /* Length of Supercalifragilisticexpialidocious + 1*/
#define LOSS_COUNT 		6
#define GUESS_LENGTH 	2

void
printUsage(void);

int
validateGuess(char *guess);

int
checkGuess(char *guess, char *secretWord, char *displayWord);

int 
main(int argc, char *argv[])
{
	char secretWord[MAX_LENGTH];
	int notWinning = 1;
	int guessCount = 0;
	setbuf(stdout, NULL);

	if(argc == 1)
	{
		/* Open .hangman file and .words files */
	}
	else if(argc == 2)
	{
		/* Open file on Command Line */
	}
	else
	{
		printf("Wrong amount of arguments\n");
		printUsage();
	}

	strcpy(secretWord, "hangman");	

	char *displayWord = malloc(sizeof(char) * sizeof(secretWord));
	for(unsigned int i = 0; i < strlen(secretWord); i++)
	{
		displayWord[i] = '_';
	}

	while(notWinning && guessCount < LOSS_COUNT)
	{
		char guess[GUESS_LENGTH];
		char buff[MAX_LENGTH];
		int garbage;

		printf("%s\n", displayWord);
		printf("Please enter a letter : ");
		fgets(buff, GUESS_LENGTH, stdin);
		while((garbage = getchar()) != '\n');
		strcpy(guess, buff);
		guess[1] = 0;
		if(validateGuess(guess))
		{
			if(checkGuess(guess, secretWord, displayWord))
			{
				if(strcmp(secretWord, displayWord) == 0)
				{
					notWinning = 0;
				}
			}
			else
			{
				guessCount++;
			}
		}
		else
		{
			printf("Invalid\n");
		}
	}
	printf("You win with %d chances left.\n", LOSS_COUNT - guessCount);

	free(displayWord);
	return 0;
}

void
printUsage(void)
{
	printf("USAGE: ./hangman [filename]\n");
}

int
validateGuess(char *guess)
{
	for(unsigned int i = 0; i < strlen(guess); i++)
	{
		if(isalpha(guess[i]))
		{
			if(isupper(guess[i]))
			{
				guess[i] = tolower(guess[i]);
			}
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

int
checkGuess(char *guess, char *secretWord, char *displayWord)
{
	int found = 0;
	for(unsigned int i = 0; i < strlen(secretWord); i++)
	{
		if(guess[0] == secretWord[i])
		{
			displayWord[i] = guess[0];
			found ++;
		}
	}
	if(found)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
