#include <stdlib.h>
#include <string.h>
#include "TTTCell.h"

TTTCell* _TTTCell_init_(char orig_char)
{
	TTTCell* rt = malloc(sizeof(TTTCell));
	rt->set_orig_char = &_TTTCell_set_orig_char_;
	rt->dealloc = &_TTTCell_dealloc_;
	rt->out_string = &_TTTCell_out_string_;
	rt->toggle_highlighted = &_TTTCell_toggle_highlighted_;
	rt->set_orig_char(rt, orig_char);
	rt->is_assigned = 0;
	rt->highlighted = 0;
	return rt;
}

void _TTTCell_set_orig_char_(TTTCell* cell, char new_char)
{
	cell->orig_char = new_char;
	cell->highlighted_str = calloc(9,sizeof(char));
	memcpy(cell->highlighted_str,"\x1b[7m",4);
	*(cell->highlighted_str+4) = new_char;
	strcat(cell->highlighted_str,"\033[0m");
	cell->is_assigned = 1;
}

void _TTTCell_dealloc_(TTTCell* cell)
{
	if(cell->highlighted_str) free(cell->highlighted_str);
	free(cell);
}

char* _TTTCell_out_string_(TTTCell* cell)
{
	return (cell->highlighted) ? cell->highlighted_str: &cell->orig_char;
}

void _TTTCell_toggle_highlighted_(TTTCell* cell)
{
	cell->highlighted = !cell->highlighted;
}
