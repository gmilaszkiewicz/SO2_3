#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>

using namespace std;
mutex mtx;
bool endProgram=false;

const int numWoodcutter = 4;

int treeStatus=50;
int cutTrees=0;





void showStatus(){
    mtx.lock();
		
    mvprintw(4,5,"status -->    ",treeStatus);
    mvprintw(4,5,"status --> %d",treeStatus);

    mvprintw(6,5,"ilosc scietych -->     ",cutTrees);
    mvprintw(6,5,"ilosc scietych --> %d",cutTrees);
     
	refresh();
	
	mtx.unlock();
}

//dochodzi do zagłodzenia wątku dlatego drzewa nie rosną :(
    // las to jeden wątek, zaczyna od 50% i rośnie co 1%
    // jak drwal zetnie drzewo to maleje o 1% 



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
        mtx.lock();
        if(treeStatus>0)
        {
            treeStatus--;
            cutTrees++;
        }
        mtx.unlock();
        showStatus();
        for(int i=0;i<10;i++)
        {
            usleep(rand() %1000000+1000);
        }
    }
}




int main()
{
    initscr();
	nodelay(stdscr,TRUE);   
    refresh();

    srand(time(NULL));
    thread tree;
    tree = thread(startThreedTree);

    thread woodcutter[numWoodcutter];
    
    for (int i=0;i<numWoodcutter;i++)
    {
        woodcutter[i]=thread(startThreadWoodcutter, i);
    }
    
    for (int i=0;i<numWoodcutter;i++)
    {
        woodcutter[i].join();
    }

    tree.join();

    endwin();


  return 0;
}