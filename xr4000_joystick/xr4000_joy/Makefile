xr4000run: xr4000.o serialib.o galilcontroller.o
	g++ xr4000.o serialib.o galilcontroller.o -lgclib -lgclibo -o xr4000run 

galilcontroller.o: galilcontroller.cpp galilcontroller.h
	g++ -c galilcontroller.cpp

xr4000.o: xr4000.cpp
	g++ -c xr4000.cpp

serialib.o: serialib.cpp serialib.h
	g++ -c serialib.cpp 
clean:
	rm *.o xr4000run


