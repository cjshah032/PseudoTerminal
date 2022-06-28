complete:
	gcc -c signalHandlers.c 
	gcc -c utils.c
	gcc -o shell shell.c signalHandlers.o utils.o
	rm signalHandlers.o 
	rm utils.o