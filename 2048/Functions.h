#ifndef _Functions_h
#define _Functions_h

void MoveLeft(int x, int y, int tx, int value, bool add);
void Points();
void tileAnimHorizontal(int x, int y, int x1, int value, bool add);
void tileAnimVertical(int x, int y, int y1, int value, bool add);
void randomGen();
void copyBoard();
void upMove(); 
void downMove();
void leftMove();
void rightMove();
bool checkWin();
bool checkLose();
void LoadBitmaps();
void DrawTile(int i, int j, int value, bool coord);
void DrawResult();
void MoveUp(int x, int y, int x1, int value,bool add);
void MoveDown(int x, int y, int x1, int value, bool add);
void MoveLeft(int x, int y, int x1, int value, bool add);
void MoveRight(int x, int y, int x1, int value, bool add);
void Title();

#endif