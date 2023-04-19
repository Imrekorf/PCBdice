#include <xc.h>
#include "events.h"

unsigned char eventExecute (unsigned short* event, unsigned short eventtime)
{
	if ((eventtime) < (TMR1 - *event)) {
		*event += eventtime;
		return 1;
	}
	return 0;
}
