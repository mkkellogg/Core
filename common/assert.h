#pragma once

#include "debug.h"
#include <stdlib.h>

#define ASSERT(exp, msg)    				        \
{										   	                    \
	if(!(exp))					     		              \
	{										                      \
		Debug::PrintError((msg));			            \
		exit(-1);							                  \
	}									   	                    \
}
