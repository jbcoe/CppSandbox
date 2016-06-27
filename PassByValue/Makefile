all:
	clang++ -std=c++14 -O0 -D PASS_BY_REF -S -emit-llvm PassByValue.cpp && mv PassByValue.ll PassByValue.ref.O0.ll
	clang++ -std=c++14 -O0 -D PASS_BY_VALUE -S -emit-llvm PassByValue.cpp && mv PassByValue.ll PassByValue.val.O0.ll
	clang++ -std=c++14 -O3 -D PASS_BY_REF -S -emit-llvm PassByValue.cpp && mv PassByValue.ll PassByValue.ref.O3.ll
	clang++ -std=c++14 -O3 -D PASS_BY_VALUE -S -emit-llvm PassByValue.cpp && mv PassByValue.ll PassByValue.val.O3.ll

