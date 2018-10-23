#include <stdarg.h>
#include <stdio.h>
#include "colors.h"

void cprintf(char* color_code, char* format, ...)
{
	va_list args; 
	va_start(args,format);
	printf("%s",color_code);
	vprintf(format,args);
	printf("%s",RESET_COLOR);
	va_end(args);
}
