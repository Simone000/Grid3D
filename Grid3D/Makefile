testlista: main.o 
	g++ main.o -o NomeEseguibile

main.o: main.cpp
	g++ -I./ -c main.cpp -o main.o

.PHONY: clean
clean:
	rm *.o NomeEseguibile
