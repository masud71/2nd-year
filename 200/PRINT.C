#include "bios.h"
#include "alloc.h"
#include "graphics.h"

extern display_a_page(int);
extern maxline;

void reset_ptr();
void init_ptr();
void init_each_line(int ptr_head_move);
void print();

void init_ptr(){
 biosprint(1,0,0);
 biosprint(0,27,0);
 biosprint(0,49,0);
}
void init_each_line(int ptr_head_move){
 biosprint(0,10,0);
 biosprint(0,13,0);
 biosprint(0,27,0);
 biosprint(0,76,0);
 biosprint(0,ptr_head_move%256,0);
 biosprint(0,ptr_head_move/256,0);
}
void print(){
 char data,*buffer,parray[8]={128,64,32,16,8,4,2,1};
 int hori,vert,start,lcv1,lcv2,lcv3,line_no=0,ltp=20,ptr_stat;
 buffer=malloc(imagesize(180,180,500,220));
 getimage(180,180,500,220,buffer);
 setfillstyle(SOLID_FILL,WHITE);
 bar(180,180,500,220);
 setcolor(BLACK);
 ptr_stat=biosprint(2,0,0);
 if(ptr_stat==200){
      outtextxy(200,200,"PRINTER IS OFF QUIT PRINTING");
      delay(1000);
      putimage(180,180,buffer,COPY_PUT);
      return;}
 if(ptr_stat==48){
      outtextxy(200,200,"PRINTER IS OFF QUIT PRINTING");
      delay(1000);
      putimage(180,180,buffer,COPY_PUT);
      return;}
 if(ptr_stat==56){
      do{outtextxy(200,200,"PAPER OUT INSERT PAPER & PRESS KEY");
	 getch();
	 ptr_stat=biosprint(2,0,0);
       }while(ptr_stat==56);
 }
 outtextxy(200,200,"PRINTING CURRENT FILE........");
 setcolor(WHITE);
 init_ptr();
while(line_no<maxline){
 setactivepage(1);
 setfillstyle(SOLID_FILL,BLACK);
 bar(1,1,715,320);
 setfillstyle(SOLID_FILL,WHITE);
 display_a_page(line_no);
 if((maxline-line_no)<20)
    ltp=maxline-line_no;
 line_no+=20;
 hori=700;
 start=58/8;
 vert=((ltp+1)*12)/8+start;
 for (lcv1=start ;lcv1<=vert;++lcv1){
	   init_each_line(hori);
	   for (lcv2=0;lcv2<=hori;++lcv2){
		    data=0;
		    for (lcv3=0;lcv3<=7;++lcv3){
		      if (getpixel(lcv2,lcv1*8+lcv3)>0)
			 data+=parray[lcv3];
		    }
	     biosprint(0,data,0);
	   }
   }
 }
 reset_ptr();
 setactivepage(0);
 setfillstyle(SOLID_FILL,WHITE);
 putimage(180-12,180-58,buffer,COPY_PUT);
 biosprint(0,12,0);
}
void reset_ptr(){
   biosprint(0,27,0);
   biosprint(0,64,0);
}