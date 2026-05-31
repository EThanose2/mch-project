#include <reg51.h>

#define LCD P0

sbit RS=P2^0;
sbit EN=P2^1;

sbit LF=P1^0;
sbit LW=P1^1;
sbit LR=P1^2;
sbit LS=P1^3;
sbit LD=P1^4;
sbit MO=P1^5;
sbit VA=P1^6;
sbit BZ=P1^7;

sbit BS=P3^0;
sbit BP=P3^1;
sbit BM=P3^2;
sbit BW=P3^3;

unsigned int t=0;
bit bl=0;

unsigned char st=0,md=0,wl=1,rn=0,ps=0;

/* speed x2 */

code unsigned int FL[3]={50,100,150};
code unsigned int WA_T[4]={600,400,300,200};
code unsigned int DR_T[4]={200,200,150,150};
code unsigned int RI_T[4]={300,250,200,150};
code unsigned int SP_T[4]={500,400,300,250};

code unsigned char RC[4]={3,2,2,1};

void dly(unsigned int n){

    unsigned int i,j;

    for(i=0;i<n;i++)
        for(j=0;j<120;j++);
}

void ep(){

    EN=1;

    dly(1);

    EN=0;
}

void cmd(unsigned char c){

    RS=0;

    LCD=c;

    ep();
}

void dat(unsigned char c){

    RS=1;

    LCD=c;

    ep();
}

void str(char *s){

    while(*s)
        dat(*s++);
}

void clr(){

    cmd(0x01);

    dly(2);
}

void pos(unsigned char r,unsigned char c){

    cmd((r?0xC0:0x80)+c);
}

void lcd(){

    dly(30);

    cmd(0x38);
    cmd(0x0C);
    cmd(0x06);

    clr();
}

void tm0() interrupt 1{

    /* 50ms */

    TH0=0x4B;
    TL0=0xFD;

    /* pause stop timer */

    if(st!=7)
        t++;

    if(t%10==0)
        bl=!bl;
}

void off(){

    P1=0;
}

void hd(char *s){

    pos(0,0);

    str(s);

    str("        ");
}

void rem(unsigned int v){

    unsigned int r;

    if(t>=v)
        r=0;
    else
        r=(v-t)/20;

    pos(1,0);

    dat(r/10+'0');
    dat(r%10+'0');
    dat('s');

    str("     ");
}

void idle(){

    off();

    hd("IDLE");

    pos(1,0);

    if(md==0) str("CT ");
    else if(md==1) str("SY ");
    else if(md==2) str("DE ");
    else str("QK ");

    if(wl==0) str("L");
    else if(wl==1) str("M");
    else str("H");
}

void fill(){

    LW=0;
    LR=0;
    LS=0;

    LF=1;
    VA=1;

    hd("FILL");

    rem(FL[wl]);

    if(t>=FL[wl]){

        t=0;

        VA=0;
        LF=0;

        st=2;

        clr();
    }
}

void wash(){

    LF=0;
    LR=0;
    LS=0;

    LW=1;

    MO=(t&8)?1:0;

    hd("WASH");

    rem(WA_T[md]);

    if(t>=WA_T[md]){

        t=0;

        LW=0;
        MO=0;

        st=3;

        clr();
    }
}

void drain(){

    LF=0;
    LW=0;
    LR=0;
    LS=0;

    VA=1;

    hd("DRAIN");

    rem(DR_T[md]);

    if(t>=DR_T[md]){

        t=0;

        VA=0;

        if(rn<RC[md])
            st=4;
        else{

            rn=0;

            st=5;
        }

        clr();
    }
}

void rinse(){

    LF=0;
    LW=0;
    LS=0;

    LR=1;
    VA=1;
    MO=1;

    hd("RINSE");

    rem(RI_T[md]);

    if(t>=RI_T[md]){

        t=0;

        LR=0;
        VA=0;
        MO=0;

        rn++;

        st=3;

        clr();
    }
}

void spin(){

    LF=0;
    LW=0;
    LR=0;

    LS=1;
    MO=1;

    hd("SPIN");

    rem(SP_T[md]);

    if(t>=SP_T[md]){

        t=0;

        LS=0;
        MO=0;

        st=6;

        clr();
    }
}

void done(){

    LF=0;
    LW=0;
    LR=0;
    LS=0;

    LD=bl;
    BZ=bl;

    hd("DONE");

    pos(1,0);

    str("STOP");
}

void pause(){

    off();

    if(ps==1) LF=bl;
    if(ps==2) LW=bl;
    if(ps==4) LR=bl;
    if(ps==5) LS=bl;

    hd("PAUSE");

    pos(1,0);

    str("WAIT");
}

void key(){

    /* START */

    if(!BS){

        dly(20);

        if(!BS){

            /* idle -> start */

            if(st==0){

                rn=0;

                t=0;

                st=1;

                clr();
            }

            /* running -> stop */

            else if(st>0 && st<7){

                st=0;

                rn=0;

                t=0;

                off();

                clr();
            }

            while(!BS);
        }
    }

    /* PAUSE */

    if(!BP){

        dly(20);

        if(!BP){

            /* resume */

            if(st==7){

                st=ps;
            }

            /* pause */

            else if(st>0 && st<6){

                ps=st;

                st=7;
            }

            clr();
        }

        while(!BP);
    }

    /* MODE */

    if(!BM){

        dly(20);

        if(!BM && st==0){

            md++;

            if(md>3)
                md=0;

            clr();
        }

        while(!BM);
    }

    /* WATER */

    if(!BW){

        dly(20);

        if(!BW && st==0){

            wl++;

            if(wl>2)
                wl=0;

            clr();
        }

        while(!BW);
    }
}

void main(){

    off();

    TMOD=0x11;

    /* 50ms */

    TH0=0x4B;
    TL0=0xFD;

    ET0=1;
    EA=1;

    TR0=1;

    lcd();

    hd("WASH");

    pos(1,0);

    str("READY");

    dly(1000);

    clr();

    while(1){

        key();

        switch(st){

            case 0:
                idle();
                break;

            case 1:
                fill();
                break;

            case 2:
                wash();
                break;

            case 3:
                drain();
                break;

            case 4:
                rinse();
                break;

            case 5:
                spin();
                break;

            case 6:
                done();
                break;

            case 7:
                pause();
                break;
        }
    }
}