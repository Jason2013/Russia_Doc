#include <bios.h>
#include <stdio.h>

#define VK_ESC   0x011b

void main()
{
	int key;
	for(;;)
	{
		key=bioskey(0);
		printf("0x%x\n",key);
		if (key==VK_ESC) break;
	}
}