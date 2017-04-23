#include <stdio.h>
#include <graphics.h>
#include <math.h>
float ab=30,bc=135,bc1,cd=70,ad=135,l1,l2,l3,l13,l4,l5,l6,df1,df,de,ef,ac2,bx,by,cx,cy,ex,ey,fx,fy,tl5,tl4,tl6;
int a=DETECT,b;
void hua()
{float i;int t,q;
clrscr();
printf("you should give a value to DF1 !\n  in put DF1(DF1>9.3 ):");
scanf("%f",&df1);
if(df1<=9.3)
{printf("error in put");
sleep(1);
hua();}
if (df1>9.3)
{l1=1.2100/1.8900; l3= (-0.4100)/1.8900;
l13=l1-l3;
de=(79.4*df1+1576.09)/(2*df1*l13-79.4*l3);
for(i=0.1;;)
 {if (i>39) {q=getch();
	       if (q==99){hua();       }
	       if(q==113)  exit(0);}
if(i<=39)
  {df= df1+i;
  ef=sqrt(de*de+df1*df1+2*de*df1*l1);
  l2=(-1)*(de*de+df*df-ef*ef)/(2*de*df);
  ac2=sqrt(ad*ad+cd*cd-2*ad*cd*l2);
  l5=(ac2*ac2+ad*ad-cd*cd)/(2*ac2*ad);
  l4=(ab*ab+ac2*ac2-bc*bc)/(2*ab*ac2);
  tl4=acos(l4);tl5=acos(l5);
  tl6=tl4+tl5;
  l6=l4*l5-sqrt((1-l4*l4)*(1-l5*l5));
  bx=100+ab*l6; by=100-ab*sin(tl6);
  cx=235-cd*l2;cy=100-cd*sqrt(1-l2*l2);
  ex=235-de*l2;ey=100-de*sqrt(1-l2*l2);
  fx=235+df;fy=100;
  initgraph(&a,&b," ");
  line(100,100,bx,by);
  line(bx,by,cx,cy);
  line(cx,cy,235,100);
  line(100,100,fx,fy);
  line(ex,ey,fx,fy);
  outtextxy(95,100,"A");
  outtextxy(bx-5,by,"B");
  outtextxy(cx,cy-5,"C");
  outtextxy(235,103,"D");
  outtextxy(ex+5,ey-3,"E");
  outtextxy(fx+3,fy,"F");
  gotoxy(3,18);
  printf("DE=%f  DF=%f  EF=%f \n",de,df,ef);
  printf("      Q:  ESC      C: next   ENTYER:continue\n      cekong044   xiaofei   041541108    ");
}
t=getch();
if(t==13)  i+=5; }
}
}
main()
{hua();
}

