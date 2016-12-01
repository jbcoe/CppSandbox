all:
	clang++ -std=c++14 -O0 -D VALUE_SIZE=0 -S -emit-llvm PassByValue.cpp && mv PassByValue.ll PassByValue.ref.O0.ll
	clang++ -std=c++14 -O0 -S -emit-llvm PassByValue.cpp && mv PassByValue.ll PassByValue.val.O0.ll
	clang++ -std=c++14 -O3 -D VALUE_SIZE=0 -S -emit-llvm PassByValue.cpp && mv PassByValue.ll PassByValue.ref.O3.ll
	clang++ -std=c++14 -O3 -S -emit-llvm PassByValue.cpp && mv PassByValue.ll PassByValue.val.O3.ll
	clang++ -std=c++14 -O0 -D VALUE_SIZE=0 -S PassByValue.cpp && mv PassByValue.S PassByValue.ref.O0.S
	clang++ -std=c++14 -O0 -S PassByValue.cpp && mv PassByValue.S PassByValue.val.O0.S
	clang++ -std=c++14 -O3 -D VALUE_SIZE=0 -S PassByValue.cpp && mv PassByValue.S PassByValue.ref.O3.S
	clang++ -std=c++14 -O3 -S PassByValue.cpp && mv PassByValue.S PassByValue.val.O3.S

