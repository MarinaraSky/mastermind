#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 		35 /* Length of Supercalifragilisticexpialidocious + 1*/
#define LOSS_COUNT 		5
#define GUESS_LENGTH 	2

void
printUsage(void);

int
validateGuess(char *guess);

void
checkGuess(char *guess, char *secretWord);

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

	while(notWinning || guessCount < LOSS_COUNT)
	{
		char guess[GUESS_LENGTH];
		char buff[MAX_LENGTH];
		int garbage;
	

		printf("Please enter a letter : ");
		fgets(buff, GUESS_LENGTH, stdin);
		while((garbage = getchar()) != '\n');
		strcpy(guess, buff);
		guess[1] = 0;
		if(validateGuess(guess))
		{
			printf("Valid\n");
		}
		else
		{
			printf("Invalid\n");
		}
		printf("%s\n", guess);
	}

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

void
checkGuess(char *guess, char *secretWord)
{
	/* Check Here */
}
