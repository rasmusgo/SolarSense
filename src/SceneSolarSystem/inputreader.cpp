#include "inputreader.h"

std::queue<command> cmd_q;
std::mutex m_cmd_q;

bool readInput_;
std::thread inputThread;


void readInput(){
	readInput_ = true;
	inputThread = std::thread([](){
		   int opcode;
		   int count;
		   char *buf;
           while(readInput_){
           		scanf("%d %d", &opcode, &count);
           		buf = new char[count+10];
           		scanf("%[^\n]", buf);
           		m_cmd_q.lock();
           		cmd_q.push(command{opcode, count, buf});
           		m_cmd_q.unlock();
           }
        });


}
