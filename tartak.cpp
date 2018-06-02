#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>

using namespace std;
mutex mtx;
bool endProgram=false;

const int numWoodcutter = 5;
const int numDriver = 2;
const int numSawmill = 6;
const int numCarpenterChair = 4;
const int numCarpenterTable = 4;
const int numCarpenterBench = 4;
const int numClient=3;

int forestStatus=50;
int cutTrees=90;
int sawmillTreeStatus;

int longPlankStatus=20;
int shortPlankStatus=20;

int driverStatus[numDriver];

int counterWoodcuter[numWoodcutter];

int chariStatus;
int tableStatus;
int benchStatus;

int cash;
int growthRate=1;

void showStatus(){
    mtx.lock();

    mvprintw(15,70,"krzeslo -->     ");
    mvprintw(15,70,"krzeslo --> %d",chariStatus);
    mvprintw(16,70,"stol -->     ");
    mvprintw(16,70,"stol --> %d",tableStatus);
    mvprintw(17,70,"lawka -->     ");
    mvprintw(17,70,"lawka --> %d",benchStatus);
    
    
    mvprintw(25,15,"pieniadze -->     ");
    mvprintw(25,15,"pieniadze --> %d",cash);

    mvprintw(26,15,"growthrate -->     ");
    mvprintw(26,15,"growthrate --> %d",growthRate);
  
	refresh();	
	mtx.unlock();
}

void showStatusSawmillTree(){
    mtx.lock();
    int counter = sawmillTreeStatus;
    mvprintw(1,173,"ilosc drzew w tartaku -->     ");
    mvprintw(1,173,"ilosc drzew w tartaku --> %d",sawmillTreeStatus);

    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter>0)
            {  
                mvprintw(i+2,j+182,"*");
                counter--;
            }
            else
            {
                mvprintw(i+2,j+182," ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusForest(){
    mtx.lock();
    int counter = forestStatus;
    counter*=2;
    mvprintw(1,4,"Stan lasu -->    ",forestStatus);
    mvprintw(1,4,"Stan lasu --> %d",forestStatus);
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<20;j++)
        {
            if(counter>0)
            {
                attron(COLOR_PAIR(3));	
                mvprintw(i+2,j+2," ");
                attroff(COLOR_PAIR(3));
                counter--;
            }
            else
            {
                attron(COLOR_PAIR(2));
                mvprintw(i+2,j+2," ");
                attroff(COLOR_PAIR(2));
            } 
        }
    }
    
    refresh();
    mtx.unlock();
    
}

void showStatusWoodcuter(int tID, bool work){
    mtx.lock();
    if(work)
    {
        attron(COLOR_PAIR(2));
        mvprintw(13+tID,2,"drwal %d: %d",tID, counterWoodcuter[tID]);
        attroff(COLOR_PAIR(2));
    }
    else
    {
        mvprintw(13+tID,2,"drwal %d: %d",tID, counterWoodcuter[tID]);
    }
    refresh();
    mtx.unlock();
}


void showStatusCutTrees(){
    mtx.lock();
    int counter = cutTrees;
    mvprintw(1,27,"Ilosc pni -->     ");
    mvprintw(1,27,"Ilosc pni --> %d",cutTrees);
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter>0)
            {  
                mvprintw(i+2,j+29,"*");
                counter--;
            }
            else
            {
                mvprintw(i+2,j+29," ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusSawmill(){
    mtx.lock();
    int counter1 = longPlankStatus;
    int counter2 = shortPlankStatus;
    mvprintw(19,173,"dlugie deski -->     ");
    mvprintw(19,173,"dlugie deski --> %d",longPlankStatus);

    mvprintw(29,173,"krotkie deski -->     ");
    mvprintw(29,173,"krotkie deski --> %d",shortPlankStatus);
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter1>0)
            {  
                attron(COLOR_PAIR(2));	
                mvprintw(i+20,j+175,"D");
                attroff(COLOR_PAIR(2));
                counter1--;
            }
            else
            {
                mvprintw(i+20,j+175," ");
            }
            if(counter2>0)
            {
                attron(COLOR_PAIR(2));
                mvprintw(i+30,j+175,"K");
                attroff(COLOR_PAIR(2));
                counter2--;
            }
            else
            {
                 mvprintw(i+30,j+175," ");
            }
        }
    }
    refresh();
    mtx.unlock();
                
}

    
void showStatusDriver1(int tID, int position){
    mtx.lock();
    if(position == 0)
    {
        mvprintw(2+tID*5,45," _______________________");
        mvprintw(3+tID*5,45,"|SO2                    |^_ __");
        mvprintw(4+tID*5,45,"|                       ||=|##|_");
        mvprintw(5+tID*5,45,"|________________.====._||_|__._]");
        mvprintw(6+tID*5,45," `(_)(_)`       `(_)(_)\"\"\"=\"=(_)");
        refresh();
    }
    else
    {
        for(int i=45;i<145;i++)
        {
            for(int j=2;j<=6;j++)
            {
                 mvprintw(j+tID*5,i-1,"                                     ");
            }
            mvprintw(2+tID*5,i," _______________________");
            mvprintw(3+tID*5,i,"|SO2  * * * * * * * * * |^_ __");
            mvprintw(4+tID*5,i,"|* * * * * * * * * * * *||=|##|_");
            mvprintw(5+tID*5,i,"|________________.====._||_|__._]");
            mvprintw(6+tID*5,i," `(_)(_)`       `(_)(_)\"\"\"=\"=(_)");
            refresh();
            usleep(20000);
        }
    }
    mtx.unlock();
}

void showStatusDriver2(int tID){
    mtx.lock();
    for(int i=145;i>45;i--)
    {
       for(int j=2;j<=6;j++)
        {
            mvprintw(j+tID*5,i-1,"                                     ");
        }
        mvprintw(2+tID*5,i,"         ______________________");
        mvprintw(3+tID*5,i,"   __ _^|                   SO2|");
        mvprintw(4+tID*5,i," _|##||                        |");
        mvprintw(5+tID*5,i,"[_.__|_||_.====._______________|");
        mvprintw(6+tID*5,i," (_)=\"=\"\"\"(_)(_)`       `(_)(_)`");
        refresh();
        usleep(20000);
    } 
    for(int j=2;j<=6;j++)
    {
        mvprintw(j+tID*5,45,"                                     ");
    }
    mtx.unlock();
}


void showStatusCarpenterChair(int tID, int position){
    mtx.lock();

    mvprintw(15,110,"Produkcja krzesel"); // wywalić gdzieś do głównego showStatus
    
    mvprintw(16+tID,100,"Stolarz %d: ",tID);
    for(int i=0;i<10;i++)
    {
        mvprintw(16+tID,111+i," ");
    }
    for(int i=0;i<position;i++)
    {
        attron(COLOR_PAIR(2));
        mvprintw(16+tID,111+i," ");
        attroff(COLOR_PAIR(2));
    }
    char procent = 37;
    mvprintw(16+tID,121,"     ");
    mvprintw(16+tID,121,"%d %c",position*10,procent);
    refresh();
    mtx.unlock();
}

void showStatusCarpenterTable(int tID, int position){
    mtx.lock();

    mvprintw(22,110,"Produkcja stolow"); // wywalić gdzieś do głównego showStatus
    
    mvprintw(23+tID,100,"Stolarz %d: ",tID);
    for(int i=0;i<10;i++)
    {
        mvprintw(23+tID,111+i," ");
    }
    for(int i=0;i<position;i++)
    {
        attron(COLOR_PAIR(2));
        mvprintw(23+tID,111+i," ");
        attroff(COLOR_PAIR(2));
    }
    char procent = 37;
    mvprintw(23+tID,121,"     ");
    mvprintw(23+tID,121,"%d %c",position*10,procent);
    refresh();
    mtx.unlock();
}

void showStatusCarpenterBench(int tID, int position){
    mtx.lock();

    mvprintw(29,110,"Produkcja lawek"); // wywalić gdzieś do głównego showStatus
    
    mvprintw(30+tID,100,"Stolarz %d: ",tID);
    for(int i=0;i<10;i++)
    {
        mvprintw(30+tID,111+i," ");
    }
    for(int i=0;i<position;i++)
    {
        attron(COLOR_PAIR(2));
        mvprintw(30+tID,111+i," ");
        attroff(COLOR_PAIR(2));
    }
    char procent = 37;
    mvprintw(30+tID,121,"     ");
    mvprintw(30+tID,121,"%d %c",position*10,procent);
    refresh();
    mtx.unlock();
}

void startThreedTree(){

    while(1)
    {
            showStatusForest();   
            while(forestStatus!=100)
            {
                usleep(rand()%1000000 + (int)(1000000/growthRate));
                forestStatus++;
                showStatusForest();
            }      
    
    }
}

void startThreadWoodcutter(int tID){
    while(1)
	{
            bool work=false;
            showStatusWoodcuter(tID, work);
            usleep(1000000);
            if(forestStatus<25)
            {
                while(forestStatus<50)
                {
                    usleep(100000);
                }
            }
            if((counterWoodcuter[tID]-3)<=counterWoodcuter[(2*numWoodcutter+tID-1)%numWoodcutter] &&
            (counterWoodcuter[tID]-3)<=counterWoodcuter[(2*numWoodcutter+tID+1)%numWoodcutter])
            {
                
                mtx.lock();
                if(forestStatus>0 && cutTrees<100)
                {
                    counterWoodcuter[tID]++;
                    forestStatus--;
                    cutTrees++;
                    work=true;
                }
                mtx.unlock();
            }       
            showStatusForest();
            showStatusCutTrees();
            if(work)
            {
                for(int i=0;i<10;i++)
                {
                    showStatusWoodcuter(tID, work);
                    usleep(rand()%100000+100000);
                }
            }
        
    }
}

//jak starczy czasu to dodać wizualizacje produkcji desek
//dodać wizualizacje produkcji mebli

void startThreedDriver(int tID){
    bool go;
    while(1)
    {   
        showStatusDriver1(tID, 0);
        go=false;
        mtx.lock();
        if(cutTrees>=20)
        {       
            cutTrees-=20;
            go=true;
        }
        mtx.unlock();
        if(go)
        {
             showStatusCutTrees();
             usleep(100000);
             showStatusDriver1(tID,1);
             usleep(500000); //czas postoju ciężarówki
             while(sawmillTreeStatus>=80)
             {
                 usleep(500000);
             }
             sawmillTreeStatus+=20;
             showStatusSawmillTree();
             usleep(100000);
             showStatusDriver2(tID);
        }
    }
}

void startThreedSawmill(int tID){
    bool make;
    while(1)
    {
        make=false;
        mtx.lock();
        if(sawmillTreeStatus>0)
        {       
            sawmillTreeStatus--;
            make=true;
        }
        mtx.unlock();
        showStatusSawmillTree();

        if(make)
        {
            for(int i=0;i<10;i++)
            {
                usleep(rand()%1000000+1000000);
                shortPlankStatus++;               
                showStatusSawmill();     
                if(i%2)
                {
                    usleep(rand()%1000000+1000000);
                    longPlankStatus++;               
                    showStatusSawmill();
                }
            }
        }
        if(cash>=100 && growthRate<1000000)
        {
            cash-=100;
            growthRate*=2;
        }
    }
}


void startThreedCarpenterChair(int tID){
    bool make;
    while(1)
    {
        make = false;
        mtx.lock();
        if((shortPlankStatus>=5) && (chariStatus<=tableStatus) && (chariStatus<=benchStatus))
        {       
            shortPlankStatus-=5;
            make=true;
        }
        mtx.unlock();
        showStatusSawmill();
        if(make)
        {
            for(int i=1;i<=10;i++)
            {
                usleep(rand()%100000+100000);
                showStatusCarpenterChair(tID, i);
            }
            chariStatus++;
        }
    }
}

void startThreedCarpenterTable(int tID){
    bool make;
    while(1)
    {
        make = false;
        mtx.lock();
        if((shortPlankStatus>=4 && longPlankStatus>=3) && tableStatus<=chariStatus && tableStatus<=benchStatus)
        {       
            shortPlankStatus-=4;
            longPlankStatus-=3;
            make=true;
        }
        mtx.unlock();
        showStatusSawmill();
        if(make)
        {
            for(int i=1;i<=10;i++)
            {
                usleep(rand()%100000+100000);
                showStatusCarpenterTable(tID, i);
            }
            tableStatus++;
        }
    }  
}

void startThreedCarpenterBench(int tID){
    bool make;
    while(1)
    {
        make = false;
        mtx.lock();
        if((shortPlankStatus>=4 && longPlankStatus>=3) && benchStatus<=tableStatus && benchStatus<=chariStatus)
        {       
            shortPlankStatus-=4;
            longPlankStatus-=3;
            make=true;
        }
        mtx.unlock();
        showStatusSawmill();
        if(make)
        {
            for(int i=1;i<=10;i++)
            {
                usleep(rand()%100000+100000);
               showStatusCarpenterBench(tID, i);
                showStatus();
            }
            benchStatus++;
        }
    }
}

void startThreedClient(int tID){

    while(1)
    {
        int choice = rand()%3+1;
        mtx.lock();
        if(choice==1)
        {
            if(chariStatus>0)
            {
                chariStatus--;
                cash+=10;
            }
        }
        else if(choice==2)
        {
            if(tableStatus>0)
            {
                tableStatus--;
                cash+=15;
            }
        }
        else if(choice==3)
        {
            if(benchStatus>0)
            {
                benchStatus--;
                cash+=20;
            }
        }
        mtx.unlock();
        showStatus();
        usleep(5000000);
    }
}

int main()
{
    initscr();
	nodelay(stdscr,TRUE);   

    refresh();
    srand(time(NULL));
    clear();
    start_color();
    init_pair(1,  COLOR_BLACK,COLOR_RED);
	init_pair(2,  COLOR_BLACK,COLOR_YELLOW);
	init_pair(3,  COLOR_BLACK,COLOR_GREEN);
	init_pair(4,  COLOR_BLACK,COLOR_BLUE);
	init_pair(5,  COLOR_BLACK,COLOR_MAGENTA);
	init_pair(6,  COLOR_BLACK,COLOR_WHITE);
	init_pair(7,  COLOR_BLACK,COLOR_CYAN);
    curs_set(0);


//------------------deklaracja wątków--------------------
    thread tree;
    thread woodcutter[numWoodcutter];
    thread driver[numDriver];
    thread sawmill[numSawmill];
    thread carpenterChair[numCarpenterChair];
    thread carpenterTable[numCarpenterTable];
    thread carpenterBench[numCarpenterBench];
    thread client[numClient];
//-------------------------------------------------------   

//------------------uruchamianie wątków------------------
    tree = thread(startThreedTree);
    for (int i=0;i<numWoodcutter;i++)
    {
        woodcutter[i]=thread(startThreadWoodcutter, i);
        counterWoodcuter[i]=0;
    }
    for (int i=0;i<numDriver;i++)
    {
        driver[i]=thread(startThreedDriver, i);
    }

    for (int i=0;i<numSawmill;i++)
    {
        sawmill[i]=thread(startThreedSawmill, i);
    }

    for (int i=0;i<numCarpenterChair;i++)
    {
        carpenterChair[i]=thread(startThreedCarpenterChair, i);
    }
    for (int i=0;i<numCarpenterTable;i++)
    {
        carpenterTable[i]=thread(startThreedCarpenterTable, i);
    }
    for (int i=0;i<numCarpenterBench;i++)
    {
        carpenterBench[i]=thread(startThreedCarpenterBench, i);
    }
    for (int i=0;i<numClient;i++)
    {
        client[i]=thread(startThreedClient, i);
    }
//-------------------------------------------------------   

//-------------------join-------------------------

    tree.join();
    for (int i=0;i<numWoodcutter;i++)
    {
        woodcutter[i].join();
    }
    for (int i=0;i<numDriver;i++)
    {
        driver[i].join();
    }
    for (int i=0;i<numSawmill;i++)
    {
        sawmill[i].join();
    }
    for (int i=0;i<numCarpenterChair;i++)
    {
        carpenterChair[i].join();
    }
    for (int i=0;i<numCarpenterTable;i++)
    {
        carpenterTable[i].join();
    }
    for (int i=0;i<numCarpenterBench;i++)
    {
        carpenterBench[i].join();
    }
    for (int i=0;i<numClient;i++)
    {
        client[i].join();
    }
//------------------------------------------------

    endwin();
    return 0;
}