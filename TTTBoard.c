#include <stdlib.h>
#include <stdio.h>
#include "TTTBoard.h"

TTTBoard* _TTTBoard_init_()
{
	TTTBoard* rt = malloc(sizeof(TTTBoard));
	rt->rows = DEFAULT_BOARD_SIZE;
	rt->cols = DEFAULT_BOARD_SIZE;
	rt->pos_x = 0;
	rt->pos_y = 0;
	rt->view_is_visible = 0;
	rt->dealloc = &_TTTBoard_dealloc_;
	rt->render = &_TTTBoard_render_;
	rt->clear = &_TTTBoard_clear_;
	rt->toggle_selected = &_TTTBoard_toggle_selected_;
	rt->set_selected_to = &_TTTBoard_set_selected_to_;
	rt->move_cursor_to = &_TTTBoard_move_cursor_to_;
	rt->get_selected_cell = &_TTTBoard_get_selected_cell;
	rt->flush_board_view = &_TTTBoard_flush_board_view_;
	rt->get_cell_at = &_TTTBoard_get_cell_at_;
	rt->is_full = &_TTTBoard_is_full_;
	__init_cells__(rt);
	return rt;
}

void __init_cells__(TTTBoard* board)
{
	int i = 0, j = 0;
	board->cells = malloc(sizeof(TTTCell*)*board->rows);
	for (i = 0; i < board->rows; i++){
		board->cells[i] = malloc(sizeof(TTTCell)*board->cols);
		for (j = 0; j < board->cols; j++){
			board->cells[i][j] = *_TTTCell_init_(' ');
		}
	}
}

void _TTTBoard_dealloc_(TTTBoard* board)
{
	if(board)
		free(board);
}

void _TTTBoard_clear_(TTTBoard* board)
{
	fill_2D_array(board->cells, board->rows, board->cols, ' ');
}

void _TTTBoard_toggle_selected_(TTTBoard* board)
{
	board->get_selected_cell(board)->toggle_highlighted(board->get_selected_cell(board));
}

void fill_2D_array(TTTCell** cells, int rows, int cols, char c)
{
	int i = 0, j = 0;
	for (; i < rows; i++)
	{
		for (j = 0; j < cols; j++){
			cells[i][j].set_orig_char(&cells[i][j],c);
		}
	}
}

void _TTTBoard_render_(TTTBoard* board)
{
	int i = 0;
	int col = 0;

	for (; i < 5; i++){
		if (i % 2)
		{
			__print__middle__join__();
		} else {
			__print__vert__bars__(
				(board->cells[col][0].orig_char) ? board->cells[col][0].out_string(&board->cells[col][0]) : " ",
				(board->cells[col][1].orig_char) ? board->cells[col][1].out_string(&board->cells[col][1]) : " ",
				(board->cells[col][2].orig_char) ? board->cells[col][2].out_string(&board->cells[col][2]) : " ");
			col++;
		}
	}

	board->view_is_visible = 1;
}

void _TTTBoard_set_selected_to_(TTTBoard* board, char c)
{
	board->get_selected_cell(board)->set_orig_char(board->get_selected_cell(board),c);
}

void _TTTBoard_move_cursor_to_(TTTBoard* board, int x, int y)
{
	board->pos_y = y;
	board->pos_x = x;
}

TTTCell* _TTTBoard_get_selected_cell(TTTBoard* board)
{
	return &board->cells[board->pos_y][board->pos_x];
}

void _TTTBoard_flush_board_view_(TTTBoard* board)
{	
	int i = 0;
	if (!board->view_is_visible) return;
	
	for (; i < board->rows*2-1; i++)
	{
		printf("\033[1A");
		printf("\033[K");
	}
	board->view_is_visible = 0;
}

TTTCell* _TTTBoard_get_cell_at_(TTTBoard* board, int x, int y)
{
	return &board->cells[y][x];
}

__attribute__((always_inline))
void __print__middle__join__()
{
	printf("═══╬═══╬═══\n");
}

__attribute__((always_inline))
void __print__vert__bars__(char *s1, char *s2, char *s3)
{
	printf(" %s ║ %s ║ %s \n", s1,s2,s3);
}

unsigned int _TTTBoard_is_full_(TTTBoard* board)
{
	int row_c, col_c;
	int tot = 0;
	for (row_c = 0; row_c < board->rows; row_c++)
	{
		for(col_c = 0; col_c < board->cols; col_c++)
			tot += (board->get_cell_at(board, row_c, col_c)->orig_char == ' ') ? 0 : 1;
	}
	if (tot == board->rows*board->cols)
		return 1;
	return 0;
}
