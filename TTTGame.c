#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "TTTGame.h"
#include <time.h>

void poll_raw(TTTGame* game);
void disable_raw();
void enable_raw();

TTTGame* _TTTGame_init_(char* username_1, char* username_2, char character_1, char character_2)
{
	TTTGame* game = (TTTGame*)malloc(sizeof(TTTGame));
	game->board = _TTTBoard_init_();
	game->username_1 = username_1;
	game->username_2 = username_2;
	game->character_1 = character_1;
	game->character_2 = character_2;
	game->turn_count = 0;
	game->selection_mode = 0;
	game->game_status = TTTGame_Status_Ongoing;
	game->clear_board = &_TTTGame_clear_board_;
	game->render_board = &_TTTGame_render_board_;
	game->clear_screen = &_TTTGame_clear_screen_;
	game->set_selection_mode = &_TTTGame_set_selection_mode_;
	game->deselect_current_cell = &_TTTGame_deselect_current_cell_;
	game->navigate_up_arrow = &_TTTGame_navigate_up_arrow_;
	game->navigate_down_arrow = &_TTTGame_navigate_down_arrow_;
	game->navigate_left_arrow = &_TTTGame_navigate_left_arrow_;
	game->navigate_right_arrow = &_TTTGame_navigate_right_arrow_;
	game->get_valid_cell = &_TTTGame_get_valid_cell_;
	game->request_and_set_user_cell = &_TTTGame_request_and_set_user_cell_;
	game->calc_game_status = &_TTTGame_calc_game_status_;
	game->AI_move = &_TTTGame_AI_move_;
	return game;
}

void _TTTGame_clear_board_(TTTGame* game)
{	
	TTTBoard *board = game->board;
	board->clear(board);
}

void _TTTGame_clear_screen_(TTTGame* game)
{
	TTTBoard *board = game->board;
	board->flush_board_view(board);
}

void _TTTGame_render_board_(TTTGame* game)
{
	TTTBoard *board = game->board;
	game->clear_screen(game);
	board->render(board);
}

void _TTTGame_navigate_up_arrow_(TTTGame* game) {
	TTTBoard *board = game->board;
	_navigate_(game,board->pos_y,&board->pos_y,-1);
}

void _TTTGame_navigate_down_arrow_(TTTGame* game) {
	TTTBoard *board = game->board;
	_navigate_(game,board->pos_y < board->rows-1,&board->pos_y,1);
}

void _TTTGame_navigate_left_arrow_(TTTGame* game) {
	TTTBoard *board = game->board;
	_navigate_(game,board->pos_x,&board->pos_x,-1);
}

void _TTTGame_navigate_right_arrow_(TTTGame* game) {
	TTTBoard *board = game->board;
	_navigate_(game,board->pos_x < board->cols-1,&board->pos_x,1);
}

void _TTTGame_deselect_current_cell_(TTTGame* game)
{
	TTTBoard *board = game->board;
	TTTCell* cell = &board->cells[board->pos_y][board->pos_x];
	if (cell->highlighted)
		cell->toggle_highlighted(cell);
}

void _navigate_(TTTGame* game, unsigned int condition, unsigned int *d, int increment)
{
	TTTBoard *board = game->board;
	if(condition) {
		game->deselect_current_cell(game);
		*d += increment;
		board->toggle_selected(board);
		game->render_board(game);
	}
}

void disable_raw()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal_attribs);
}

/*
https://linux.die.net/man/3/tcgetattr
*/
void enable_raw()
{	
	tcgetattr(STDIN_FILENO, &original_terminal_attribs);
	struct termios local_attribs = original_terminal_attribs;
	local_attribs.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &local_attribs);
}

void poll_raw(TTTGame* game)
{	
	int index = 0;
	char curr_bytes[3] = {0};
	char c;
	enable_raw();
	atexit(disable_raw);

	// printf("Entering gamemode, Cltr-C to exit...\n");
	while (read(STDIN_FILENO, &c, 1) == 1)
	{	
		if (c == 10){
			disable_raw();
			return;
		}

		curr_bytes[(index++)%3] = c;
		if (curr_bytes[0] == 27 && curr_bytes[1] == 91)
		{
			if (curr_bytes[2] == 0)
				continue;
			switch(curr_bytes[2])
			{
				case 65:
					game->navigate_up_arrow(game);
					break;
				case 66:
					game->navigate_down_arrow(game);
					break;
				case 68:
					game->navigate_left_arrow(game);
					break;
				case 67:
					game->navigate_right_arrow(game);
			}
			curr_bytes[0] = 0;
			curr_bytes[1] = 0;
			curr_bytes[2] = 0;
			index = 0;
		} else if (curr_bytes[0] != 27) {
			index = 0;
		}
	}

	disable_raw();
}

void _TTTGame_set_selection_mode_(TTTGame* game, unsigned int active)
{
	game->selection_mode = active;
	if (active){
		if (!game->board->get_selected_cell(game->board)->highlighted)
			game->board->toggle_selected(game->board);
		game->render_board(game);
		poll_raw(game);
	}
}

/* 
	0 got vaid cell
	-1 no valid cells available
*/
int _TTTGame_get_valid_cell_(TTTGame* game)
{	
	do
	{
		game->set_selection_mode(game, 1);
	} while (game->board->get_selected_cell(game->board)->is_assigned);
	game->set_selection_mode(game, 0);
	return 0;
}

/* 
	0 got vaid cell
	-1 no valid cells available
*/
void _TTTGame_request_and_set_user_cell_(TTTGame* game)
{
	if (game->get_valid_cell(game) != -1)
	{
		char c = (game->turn_count % 2) ? game->character_2 : game->character_1;
		game->board->set_selected_to(game->board, c);
		game->turn_count = ~(game->turn_count);
		game->deselect_current_cell(game);
		game->render_board(game);
		game->calc_game_status(game);
	}
}

void _TTTGame_calc_game_status_(TTTGame* game)
{
	int col_n = 0, row_n = 0;
	TTTBoard* board = game->board;
	unsigned int win1 = 1;
	unsigned int win2 = 1;
	unsigned int d_w = 0;

	for (; row_n < board->rows; row_n++)
	{
		char d1 = board->get_cell_at(board, row_n, row_n)->orig_char;
		if (d1 == ' ')
			continue;
		win1 = 1;
		win2 = 1;
		for (col_n = 0; col_n < board->rows; col_n++)
		{
			char cell1_c = board->get_cell_at(board, row_n, col_n)->orig_char;
			char cell2_c = board->get_cell_at(board, col_n, row_n)->orig_char;

			if (d1 != cell1_c && win1)
				win1 = 0;
			if (d1 != cell2_c && win2)
				win2 = 0;
		}

		if (row_n == (board->rows-1)/2)
		{
			if ((d1 == board->get_cell_at(board, board->rows-1,board->cols-1)->orig_char &&
				d1 == board->get_cell_at(board, 0,0)->orig_char) || 
				(d1 == board->get_cell_at(board, 0,board->cols-1)->orig_char &&
				d1 == board->get_cell_at(board, board->rows-1,0)->orig_char)){
				d_w = 1;
			}
		}

		if (win1 || win2 || d_w){
			game->game_status = (d1 == game->character_1) ? TTTGame_Status_Player1_Won : TTTGame_Status_Player2_Won;
			return;
		}

	} game->game_status = (game->board->is_full(game->board)) ? TTTGame_Status_Draw : TTTGame_Status_Ongoing;
}

void _TTTGame_AI_move_(TTTGame* game)
{
	srand(time(NULL));
	int rand_row, rand_col;
	do {
		rand_row = rand() % game->board->rows;
		rand_col = rand() % game->board->cols;
	}
	while(game->board->get_cell_at(game->board, rand_row, rand_col)->is_assigned);
	
	game->turn_count = ~(game->turn_count);
	game->board->move_cursor_to(game->board, rand_row, rand_col);
	game->board->set_selected_to(game->board, game->character_2);
	game->render_board(game);
	game->calc_game_status(game);
}
