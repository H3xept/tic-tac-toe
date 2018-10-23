#include "TTTCell.h"

#define DEFAULT_BOARD_SIZE 3

typedef struct TTTBoard TTTBoard;
struct TTTBoard {
	TTTCell **cells;
	int rows;
	int cols;
	unsigned int pos_x;
	unsigned int pos_y;
	unsigned int view_is_visible;
	void (*render)(TTTBoard* board);
	void (*clear)(TTTBoard* board);
	void (*toggle_selected)(TTTBoard* board);
	void (*set_selected_to)(TTTBoard* board, char c);
	TTTCell* (*get_selected_cell)(TTTBoard* board);
	void (*move_cursor_to)(TTTBoard* board, int x, int y);
	void (*flush_board_view)(TTTBoard* board);
	TTTCell* (*get_cell_at)(TTTBoard* board, int x, int y);
	void (*dealloc)(TTTBoard* board);
	unsigned int (*is_full)(TTTBoard* board);
};

TTTBoard* _TTTBoard_init_();
TTTCell* _TTTBoard_get_selected_cell(TTTBoard* board);
TTTCell* _TTTBoard_get_cell_at_(TTTBoard* board, int x, int y);
void __init_cells__(TTTBoard* board);
void _TTTBoard_render_(TTTBoard* board);
void _TTTBoard_clear_(TTTBoard* board);
void _TTTBoard_dealloc_(TTTBoard* board);
void _TTTBoard_toggle_selected_(TTTBoard* board);
void _TTTBoard_set_selected_to_(TTTBoard* board, char c);
void _TTTBoard_move_cursor_to_(TTTBoard* board, int x, int y);
void _TTTBoard_flush_board_view_(TTTBoard* board);
unsigned int _TTTBoard_is_full_(TTTBoard* board);
void __print__middle__join__();
void __print__vert__bars__(char *s1, char *s2, char *s3);
void fill_2D_array(TTTCell** cells, int rows, int cols, char c);
