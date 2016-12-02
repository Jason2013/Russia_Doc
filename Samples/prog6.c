#include <graphics.h>
#include <dos.h>


#define SIZE 16
#define BGCOLOR DARKGRAY
#define FORECOLOR WHITE


/* global variables*/
int board[12][22]; /* game board */
int CurShape; /* index of shape which is falling */
int sx,sy; /* coordernate of shape which is falling  */


struct shape
{
	int xy[8];
	int color;
	int next;
};

struct shape shapes[19]=
	{
  /*{x1,y1,x2,y2,x3,y3,x4,y4, color, next}*/
	{ 0,-2, 0,-1, 0, 0, 1, 0,  CYAN,    1}, /*        */
	{-1, 0, 0, 0, 1,-1, 1, 0,  CYAN,    2}, /*   #    */
	{ 0,-2, 1,-2, 1,-1, 1, 0,  CYAN,    3}, /*   #    */
	{-1,-1,-1, 0, 0,-1, 1,-1,  CYAN,    0}, /*   ##   */

	{ 0,-2, 0,-1, 0, 0, 1,-2,MAGENTA,   5}, /*        */
	{-1,-1,-1, 0, 0, 0, 1, 0,MAGENTA,   6}, /*   ##   */
	{ 0, 0, 1,-2, 1,-1, 1, 0,MAGENTA,   7}, /*   #    */
	{-1,-1, 0,-1, 1,-1, 1, 0,MAGENTA,   4}, /*   #    */

	{-1, 0, 0,-1, 0, 0, 1, 0,YELLOW,    9}, /*        */
	{-1,-1, 0,-2, 0,-1, 0, 0,YELLOW,   10}, /*        */
	{-1,-1, 0,-1, 0, 0, 1,-1,YELLOW,   11}, /*   #    */
	{ 0,-2, 0,-1, 0, 0, 1,-1,YELLOW,    8}, /*  ###   */

	{-1, 0, 0,-1, 0, 0, 1,-1, BROWN,   13}, /*   ##   */
	{ 0,-2, 0,-1, 1,-1, 1, 0, BROWN,   12}, /*  ##    */

	{-1,-1, 0,-1, 0, 0, 1, 0, WHITE,   15}, /*  ##    */
	{ 0,-1, 0, 0, 1,-2, 1,-1, WHITE,   14}, /*   ##   */

	{ 0,-3, 0,-2, 0,-1, 0, 0,   RED,   17},/*  #     */
	{-1, 0, 0, 0, 1, 0, 2, 0,   RED,   16},/*  #     */
										   /*  #     */
										   /*  #     */

	{ 0,-1, 0, 0, 1,-1, 1, 0,  BLUE,   18},/*  ##    */
										   /*  ##    */

	};



void DrawBlock(int x,int y)
{
	if (y<1) return;
	bar(SIZE*(x+9)+1,SIZE*(y+4)+1,SIZE*(x+10)-1,SIZE*(y+5)-1);
}
void DrawShape(int index,int sx,int sy,int color)
{
	int i,c;
	setfillstyle(SOLID_FILL,color);
	for (i=0;i<=7;i++,i++)
		DrawBlock(shapes[index].xy[i]+sx,shapes[index].xy[i+1]+sy);
}
void NewGame()
{
	int i,j;
	/* reset game board */
	for (j=0;j<=20;j++)
		for (i=0;i<=11;i++)
		{
			if (i==0||i==11) board[i][j]=1;
			else board[i][j]=0;
		}
	for (i=0;i<=11;i++)
		board[i][21]=1;

	/* draw game board border*/
	setfillstyle(SOLID_FILL,BLACK);
	bar(SIZE*10,SIZE*5,SIZE*20,SIZE*25);
	setcolor(WHITE);
	rectangle(SIZE*10,SIZE*5,SIZE*20,SIZE*25);

	/* draw game board background */
	setfillstyle(SOLID_FILL,BGCOLOR);

	for (j=1;j<=20;j++)
		for (i=1;i<=10;i++)
			DrawBlock(i,j);

	/* draw next board background */
	for (j=3;j<=6;j++)
		for (i=14;i<=17;i++)
			DrawBlock(i,j);
}



void main()
{

	int graphdriver=VGA,graphmode=VGAHI;
	/*registerbgidriver(EGAVGA_driver);*/
	initgraph(&graphdriver,&graphmode,"e:\\tc2");

	NewGame();
	sx=15;sy=6; /* 在显示“下一个”的地方显示形状 */
	for (CurShape=0;CurShape<=18 ;CurShape
	++)
	{
		DrawShape(CurShape,sx,sy,shapes[CurShape].color);
		bioskey(0);
		DrawShape(CurShape,sx,sy,BGCOLOR);
	}

	closegraph();
}

