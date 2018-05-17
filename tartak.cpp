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


void showStatus(){
    mtx.lock();
		
    mvprintw(4,5,"status -->    ",forestStatus);
    mvprintw(4,5,"status --> %d",forestStatus);

    mvprintw(6,5,"ilosc scietych -->     ");
    mvprintw(6,5,"ilosc scietych --> %d",cutTrees);

    mvprintw(6,40,"ilosc drzew w tartaku -->     ");
    mvprintw(6,40,"ilosc drzew w tartaku --> %d",sawmillTreeStatus);


    mvprintw(6,80,"dlugie deski -->     ");
    mvprintw(6,80,"dlugie deski --> %d",longPlankStatus);

    mvprintw(7,80,"krotkie deski -->     ");
    mvprintw(7,80,"krotkie deski --> %d",shortPlankStatus);

    mvprintw(7,110,"krzeslo -->     ");
    mvprintw(7,110,"krzeslo --> %d",chariStatus);
    mvprintw(8,110,"stol -->     ");
    mvprintw(8,110,"stol --> %d",tableStatus);
    mvprintw(9,110,"lawka -->     ");
    mvprintw(9,110,"lawka --> %d",benchStatus);

    for(int i=0;i<numDriver;i++)
    {
        mvprintw(8+i,25,"kierowca %d:    ",i);
        mvprintw(8+i,25,"kierowca %d: %d",i, driverStatus[i]);
    }

    for(int i=0;i<numWoodcutter;i++)
    {
        mvprintw(8+i,5,"drwal %d: %d",i, counterWoodcuter[i]);
    }
    
     
	refresh();	
	mtx.unlock();
}




void startThreedTree(){

    while(1)
    {
            showStatus();   
            while(forestStatus!=100)
            {
                usleep(rand()%1000000 + 1000);
                forestStatus++;
                showStatus();
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
                if(forestStatus>0)
                {
                    forestStatus--;
                    cutTrees++;
                }
                mtx.unlock();
            }       
            showStatus();

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
    }
}


void startThreedCarpenterChair(int tID){
    bool make;
    while(1)
    {
        make = false;
        mtx.lock();
        if((shortPlankStatus>=5) && ((chariStatus-2)<=tableStatus) && ((chariStatus-2)<=benchStatus))
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
        if((shortPlankStatus>=2 && longPlankStatus>=3) && tableStatus-2<=chariStatus && tableStatus-2<=benchStatus)
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
        if((shortPlankStatus>=4 && longPlankStatus>=3) && benchStatus-2<=tableStatus && benchStatus-2<=chariStatus)
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

//uzupełnić wątek klienta kupującego meble
// z mebli ma być hajs który posłóży do szybszego rośnięcia lasu

void startThreedClient(int tID){

    while(1)
    {

    }
}

int main()
{
    initscr();
	nodelay(stdscr,TRUE);   
    refresh();
    srand(time(NULL));

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