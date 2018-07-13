#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

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
	char switches = 0;
	int guess_count = 0;
	int red = 0;
	int white = 0;
	int i = 0;
	time_t start_time = 0;
	time_t end_time = 0;
	int total_time = 0;

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
						printf("Optional file .mm can be in the same directory.\n"
								"If .mm is used the -m flag requirements still"
								" apply.\n");
						goto end;

					case(109):	// case for -m
						switches |= MULTI;
						break;
					
					case(97):	//case for -a
						switches |= BOT;
						break;
						
					default:
						printf("Invalid switch. Try ./mastermind -h. Exiting\n");
						goto end;
				}
			}
		}
	}
	get_rand(mm_num, switches);
	printf("%s\n", mm_num); // uncomment to see answer
	while(red != 4) // win condition
	{
		if((switches & BOT) == BOT )
		{
			i++;
			start_time = time(NULL);
			sprintf(guess, "%04d\n", i);
			printf("Please enter a 4 digit number Mr. Robot: %s", guess);
		}
		else
		{
			printf("Plese enter a 4 digit number: ");
			start_time = time(NULL); 
			fgets(guess, sizeof(guess), stdin);
		}
		if(strlen(guess) == 5)
		{
			guess[4] = 0;
			if(validate_num(guess, switches) == 1)
			{
				end_time = time(NULL);
				total_time += (end_time - start_time);
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
	printf("Average time per guess: %.2f seconds\n", 
			(total_time / (float) guess_count));
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
	char buff[16];
	unsigned int rand_int;
	int found_mm = 0;
	int randy;
	int loop = 0;	// used to only check .mm once

	do
	{
		if(found_mm == 0 && loop == 0)
		{
			randy = open(".mm", O_RDONLY);
			printf("I may have a good code stashed around here\n");
			sleep(2);
			if(randy == -1)
			{
				printf("Strange. Thought I left it right here.\n");
				sleep(1);
				found_mm = 1; // failed to find .mm
			}
		}
		else
		{
			randy = open("/dev/urandom", O_RDONLY);
			if(randy == -1) // file fails to open
			{
				perror("Error getting random seed ");
				exit(1);
			}
		}
		read(randy, buff, sizeof(buff));
		if(found_mm == 0 && loop == 0)
		{
			buff[4] = '\0';
			sprintf(mm_string, "%4s", buff);
			loop++;
		}
		else if(loop == 1)
		{
			printf("My secret code had been foiled.\n");
			sleep(1);
			loop++;
		}
		else
		{
			printf("Thinking of a really hard code for you.\n");
			sleep(1);
			rand_int = (unsigned int) *buff; // typecasted for srand()
			srand(rand_int);
			rand_int = rand() % 10000; // random number between 0 and 9999
			sprintf(mm_string, "%04d", rand_int); // pads with zeros if needed
		}
		close(randy);

	}while(validate_num(mm_string, switches) == 0);
}

int validate_num(char *mm_string, char switches)
{
	int return_code = 1;
	if(!(switches & MULTI))
	{
		for(int i = 0; i < 4; i++)
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
