#include <iostream>

template <typename F_t>
class ScopeExitRunner
{
	F_t f;
	bool run;
public:
	ScopeExitRunner(F_t f_) : f(f_), run(true) {}
	~ScopeExitRunner() { if (run) f(); }
	void Release() { run=false; }
};

template <typename F_t>
ScopeExitRunner<F_t> run_on_scope_exit(F_t f)
{
	return ScopeExitRunner<F_t>(f);
}

int main(int argc, char* argv[])
{
	auto printHelloWorldOnExit = run_on_scope_exit([](){std::cout << "Hello world" << std::endl;});
	auto printGoodbyeWorldOnExit = run_on_scope_exit([](){std::cout << "Goodbye cruel world" << std::endl;});
	printGoodbyeWorldOnExit.Release();
}

