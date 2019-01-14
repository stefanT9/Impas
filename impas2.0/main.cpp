#include <iostream>
#include <stdio.h>
#include <winbgim.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#define REZOLUTION_HEIGHT 720
#define REZOLUTION_WIDTH  1080
#define MAX_SIZE 101

#define MAIN_MENU 1
#define OPTIONS_MENU 2



using namespace std;

const int WINDOW = initwindow(REZOLUTION_WIDTH, REZOLUTION_HEIGHT, "Impas");

struct coord
{
    int x=0;
    int y=0;
};

struct impas
{
    int game_mode=0;
    int difficulty=1;
    int latura=6;                     ///latura tablei/nr patratele per margine
    bool tabla[MAX_SIZE][MAX_SIZE];   ///1-piesa | 0-spatiu liber
    bool rand;                        ///0-jucator 1
                                      ///1-jucator 2
    bool player=0;


    void initializare(int n)
    {
        /**
            pregateste tabla pt joc mutand piesele pe pozitiile intiale
        */

        latura=n;
        int i,j;
        for(i=0; i<latura; i++)     ///initializeza tabla cu 0
            for(j=0; j<latura; j++)
                tabla[i][j]=0;

        for(i=(latura+1)/2; i<latura; i++)  /// initializare piese
            for(j=0; j<latura/2; j++)
                tabla[i][j]=1;
    }

    bool verif_game_over()
    {
        /**
            verifica daca exista vreo mutare valida in functie de randul jucatorului
            pentru a avea macar o mutare valida trebuie sa existe macar un spatiu adiacent liber
                ->in sus (player 1)
                ->in dreapta (player 2)
        */
        if(rand==0)///verificare player 1
        {
            for(int i=1; i<latura; i++)
            {
                for(int j=0; j<latura; j++)
                {
                    if(tabla[i][j]==true)///este piesa pe i,j
                    {
                        if(tabla[i-1][j]==false)///spatiul de deasupra este liber
                            return false;
                    }

                }
            }
            return true;
        }
        else if (rand==1)///verifica pt player 2
        {
            for(int i=0; i<latura; i++)
            {
                for(int j=0; j<latura-1; j++)
                {
                    if(tabla[i][j]==true)///este piesa pe i,j
                    {
                        if(tabla[i][j+1]==false)///spatiul din dreapta este liber
                            return false;
                    }
                }
            }
            return true;
        }
    }
    bool validare_mutare(int x,int y)   ///false-miscare invalida
    {
        /**
            verifica daca exista o piesa pe
            ->linia x (player 1)
            ->coloana y(player 2)
            daca conditia este indeplinita realizeaza mutarea cu singura piesa posibila
        */

        if (tabla[y][x]==true || x<0 || y <0 || x>=latura || y>=latura)
            return false;


        if(rand==0)
        {
            for(int i=y; i<latura; i++)
            {
                if(tabla[i][x]==true) ///pe prima piesa gasita
                {
                    swap(tabla[y][x],tabla[i][x]);
                    rand=1;
                    return true;
                }
            }
            return false;
        }
        else if(rand==1)
        {
            for(int i=x; i>=0; i--)
            {
                if(tabla[y][i]==true)
                {
                    swap(tabla[y][x],tabla[y][i]);
                    rand=0;
                    return true;
                }
            }
            return false;
        }
    }
    void print_tabla()
    {
        for(int i=0; i<latura; i++)
        {
            for(int j=0; j<latura; j++)
            {
                cout<<tabla[i][j];
            }
            cout<<endl;
        }
    }
} singleton;



coord get_click();
coord get_clickXY();

void draw_ALL();
void auto_move();
void draw_main();
void game_loop();
void draw_optiuni();

int buton_apasat(coord clk,int fereastra);

int main()
{
    ///mcirez=mciSendString(comm,NULL, 0, NULL)    setcurrentwindow(WINDOW);
    int x,y;
    draw_main();



    return 0;
}
void auto_move()
{
    int nr_elem_linii[MAX_SIZE]= {0};
    int nr_elem_coloane[MAX_SIZE]= {0};

    for(int i=0; i<singleton.latura; i++)
    {
        for(int j=0; j<singleton.latura; j++)
        {
            nr_elem_linii[i]+=singleton.tabla[i][j];
            nr_elem_coloane[j]+=singleton.tabla[i][j];
        }
    }
    int maxi=-1;

    if (singleton.rand==0)
    {
        for(int i=0; i<singleton.latura; i++)
        {
            for(int j=0; j<singleton.latura; j++)
            {
                if(maxi==-1)
                {
                    maxi=i;
                }
                else
                {
                    if(nr_elem_linii[i]>nr_elem_linii[maxi])
                    {
                        maxi=i;
                    }
                }
            }

            for(int j=0; j<singleton.latura; j++)
            {
                if(singleton.tabla[i][j]==1)
                    nr_elem_linii[maxi]--;
                if(nr_elem_linii[maxi]==0)
                {
                    if(singleton.validare_mutare(j,singleton.latura-i)==1)
                    {
                        return;
                    }
                }
            }
            nr_elem_linii[maxi]=-1;

        }
    }
    else if(singleton.rand==1)
    {
        for(int i=0; i<singleton.latura; i++)
        {
            for(int j=0; j<singleton.latura; j++)
            {
                if(maxi==-1)
                {
                    maxi=j;
                }
                else
                {
                    if(nr_elem_coloane[j]>nr_elem_coloane[maxi])
                    {
                        maxi=j;
                    }
                }
            }

            for(int j=0; j<singleton.latura; j++)
            {
                if(singleton.tabla[j][i]==1)
                    nr_elem_coloane[maxi]--;
                if(nr_elem_coloane[maxi]==0)
                {
                    if(singleton.validare_mutare(i,j)==1)
                    {
                        return;
                    }
                }
            }
            nr_elem_coloane[maxi]=-1;

        }
    }

    for(int i=0; i<singleton.latura; i++)
    {
        for(int j=0; j<singleton.latura; j++)
        {
            if(singleton.validare_mutare(i,j))
                return;
        }
    }

}
void draw_ALL()
{
    const int lungime=REZOLUTION_HEIGHT/singleton.latura;
    setfillstyle(SOLID_FILL,RED);
    setlinestyle(0,0,5);
    bar(0, 0,REZOLUTION_WIDTH, REZOLUTION_HEIGHT);

    ///tabla
    for(int i=0; i<singleton.latura; i++)
    {
        for(int j=0; j<singleton.latura; j++)
        {
            setcolor(CYAN);
            setfillstyle(SOLID_FILL,4);
            setlinestyle(0,0,5);
            rectangle(i*lungime,j*lungime,(i+1)*lungime,(j+1)*lungime);
            if(singleton.tabla[j][i]==true)    /// este piesa
            {
                ///printf("(%d %d)\n",i,j);
                setcolor(BLACK);
                setlinestyle(0,0,0);
                setfillstyle(SOLID_FILL,COLOR(254, 193, 7));
                fillellipse(i*lungime+lungime/2,j*lungime+lungime/2,lungime/4,lungime/3);

                setcolor((COLOR(254, 193, 7)));
                setfillstyle(SOLID_FILL,COLOR(255,235,60));

                fillellipse(i*lungime+lungime/2,j*lungime+lungime/2,lungime/8,lungime/6);

            }
        }
    }

    ///butoanele si meniul din dreapta
    settextstyle(COMPLEX_FONT,0,2);
    settextjustify(CENTER_TEXT,CENTER_TEXT);

    if(singleton.rand==0)
    {
        setfillstyle(SOLID_FILL,YELLOW);///este randul jucatorului
        setbkcolor(YELLOW);
    }
    else
    {
        setbkcolor(WHITE);
        setfillstyle(SOLID_FILL,WHITE);
    }
    bar(REZOLUTION_HEIGHT,0,REZOLUTION_WIDTH,36);
    setcolor(CYAN);
    rectangle(REZOLUTION_HEIGHT,0,REZOLUTION_WIDTH,36);
    setcolor(BLACK);
    if(singleton.game_mode==0||singleton.player==0)
    {
        outtextxy(REZOLUTION_WIDTH-180,24,"PLAYER 1");
    }
    else
    {
        outtextxy(REZOLUTION_WIDTH-180,24,"NOT_A_ROBOT_01");
    }
    if(singleton.rand==1)
    {
        setfillstyle(SOLID_FILL,YELLOW);///este randul jucatorului
        setbkcolor(YELLOW);
    }
    else
    {
        setbkcolor(WHITE);
        setfillstyle(SOLID_FILL,WHITE);
    }
    bar(REZOLUTION_HEIGHT,REZOLUTION_HEIGHT-36,REZOLUTION_WIDTH,REZOLUTION_HEIGHT);
    setcolor(CYAN);
    rectangle(REZOLUTION_HEIGHT,REZOLUTION_HEIGHT-36,REZOLUTION_WIDTH,REZOLUTION_HEIGHT);
    setcolor(BLACK);
    if(singleton.game_mode==0||singleton.player==1)
    {
        outtextxy(REZOLUTION_WIDTH-180,REZOLUTION_HEIGHT-12,"PLAYER 2");
    }
    else
    {
        outtextxy(REZOLUTION_WIDTH-180,REZOLUTION_HEIGHT-12,"NOT_A_ROBOT_10");
    }
    setbkcolor(RED);

    settextjustify(LEFT_TEXT,CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT,0,1);
    outtextxy(REZOLUTION_HEIGHT+10,200,"-jucatorul 1 poate sa mute doar");
    outtextxy(REZOLUTION_HEIGHT+10,220," in sus");
    outtextxy(REZOLUTION_HEIGHT+10,240,"-jucatorul 2 poate sa mute doar");
    outtextxy(REZOLUTION_HEIGHT+10,260," in dreapta");
    outtextxy(REZOLUTION_HEIGHT+10,280,"-pentru a face o mutare");
    outtextxy(REZOLUTION_HEIGHT+10,300,"selectati pozitia pe ");
    outtextxy(REZOLUTION_HEIGHT+10,320,"care ati dori sa ");
    outtextxy(REZOLUTION_HEIGHT+10,340,"faceti mutarea");
    outtextxy(REZOLUTION_HEIGHT+10,360,"-jocul se termina cand jucatorul");
    outtextxy(REZOLUTION_HEIGHT+10,380,"nu mai poate sa mute");
}
coord get_click()
{
    int mX,mY,x,y;
    coord aux;
    while(1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            aux.x=mousex();
            aux.y=mousey();

            for(int i=1; i<=singleton.latura; i++)
            {
                if(aux.x <= i*(REZOLUTION_HEIGHT/singleton.latura))
                {
                    aux.x=i-1;
                    break;
                }
            }
            for(int i=1; i<=singleton.latura; i++)
            {
                if(aux.y<=i*(REZOLUTION_HEIGHT/singleton.latura))
                {
                    aux.y=i-1;
                    break;
                }
            }
            return aux;
        }
    }
}
coord get_clickXY()
{
    coord aux;
    while(1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            aux.x=mousex();
            aux.y=mousey();
            return aux;
        }
    }

}
void draw_main()
{

    int left, top, right, bottom;
    setbkcolor(CYAN);
    settextjustify(1,1);
    settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
    setfillstyle(SOLID_FILL,CYAN);

    bar(0,0,REZOLUTION_WIDTH,REZOLUTION_HEIGHT);

    int lungime=500;

    setcolor(BLACK);
    setlinestyle(0,0,0);
    setfillstyle(SOLID_FILL,COLOR(254, 193, 7));
    fillellipse(REZOLUTION_WIDTH/4,REZOLUTION_HEIGHT/2,lungime/4,lungime/3);
    setcolor((COLOR(254, 193, 7)));
    setfillstyle(SOLID_FILL,COLOR(255,235,60));
    fillellipse(REZOLUTION_WIDTH/4,REZOLUTION_HEIGHT/2,lungime/8,lungime/6);

    setcolor(BLACK);
    setlinestyle(0,0,0);
    setfillstyle(SOLID_FILL,COLOR(254, 193, 7));
    fillellipse(REZOLUTION_WIDTH-REZOLUTION_WIDTH/4,REZOLUTION_HEIGHT/2,lungime/4,lungime/3);
    setcolor((COLOR(254, 193, 7)));
    setfillstyle(SOLID_FILL,COLOR(255,235,60));
    fillellipse(REZOLUTION_WIDTH-REZOLUTION_WIDTH/4,REZOLUTION_HEIGHT/2,lungime/8,lungime/6);

    coord clk;


    setcolor(WHITE);
    left = getmaxx() / 2-100;
    top = getmaxy() / 2 - 25;
    right = getmaxx() / 2+100;
    bottom = getmaxy() / 2 + 25;

    outtextxy((left+right)/2,(top+bottom)/2-250,"IMPAS");


    outtextxy((left+right)/2,(top+bottom)/2-75,"Joc nou");
    rectangle(left,top-85,right,bottom-85);

    outtextxy((left+right)/2,(top+bottom)/2+10,"Setari");
    rectangle(left,top,right,bottom);

    outtextxy((left+right)/2,(top+bottom)/2+95,"Iesire");
    rectangle(left,top+85,right,bottom+85);

    clk=get_clickXY();
    while(buton_apasat(clk,MAIN_MENU)==-1)
    {
        clk=get_clickXY();
    }
    if(buton_apasat(clk,MAIN_MENU)==1)
    {
        singleton.initializare(singleton.latura);
        printf("game_loop \n");
        game_loop();
    }
    else if(buton_apasat(clk,MAIN_MENU)==2)
    {
        printf("setari \n");
        draw_optiuni();
    }
    else if(buton_apasat(clk,MAIN_MENU)==3)
    {
        printf("QUIT \n");
        closegraph();
    }
}

void game_loop()
{
    draw_ALL();
    while(singleton.verif_game_over()==false)
    {
        printf("player %d\n",singleton.rand);
        ///cin>>x>>y;

        if(singleton.game_mode==1&&singleton.rand!=singleton.player)
        {

            auto_move();
            int sleep_time=1000+rand()%1500;
            Sleep(sleep_time);
            draw_ALL();
        }
        else
        {
            coord mutare=get_click();
            ///cout<<mutare.x<<" "<<mutare.y<<endl;
            ///cout<<x<<" "<<y<<endl;
            if(singleton.validare_mutare(mutare.x,mutare.y))
            {
                ///singleton.print_tabla();
                draw_ALL();
            }
            else
            {
                cout<<"please try again\n";
            }
        }
    }

}
void draw_optiuni()
{
    setlinestyle(0,0,0);


    char text[100]="nr de patrate: ";
    sprintf(text,"%s%d",text,singleton.latura*singleton.latura);

    int left,right,top,bot;
    coord clk;
    setcolor(WHITE);
    setbkcolor(CYAN);
    setfillstyle(SOLID_FILL,CYAN);

    bar(0,0,REZOLUTION_WIDTH,REZOLUTION_HEIGHT);
    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
    settextjustify(LEFT_TEXT,TOP_TEXT);
    outtextxy(0,0,"BACK");
    rectangle(0,0,textwidth("BACK"),textheight("BACK"));
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setfillstyle(SOLID_FILL,CYAN);

    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
    outtextxy(REZOLUTION_WIDTH/2,100,"OPTIUNI");

    settextstyle(COMPLEX_FONT,HORIZ_DIR,4);

    settextjustify(RIGHT_TEXT,CENTER_TEXT);
    outtextxy(REZOLUTION_WIDTH/2-10,200,"Modul de joc:");
    outtextxy(REZOLUTION_WIDTH/2-10,300,"Primul la mutare:");

    outtextxy(REZOLUTION_WIDTH/2-10,400,text);

    settextjustify(LEFT_TEXT,CENTER_TEXT);

///     cout<<outt
    outtextxy(REZOLUTION_WIDTH/2+10,200,"player vs player");
    outtextxy(REZOLUTION_WIDTH/2+10,250,"player vs AI");
    outtextxy(REZOLUTION_WIDTH/2+10,300,"Player 1");
    outtextxy(REZOLUTION_WIDTH/2+10,350,"Player 2");

    if(singleton.game_mode==0)
    {
        left=REZOLUTION_WIDTH/2+5;
        right=REZOLUTION_WIDTH/2+10+textwidth("player vs player");
        top=200-textheight("player vs player");
        bot=200+textheight("player vs player")-20;
        rectangle(left,top,right,bot);
    }
    else
    {
        outtextxy(REZOLUTION_WIDTH/2+10,350,"AI        ");
        left=REZOLUTION_WIDTH/2+5;
        right=REZOLUTION_WIDTH/2+5+textwidth("player vs AI");
        top=250-textheight("player vs AI");
        bot=250+textheight("player vs AI")-20;
        rectangle(left,top,right,bot);
    }
    if(singleton.rand==0)
    {
        left=REZOLUTION_WIDTH/2+5;
        right=REZOLUTION_WIDTH/2+10+textwidth("player 1");
        top=300-textheight("player 1");
        bot=300+textheight("player 1")-20;
        rectangle(left,top,right,bot);
    }
    else
    {

        left=REZOLUTION_WIDTH/2+5;
        if(singleton.game_mode==0)
            right=REZOLUTION_WIDTH/2+10+textwidth("player 2");
        else
            right=REZOLUTION_WIDTH/2+10+textwidth("AI");
        top=350-textheight("player 2");
        bot=350+textheight("player 2")-20;
        rectangle(left,top,right,bot);
    }


    ///minus
    rectangle(REZOLUTION_WIDTH/2+5, 400-textheight(" "), REZOLUTION_WIDTH/2+5+2*textheight(" ")-20, 400+textheight(" ")-20);
    ///plus
    rectangle(REZOLUTION_WIDTH/2+2*textheight(" "), 400-textheight(" "), REZOLUTION_WIDTH/2+4*textheight(" ")-20, 400+textheight(" ")-20);
    ///minus

    setlinestyle(0,0,0);
    setfillstyle(SOLID_FILL,WHITE);

    bar(REZOLUTION_WIDTH/2+10, 387, REZOLUTION_WIDTH/2+5+2*textheight(" ")-25, 393);
    ///plus
    bar(REZOLUTION_WIDTH/2+2*textheight(" ")+5, 387, REZOLUTION_WIDTH/2+4*textheight(" ")-25, 393);
    bar(REZOLUTION_WIDTH/2+3*textheight(" ")-13, 400-textheight(" ")+5, REZOLUTION_WIDTH/2+3*textheight(" ")-7, 400+textheight(" ")-25);

    clk=get_clickXY();
    while(buton_apasat(clk,OPTIONS_MENU)==-1)
    {
        clk=get_clickXY();
    }
    if(buton_apasat(clk,OPTIONS_MENU)==1)///back
    {
        draw_main();
    }
    else if(buton_apasat(clk,OPTIONS_MENU)==2)///p vs p
    {
        singleton.game_mode=0;
        draw_optiuni();
    }
    else if(buton_apasat(clk,OPTIONS_MENU)==3)///p vs AI
    {
        singleton.game_mode=1;
        draw_optiuni();
    }
    else if(buton_apasat(clk,OPTIONS_MENU)==4)///p1 muta primul
    {
        singleton.player=0;
        singleton.rand=0;
        draw_optiuni();
    }
    else if(buton_apasat(clk,OPTIONS_MENU)==5)///p2 muta primul
    {
        singleton.player=1;
        singleton.rand=1;


        draw_optiuni();
    }
    else if(buton_apasat(clk,OPTIONS_MENU)==6)///mai putine patrate
    {
        if(singleton.latura>4)
        {
            singleton.latura--;
        }
        draw_optiuni();
    }
    else if(buton_apasat(clk,OPTIONS_MENU)==7)///mai multe patrate
    {
        if(singleton.latura<14)
        {
            singleton.latura++;
        }
        draw_optiuni();
    }

}

int buton_apasat(coord clk,int fereastra)
{

    if(fereastra==MAIN_MENU)
    {
        int left, top, right, bottom;
        left = getmaxx() / 2-100;
        top = getmaxy() / 2 - 25;
        right = getmaxx() / 2+100;
        bottom = getmaxy() / 2 + 25;

        if(clk.x>=left&&clk.x<+right)
        {
            if(clk.y>=top-85&&clk.y<=bottom-85)
            {
                printf("click pe new game\n");
                return 1;
            }
            if(clk.y>=top&&clk.y<=bottom)
            {
                printf("clock pe optiuni\n");
                return 2;
            }
            if(clk.y>=top+85&&clk.y<=bottom+85)
            {
                printf("clock pe iesire\n");
                return 3;
            }
        }

        return -1;
    }
    if(fereastra==OPTIONS_MENU)
    {
        printf("fereastra optiuni:\n");
        if(clk.x<=textwidth("BACK")&&clk.y<=textheight("BACK"))
        {
            return 1;
        }
        else if(clk.x>=REZOLUTION_WIDTH/2+5)
        {
            if((clk.x<=REZOLUTION_WIDTH/2+10+textwidth("player vs player"))&&(abs(clk.y-200)<=textheight("player vs player")))
            {
                printf("game mode set pvp \n");
                return 2;
            }
            else if((clk.x <= REZOLUTION_WIDTH/2+10+textwidth("player vs ai"))&&(abs(clk.y-250)<=textheight("player vs ai")))
            {
                printf("game mode set pve \n");
                return 3;
            }
            else if(clk.x <= REZOLUTION_WIDTH/2+10+textwidth("player 1")&&(abs(clk.y-300)<=textheight("player 1")))
            {
                printf("player 1 muta primul \n");
                return 4;
            }
            else if(clk.x <= REZOLUTION_WIDTH/2+10+textwidth("player 2")&&(abs(clk.y-350)<=textheight("player 2"))&&(singleton.game_mode==0))
            {
                printf("player 2 muta primul \n");
                return 5;
            }
            else if(clk.x <= REZOLUTION_WIDTH/2+10+textwidth("AI")&&(abs(clk.y-350)<=textheight("AI "))&&(singleton.game_mode==1))
            {
                printf("AI muta primul \n");
                return 5;
            }
            else if((clk.x <= REZOLUTION_WIDTH/2+2*textheight(" ")-15)&&(clk.y>=400-textheight(" "))&&(clk.y<=400+textheight(" ")))
            {
                printf("- \n");
                return 6;
            }
            else if((clk.x >=REZOLUTION_WIDTH/2+2*textheight(" ") )&&(clk.x <= REZOLUTION_WIDTH/2+4*textheight(" ")-20)&&(clk.y>=400-textheight(" "))&&(clk.y<=400+textheight(" ")))
            {
                printf("+ \n");
                return 7;
            }
            /**
                ///minus
                    rectangle(REZOLUTION_WIDTH/2+5, 400-textheight(" "), REZOLUTION_WIDTH/2+5+2*textheight(" ")-20, 400+textheight(" ")-20);
                ///plus
                    rectangle(REZOLUTION_WIDTH/2+2*textheight(" "), 400-textheight(" "), REZOLUTION_WIDTH/2+4*textheight(" ")-20, 400+textheight(" ")-20);

            */
        }
        printf("pe langa\n");
        return -1;
    }
    return -1;

}
