#pragma once

#include "debug.h"
#include <stdlib.h>

#define ASSERT(exp, msg)    				        \
{										   	                    \
	if(!(exp))					     		              \
	{										                      \
		DebugPrintError((msg));			            \
		exit(-1);							                  \
	}									   	                    \
}
