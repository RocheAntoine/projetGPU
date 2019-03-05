CC=gcc -g -O2 -fopenmp
CXX=g++ -g -O2 -fopenmp
IOPENCV=-I/home/jmetancelin/coursTools/opencv-3.0.0/include
LOPENCV=-L/home/jmetancelin/coursTools/opencv-3.0.0/lib -lopencv_core -lopencv_highgui
MAINS=main

all: $(MAINS)

main: main.o solveur.o imageWriter.o
	$(CXX) $(LOPENCV) $^ -o $@


%.o: %.c
	$(CXX) -c $(IOPENCV) $< -o $@

clean:
	rm -f  $(MAINS) *.o *.png

clean-img:
	rm -f *.png
