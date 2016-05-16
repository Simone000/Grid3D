#include <iostream>
#include <string> //per usare le stringhe
#include "grid3d.h"
#include <algorithm>
using namespace std; //da usare SOLO nei .cpp


//709441 Simone Invernizzi
int main()
{
	Grid3d<double> prova(10, 9, 8);

	prova(1,2,3)=88;
	Grid3d<int> prova2 = prova; //costruttore di conversione templato
	cout << prova2(1,2,3) << endl;

	prova2(5, 5, 5) = 7;
	cout << prova2(5, 5, 5) << endl;

	prova2(5, 5, 5) = 3;	//funtore()
	cout << prova2(5, 5, 5) << endl;
	prova2(5, 5, 5) = prova2(2, 2, 2); //funtore()
	cout << prova2(5, 5, 5) << endl;


	prova2(2, 2, 2) = 77;
	prova=prova2; //operatore di assegnamento
	cout << prova2(2, 2, 2) << endl;
	cout << prova(2, 2, 2) << endl;

	prova(1,1,1) = 5;
	Grid3d<double> prova3(prova); //copy constructor
	cout << prova3(1, 1, 1) << endl;

	
	//provo gli operatori
	prova(4,4,4) = prova(1,1,1) + prova(2,2,2);
	cout << "somma " << prova(4,4,4) << endl;
	prova(4,4,4) += 1;
	cout << "somma+1 " << prova(4,4,4) << endl;
	prova(4,4,4) += prova(1,1,1);
	cout << "somma+5 " << prova(4,4,4) << endl;


	//evaluate con i cognomi
	Grid3d<NomeCognome> prova5(10, 10, 10);
	NomeCognome a = {"paolo", "rossi"};
	NomeCognome b = {"pablo", "escobar"};
	prova5(7,7,7) = a;
	prova5(3,3,3) = b;
	StartWith cerco = {"escobar"}; //il cognome che cerco
	Grid3d<bool> evaluata = evaluate(prova5, cerco);
	cout << evaluata(7,7,7) << endl;
	cout << evaluata(3,3,3) << endl;


	//evaluate con intervalli
	Grid3d<double> prova4(10, 10, 10);
	prova4(7,7,7) = 5;
	prova4(3,3,3) = 15;
	IsInRange<int> cerco2 = {10, 20}; //l'intervallo che cerco
	Grid3d<bool> evaluata2 = evaluate(prova4, cerco2);
	cout << evaluata2(7,7,7) << endl;
	cout << evaluata2(3,3,3) << endl;


	//operatore di stream
	Grid3d<int> prova6(3,3,3);
	prova6(0,1,2) = 5;
	prova6(2,1,0) = 3;
	prova6(2,1,2) = 4;
	cout << prova6 << endl;


	
	Grid3d<int> prova7(3,3,3);
	//iterator
	Grid3d<int>::iterator i = prova7.begin();
	Grid3d<int>::iterator ie = prova7.end();

	prova7(0,0,0) = 5;
	cout << *i << endl; //operatore di deferenziamento(read)
	*i = 22; //operatore di deferenziamento(write)
	cout << "iter " << *i << " val " << prova7(0,0,0) << endl;

	Grid3d<int>::iterator i2;
	i2 = i;	//operatore di assegnamento
	cout << *i2 << endl;

	Grid3d<int>::iterator i3 = i; //costruttore di copia
	cout << *i3 << endl;


	//operatore di preincremento
	prova7(2,2,2) = 7;
	++++++++++++++++++++++++++++++++++++++++++++++++++++++i3;
	cout << *i3 << endl;

	//operatore di postincremento
	prova7(0,0,1) = 3;
	i2++++++++++++++++++++++++++++++++++++++++++++++++++++++;
	cout << *i2 << endl;

	
	//op. +
	i2 + 5;
	cout << "somma it+val " << *i2 << endl;
	i2 + i3;
	cout << "somma it+it " << *i2 << endl;

	//op. -
	i2 - i3;
	cout << "sottrazione it+it " << *i2 << endl;
	i2 - 5;
	cout << "sottrazione it+val " << *i2 << endl;

	//op. +=
	i2 += 5;
	cout << "+= " << *i2 << endl;
	i2 += i3;
	cout << "+= " << *i2 << endl;

	//op. -=
	i2 -= i3;
	cout << "-= " << *i2 << endl;
	i2 -= 5;
	cout << "-= " << *i2 << endl;

	//op. < > <= =>
	bool x = i2 < 10;
	cout << "*i2 < 10 ? " << x << endl;
	x = i2 < 2;
	cout << "*i2 < 2 ? " << x << endl;
	*i3 = 10;
	x = i2 < i3;
	cout << "*i2 < 10 ? " << x << endl;
	*i3 = 2;
	x = i2 < 2;
	cout << "*i2 < 2 ? " << x << endl;
	x = i2 > 5;
	x = i2 > i3;
	x = i2 >= 10;
	x = i2 <= i3;

	//non lo riempio
	prova7(2,2,2) = 99;
	cout << "stampo tutto iterator:" << endl;
	Grid3d<int>::iterator inizio, fine;
	for(inizio = prova7.begin(), fine = prova7.end(); inizio != fine; ++inizio)
		cout << *inizio << " " << endl;
	cout << *inizio << endl; //ultimo elemento

	//const_iterator
	Grid3d<int> prova8(3,3,3);
	Grid3d<int>::const_iterator i4 = prova8.begin();
	prova8(0,0,0) = 77;
	cout << *i4 << endl;

	//lo riempio
	fill(prova8.begin(), prova8.end(), 33); //il mio ultimo elemento non è null
	*prova8.end() = 33; //assegno anche l'ultimo elemento
	cout << "stampo tutto const_iterator:" << endl;
	Grid3d<int>::iterator inizio2 = prova8.begin(), fine2 = prova8.end();
	for(inizio2 = prova8.begin(), fine2 = prova8.end(); inizio2 != fine2; ++inizio2)
		cout << *inizio2 << " " << endl;
	cout << *inizio2 << endl; //ultimo elemento


	Grid3d<int>::iterator i5 = prova8.begin();
	cout << *i5 << endl;
	Grid3d<int>::const_iterator i6 = i5; //costruttore di conversione da iterator a const_iterator
	cout << *i6 << endl;
	Grid3d<int>::const_iterator i7 = i6; //costruttore di copia
	cout << *i7 << endl;

	return 0;
}
