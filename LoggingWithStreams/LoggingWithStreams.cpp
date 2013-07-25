#include <iostream>
#include <vector>

std::ostream* g_logStream = &std::cout;

enum class LEVEL
{
	INFO = 3,
	WARNING = 2,
	ERROR = 1,
	CRITICAL = 0
} g_logLevel = LEVEL::CRITICAL;

void SetLogLevel(LEVEL logLevel)
{
	g_logLevel = logLevel;
}

#define LOG(level) if(g_logLevel<level); else (*g_logStream)

int main(int argc, char* argv[])
{
	std::vector<LEVEL> logLevels = {LEVEL::INFO,LEVEL::WARNING,LEVEL::ERROR,LEVEL::CRITICAL};
	for ( auto level : logLevels )
	{
    SetLogLevel(level);

		LOG(LEVEL::INFO) << "It's getting warm" << std::endl;
		LOG(LEVEL::WARNING) << "It's getting dangerously hot" << std::endl;
		LOG(LEVEL::ERROR) << "I'm starting to singe a bit" << std::endl;
		LOG(LEVEL::CRITICAL) << "I am on fire!\n" << std::endl;
	}

	return 0;
}

