all: GRS ILS

GRS: GRASP.cpp ./tools/file.cpp
	g++ -o GRASP -O3 GRASP.cpp ./tools/file.cpp

ILS: ILS.cpp
	g++ -o ILS -O3 ILS.cpp ./tools/file.cpp
