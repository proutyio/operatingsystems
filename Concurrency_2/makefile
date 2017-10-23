all: compile

compile: Dining_Philosophers.c mt19937ar.c
	gcc -o Dining_Philosophers Dining_Philosophers.c -pthread -std=c99

run:
	./Dining_Philosophers

clean:
	rm -f Dining_Philosophers *~
