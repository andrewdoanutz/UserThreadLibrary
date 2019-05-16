all: test p3_grader


thread_lib:threads.cpp
	g++ -m64 -c threads.cpp -o threads.o


test:test.cpp thread_lib 
	g++ -m64 test.cpp -o test
 



p3_grader: autograder_main.c threads.o
	g++ -m64  autograder_main.c threads.o -o p3_grader

 clean:
	rm *.o sample_grader test p3_grader