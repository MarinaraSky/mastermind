#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/*  get_rand will generate a random number between 0123-9876 */
void get_rand(char *mm_string);
/* validate_num is used to make sure player guess and mastermind
 * code numbers have uniqe digits as well as no symbols */
int validate_num(char *mm_string);
/* check_reds will see how many matching digits the player 
 * has guessed correctly */
int check_reds(char *guess, char *mm_string);
/* check_whites will see how many digits in the players guesse
 * are in the mastermind number but not in the right location */
int check_whites(char *guess, char *mm_string);

int main(void)
{
	char mm_num[5];
	char guess[255];
	int guess_count = 0;
	int red = 0;
	int white = 0;
	get_rand(mm_num);
//	printf("%s\n", mm_num); // uncomment to see answer
	while(red != 4) // win condition
	{
		printf("Plese enter a 4 digit number: ");
		fgets(guess, sizeof(guess), stdin);
		if(strlen(guess) == 5)
		{
			if(validate_num(guess) == 1)
			{
				red = check_reds(guess, mm_num);
				white = check_whites(guess, mm_num);
				printf("reds %d\nwhite %d\n", red, white);
				guess_count++;
			}
			else
			{
				printf("Bad number selection.\n");
			}
		}
		else
		{
			printf("Wrong amount of digits.\n");
		}
	}
	printf("You win with %d guesses.\n", guess_count);
	return 0;
}

int check_whites(char *guess, char *mm_string)
{
	int count = 0;
	char *temp_guess = guess;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(temp_guess[i] == mm_string[j] && i != j && temp_guess[i] != 0)
			{
				temp_guess[i] = 0;
				count++;	
			}
		}
	}
	return count;
}

int check_reds(char *guess, char *mm_string)
{
	int count = 0;
	while(*guess)
	{
		if(*guess++ == *mm_string++)
		{
			count++;	
		}
	}
	return count;
}

void get_rand(char *mm_string)
{
	char buff[1];
	unsigned int rand_int;

	do
	{
		int randy = open("/dev/urandom", O_RDONLY);
		if(randy == -1) // file fails to open
		{
			perror("Error getting random seed ");
			exit(1);
		}
		for(int i = 0; i < 4; i++)
		{
			read(randy, buff, sizeof(buff));
			rand_int = (unsigned int) *buff; // typecasted for srand()
		}
		close(randy);
		srand(rand_int);
		rand_int = rand() % 10000; // random number between 0 and 9999
		sprintf(mm_string, "%04d", rand_int); // pads with zeros if needed

	}while(validate_num(mm_string) == 0);
}

int validate_num(char *mm_string)
{
	int return_code = 1;
	for(int i = 0; i < 3; i++)
	{
		for(int j = i + 1; j <= 4; j++)
		{
			if(mm_string[i] == mm_string[j] || 
					mm_string[i] < 48 || mm_string[i] > 57)
			{	
				return_code = 0;
			}
		}
	}
	return return_code;
}
