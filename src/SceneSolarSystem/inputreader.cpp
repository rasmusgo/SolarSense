#include "inputreader.h"

std::queue<command> cmd_q;
std::mutex m_cmd_q;

bool readInput_;
std::thread inputThread;


void readInput(){
	readInput_ = true;
	inputThread = std::thread([](){
		   char c;
           while(readInput_){
           		scanf("%c", &c);
           		m_cmd_q.lock();
           		cmd_q.push(command{c});
           		m_cmd_q.unlock();
           }
        });


}
