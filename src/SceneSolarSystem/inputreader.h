#include <thread>
#include <mutex>
#include <vector>
#include <queue>


struct command {
	int opcode;
	int len;
	char* buffer;
};

extern std::queue<command> cmd_q;
extern std::mutex m_cmd_q;

void readInput();