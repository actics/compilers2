build:
	g++ llvmsqrt.cpp -o llvmsqrt `llvm-config --cppflags --ldflags --libs` -ldl 
	./llvmsqrt
	llc -disable-cfi -mattr=-avx llvmsqrt.bc
	g++ print.cpp -c -o print.o
	gcc print.o llvmsqrt.s -o llvmsqrt_b

