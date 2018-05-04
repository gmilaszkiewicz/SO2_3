#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>

using namespace std;
mutex mtx;

const int numTree = 10;
int treeStatus[numTree];
bool endProgram=false;
bool treeReady[numTree];

void showStatus(){
    mtx.lock();
	for (int i=0;i<numTree;i++)
	{
		mvprintw(i+4,5,"drzewo --> %d",treeStatus[i]);
		refresh();
	}
	mtx.unlock();
}

void startThreadTree(int tID){

    while(!endProgram)
	{
		if(getch()==27)
		{
			endProgram=true;
			break;
		}

showStatus();
    
    if(treeReady[tID]==false)
    {
         
        for(int i=1;i<=10;i++)
        {
            usleep(rand() %1000000+100000);
            treeStatus[tID]+=10;
           showStatus();
        }
        treeReady[tID]=true;
    }
    


    }
}






int main()
{
    initscr();
	nodelay(stdscr,TRUE);   
    refresh();

    srand(time(NULL));
    thread tree[numTree];

    for (int i = 0; i < numTree; i++)
	{
         treeReady[i]=false;
    }
    for (int i=0;i<5;i++)
    {
        treeReady[i]=true;
        treeStatus[i]=100;
    }
     

    for (int i = 0; i < numTree; i++)
	{
		tree[i] = thread(startThreadTree, i);
	}

    
    for (int i = 0; i < numTree; i++)
	{
		tree[i].join();
	}
    endwin();


  return 0;
}