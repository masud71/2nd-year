/*@@@@@@@@@@@@@@@@@@@@@@@@     CALCULATOR     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* OBJECTIVE OF THIS PROGRAM IS TO CREAT A SCIENTIFIC CALCULATOR.THE
   CALCULATOR CAN BE USED FOR SCIENTIFIC CALCULATION AVAILABLE IN
   ORDINARY SCIENTIFIC CALCULATOR.
	   CALCULATION IS STARTED PRESSING 'A' AND THE CALCULATOR CAN
   BE TURNED OFF PRESSING <ESC>.TO QUIT THE CALCULATOR ^Q IS TO PRESSED.
   FOR CHANGING THE MODE OF THE CALCULATOR F10 SHOULD BE PRESSED.*/

#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

char ch,scode,cmode[7]="DEG",num[17],oper1,oper2,oper3,unioper,
     ckey[37]={17,27,65,71,8,'1','2','3','4','5','6','7','8','9','0','.','+',
	     '-','*','/','=','%',77,109,96,69,82,84,80,83,70,72,76,78,67,88},
     fkey[15]={31,46,20,45,19,38,18,68,48,32,24,35,50,62};
double op1,op2,op3,result;
int i,test,range_exceed=0,memcall=0,memory=0,nsystem=10,negdec,negbin,negoct
								 ,neghex;
void *buf;

extern void calculator();

void display(void);
void allclear(void);
void showmode(void);
int isvalid(void);
double operand(void);
void take_oper1(void);
void take_oper2(void);
void take_oper3(void);
char unioperator(char);
double unioperation(char,double);
char bioperator(char,char);
double calculation(char,double,double);
void base_n(char);
double take_n(char);
void decimalto(double);
double todecimal(char*);
void show(double);
void showbase(void);
void errorsignal(void);
void cquit(void);

void calculator()
{
  struct viewporttype vport;
  buf=malloc(imagesize(139,0,490,326));
  getimage(139,0,490,326,buf);
  putimage(139,0,buf,XOR_PUT);
  display();
  getviewsettings(&vport);
  setviewport(181,33,439,54,1);
  for(;;) {
    do {
      ch=getch();
      if(ch==0) ch=getch();
    }while(!((toupper(ch)=='A')||(ch=='\x11')));
    settextstyle(SMALL_FONT,HORIZ_DIR,5);
    if(toupper(ch)=='A') {
      allclear();
      while((ch=getch())!=27) {
	if((test=isvalid())==0) continue;
	if(ch=='\x11') break;
	if((ch>='0'&&ch<='9')||(ch=='.'))
	  if(oper1=='í') take_oper1();
	  else {
	    if(oper2=='í') take_oper2();
	    else take_oper3();
	  }
	else if(oper1=='í')
               do {
		 if(ch==96) break;
		 if(toupper(ch)=='G') break;
		 if((ch==0)&&(scode==68)) break;
		 unioper=unioperator(ch);
		 if(unioper!='í') {
		   op1=unioperation(ch,op1);
		   show(op1);
		   if(!((ch=='\x11')||(ch==27)||(toupper(ch)=='A')))
		     do
		       ch=getch();
		     while((test=isvalid())==0);
		   if(ch=='\x11') break;
		   if(toupper(ch)=='A') break;
		   if(ch==27) break;
                   if((ch==0)&&(scode==68)) break;
		   if((oper1=='í')&&(memory==0)&&((ch==109)||(ch==77)||
		      (ch==13)||(ch==13)||((ch==0)&&(scode==50)))) {
		       if((ch==77)&&(memcall==1)) {
			 setcolor(0);
			 outtextxy(210,3,"M");
			 setcolor(15);
		       }
		       continue;
		   }
		   oper1=bioperator(ch,oper1);
		 }
		 else {
		   oper1=bioperator(ch,oper1);
		   if((oper1=='í')||(oper1=='=')) break;
		 }
	       }while((oper1=='í')||(oper1=='='));
	     else {
	       if((ch=='+')||(ch=='-')||(ch=='*')||(ch=='/')) {
		 if(oper2=='í') oper1=ch;
		 else if(oper3=='í') oper2=ch;
	       }
	     }
        if((oper1=='í')&&(memory==0)&&((ch==109)||(ch==77)||(ch==13)||
				       ((ch==0)&&(scode==50)))) {
	  if((ch==77)&&(memcall==1)) {
            setcolor(0);
	    outtextxy(210,3,"M");
	    setcolor(15);
	  }
	  continue;
	}
	if((ch==0)&&(scode==68)) {
	  ch=getch();
	  if(ch=='1') strcpy(cmode,"BASE_N");
	  else if(ch=='2') strcpy(cmode,"DEG");
	  else if(ch=='3') strcpy(cmode,"RAD");
	  else if(ch=='4') strcpy(cmode,"GRAD");
	}
	if(!strcmp(cmode,"BASE_N")) base_n('0');
	if(toupper(ch)=='A') {
	  allclear();
	  continue;
	}
	if(ch==27) {
	  clearviewport();
	  break;
	}
      }
      if(ch==27) clearviewport();
    }
    else {
      setviewport(vport.left,vport.top,vport.right,vport.bottom,vport.clip);
	  cquit();
      break;
	}
  }
}
/*************************************************************************/
/*  NAME          :isvalid
    FUNCTION      : checking if ckey is valid
    ROUTINE CALLED:
    VARIABLES USED:                                                      */
/**************************************************************************/
int isvalid(void)
{
  test=0;
  if(ch==0)
  {
    scode=getch();
    for(i=0;fkey[i];i++)
      if(scode==fkey[i])
      {
	test=1; break;
      }
  }
  else for(i=0;ckey[i];i++)
	 if(toupper(ch)==ckey[i])
	 {
	   test=1; break;
	 }
  return test;
}
/*************************************************************************/
/*  NAME          : allclear
    FUNCTION      : clear calculator's screen
    ROUTINE CALLED:
    VARIABLES USED:                                                      */
/**************************************************************************/
void allclear(void)
{
  clearviewport();
  outtextxy(192,5,"0");
  settextstyle(SMALL_FONT,HORIZ_DIR,4);
  outtextxy(30,3,cmode);
  if((memcall==1)&&(memory!=0)) outtextxy(210,3,"M");
  settextstyle(SMALL_FONT,HORIZ_DIR,5);
  op1=0; op2=0;
  oper1='í'; oper2='í'; oper3='í';
}
/*************************************************************************/
/*  NAME          :operand
    FUNCTION      :to get operand
    ROUTINE CALLED:showmode
    VARIABLES USED:c,op[17],digit,width                                                      */
/**************************************************************************/
double operand(void)
{
  char c,op[17];
  int digit=0,width=0;
  if(ch=='.')  op[digit++]='0';
  op[digit++]=ch;
  op[digit]='\0';
  for(;;) {
    width=textwidth(op);
    clearviewport();
    settextstyle(SMALL_FONT,HORIZ_DIR,4);
    outtextxy(30,3,cmode);
    settextstyle(SMALL_FONT,HORIZ_DIR,5);
    outtextxy(200-width,5,op);
    do
      ch=getch();
    while((test=isvalid())==0);
    if(ch=='\x11') return 0;
    if(toupper(ch)=='A') return 0;
    if((ch==0)&&(scode==68)) {
      ch=getch();
      c=cmode[0];
      if(((ch=='2')&&(c=='D'))||((ch=='3')&&(c=='R'))||((ch=='3')&&(c=='G')))
	continue;
      else {
	showmode();
	return 0;
      }
    }
    if(((ch=='=')||(ch=='%'))&&(oper1=='í')) continue;
    if(toupper(ch)=='G')
    {
      digit=0;
      op[digit++]='0';
      op[digit]='\0';
    }
    for(i=16;ckey[i];i++)
      if((toupper(ch)==ckey[i])&&(ch!=96)) return atof(op);
    if(ch==0) for(i=0;fkey[i];i++)
		if(scode==fkey[i]) return atof(op);
    if((ch>='0'&&ch<='9')||(ch=='.')||(ch==8)) {
      if((digit<16)||((digit==16)&&(ch==8))||((digit==16)&&(ch==96))) {
	if(((digit==1)&&(op[0]=='0'))&&(ch!='.')) op[--digit]='\0';
	if(ch!=8) {
	  if(ch=='.')
	    for(i=0;(op[i]!='.')&&(op[i]);i++);
	      if(op[i]=='.') continue;
	  op[digit++]=ch;
	  op[digit]='\0';
	}
	else {
	  if(digit>1) op[--digit]='\0';
	  else if(digit==1) op[0]='0';
	}
      }
    }
    else if(ch==96) {
      if(op[0]!='-') {
	for(i=digit;i>0;i--)
	  op[i]=op[i-1];
	if(digit > 0) {
	  op[0]='-';
	  op[++digit]='\0';
	}
      }
      else {
	for(i=0;i<digit;i++)
	  op[i]=op[i+1];
	digit--;
      }
    }
  }
}
/*************************************************************************/
/*  NAME          :take_oper1
    FUNCTION      :take first operand
    ROUTINE CALLED:unioperator,unioperation,bioperator,operand,isvalid
		   showmode
    VARIABLES USED:c                                                      */
/**************************************************************************/
void take_oper1(void)
{
  char c;
  op1=operand();
  if((ch==27)||(toupper(ch)=='A')) return;
  do {
    unioper=unioperator(ch);
    if(unioper!='í') {
      op1=unioperation(ch,op1);
      show(op1);
      if(!((oper1=='í')||(oper1=='='))) return;
      if(!((ch=='\x11')||(toupper(ch)=='G')||(toupper(ch)=='A')))
	do
	  ch=getch();
	while((test=isvalid())==0);
      if(ch=='\x11') return;
      if((toupper(ch)=='A')||(ch==27)) return;
      if((ch==0)&&(scode==68)) {
        ch=getch();
	c=cmode[0];
	if(((ch=='2')&&(c=='D'))||((ch=='3')&&(c=='R'))||((ch=='3')&&(c=='G')))
	  continue;
	else {
	  showmode();
	  return;
	}
      }
      if(toupper(ch)=='G')
      {
	allclear();
	return;
      }
      if((ch>='0'&&ch<='9')||(ch=='.')) {
	take_oper1();
	return;
      }
      oper1=bioperator(ch,oper1);
    }
    else oper1=bioperator(ch,oper1);
  }while((oper1=='í')||(oper1=='=')||(oper1=='%'));
}
/*************************************************************************/
/*  NAME          : take_oper2
    FUNCTION      :take second operator
    ROUTINE CALLED:show,unioperator,unioperation,bioperator,showmode
    VARIABLES USED: pass,c                                                     */
/**************************************************************************/
void take_oper2(void)
{
  int pass=0;
  char c;
  op2=operand();
  if((toupper(ch)=='A')||(ch==27)) return;
  do {
    unioper=unioperator(ch);
    if(unioper!='í') {
      op2=unioperation(ch,op2);
      show(op2);
      if((ch=='\x11')||(ch==27)||(toupper(ch)=='A')) return;
      if(oper2!='í') break;
      if(!((ch=='\x11')||(toupper(ch)=='G')||(toupper(ch)=='A')))
	do
	  ch=getch();
	while((test=isvalid())==0);
      if(ch=='\x11') return;
      if((toupper(ch)=='A')||(ch==27)) break;
      if((ch==0)&&(scode==68)) {
        ch=getch();
	c=cmode[0];
	if(((ch=='2')&&(c=='D'))||((ch=='3')&&(c=='R'))||((ch=='3')&&(c=='G')))
	  continue;
	else {
	  showmode();
	  return;
	}
      }
      if((ch>='0'&&ch<='9')||(ch=='.')) {
	take_oper2();
	return;
      }
      if(toupper(ch)=='G')
      {
	clearviewport();
        settextstyle(SMALL_FONT,HORIZ_DIR,4);
	outtextxy(30,3,cmode);
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	outtextxy(192,5,"0");
	ch='0';
	take_oper2();
	return;
      }
      oper2=bioperator(ch,oper2);
    }
    else oper2=bioperator(ch,oper2);
  }while(oper2=='í');
  if((oper2=='=')||(oper2=='%')||(oper1=='*')||(oper1=='/')||
     (((oper1=='+')||(oper1=='-'))&&((oper2=='+')||(oper2=='-')))) {
     if(oper2=='%') result=calculation(oper2,op1,op2);
     else result=calculation(oper1,op1,op2);
     show(result);
     if((ch=='\x11')||(ch==27)||(toupper(ch)=='A')) return;
     op1=result;
     if(!((oper2=='=')||(oper2=='%'))) {
       oper1=oper2;
       oper2='í';
       return;
     }
     else {
       oper1='í'; oper2='í';
       do {
	 if(!((ch=='\x11')||(toupper(ch)=='G')||(toupper(ch)=='A')))
	   do
	     ch=getch();
	   while((test=isvalid())==0);
	 if(ch=='\x11') return;
	 if((toupper(ch)=='A')||(ch==27)) break;
	 if((ch==0)&&(scode==68)) {
           ch=getch();
	   c=cmode[0];
	   if(((ch=='2')&&(c=='D'))||((ch=='3')&&(c=='R'))||((ch=='3')&&(c=='G')))
	     continue;
	   else {
	     showmode();
	     return;
	   }
	 }
	 if((ch>='0'&&ch<='9')||(ch=='.')) {
	   take_oper1();
	   return;
	 }
	 if((toupper(ch)=='G')&&pass)
	 {
	   allclear();
	   return;
	 }
	 unioper=unioperator(ch);
	 if(unioper!='í') {
	   pass++;
	   op1=unioperation(ch,op1);
	   show(op1);
           if((ch=='\x11')||(ch==27)||(toupper(ch)=='A')) return;
	 }
	 else oper1=bioperator(ch,oper1);
       }while((oper1=='í')||(oper1=='='));
     }
  }
}
/*************************************************************************/
/*  NAME          :take_oper3
    FUNCTION      :take third operator
    ROUTINE CALLED:operand,unioperator,unioperation,show,showmode,bioperator
		   calculation,isvalid
    VARIABLES USED:c,pass                                                      */
/**************************************************************************/
void take_oper3(void)
{
  int pass=0;
  char c;
  op3=operand();
  if((toupper(ch)=='A')||(ch==27)) return;
  do {
    unioper=unioperator(ch);
    if(unioper!='í') {
      op3=unioperation(ch,op3);
      show(op3);
      if((ch=='\x11')||(ch==27)||(toupper(ch)=='A')) return;
      if(oper3!='í') break;
      if(!((ch=='\x11')||(toupper(ch)=='G')||(toupper(ch)=='A')))
	do
	  ch=getch();
	while((test=isvalid())==0);
      if(ch=='\x11') return;
      if((toupper(ch)=='A')||(ch==27)) break;
      if((ch==0)&&(scode==68)) {
        ch=getch();
	c=cmode[0];
	if(((ch=='2')&&(c=='D'))||((ch=='3')&&(c=='R'))||((ch=='3')&&(c=='G')))
	  continue;
	else {
	  showmode();
	  return;
	}
      }
      if((ch>='0'&&ch<='9')||(ch=='.')) {
	take_oper3();
	return;
      }
      if(toupper(ch)=='G')
      {
	clearviewport();
        settextstyle(SMALL_FONT,HORIZ_DIR,4);
	outtextxy(30,3,cmode);
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	outtextxy(192,5,"0");
	ch='0';
	take_oper3();
	return;
      }
      oper3=bioperator(ch,oper3);
    }
    else oper3=bioperator(ch,oper3);
  }while(oper3=='í');
  if(oper3=='%') result=calculation(oper3,op2,op3);
  else result=calculation(oper2,op2,op3);
  if((oper3=='*')||(oper3=='/')||(oper3=='%')) {
    show(result);
    if((ch=='\x11')||(ch==27)||(toupper(ch)=='A')) return;
    op2=result;
    if(oper3!='%') {
      oper2=oper3;
      do {
	ch=getch();
	if(ch=='\x11') return;
	if((toupper(ch)=='A')||(ch==27)) break;
	if((ch==0)&&(scode==68)) {
	  ch=getch();
	  c=cmode[0];
	  if(((ch=='2')&&(c=='D'))||((ch=='3')&&(c=='R'))||((ch=='3')&&(c=='G')))
	    continue;
	  else {
	    showmode();
	    return;
	  }
	}
      }while(!((ch>='0'&&ch<='9')||(ch=='.')));
      take_oper3();
      return;
    }
  }
  result=calculation(oper1,op1,result);
  show(result);
  if((ch=='\x11')||(ch==27)||(toupper(ch)=='A')) return;
  op1=result;
  if(!((oper3=='=')||(oper3=='%'))) {
    oper1=oper3;
    oper2='í';
    return;
  }
  else {
    oper1='í'; oper2='í';
    do {
      if(!((ch=='\x11')||(toupper(ch)=='G')||(toupper(ch)=='A')))
	do
	  ch=getch();
	while((test=isvalid())==0);
      if(ch=='\x11') return;
      if((toupper(ch)=='A')||(ch==27)) break;
      if((ch==0)&&(scode==68)) {
        ch=getch();
	c=cmode[0];
	if(((ch=='2')&&(c=='D'))||((ch=='3')&&(c=='R'))||((ch=='3')&&(c=='G')))
	  continue;
	else {
	  showmode();
	  return;
	}
      }
      if((ch>='0'&&ch<='9')||(ch=='.')) {
	take_oper1();
	return;
      }
      if((toupper(ch)=='G')&&pass)
      {
	allclear();
	return;
      }
      unioper=unioperator(ch);
      if(unioper!='í') {
	pass++;
	op1=unioperation(ch,op1);
	show(op1);
        if((ch=='\x11')||(ch==27)||(toupper(ch)=='A')) return;
      }
      else oper1=bioperator(ch,oper1);
    }while((oper1=='í')||(oper1=='='));
  }
  oper2='í'; oper3='í';
}
/*************************************************************************/
/*  NAME          :unioperator
    FUNCTION      :return unary operator
    ROUTINE CALLED:
    VARIABLES USED:                                                      */
/**************************************************************************/
char unioperator(char ch)
{
  if(ch==0) {
    for(i=0;i<7;i++)
      if(scode==fkey[i]) return 0;
  }
  else for(i=23;ckey[i];i++)
    if(toupper(ch)==ckey[i]) return ckey[i];
  return 'í';
}
/*************************************************************************/
/*  NAME          :bioperator
    FUNCTION      :return binary operator
    ROUTINE CALLED:
    VARIABLES USED:                                                      */
/**************************************************************************/
char bioperator(char ch,char oper1)
{
  for(i=16;i<22;i++)
    if(ch==ckey[i]) return ckey[i];
  return oper1;
}
/*************************************************************************/
/*  NAME          :unioperation
    FUNCTION      :return value after unioperation
    ROUTINE CALLED:errorsignal,unioperation,unioperator,operand,bioperator
    VARIABLES USED:power,prod,ex[4]                                                      */
/**************************************************************************/
double unioperation(char operator,double op1)
{
  double power,prod=1;
  char ex[4]="00";
  if(operator) switch(toupper(operator)) {
	case 'S':
	  if(!strcmp(cmode,"DEG")) op1=op1*180/exp(1);
	  else if(!strcmp(cmode,"GRAD")) op1=op1*200/exp(1);
	  return sin(op1);
	case 'C':
          if(!strcmp(cmode,"DEG")) op1=op1*180/exp(1);
	  else if(!strcmp(cmode,"GRAD")) op1=op1*200/exp(1);
	  return cos(op1);
	case 'T':
          if(!strcmp(cmode,"DEG")) op1=op1*180/exp(1);
	  else if(!strcmp(cmode,"GRAD")) op1=op1*200/exp(1);
	  if(((!strcmp(cmode,"DEG"))&&(op1==90))||
	     ((!strcmp(cmode,"RAD"))&&(op1==exp(1)/2))||
	     ((!strcmp(cmode,"GRAD"))&&(op1==100))) {
	    range_exceed=1;
	    errorsignal();
	    break;
	  }
	  else return tan(op1);
	case 'R':
	  if(op1<0.0) {
	    range_exceed=1;
	    errorsignal();
	    break;
	  }
	  else return sqrt(op1);
	case 'X':
	   return op1*op1;
	case 'L':
	   if(op1<=0) {
	     range_exceed=1;
	     errorsignal();
	     break;
	   }
	   else return log10(op1);
	case 'N':
	   if(op1<=0) {
	     range_exceed=1;
	     errorsignal();
	     break;
	   }
	   else return log(op1);
	case 'P':
	   do
	     ch=getch();
	   while((ch>='0'&&ch<='9')||(ch=='.'));
	   if(oper1=='í') oper1='è';
	   power=operand();
	   if(oper1=='è') oper1='í';
	   return pow(op1,power);
	case 'F':
	   if(op1==floor(op1)) {
	     if(op1>1)
	       for(i=2;i<=op1;i++)
		 prod *= i;
	     return (prod);
	   }
	   else {
	     range_exceed=1;
	     errorsignal();
	     break;
	   }
	case 'E':
	  setviewport(383,34,400,45,1);
	  settextstyle(SMALL_FONT,HORIZ_DIR,4);
	  outtextxy(0,0,ex);
	  i=0;
	  for(;;) {
	    do
	      ch=getch();
	    while((test=isvalid())==0);
	    if(ch=='\x11') break;
	    if((toupper(ch)=='A')||(toupper(ch)=='G')||(ch==27)) break;
	    if(((ch>='0'&&ch<='9')||(ch==96))&&((atoi(ex)<100)&&(atoi(ex)>-100))) {
	      if(ch!=96) {
		ex[0]=ex[1];
		ex[1]=ch;
	      }
	      else if(ex[0]=='-')
		   { ex[0]=ex[1]; ex[1]=ex[2]; ex[3]='\0';}
	      else { ex[2]=ex[1]; ex[1]=ex[0]; ex[0]='-'; ex[3]='\0';}
	      clearviewport();
	      outtextxy(0,0,ex);
	      i++;
	    }
	    else {
	      unioper=unioperator(ch);
	      if(unioper!='í') {
		op1=op1*pow(10,atoi(ex));
		op1=unioperation(ch,op1);
                settextstyle(SMALL_FONT,HORIZ_DIR,5);
		setviewport(181,33,439,54,1);
		return op1;
	      }
	      else {
		if((oper1=='í')||(oper1=='=')) {
		  oper1=bioperator(ch,oper1);
		  if((oper1=='í')||(oper1=='=')) continue;
		}
		else if(oper2=='í') {
		       oper2=bioperator(ch,oper2);
		       if(oper2=='í') continue;
		}
		else {
		  oper3=bioperator(ch,oper3);
		  if(oper3=='í') continue;
		}
		settextstyle(SMALL_FONT,HORIZ_DIR,5);
		setviewport(181,33,439,54,1);
		return op1*pow(10,atoi(ex));
	      }
	    }
	  }
          settextstyle(SMALL_FONT,HORIZ_DIR,5);
	  setviewport(181,33,439,54,1);
	  break;
	case 'H':
	  do
		operator=getch();
	  while(!((toupper(operator)=='S')||(toupper(operator)=='C')||
		 (toupper(operator)=='T')));
	  switch(toupper(operator)) {
		case 'S':
		  return sinh(op1);
		case 'C':
		  return cosh(op1);
		case 'T':
		  return tanh(op1);
	  }
	case 96:
	  return op1*(-1);
  }
  else switch(scode) {
    case 31:
      if(fabs(op1)<=1.0) {
	if(!strcmp(cmode,"DEG")) return asin(op1)*180/exp(1);
	else if(!strcmp(cmode,"RAD")) return asin(op1);
	else if(!strcmp(cmode,"GRAD")) return asin(op1)*200/exp(1);
      }
      else {
	range_exceed=1;
	errorsignal();
	break;
      }
    case 46:
      if(fabs(op1)<=1.0) {
        if(!strcmp(cmode,"DEG")) return asin(op1)*180/exp(1);
	else if(!strcmp(cmode,"RAD")) return asin(op1);
	else if(!strcmp(cmode,"GRAD")) return asin(op1)*200/exp(1);
      }
      else {
	range_exceed=1;
	errorsignal();
	break;
      }
    case 20:
      if(!strcmp(cmode,"DEG")) return asin(op1)*180/exp(1);
      else if(!strcmp(cmode,"RAD")) return asin(op1);
      else if(!strcmp(cmode,"GRAD")) return asin(op1)*200/exp(1);
    case 45:
      if(op1!=0.0)
	return pow(op1,-1);
      else {
	range_exceed=1;
	errorsignal();
	break;
      }
    case 19:
      return pow(op1,1/3);
    case 38:
      return pow(10,op1);
    case 18:
      return exp(op1);
  }
}
/*************************************************************************/
/*  NAME          :calculation
    FUNCTION      :calculate result after binary operation
    ROUTINE CALLED:errorsignal
    VARIABLES USED:oper                                                      */
/**************************************************************************/
double calculation(char operator,double op1,double op2)
{
  char oper;
  switch(operator) {
	  case '+':
	    return (op1+op2);
	  case '-':
	    return (op1-op2);
	  case '*':
	    return (op1*op2);
	  case '/':
	    if(op2!=0) return (op1/op2);
	    else {
	      range_exceed=1;
	      errorsignal();
	      break;
	    }
	  case '%':
	    if(oper2=='%') oper=oper1;
	    else if(oper3=='%') oper=oper2;
	    switch(oper) {
	      case '+':
		return (op1+op2)*100/op2;
	      case '-':
		return (op1-op2)*100/op2;
	      case '*':
		return (op1*op2)/100;
	      case '/':
		return op1*100/op2;
	    }
  }
}
/*************************************************************************/
/*  NAME          : show
    FUNCTION      : to print number on calculator's screen
    ROUTINE CALLED: errorsignal
    VARIABLES USED:width,str[22],mantissa[17],c,ex[5]                                                      */
/**************************************************************************/
void show(double num)
{
  char str[22],mantissa[17],c,ex[5];
  int width=0;
  if(!range_exceed) {
    clearviewport();
    settextstyle(SMALL_FONT,HORIZ_DIR,4);
    outtextxy(30,3,cmode);
    settextstyle(SMALL_FONT,HORIZ_DIR,5);
    if((num>9999999999999999)||(num<-9999999999999999)) {
      sprintf(str,"%.15e",num);
      sscanf(str,"%[^e]%c%s",mantissa,&c,ex);
      if((atof(ex)<100)&&(atof(ex)>-100)) {
	num=atof(mantissa);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	if(ex[0]=='+')
	  for(i=0;ex[i];i++)
	    ex[0+i]=ex[1+i];
	outtextxy(202,1,ex);
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
      }
      else {
	range_exceed=1;
	errorsignal();
      }
    }
    if(!range_exceed) {
      sprintf(str,"%.15g",num);
      width=textwidth(str);
      outtextxy(200-width,5,str);
    }
  }
  range_exceed=0;
}
/*************************************************************************/
/*  NAME          :errorsignal
    FUNCTION      :print error message
    ROUTINE CALLED:
    VARIABLES USED:str[4]                                                      */
/**************************************************************************/
void errorsignal(void)
{
  char str[4]="-E-";
  clearviewport();
  settextstyle(SMALL_FONT,HORIZ_DIR,4);
  outtextxy(30,3,cmode);
  settextstyle(SMALL_FONT,HORIZ_DIR,5);
  outtextxy(175,5,str);
  do
    ch=getch();
  while(!((toupper(ch)=='A')||(ch==27)||(ch=='\x11')));
}
/*************************************************************************/
/*  NAME          :quit
    FUNCTION      :quit from calculator function
    ROUTINE CALLED:
    VARIABLES USED:buff                                                      */
/**************************************************************************/
void cquit(void)
{
  void *buff;
  buff=malloc(imagesize(139,0,490,326));
  getimage(139,0,490,326,buff);
  putimage(139,0,buff,XOR_PUT);
  putimage(139,0,buf,XOR_PUT);
  free(buf);
  free(buff);
}
/*************************************************************************/
/*  NAME          : display
    FUNCTION      : show calculator
    ROUTINE CALLED:
    VARIABLES USED:i,j,n,x1,y1,x2,y2                                                      */
/**************************************************************************/
void display(void)
{
	int i,j,n,x1,y1,x2,y2;
	char pattern[]={0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa};
	setlinestyle(SOLID_LINE,0,THICK_WIDTH);
	rectangle(140,5,480,325);
	setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	setfillpattern(pattern,1);
	rectangle(170,27,450,60);
	rectangle(180,32,440,55); /* screen */
	floodfill(175,30,1);
	line(170,27,180,32);
	line(170,60,180,55);
	line(450,27,440,32);
	line(450,60,440,55);
	rectangle(166,71,454,86);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	outtextxy(170,73,"MODE-> 0 COMP  1 BASE-N  2 DEG   3 RAD  4 GRAD");
	line(140,5,150,0);
	line(150,0,490,0);
	line(490,0,490,320);
	line(480,325,490,320);
	floodfill(152,2,1);
	line(480,4,490,0);
	setfillstyle(SOLID_FILL,WHITE);
	y1=73;
	y2=y1+12;
	for(i=0;i<4;++i)
	  {
		y1=y1+25;
		y2=y2+25;
		x1=120;
		x2=x1+30;
		for(j=0;j<6;++j)
		  {
			x1=x1+49;
			x2=x2+49;
			bar3d(x1,y1,x2,y2,5,1);
		  }
	  }
	y1=180;
	y2=y1+16;
	for(i=0;i<4;++i)
	  {
		y1=y1+27;
		y2=y2+27;
		x1=114;
		x2=x1+35;
		for(j=0;j<5;++j)
		  {
			x1=x1+58;
			x2=x2+58;
			bar3d(x1,y1,x2,y2,5,1);
		  }
	  }

	  setcolor(0);
	  outtextxy(177,100,"Alt");
	  outtextxy(171+49,100,"Kout");
	  outtextxy(171+2*49,100,"ENG");
	  outtextxy(179+3*49,100,"%");
	  moveto(171+4*49,97);
          settextstyle(SMALL_FONT,HORIZ_DIR,3);
	  outtext("M O D E");
	  outtextxy(180+4*49,103,"F 10");
	  settextstyle(SMALL_FONT,HORIZ_DIR,4);
	  moveto(171+5*49,100);
	  outtext("OFF");
	  moveto(174,100+25);
	  outtext("1/x");
	  moveto(172+49,100+25);
	  outtext("Root");
	  settextstyle(SMALL_FONT,HORIZ_DIR,3);
	  moveto(182+2*49,98+25);
	  outtext("2");
	  settextstyle(SMALL_FONT,HORIZ_DIR,4);
	  moveto(178+2*49,100+25);
	  outtext("x");
	  moveto(172+3*49,100+25);
	  outtext("log");
	  moveto(176+4*49,100+25);
	  outtext("ln");
	  settextstyle(SMALL_FONT,HORIZ_DIR,3);
	  moveto(185+5*49,98+25);
	  outtext("y");
	  settextstyle(SMALL_FONT,HORIZ_DIR,4);
	  moveto(178+5*49,100+25);
	  outtext("x");
	  outtextxy(178,98+2*25,"b");
	  moveto(171,100+2*25);
	  outtext("a /c");
	  moveto(171+49,96+2*25);
	  outtext(".,,,");
	  moveto(173+2*49,98+2*25);
	  outtext("hyp");
	  moveto(173+3*49,99+2*25);
	  outtext("sin");
	  moveto(173+4*49,99+2*25);
	  outtext("cos");
	  moveto(173+5*49,99+2*25);
	  outtext("tan");
	  moveto(171,100+3*25);
	  outtext("+/-");
	  moveto(172+49,100+3*25);
	  outtext("BAK");
	  moveto(172+2*49,100+3*25);
	  outtext("[(..");
	  moveto(173+3*49,100+3*25);
	  outtext("..)]");
	  moveto(174+4*49,100+3*25);
	  outtext("Min");
	  moveto(176+5*49,100+3*25);
	  outtext("MR");

	    /* fill second */

	  settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	  moveto(184,213);
	  outtext("7");
	  moveto(184+58,213);
	  outtext("8");
	  moveto(184+2*58,213);
	  outtext("9");
	  moveto(184+3*58,213);
	  outtext("G");
	  moveto(184+4*58,213);
	  outtext("AC");
	  moveto(184,213+27);
	  outtext("4");
	  moveto(184+58,213+27);
	  outtext("5");
	  moveto(184+2*58,213+27);
	  outtext("6");
	  moveto(184+3*58,213+27);
	  outtext("*");
	  moveto(184+4*58,213+27);
	  outtext("/");
	  moveto(184,213+2*27);
	  outtext("1");
	  moveto(184+58,213+2*27);
	  outtext("2");
	  moveto(184+2*58,213+2*27);
	  outtext("3");
	  moveto(184+3*58,213+2*27);
	  outtext("+");
	  moveto(184+4*58,213+2*27);
	  outtext("-");
	  moveto(184,213+3*27);
	  outtext("0");
	  moveto(184+58,210+3*27);
	  outtext(".");
	  moveto(180+2*58,211+3*27);
	  outtext("EXP");
	  moveto(185+3*58,212+3*27);
	  outtext("=");
	  moveto(184+4*58,213+3*27);
	  outtext("M+");
          setcolor(15);
}
/*************************************************************************/
/*  NAME          :showmode
    FUNCTION      :show the current mode
    ROUTINE CALLED:
    VARIABLES USED:                                                      */
/**************************************************************************/
void showmode(void)
{
  settextstyle(SMALL_FONT,HORIZ_DIR,4);
  setcolor(0);
  outtextxy(30,3,cmode);
  setcolor(15);
  if(ch=='1') strcpy(cmode,"BASE_N");
  else if(ch=='2') strcpy(cmode,"DEG");
  else if(ch=='3') strcpy(cmode,"RAD");
  else if(ch=='4') strcpy(cmode,"GRAD");
  outtextxy(30,3,cmode);
  settextstyle(SMALL_FONT,HORIZ_DIR,5);
}
/*************************************************************************/
/*  NAME          :base_n
    FUNCTION      :
    ROUTINE CALLED:todecimal,decimalto,showmode
    VARIABLES USED:number                                                      */
/**************************************************************************/
void base_n(char c)
{
  double number;
  number=take_n(c);
  if(!strcmp(cmode,"BASE_N")) decimalto(number);
  for(;;) {
    ch=getch();
    if(ch=='\x11') return;
    if(toupper(ch)=='A') break;
    if(ch==27) return;
    if(toupper(ch)=='G') decimalto(0.0);
    if(ch==0) {
      scode=getch();
      if(((scode==48)&&(nsystem!=2))||((scode==32)&&(nsystem!=10))||
	 ((scode==24)&&(nsystem!=8))||((scode==35)&&(nsystem!=16))) {
        number=todecimal(num);
	if(scode==48) nsystem=2;
	else if(scode==32) nsystem=10;
	else if(scode==24) nsystem=8;
	else nsystem=16;
	decimalto(number);
      }
      else if(scode==68) {
	     ch=getch();
	     if(ch!='1') {
	       allclear();
	       showmode();
	       return;
	     }
	     else continue;
	   }
    }
    if(((nsystem==2)&&((ch >= '0' && ch <= '1')||(ch=='.')))||
       ((nsystem==8)&&((ch >= '0' && ch <= '7')||(ch=='.')))||
       ((nsystem==10)&&((ch >= '0' && ch <= '9')||(ch=='.')))||
       ((nsystem==16)&&((ch >= '0' && ch <= '9')||(toupper(ch)>='A'
			 &&toupper(ch)<='F')||(ch=='.'))))
      base_n(ch);
  }
  if(toupper(ch)=='A') base_n('0');
}
/*************************************************************************/
/*  NAME          :decimalto
    FUNCTION      :convert decimal to all number system
    ROUTINE CALLED:errorsignal,showbase
    VARIABLES USED:roundnum,fraction,t,width,sysnum[17],fract[17],round[17]                                                      */
/**************************************************************************/
void decimalto(double decimalnum)
{
  double roundnum,fraction;
  char fract[17],round[17];
  int t=0,width=0,sysnum[17];
  range_exceed=0;
  if(((nsystem==2)&&((decimalnum>32767)||(decimalnum<-32768)))||
     ((nsystem==10)&&((decimalnum>536870911)||(decimalnum<-536870912)))||
     ((nsystem==16)&&((decimalnum>2147483646)||(decimalnum<-2147483647))))
  {
    range_exceed=1;
    errorsignal();
    showbase();
  }
  else {
    if((nsystem==2)&&(negdec||negoct||neghex)) decimalnum+=65535+1;
    else if((nsystem==8)&&(negdec||negbin||neghex)) decimalnum+=1073741823+1;
    else if((nsystem==16)&&(negdec||negbin||negoct)) decimalnum+=pow(2,32);
    if(nsystem==10) sprintf(num,"%.15g",decimalnum);
    else {
      i=0;
      roundnum=floor(decimalnum);
      fraction=decimalnum-roundnum;
      do {
	sysnum[i++]=fmod(roundnum,nsystem);
	roundnum=roundnum / nsystem;
      }while(roundnum>=1);
      sysnum[i]='\0';
      for(i=i-1;i>=0;i--)
	if(sysnum[i]>=0 && sysnum[i]<=9)
	  round[t++]=(char)(48+sysnum[i]);
	else round[t++]=(char)(65+sysnum[i]-10);
      round[t]='\0';
      if(fraction!=0.0) {
	i=0;
	do {
	  fraction *= nsystem;
	  sysnum[i++]=floor(fraction);
	  fraction -= sysnum[i-1];
       }while(fraction>pow(1,-14));
       sysnum[i]='\0';
       for(i=0;sysnum[i];i++)
	 if(sysnum[i]>=0 && sysnum[i]<=9)
	   fract[i]=(char)(48+sysnum[i]);
	 else fract[i]=(char)(65+sysnum[i]-10);
       fract[i]='\0';
      }
      if(atof(fract))
	sprintf(num,"%s.%s",round,fract);
      else strcpy(num,round);
    }
    clearviewport();
    width=textwidth(num);
    outtextxy(200-width,5,num);
    showbase();
  }
}
/*************************************************************************/
/*  NAME          : take_n
    FUNCTION      : take base
    ROUTINE CALLED: decimalto,allclear,showbase,showmode
    VARIABLES USED:i,t,width,number                                                      */
/**************************************************************************/
double take_n(char c)
{
  int i=0,t,width=0;
  double number;
  negdec=0,negbin=0,negoct=0,neghex=0;
  if(c=='.') num[i++]='0';
  num[i++]=c; num[i]='\0';
  for(;;) {
    width=textwidth(num);
    clearviewport();
    outtextxy(200-width,5,num);
    showbase();
    ch=toupper(getch());
    if(ch=='\x11') return 0;
    if(toupper(ch)=='A') return 0;
    if(ch==27) return 0;
    if(toupper(ch)=='G')
    {
      i=0;
      num[i++]='0';
      num[i]='\0';
      continue;
    }
    if(((nsystem==2)&&(i<16)&&((ch >= '0' && ch <= '1')||(ch=='.')))||
      ((nsystem==8)&&(i<10)&&((ch >= '0' && ch <= '7')||(ch=='.')))||
      ((nsystem==10)&&(i<10)&&((ch >= '0' && ch <= '9')||(ch=='.')))||
      ((nsystem==16)&&(i<8)&&((ch >= '0' && ch <= '9')||
		      (toupper(ch)>='A'&&toupper(ch)<='F')||(ch=='.'))))
      { if((i==1)&&(ch=='0')&&(num[0]=='0')) continue;
	else if((i==1)&&(num[0]=='0')&&(ch!='.')) num[i-1]=ch;
	else num[i++]=ch;
      }
    else if((nsystem==10)&&(num[0]=='-')&&(ch >= '0' && ch <= '9'))
	   num[i++]=ch;
    else if((nsystem==10)&&(ch==96)) {
	   for(t=i;t>0;t--)
	     num[t]=num[t-1];
	   if(i>0) {
	     num[0]='-';
	     i++;
	   }
	 }
    else if((i>1)&&(ch==8)) i--;
    else if(ch=='.') {
	   for(t=0;(num[t]!='.')&&num[t];t++);
	   if(num[t]=='.') continue;
	   num[i++]=ch;
	 }
    else if(ch==0) {
      ch=getch();
      if(ch==68) {
	ch=getch();
	if(ch!='1') {
	  allclear();
	  showmode();
	  return 0;
	}
	else continue;
      }
      if((ch==48)&&(nsystem!=2))
      {
	number=todecimal(num);
	nsystem=2;
	if(number) break;
	else continue;
      }
      if((ch==32)&&(nsystem!=10))
      {
	number=todecimal(num);
	nsystem=10;
	if(number) break;
	else continue;
      }
      if((ch==24)&&(nsystem!=8))
      {
	number=todecimal(num);
	nsystem=8;
	if(number) break;
	else continue;
      }
      if((ch==35)&&(nsystem!=16))
      {
	number=todecimal(num);
	nsystem=16;
	if(number) break;
	else continue;
      }
    }
    num[i]='\0';
  }
  return number;
}
/*************************************************************************/
/*  NAME          :todecimal
    FUNCTION      :converting a number in system other than decimal to
		   decimal
    ROUTINE CALLED:None
    VARIABLES USED:multiplier,decinum,round[17],fract[17],c                                                      */
/**************************************************************************/
double todecimal(char* num)
{
  double multiplier,decinum=0;
  char round[17],fract[17],c;
  multiplier=1;
  sscanf(num,"%[^.]%c%s",round,&c,fract);
  for(i=strlen(round)-1;i>=0;i--) {
    if(round[i]>='0'&&round[i]<='9')
      decinum += multiplier*(round[i]-'0');
    else if(round[i]>='A'&&round[i]<='F')
	   decinum += multiplier*(round[i]-'A'+10);
    else if(round[i]=='-') negdec=1;
    multiplier *= nsystem;
  }
  multiplier=1.0/nsystem;
  for(i=0;i<strlen(fract);i++) {
    if(fract[i]>='0'&&fract[i]<='9')
      decinum += multiplier*(fract[i]-'0');
    else if(fract[i]>='A'&&fract[i]<='F')
	   decinum += multiplier*(fract[i]-'A'+10);
    multiplier /= nsystem;
  }
  if((nsystem==2)&&(strlen(round)==16)&&(round[0]=='1')) {
    negbin=1;
    decinum=65535-decinum+1;
    decinum *= -1;
  }
  if((nsystem==8)&&(strlen(round)==10)&&(round[0]>='4')) {
    negbin=1;
    decinum=1073741823-decinum+1;
    decinum *= -1;
  }
  if((nsystem==16)&&(strlen(round)==8)&&((round[0]>='8')||(round[0]>='A'))) {
    negbin=1;
    decinum=4294967295-decinum+1;
    decinum *= -1;
  }
  if((nsystem==10)&&negdec) decinum *= -1;
  return decinum;
}

/*************************************************************************/
/*  NAME          :showbase
    FUNCTION      :showing the base
    ROUTINE CALLED:None
    VARIABLES USED:numsys                                                      */
/**************************************************************************/

void showbase(void)
{
  char *numsys;
  settextstyle(SMALL_FONT,HORIZ_DIR,4);
  outtextxy(30,3,"BASE_N");
  if(nsystem==2) numsys="b";
  else if(nsystem==8) numsys="o";
  else if(nsystem==10) numsys="d";
  else numsys="H";
  outtextxy(220,3,numsys);
  settextstyle(SMALL_FONT,HORIZ_DIR,5);
}