all: GRS ILS

GRS: GRASP.cpp ./tools/file.cpp
	g++ -o GRASP GRASP.cpp ./tools/file.cpp

ILS: ILS.cpp
	g++ -o ILS ILS.cpp ./tools/file.cpp