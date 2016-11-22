main: main.cpp Control.cpp Data.cpp
	g++ -g -std=c++11 main.cpp Control.cpp Data.cpp -o main

clean: 
	rm -rf input.txt *.o main 

commit: 
	git add .; git commit; git push; git status