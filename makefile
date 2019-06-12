prog: ledserver.o
	gcc -o prog ledserver.o -lpthread -lwiringPi
ledserver.o: ledserver.c
	gcc - c ledserver.c
