all: dz2

dz2: main.o class.o func.o
	g++ main.o class.o func.o -o dz2 -lfltk
	
main.o: main.cpp
	 g++ -c main.cpp
    
class.o: class.cpp
	 g++ -c class.cpp
    
func.o: func.cpp
	 g++ -c func.cpp
	 
clean:
	 rm -rf *.o dz2
