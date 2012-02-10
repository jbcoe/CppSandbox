#include <iostream>
#include <future>
#include <vector>

class TaskCommon
{       
	public:
		thread_local static int instanceCount = 0;
		TaskCommon() { ++instanceCount; }
};
                            
int main(int argc, char* argv[])
{
	std::vector<std::future<int>> threadTaskCounts;

	threadTaskCounts.push_back(
			std::async([]()->int
			{
			TaskCommon a; 
			TaskCommon b; 
			TaskCommon c;
			return TaskCommon::instanceCount;
			}));

	threadTaskCounts.push_back(
			std::async([]()->int
			{
			TaskCommon a; 
			TaskCommon b; 
			return TaskCommon::instanceCount;
			}));

	for ( std::future<int>& result : threadTaskCounts )
	{
		static int i=0;
		std::cout << "Thread " << i++ << " used " << result.get() << " task objects" << std::endl;
	}
}

