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
const int numSawmill = 3;

int forestStatus=50;
int cutTrees;
int sawmillTreeStatus;

int longPlankStatus;
int shortPlankStatus;

int driverStatus[numDriver];

int counterWoodcuter[numWoodcutter];


//int ctW, ctT;   //counter threed woodcuter, tree


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

//dodać wątek tartaku (przerabianie drzew na deski)

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
                usleep(1000000);
                longPlankStatus++;               
                showStatus();
            }
            for(int i=0;i<5;i++)
            {
                usleep(1000000);
                shortPlankStatus++;               
                showStatus();
            }
        }
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

//------------------------------------------------

    endwin();


  return 0;
}