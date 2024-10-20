//
// COMP-GENG 421 - Tom Lupfer
//
// Navigation switch module header
//

#ifndef NAV_H_
#define NAV_H_

void NavInit(void);
void NavClock(void);
Bool NavUpIsPressed(void);
void NavClearUpIsPressed(void);
Bool NavDownIsPressed(void);
void NavClearDownIsPressed(void);
Bool NavLeftIsPressed(void);
void NavClearLeftIsPressed(void);
Bool NavRightIsPressed(void);
void NavClearRightIsPressed(void);
Bool NavSelectIsPressed(void);
void NavClearSelectIsPressed(void);

#endif	//NAV_H_
