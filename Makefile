all: dmra

dmra: main.cpp definition.cpp
	g++ -g -std=c++17 -o dmra main.cpp definition.cpp

clean:
	rm dmra