#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MULTI 	0x01
#define BOT 	0x02

/*  get_rand will generate a random number between 0123-9876 */
void get_rand(char *mm_string, char switches);
/* validate_num is used to make sure player guess and mastermind
 * code numbers have uniqe digits as well as no symbols */
int validate_num(char *mm_string, char switches);
/* check_reds will see how many matching digits the player 
 * has guessed correctly */
int check_reds(char *guess, char *mm_string);
/* check_whites will see how many digits in the players guesse
 * are in the mastermind number but not in the right location */
int check_whites(char *guess, char *mm_string);

int main(int argc, char *argv[])
{
	char mm_num[5];
	char guess[255];
	int guess_count = 0;
	int red = 0;
	int white = 0;
	char switches = 0;

	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			if(argv[i][0] == 45) // checks if there is a hyphen
			{
				switch(argv[i][1])
				{
					case(104):	// case to check if -h is set
						printf("USAGE ./mastermind [-h] [-a] [-m]\n");
						printf("-h = Use this to see what you're seeing.\n");
						printf("-a = Auto play.\n");
						printf("-m = Multiple Digit Mode.\n");
						goto end;

					case(109):
						switches |= MULTI;
						break;
					
					case(97):
						switches |= BOT;
						break;
						
					default:
						printf("Invalid switch. Exiting\n");
						goto end;
				}
			}
		}
	}
	get_rand(mm_num, switches);
	printf("%s\n", mm_num); // uncomment to see answer
	int i = 0;
	while(red != 4) // win condition
	{
		if((switches & BOT) == BOT )
		{
			i++;
			sprintf(guess, "%04d\n", i);
			printf("Please enter a 4 digit number Mr. Robot: %s", guess);
		}
		else
		{
			printf("Plese enter a 4 digit number: ");
			fgets(guess, sizeof(guess), stdin);
		}
		if(strlen(guess) == 5)
		{
			guess[4] = 0;
			if(validate_num(guess, switches) == 1)
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
	end:
		return 0;
}

int check_whites(char *guess, char *mm_string)
{
	int count = 0;
	int last_count = 0;
	char *temp_guess = guess;
	char temp_mm_string[5];
	strcpy(temp_mm_string, mm_string);
	
	for(int i = 0; i <= 4; i++)	// gets rid of red pins
	{
		if(temp_guess[i] == temp_mm_string[i])
		{
			temp_guess[i] ^= temp_guess[i];
			temp_mm_string[i] ^= temp_mm_string[i];
		}
	}

	for(int i = 0; i <= 4; i++)	// loops though and nulls matches and 
								// counts the first match as white
	{
		for(int j = 0; j <= 4; j++)
		{
			if(temp_guess[i] == temp_mm_string[j] && i != j && temp_mm_string[j] != 0) 
			{
				if(last_count == count)
				{
					count++;
				}
				temp_mm_string[j] ^= temp_mm_string[j];
			}
		}
		last_count = count;
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

void get_rand(char *mm_string, char switches)
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

	}while(validate_num(mm_string, switches) == 0);
}

int validate_num(char *mm_string, char switches)
{
	int return_code = 1;
	if(!(switches & MULTI))
	{
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
	}

	return return_code;
}
