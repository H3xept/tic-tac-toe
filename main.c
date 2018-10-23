#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "TTTGame.h"

#define MAX_USER_INPUT 30
#define USERNAME_1_REQUEST_MESSAGE "Please insert an username for player 1 : "
#define USERNAME_2_REQUEST_MESSAGE "Please insert an username for player 2 : "
#define CHARACTER_REQUEST_MESSAGE "Please choose 'X' or 'O' as player 1 char: "
#define N_PLAYERS_REQUEST_MESSAGE "How many players? "
#define N_PLAYS_REQUEST_MESSAGE "How many games do you want to play?\n"
#define DRAW_MESSAGE "It's a draw!\n"
#define REPLAY_MESSAGE "Do you want to play another game? [y/n]"
#define PRINT_PLAYER_WON_MESSAGE(X) y_printf("Congrats! Player %s won!\n",X)
#define PRINT_SCORES(u0, u1, s0,s1) y_printf("Scores:\n'%s':%d points\n'%s':%d points\n",u0,s0,u1,s1)
#define WELCOME_MESSAGE "********************************************\n\
*  _______    ______        ______         *\n\
* /_  __(_)__/_  __/__ ____/_  __/__  ___  *\n\
*  / / / / __// / / _ `/ __// / / _ \\/ -_) *\n\
* /_/ /_/\\__//_/  \\_,_/\\__//_/  \\___/\\__/  *\n\
********************************************\n"

void user_prompt(char *message, char *answer);

int main(int argc, char const **argv)
{
	char* username_1 = calloc(MAX_USER_INPUT,sizeof(char));
	char* username_2 = calloc(MAX_USER_INPUT,sizeof(char));
	char* character = malloc(sizeof(char)*MAX_USER_INPUT);
	char* players_n = calloc(MAX_USER_INPUT,sizeof(char));
	int p1_wins = 0, p2_wins = 0;
	char* play_again = calloc(MAX_USER_INPUT,sizeof(char));
	y_printf("%s",WELCOME_MESSAGE);

	user_prompt(N_PLAYERS_REQUEST_MESSAGE, players_n);
	user_prompt(USERNAME_1_REQUEST_MESSAGE, username_1);

	if (atoi(players_n) > 1)
		user_prompt(USERNAME_2_REQUEST_MESSAGE, username_2);
	else
		memcpy(username_2, "Computer", 8);
	do
	{
		user_prompt(CHARACTER_REQUEST_MESSAGE, character);
	} while ((*character != 'X') && (*character != 'O'));


	TTTGame* game;

	do {
	game = _TTTGame_init_(username_1,username_2,*character,(*character == 'X') ? 'O' : 'X');
	game->render_board(game);

	void (*exec_player_2_action)(TTTGame* game);
	if (atoi(players_n) > 1)
		exec_player_2_action = game->request_and_set_user_cell;
	else exec_player_2_action = game->AI_move;

	while(game->game_status == TTTGame_Status_Ongoing)
	{
		if(game->turn_count % 2)
			exec_player_2_action(game);
		else
			game->request_and_set_user_cell(game);
	}

	switch(game->game_status)
	{
		case TTTGame_Status_Draw:
			y_printf("%s", DRAW_MESSAGE);
			break;
		default:
			PRINT_PLAYER_WON_MESSAGE((game->game_status == 0) ? game->username_1 : game->username_2);
			if (game->game_status == 0)
				p1_wins++;
			else
				p2_wins++;
	}

	PRINT_SCORES(game->username_1, game->username_2, p1_wins, p2_wins);
	user_prompt(REPLAY_MESSAGE,play_again);
	} while(*play_again == 'y');
	return 0;
}

void user_prompt(char *message, char *answer)
{	
	printf("%s\n",message);
	y_printf("%s","ticktacktoe $ ");
	fgets(answer, MAX_USER_INPUT, stdin);
	*(answer+strlen(answer)-1) = '\0';
}
