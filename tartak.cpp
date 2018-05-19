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
int cutTrees;
int sawmillTreeStatus;

int longPlankStatus;
int shortPlankStatus;

int driverStatus[numDriver];

int counterWoodcuter[numWoodcutter];

int chariStatus;
int tableStatus;
int benchStatus;

int cash;
int growthRate=1;

void showStatus(){
    mtx.lock();

   

    mvprintw(15,40,"ilosc drzew w tartaku -->     ");
    mvprintw(15,40,"ilosc drzew w tartaku --> %d",sawmillTreeStatus);


    mvprintw(15,80,"dlugie deski -->     ");
    mvprintw(15,80,"dlugie deski --> %d",longPlankStatus);

    mvprintw(16,80,"krotkie deski -->     ");
    mvprintw(16,80,"krotkie deski --> %d",shortPlankStatus);

    mvprintw(15,110,"krzeslo -->     ");
    mvprintw(15,110,"krzeslo --> %d",chariStatus);
    mvprintw(16,110,"stol -->     ");
    mvprintw(16,110,"stol --> %d",tableStatus);
    mvprintw(17,110,"lawka -->     ");
    mvprintw(17,110,"lawka --> %d",benchStatus);
    
    
    mvprintw(17,150,"pieniadze -->     ");
    mvprintw(17,150,"pieniadze --> %d",cash);

    mvprintw(18,150,"growthrate -->     ");
    mvprintw(18,150,"growthrate --> %d",growthRate);

    for(int i=0;i<numDriver;i++)
    {
        mvprintw(20+i,25,"kierowca %d:    ",i);
        mvprintw(20+i,25,"kierowca %d: %d",i, driverStatus[i]);
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
            attron(COLOR_PAIR(1));
            mvprintw(i+2,j+2," ");
            attroff(COLOR_PAIR(1));
        }
    } 
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
        }
    }
    for(int i=0;i<numWoodcutter;i++)
    {
        mvprintw(13+i,2,"drwal %d: %d",i, counterWoodcuter[i]);
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
            mvprintw(i+2,j+29," ");
        }
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter>0)
            {  
                mvprintw(i+2,j+29,"*");
                counter--;
            } 
        }
    }
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
                counterWoodcuter[tID]++;
                mtx.lock();
                if(forestStatus>0 && cutTrees<100)
                {
                    forestStatus--;
                    cutTrees++;
                }
                mtx.unlock();
            }       
            showStatusForest();
            showStatusCutTrees();
            for(int i=0;i<10;i++)
            {
                usleep(rand()%100000+100000);
            }
        
    }
}

void startThreedDriver(int tID){
    bool go;
    while(1)
    {   
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
            for(int i=0;i<10;i++)
            {
                usleep(100000);
                driverStatus[tID]+=10;
                showStatus();
            }
            usleep(1000000);
            sawmillTreeStatus+=20;
            for(int i=0;i<10;i++)
            {
                usleep(100000);
                driverStatus[tID]-=10;
                showStatus();
            }
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

        if(make)
        {
            for(int i=0;i<10;i++)
            {
                usleep(rand()%1000000+1000000);
                shortPlankStatus++;               
                showStatus();      
                if(i%2)
                {
                    usleep(rand()%1000000+1000000);
                    longPlankStatus++;               
                    showStatus();
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
        if(make)
        {
            for(int i=0;i<10;i++)
            {
                usleep(rand()%100000+100000);
                //jakiś tam status produkcji mebla
                showStatus();
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
        if((shortPlankStatus>=2 && longPlankStatus>=3) && tableStatus<=chariStatus && tableStatus<=benchStatus)
        {       
            shortPlankStatus-=2;
            longPlankStatus-=3;
            make=true;
        }
        mtx.unlock();
        if(make)
        {
            for(int i=0;i<10;i++)
            {
                usleep(rand()%100000+100000);
                //jakiś tam status produkcji mebla
                showStatus();
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
        if(make)
        {
            for(int i=0;i<10;i++)
            {
                usleep(rand()%100000+100000);
                //jakiś tam status produkcji mebla
                showStatus();
            }
            benchStatus++;
        }
    }
}

//stworzyć oddzielne sekcje graficzne dla każdego wątku

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
        usleep(10000000);
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