#ifndef __TTTCell__
#define __TTTCell__

typedef struct TTTCell TTTCell;

struct TTTCell 
{
	char orig_char;
	unsigned int highlighted;
	char* highlighted_str;
	unsigned int is_assigned;
	void (*set_orig_char)(TTTCell *cell, char new_char);
	char* (*out_string)(TTTCell *cell);
	void (*toggle_highlighted)(TTTCell* cell);
	void (*dealloc)(TTTCell *cell);
};
TTTCell* _TTTCell_init_(char orig_char);
void _TTTCell_set_orig_char_(TTTCell* cell, char new_char);
char* _TTTCell_out_string_(TTTCell* cell);
void _TTTCell_toggle_highlighted_(TTTCell* cell);
void _TTTCell_dealloc_(TTTCell* cell);
#endif
