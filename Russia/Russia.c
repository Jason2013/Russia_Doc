#include <graphics.h>
#include <stdlib.h>
#include <dos.h>

#define TIMER 0x1c

#define VK_LEFT  0x4b00
#define VK_RIGHT 0x4d00
#define VK_DOWN  0x5000
#define VK_UP    0x4800
#define VK_HOME  0x4700
#define VK_END   0x4f00
#define VK_SPACE 0x3920
#define VK_ESC   0x011b
#define VK_ENTER 0x1c0d

#define SIZE 16
#define BGCOLOR DARKGRAY
#define FORECOLOR WHITE
#define SCOREINTERVAL 30


/* global variables */
int board[12][22]; /* game board */
int CurShape; /* index of shape which is falling */
int sx,sy; /* coordernate of shape which is falling  */
int next=-1; /* index of next shape prepared */
long score=0,speed=0,lnNextLevelScore=SCOREINTERVAL;


enum bool {False=0,True=1};
enum direct {Left,Right,Down};



void interrupt (*oldtimer)(void);/* function pointer variable */

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



unsigned int TimerCounter=0;

void interrupt (*oldtimer)(void);
void interrupt newtimer(void);

void SetTimer(void interrupt(*)(void));
void KillTimer();

void interrupt newtimer(void)
{
	(*oldtimer)();
	TimerCounter++;
}
int GetNextShape()
{
	int i;
	i=rand()%28;
	if (i<12) return i;
	if (i<24) return (i-12)/2+12;
	return 18;
}
void SetTimer(void interrupt(*IntProc)(void))
{
	oldtimer=getvect(TIMER);
	disable();
	setvect(TIMER,IntProc);
	enable();
}
void KillTimer()
{
	disable();
	setvect(TIMER,oldtimer);
	enable();
}

void MsgBoard(int x,int y,int width,char *szMsg)
{

	/* draw message board border */
	setcolor(WHITE);
	rectangle(x,y,x+width*16,y+16);

	/* draw message */
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(x+width*8,y,szMsg);
}
void NumBoard(int x,int y,int width,long num)
{
	char szBuffer[16];

	/* draw number board background */
	setfillstyle(SOLID_FILL,GREEN);
	bar(x,y,x+width*16,y+16);

	/* draw shadow */
	setcolor(YELLOW);
	line(x+1,y+17,x+width*16,y+17);
	line(x+width*16,y+1,x+width*16,y+17);

	/* draw number */
	sprintf(szBuffer,"%ld",num);
	setcolor(RED);
	settextjustify(RIGHT_TEXT,TOP_TEXT);
	outtextxy(x+width*16,y+1,szBuffer);
}

void Welcome()
{
	setcolor(WHITE);
	rectangle(160,120,480,360);
	rectangle(165,125,475,355);

	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(GREEN);
	outtextxy(320,120+16*4,"Russia");
	setcolor(YELLOW);
	outtextxy(320,120+16*10,"Chen Chang");
	setcolor(WHITE);
	outtextxy(320,120+18*10,"QQ:7019717");

	bioskey(0);
	cleardevice();
}


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
	/* reset variables */
	speed=0;
	score=0;
	lnNextLevelScore=SCOREINTERVAL;

	MsgBoard(SIZE*22,SIZE*5,7,"Next");

	MsgBoard(SIZE*22,SIZE*14,7,"Speed");
	NumBoard(SIZE*22,SIZE*15+3,7,0);

	MsgBoard(SIZE*22,SIZE*18,7,"Score");
	NumBoard(SIZE*22,SIZE*19+3,7,0);

	next=-1; /* new game having no next shape prepared */

}

enum bool Confilict(int ShapeIndex,int x,int y)
{
	int i;
	for (i=0;i<=7;i++,i++)
	{
		if (shapes[ShapeIndex].xy[i]+x<1 ||
			shapes[ShapeIndex].xy[i]+x>10) return True;
		if (shapes[ShapeIndex].xy[i+1]+y<1) continue;
		if (board[shapes[ShapeIndex].xy[i]+x][shapes[ShapeIndex].xy[i+1]+y])
			return True;
	}
	return False;
}
enum bool Moveable(int  ShapeIndex,int x,int y,enum direct dir)
{
	switch(dir)
	{
	case Left:
		if (Confilict(ShapeIndex,x-1,y))
			return False;
		return True;
	case Right:
		if (Confilict(ShapeIndex,x+1,y))
			return False;
		return True;
	case Down:
		if (Confilict(ShapeIndex,x,y+1))
			return False;
		return True;
	}
	return False;
}
void Move(int ShapeIndex,int *x,int *y,enum direct dir)
{
	DrawShape(ShapeIndex,*x,*y,BGCOLOR); /* erease original */
	switch(dir)
	{
	case Left:
		(*x)--;
		break;
	case Right:
		(*x)++;
		break;
	case Down:
		(*y)++;
	}
	DrawShape(ShapeIndex,*x,*y,shapes[ShapeIndex].color); /* draw new */
}
enum bool Rollable(int ShapeIndex,int x,int y,int *offset)
{
	int NextShape;
	static int arrOffset[5]={0,-1,1,-2,2};
	int i;
	NextShape=shapes[ShapeIndex].next;

	for (i=0;i<=4;i++)
		if (!Confilict(NextShape,x+arrOffset[i],y))
		{
			*offset=arrOffset[i];
			return True;
		}
	return False;
}
void Roll(int *ShapeIndex,int *x,int y,int offset)
{
	DrawShape(*ShapeIndex,*x,y,BGCOLOR); /* erease original */
	*ShapeIndex=shapes[*ShapeIndex].next; /* get next shape */
	*x+=offset;
	DrawShape(*ShapeIndex,*x,y,shapes[*ShapeIndex].color);/* draw new */
}

void DeleteRow(int y)
{
	int i,j;
	for (j=y;j>=2;j--)
		for (i=1;i<=10;i++)
		{
			if (board[i][j]==board[i][j-1]) continue;
			if (board[i][j-1]==1)
			{
				board[i][j]=1;
				setfillstyle(SOLID_FILL,FORECOLOR);
			}
			else /* board[i][j-1]==0 */
			{
				board[i][j]=0;
				setfillstyle(SOLID_FILL,BGCOLOR);
			}
			DrawBlock(i,j);
		}
}
enum bool RowFull(int y)
{
	int i;
	for (i=1;i<=10;i++)
		if (!board[i][y]) return False;
	return True;
}

int DeleteRows(int y)
{
	int i,rows=0;
	for (i=0;i<=3;i++)
	{
		while (RowFull(y))
		{
			DeleteRow(y);
			rows++;
			i++;
		}
		y--;
		if (y<1) break;
	}
	return rows;
};

int Bonus(int num)
{
	switch(num)
	{
	case 1:
		return 1;
	case 2:
		return 3;
	case 3:
		return 6;
	case 4:
		return 10;
	}
	return 0;
}


enum bool CheckGameOver()
{
	int i;
	for (i=1;i<=10;i++)
		if (board[i][1]) return True;
	return False;
}
enum bool GameOver()
{
	int key;
	settextjustify(CENTER_TEXT,TOP_TEXT);

	/* output game over message */
	setcolor(RED);
	outtextxy(SIZE*15,SIZE*12,"Game Over");
	setcolor(YELLOW);
	outtextxy(SIZE*15+2,SIZE*12+2,"Game Over");

	setcolor(GREEN);
	outtextxy(SIZE*15,SIZE*14,"Enter:New ");
	outtextxy(SIZE*15,SIZE*15,"Esc  :Exit");
	for(;;)
	{
		while(!bioskey(1));
		key=bioskey(0);
		if (key==VK_ENTER) return False; /* enter for new game */
		if (key==VK_ESC) return True;    /* escape to exit program */
	}
}

void main()
{
	int i,key,xOffset,increase;
	enum bool fGameOver=False,fAccel=False;
	int graphdriver=VGA,graphmode=VGAHI,errorcode;

	
	/* registerbgidriver(EGAVGA_driver); */
	
	/* register a driver that was added into graphics.lib */
	/* For information on adding the driver, see the
	/* BGIOBJ section of UTIL.DOC */
	/* errorcode = registerbgidriver(EGAVGA_driver); */

	/* report any registration errors */
	/* if (errorcode < 0)
	{
		printf("Graphics error: %s\n", grapherrormsg(errorcode));
		printf("Press any key to halt:");
		getch();
		exit(1); 
	}*/


	initgraph(&graphdriver,&graphmode,"e:\\tc2");

	/* read result of initialization */
	errorcode = graphresult();

	if (errorcode != grOk)  /* an error occurred */
	{
		printf("Graphics error: %s\n", grapherrormsg(errorcode));
		printf("Press any key to halt:");
		getch();
		exit(1);             /* return with error code */
	}


	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	randomize();

	SetTimer(newtimer);

	Welcome();

	for(;;)
	{
		NewGame();

		bioskey(0);

		for(;;)
		{
			if (next==-1) /* first */
			{
				CurShape=GetNextShape();
				next=GetNextShape();
				DrawShape(next,15,6,shapes[next].color);
			}
			else /* other */
			{
				CurShape=next;
				DrawShape(next,15,6,BGCOLOR);
				next=GetNextShape();
				DrawShape(next,15,6,shapes[next].color);
			}

			sx=5;sy=1; /* original position */

			TimerCounter=0; /* reset timer counter */

			DrawShape(CurShape,sx,sy,shapes[CurShape].color);

			for (;;)
			{
				if (bioskey(1)) key=bioskey(0);
				else key=0;

				fAccel=False;
				switch(key)
				{
				case VK_LEFT: /* move left */
					if (Moveable(CurShape,sx,sy,Left))
						Move(CurShape,&sx,&sy,Left);
					break;
				case VK_RIGHT: /* move right */
					if (Moveable(CurShape,sx,sy,Right))
						Move(CurShape,&sx,&sy,Right);
					break;
				case VK_UP: /* roll */
				case VK_SPACE:
					if (Rollable(CurShape,sx,sy,&xOffset))
						Roll(&CurShape,&sx,sy,xOffset);
					break;
				case VK_DOWN: /* accelerate the speed of moving down */
					fAccel=True;
					break;
				case VK_END: /* pause */
					bioskey(0);
					break;
				case VK_ESC: /* end game */
					fGameOver=True;
					break;
				}
				if (fGameOver) break;
				if (fAccel||TimerCounter> (20-speed*2) )
				{
					fAccel=False;
					/* reset timer counter */
					TimerCounter=0;

					if (Moveable(CurShape,sx,sy,Down))
						Move(CurShape,&sx,&sy,Down);
					else
					{
						DrawShape(CurShape,sx,sy,FORECOLOR);
						for (i=0;i<=7;i++,i++)
						{
							if (sy+shapes[CurShape].xy[i+1]<1) continue;
							board[sx+shapes[CurShape].xy[i]][sy+shapes[CurShape].xy[i+1]]=1;
						}

						increase=DeleteRows(sy);
						if(increase) /* add score */
						{
							score+=Bonus(increase);
							NumBoard(SIZE*22,SIZE*19+3,7,score);
							if (score>lnNextLevelScore) /* add speed */
							{
								lnNextLevelScore+=SCOREINTERVAL;
								speed++;
								NumBoard(SIZE*22,SIZE*15+3,7,speed);
							}
						}
						if (CheckGameOver()) fGameOver=True;
						break;
					}
				}
			}
			if (fGameOver) break;
		}

		if (GameOver()) break;
		fGameOver=False; /* start a new game */
	}

	KillTimer();
	closegraph();
}

