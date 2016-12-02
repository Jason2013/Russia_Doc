#include <bios.h>
#include <stdio.h>

#define VK_LEFT  0x4b00
#define VK_RIGHT 0x4d00
#define VK_DOWN  0x5000
#define VK_UP    0x4800
#define VK_HOME  0x4700
#define VK_END   0x4f00
#define VK_SPACE 0x3920
#define VK_ESC   0x011b
#define VK_ENTER 0x1c0d

void main()
{
	int key;
	int fExit=0;
	for(;;)
	{
		if (bioskey(1)) /* any key is pressed */
		{
			key=bioskey(0); /* get the keystroke */
			switch (key)
			{
			case VK_ESC:
				printf("VK_ESC\n");
				fExit=1; /*  exit flag is true */
				break;
			case VK_LEFT:
				printf("VK_LEFT\n");
				break;
			case VK_RIGHT:
				printf("VK_RIGHT\n");
				break;
			case VK_UP:
				printf("VK_UP\n");
				break;
			case VK_DOWN:
				printf("VK_DOWN\n");
				break;
			case VK_HOME:
				printf("VK_HOME\n");
				break;
			case VK_END:
				printf("VK_END\n");
				break;
			case VK_SPACE:
				printf("VK_SPACE\n");
				break;
			case VK_ENTER:
				printf("VK_ENTER\n");
				break;
			default:
				printf("OTHER KEY\n");
			}

		}
		if (fExit) break;

		/* you can do other things at here. */

	}
}