#include "TTTCell.h"
#include "TTTBoard.h"
#include <termios.h>

struct termios original_terminal_attribs;

typedef enum {
	TTTGame_Status_Player1_Won,
	TTTGame_Status_Draw,
	TTTGame_Status_Player2_Won,
	TTTGame_Status_Ongoing
} TTTGame_Status;

typedef struct TTTGame TTTGame;
struct TTTGame{
	TTTBoard* board;
	char* username_1;
	char* username_2;
	char character_1;
	char character_2;
	unsigned int selection_mode;
	unsigned int turn_count;
	TTTGame_Status game_status;
	void (*render_board)(TTTGame* game);
	void (*clear_screen)(TTTGame* game);
	void (*deselect_current_cell)(TTTGame* game);
	void (*set_selection_mode)(TTTGame* game, unsigned int active);
	void (*navigate_up_arrow)(TTTGame* game);
	void (*navigate_down_arrow)(TTTGame* game);
	void (*navigate_left_arrow)(TTTGame* game);
	void (*navigate_right_arrow)(TTTGame* game);
	void (*clear_board)(TTTGame* game);
	int (*get_valid_cell)(TTTGame* game);
	void (*request_and_set_user_cell)(TTTGame* game);
	void (*calc_game_status)(TTTGame* game);
	void (*AI_move)(TTTGame* game);

} ;

TTTGame* _TTTGame_init_(char* username_1, char* username_2, char character_1, char character_2);
void _TTTGame_clear_board_(TTTGame* game);
void _TTTGame_render_board_(TTTGame* game);
void _TTTGame_clear_screen_(TTTGame* game);
void _TTTGame_set_selection_mode_(TTTGame* game, unsigned int active);
void _TTTGame_deselect_current_cell_(TTTGame* game);
void _TTTGame_navigate_up_arrow_(TTTGame* game);
void _TTTGame_navigate_down_arrow_(TTTGame* game);
void _TTTGame_navigate_left_arrow_(TTTGame* game);
void _TTTGame_navigate_right_arrow_(TTTGame* game);
int _TTTGame_get_valid_cell_(TTTGame* game);
void _TTTGame_request_and_set_user_cell_(TTTGame* game);
void _TTTGame_calc_game_status_(TTTGame* game);
void _navigate_(TTTGame* game, unsigned int condition, unsigned int *d, int increment);
void _TTTGame_AI_move_(TTTGame* game);
