#Sebastian Thiems 2013
#
#RT Customz
#www.42Volt.de

clock: spi.o listener.o clock.o 
	g++ spi.o listener.o clock.o -o clock -L/usr/local/lib -lwiringPi -lboost_system -lboost_thread

spi.o: spi.cpp
	g++ -c spi.cpp -std=gnu++0x

listener.o: listener.cpp
	g++ -c listener.cpp -std=gnu++0x

clock.o: clock.cpp
	g++ -c clock.cpp -std=gnu++0x

clean:
	rm -rf *.o clock clock
