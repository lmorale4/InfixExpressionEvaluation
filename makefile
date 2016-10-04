executionProg.exe: infixEval.o
	g++ infixEval.o -o executionProg.exe
	
infixEval.o: infixEval.cpp
	g++ -c -std=c++11 infixEval.cpp
	
clean:
	rm *.o executionProg.exe