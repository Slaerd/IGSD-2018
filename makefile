all: clean projet

projet: Projet2018.o
	g++ -std=c++11 -I/usr/local Projet2018.o -lglfw  -lGLEW  -lGL  -oProjet2018

Projet2018.o: projet2018.cpp
	g++ -std=c++11 -I/usr/local/include/ -I/public/ig/glm/ -c projet2018.cpp  -oProjet2018.o

clean:
	rm -f *.o
