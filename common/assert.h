//
// Created by Mark Kellogg on 12/14/17.
//

#ifndef MPM_ASSERT_H
#define MPM_ASSERT_H

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

#endif //MPM_ASSERT_H
