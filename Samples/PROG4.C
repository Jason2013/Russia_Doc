#include <graphics.h>
#include <bios.h>

#define VK_LEFT  0x4b00
#define VK_RIGHT 0x4d00
#define VK_DOWN  0x5000
#define VK_UP    0x4800
#define VK_HOME  0x4700
#define VK_END   0x4f00
#define VK_SPACE 0x3920
#define VK_ESC   0x011b
#define VK_ENTER 0x1c0d

#define SCREENWIDTH  640
#define SCREENHEIGHT 480
#define SIZE 16
#define BGCOLOR BLACK
#define FORECOLOR YELLOW

#define SCOREINTERVAL 30

void DrawBlock(int x,int y,int color)
{
	setfillstyle(SOLID_FILL,color);
	bar(SIZE*x,SIZE*y,SIZE*(x+1),SIZE*(y+1));
}

enum direct{Left,Up,Right,Down};
enum bool{False,True};

enum bool Moveable(int x,int y,enum direct dir)
{
	switch(dir)
	{
	case Left:
		if (x>0) return True;
		break;
	case Up:
		if (y>0) return True;
		break;
	case Right:
		if (x<SCREENWIDTH/SIZE-1) return True;
		break;
	case Down:
		if (y<SCREENHEIGHT/SIZE-1) return True;
	}
	return False;
}

void MoveBlock(int *x,int *y,enum direct dir)
{
	DrawBlock(*x,*y,BGCOLOR); /* erase block */
	switch(dir)
	{
	case Left:
		(*x)--;
		break;
	case Up:
		(*y)--;
		break;
	case Right:
		(*x)++;
		break;
	case Down:
		(*y)++;
		break;
	}
	DrawBlock(*x,*y,FORECOLOR);
}


void main()
{
	int x=0,y=0;
	int key;
	enum bool fGameOver=False;
	int graphdriver=VGA,graphmode=VGAHI;
	initgraph(&graphdriver,&graphmode,"e:\\tc2");

	DrawBlock(x,y,FORECOLOR); /* original position */

	for (;;)
	{
		if (bioskey(1)) key=bioskey(0);
		else key=0;

		switch(key)
		{
		case VK_LEFT: /* move left */
			if (Moveable(x,y,Left))
				MoveBlock(&x,&y,Left);
			break;
		case VK_RIGHT: /* move right */
			if (Moveable(x,y,Right))
				MoveBlock(&x,&y,Right);
			break;
		case VK_UP: /* move left */
			if (Moveable(x,y,Up))
				MoveBlock(&x,&y,Up);
			break;
		case VK_DOWN: /* move right */
			if (Moveable(x,y,Down))
				MoveBlock(&x,&y,Down);
			break;
		case VK_ESC: /* end game */
			fGameOver=True;
			break;
		}

		if (fGameOver) break;
	}

	closegraph();
}

