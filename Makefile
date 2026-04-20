all: main1 main2 main3

main1: main1.c
	gcc -c main1.c
	gcc -o main1 main1.o

main2: main2.c
	gcc -c main2.c
	gcc -o main2 main2.o

main3: main3.c
	gcc -c main3.c
	gcc -o main3 main3.o

clean:
	rm -f main1
	rm -f main1.o
	rm -f main2
	rm -f main2.o
	rm -f main3
	rm -f main3.o
	rm -f out1.txt
	rm -f out2.txt
	rm -f out3.txt
