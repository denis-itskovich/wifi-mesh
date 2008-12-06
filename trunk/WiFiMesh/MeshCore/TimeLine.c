#include "TimeLine.h"

struct
struct _TimeLine
{
	ListHeader* pEvents;
};

EStatus TimeLineCreate(TimeLine** ppThis);
EStatus TimeLineDestroy(TimeLine** ppThis);

EStatus TimeLineAddEvent(TimeLine* pThis, Event* pEvent);
EStatus TimeLineGetNextEvent(TimeLine* pThis, Event** pEvent);
