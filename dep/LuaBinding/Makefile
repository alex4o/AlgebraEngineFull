
LIB=libgen.so
BIN=run

all:
	g++ main.cpp -Wl,-rpath=./lib/ -std=c++11 -lAlgebraEngine -I../AlgebraEngine/ -I/usr/local/include/luajit-2.0/ -lluajit-5.1 -o $(BIN)
	g++ -shared -fPIC main.cpp -Wl,-rpath=./lib/ -std=c++11 -lAlgebraEngine -I../AlgebraEngine/ -I/usr/local/include/luajit-2.0/ -lluajit-5.1 -o $(LIB)

install:
	cp -v $(LIB) /lib
	cp -v $(LIB) /lib64
