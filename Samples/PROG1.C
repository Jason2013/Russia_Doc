
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(void)
{
   /* request auto detection */
   int gdriver = VGA, gmode=VGAHI, errorcode;

   /* initialize graphics mode */
   initgraph(&gdriver, &gmode, "e:\\tc2");

   /* read result of initialization */
   errorcode = graphresult();

   if (errorcode != grOk)  /* an error occurred */
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1);             /* return with error code */
   }

   /* draw a cross */
   line(100,100,150,150);
   line(100,150,150,100);

   /* set color to yellow */
   setcolor(YELLOW);

   settextstyle(DEFAULT_FONT,HORIZ_DIR,2);

   /* demo rectangle(),settextjustify() and outtextxy() */
   rectangle(16,16,96,32);
   settextjustify(LEFT_TEXT,TOP_TEXT);
   outtextxy(16,16,"ABC");

   rectangle(16,48,96,64);
   settextjustify(RIGHT_TEXT,TOP_TEXT);
   outtextxy(96,48,"123");

   /* demo setfillstyle() and bar() */
   setfillstyle(SOLID_FILL,RED);
   bar(16,80,96,96);

   /* clean up */
   getch();
   closegraph();
   return 0;
}
