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

int treeStatus=50;
int cutTrees=0;

int driverStatus[numDriver];

int counterWoodcuter[numWoodcutter];
int counterDriver[numDriver];


//int ctW, ctT;   //counter threed woodcuter, tree


void showStatus(){
    mtx.lock();
		
    mvprintw(4,5,"status -->    ",treeStatus);
    mvprintw(4,5,"status --> %d",treeStatus);

    mvprintw(6,5,"ilosc scietych -->     ",cutTrees);
    mvprintw(6,5,"ilosc scietych --> %d",cutTrees);


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
            while(treeStatus!=100)
            {
                usleep(rand()%1000000 + 1000);
                treeStatus++;
                showStatus();
            }      
    
    }
}

void startThreadWoodcutter(int tID){
    while(1)
	{
            if(treeStatus<25)
            {
                while(treeStatus<50)
                {
                    usleep(100000);
                }
            }

            if((counterWoodcuter[tID]-3)<=counterWoodcuter[(2*numWoodcutter+tID-1)%numWoodcutter] &&
            (counterWoodcuter[tID]-3)<=counterWoodcuter[(2*numWoodcutter+tID+1)%numWoodcutter])
            {
                counterWoodcuter[tID]++;
                mtx.lock();
                if(treeStatus>0)
                {
                    treeStatus--;
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

//dodać kolejny wątek transportu - przy 20 ma wyruszać
//drzewa mają być dodawane pojedynczo do ciężarówki 

void startThreedDriver(int tID){
    bool go=false;
    while(1)
    {   
        go=false;
        // while(cutTrees<20);
        // {
        //     usleep(100000);
        // }
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
            for(int i=0;i<10;i++)
            {
                usleep(100000);
                driverStatus[tID]-=10;
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
        counterDriver[i]=i;
    }

//-------------------------------------------------------   
   




//-------------------join-------------------------

    for (int i=0;i<numWoodcutter;i++)
    {
        woodcutter[i].join();
    }
    for (int i=0;i<numDriver;i++)
    {
        driver[i].join();
    }
    tree.join();

//------------------------------------------------

    endwin();


  return 0;
}