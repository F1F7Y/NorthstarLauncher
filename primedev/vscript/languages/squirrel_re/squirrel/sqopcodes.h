#pragma once

#include "vscript/languages/squirrel_re/include/squirrel.h"

struct alignas(4) SQInstruction
{
	int op;
	int arg1;
	int output;
	short arg2;
	short arg3;
};