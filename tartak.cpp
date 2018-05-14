#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>

using namespace std;
mutex mtx;


const int numWoodcutter = 20;

int treeStatus=50;
bool endProgram=false;


void showStatus(){
    mtx.lock();
		
    mvprintw(4,5,"status -->    ",treeStatus);
    mvprintw(4,5,"status --> %d",treeStatus);
     
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
            usleep(100000);
            treeStatus++;
            showStatus();
        }

    }
    


}



// void startThreadWoodcutter(int tID){
//     while(1)
// 	{
        
//             mtx.lock();
//             if(treeReady[i]==true)
//             {
//                 treeStatus[i]=0;
//                 treeReady[i]=false;
//             }
            
//             mtx.unlock();
//             clear();
//             showStatus();
//             for(int i=1;i<=10;i++)
//             {
//                 usleep(rand() %1000+1000);
//             }
            
        

//     }

// }




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
    //    woodcutter[i]=thread(startThreadWoodcutter, i);
    }
    
    for (int i=0;i<numWoodcutter;i++)
    {
       // woodcutter[i].join();
    }

    tree.join();

    endwin();


  return 0;
}