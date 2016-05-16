#ifndef grid3d_h
#define grid3d_h
#include <iostream>
#include <cassert>
#include <string>

//709441 Simone Invernizzi

/** 
 * la classe generica gestisce un cubo3d
 * le dimensioni sono fissate all'inizio e non possono variare una volta fissate
 * utilizzo un array di array di array completamente dinamico
*/

template <typename T>
class Grid3d{

public:
	typedef unsigned int size_type;
	typedef T value_type;

	/**
 	 * Construttore (non mi serve quello di default dato che non cambierò le dimensioni)
 	 * @param alt altezza dell'array
	 * @param largh larghezza dell'array
	 * @param prof profondita dell'array
 	 */
	Grid3d(const size_type alt, const size_type largh, const size_type prof) : _altezza(alt), _larghezza(largh), _profondita(prof){
		_ptr = new value_type** [alt];
		for(size_type i=0; i<alt; ++i){
			_ptr[i] = new value_type* [largh];
			for(size_type j=0; j<largh; ++j)
				_ptr[i][j] = new value_type [prof];
		}
	}

	/**
	 *distruttore
	*/
	~Grid3d(void){
		for(size_type i=0; i<_altezza; ++i){
			for(size_type j=0; j<_larghezza; ++j)
				delete[] _ptr[i][j];
			delete[] _ptr[i];
		}
		delete[] _ptr;
	}

	/**
	 * operatore di assegnamento
	 * @param rhs operando di destra
	 */
	Grid3d<value_type> &operator=(const Grid3d<value_type> &rhs){
		if(this != &rhs){
			value_type*** _tmp = 0; //di supporto

			_tmp = new value_type** [rhs.altezza()];
			for(size_type i=0; i<rhs.altezza(); ++i){
				_tmp[i] = new value_type* [rhs.larghezza()];
				for(size_type j=0; j<rhs.larghezza(); ++j)
					_tmp[i][j] = new value_type [rhs.profondita()];
			}

			try{
				for(size_type i=0; i<rhs.altezza(); ++i)
					for(size_type j=0; j<rhs.larghezza(); ++j)
						for(size_type y=0; y<rhs.profondita(); ++y)
							_tmp[i][j][y] = rhs._ptr[i][j][y];
			}
			catch(...) {
				for(size_type i=0; i<_altezza; ++i){
					for(size_type j=0; j<_larghezza; ++j)
						delete[] _tmp[i][j];
					delete[] _tmp[i];
				}
				delete[] _tmp;

  				throw;
			}

			//andato tutto bene
			for(size_type i=0; i<_altezza; ++i){
				for(size_type j=0; j<_larghezza; ++j)
					delete[] _ptr[i][j];
				delete[] _ptr[i];
			}
			delete[] _ptr;

			_ptr = _tmp;
  			_altezza = rhs.altezza();
			_larghezza = rhs.larghezza();
			_profondita = rhs.profondita();
		}
		return *this;
	}

	/**
	 * copy constructor(chiamato nel passaggio per valore di parametri)
	 * @param other operando di destra
	 */
	Grid3d(const Grid3d<value_type> &other){
		_ptr=0;
		_altezza=0;
		_larghezza=0;
		_profondita=0;
		if(other.altezza()>0 && other.larghezza()>0 && other.profondita()>0){
			//assegno memoria
			_ptr = new value_type** [other.altezza()];
			for(size_type i=0; i<other.altezza(); ++i){
				_ptr[i] = new value_type* [other.larghezza()];
				for(size_type j=0; j<other.larghezza(); ++j)
					_ptr[i][j] = new value_type [other.profondita()];
			}
			_altezza=other.altezza();
			_larghezza=other.larghezza();
			_profondita=other.profondita();

			//assegno valori
			try{
				for(size_type i=0; i<_altezza; ++i)
					for(size_type j=0; j<_larghezza; ++j)
						for(size_type y=0; y<_profondita; ++y)
							_ptr[i][j][y] = other.get_value(i,j,y);
			}
			catch(...){
				for(size_type i=0; i<_altezza; ++i){
					for(size_type j=0; j<_larghezza; ++j)
						delete[] _ptr[i][j];
					delete[] _ptr[i];
				}
				delete[] _ptr;
				
				throw;
			}
		}
		
	}

	/**
	 * costruttore di conversione templato(se U è castabile a T)
	 * @param other Grid3d da convertire
	 */
	template <typename U>
	Grid3d(const Grid3d<U> &other){
		_ptr=0;
		_altezza=0;
		_larghezza=0;
		_profondita=0;

		if(other.altezza()>0 && other.larghezza()>0 && other.profondita()>0){
			//assegno memoria
			_ptr = new value_type** [other.altezza()];
			for(size_type i=0; i<other.altezza(); ++i){
				_ptr[i] = new value_type* [other.larghezza()];
				for(size_type j=0; j<other.larghezza(); ++j)
					_ptr[i][j] = new value_type [other.profondita()];
			}
			_altezza=other.altezza();
			_larghezza=other.larghezza();
			_profondita=other.profondita();

			//assegno valori
			try{
				for(size_type i=0; i<_altezza; ++i)
					for(size_type j=0; j<_larghezza; ++j)
						for(size_type y=0; y<_profondita; ++y)
							_ptr[i][j][y] = static_cast<value_type>(other.get_value(i,j,y));
			}
			catch(...){
				for(size_type i=0; i<_altezza; ++i){
					for(size_type j=0; j<_larghezza; ++j)
						delete[] _ptr[i][j];
					delete[] _ptr[i];
				}
				delete[] _ptr;
				
				throw;
			}
		}
	}


	/**
	* @return altezza dell'array
	*/
	inline const size_type &altezza() const{ return _altezza;}
	/**
	* @return larghezza dell'array
	*/
	inline const size_type &larghezza() const{return _larghezza;}
	/**
	* @return profondita dell'array
	*/
	inline const size_type &profondita() const{return _profondita;}

	/**
	* @param y altezza dell'elemento
	* @param x larghezza dell'elemento
	* @param z profondita dell'elemento
	* @return il dato alla posizione
	*/
	const value_type &get_value(const size_type y, const size_type x, const size_type z) const {
  		assert(y >= 0);
		assert(x >= 0);
		assert(z >= 0);
  		assert(y < _altezza);
		assert(x < _larghezza);
		assert(z < _profondita);
  
  		return _ptr[y][x][z];
	}

	/**
	* funtore
	* @param y coordinata y
	* @param x coordinata x
	* @param z coordinata z
	* @return dato nella coordinata
	*/
	value_type &operator()(const size_type y, const size_type x, const size_type z){
		return _ptr[y][x][z];
	}


	/**
	*uso random access iterator:
	*non implemento [] perkè ho il funtore che lo fa gia e dovrei implementare una versione con 3 parametri
	*read/write: *i, i->
	*iteration: ++, --, +, -, +=, -=
	*comparison: ==, !=, <, >, <=, >=
	*/
	class const_iterator; // forward declaration

	//read/write
	class iterator{
		friend class const_iterator;
		friend class Grid3d;
	public:
		//traits per renderlo usabile con la STL
		typedef std::random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;

		//costruttore di default
		iterator() : ptr(0){}

		//costruttore di copia
		iterator(const iterator &other) : ptr(other.ptr), altezza(other.altezza), larghezza(other.larghezza), profondita(other.profondita),
								posY(other.posY), posX(other.posX), posZ(other.posZ) {}

		
		iterator& operator= (const iterator &other){
			if(this != &other){
				ptr = other.ptr;
				altezza = other.altezza;
				larghezza = other.larghezza;
				profondita = other.profondita;
				posY = other.posY;
				posX = other.posX;
				posZ = other.posZ;
			}
			return *this;
		}

		~iterator(){}

		/**
		* Operatore di dereferenziamento(read/write)
 		* 
 		* @return il dato "puntato" dall'iteratore.
 		*/
		value_type &operator*() const{
			return ptr[posY][posX][posZ];
		}

		/**
 		* Operatore freccia(read/write)
 		* 
 		* @return il puntatore al dato "puntato" dall'iteratore.
 		*/
		value_type *operator->() const {
			return &ptr[posY][posX][posZ];
		}

		/**
 		* Operatore di "elemento successivo". Versione pre-incremento. 
 		* 
 		* @return l'iteratore che "punta" all'elemento successivo
 		*/
		iterator &operator++(){
			if(posZ < profondita-1)
				++posZ;
			else if(posX < larghezza-1){
				++posX;
				posZ = 0;
			}
			else if(posY < altezza-1){
				++posY;
				posZ = 0;
				posX = 0;
			}
			return *this;
		}
		/**
 		* Operatore di "elemento successivo". Versione post-incremento. 
 		* 
 		* @return l'iteratore che "punta" all'elemento successivo
 		*/
		iterator operator++(int){
			iterator tmp(ptr, altezza, larghezza, profondita, posY, posX, posZ);
			if(posZ < profondita-1)
				++posZ;
			else if(posX < larghezza-1){
				++posX;
				posZ = 0;
			}
			else if(posY < altezza-1){
				++posY;
				posZ = 0;
				posX = 0;
			}
			return tmp;
		}

		/**
 		* Operatore di confronto (uguaglianza). 
 		* 
 		* @param other iteratore da confrontare
 		* @return true se i due iteratori "puntano" allo stesso dato
 		*/
		bool operator==(const iterator &other) const{
			return ptr == other.ptr && posY==other.posY && posX==other.posX && posZ==other.posZ;
		}
		/**
 		* Operatore di confronto (uguaglianza). 
 		* 
 		* @param other iteratore da confrontare
 		* @return true se i due iteratori "puntano" a dati diversi
 		*/
		bool operator!=(const iterator &other) const{
			return !(*this == other);
		}
		/**
 		* Operatore di confronto (uguaglianza). 
 		* 
 		* @param other const_iteratore da confrontare
 		* @return true se i due const_iteratori "puntano" allo stesso dato
 		*/
		bool operator==(const const_iterator &other) const {
			return ptr == other.ptr && posY==other.posY && posX==other.posX && posZ==other.posZ;
		}
		/**
 		* Operatore di confronto (uguaglianza). 
 		* 
 		* @param other const_iteratore da confrontare
 		* @return true se i due const_iteratori "puntano" a dati diversi
 		*/
		bool operator!=(const const_iterator &other) const {
			return !(*this == other);
		}

		/**
 		* Operatore <
 		* 
 		* @param other valore da confrontare
 		* @return true se il dato puntato da iterator è < del valore passato
 		*/
		bool operator<(const value_type other) const{
			return ptr[posY][posX][posZ] < other;
		}
		/**
 		* Operatore <
 		* 
 		* @param other iterator da confrontare
 		* @return true se il dato puntato da iterator è < del dato puntato dall'iterator passato
 		*/
		bool operator<(const iterator &other) const{
			return ptr[posY][posX][posZ] < other.ptr[other.posY][other.posX][other.posZ];
		}

		/**
 		* Operatore <=
 		* 
 		* @param other valore da confrontare
 		* @return true se il dato puntato da iterator è < del valore passato
 		*/
		bool operator<=(const value_type other) const{
			return ptr[posY][posX][posZ] <= other;
		}
		/**
 		* Operatore <=
 		* 
 		* @param other valore da confrontare
 		* @return true se il dato puntato da iterator è < del dato puntato dall'iterator passato
 		*/
		bool operator<=(const iterator &other) const{
			return ptr[posY][posX][posZ] <= other.ptr[other.posY][other.posX][other.posZ];
		}

		/**
 		* Operatore >
 		* 
 		* @param other valore da confrontare
 		* @return true se il dato puntato da iterator è > del valore passato
 		*/
		bool operator>(const value_type other) const{
			return ptr[posY][posX][posZ] > other;
		}
		/**
 		* Operatore >
 		* 
 		* @param other valore da confrontare
 		* @return true se il dato puntato da iterator è > del dato puntato dall'iterator passato
 		*/
		bool operator>(const iterator &other) const{
			return ptr[posY][posX][posZ] > other.ptr[other.posY][other.posX][other.posZ];
		}
		
		/**
 		* Operatore >=
 		* 
 		* @param other valore da confrontare
 		* @return true se il dato puntato da iterator è >= del valore passato
 		*/
		bool operator>=(const value_type other) const{
			return ptr[posY][posX][posZ] >= other;
		}
		/**
 		* Operatore >=
 		* 
 		* @param other valore da confrontare
 		* @return true se il dato puntato da iterator è >= del dato puntato dall'iterator passato
 		*/
		bool operator>=(const iterator &other) const{
			return ptr[posY][posX][posZ] >= other.ptr[other.posY][other.posX][other.posZ];
		}

		/**
 		* Operatore +
 		* 
 		* @param other valore da sommare
 		*/
		iterator& operator+ (const value_type other){
			ptr[posY][posX][posZ] += other;
			return *this;
		}
		/**
 		* Operatore +
 		* 
 		* @param other iterator che punta al valore da sommare
 		*/
		iterator& operator+ (const iterator &other){
			ptr[posY][posX][posZ] += other.ptr[other.posY][other.posX][other.posZ];
			return *this;
		}

		/**
 		* Operatore -
 		* 
 		* @param other valore da sottrarre
 		*/
		iterator& operator- (const value_type other){
			ptr[posY][posX][posZ] -= other;
			return *this;
		}
		/**
 		* Operatore +
 		* 
 		* @param other iterator che punta al valore da sottrarre
 		*/
		iterator& operator- (const iterator &other){
			ptr[posY][posX][posZ] -= other.ptr[other.posY][other.posX][other.posZ];
			return *this;
		}

		/**
 		* Operatore +=
 		* 
 		* @param other valore da sommare
 		*/
		iterator& operator+= (const value_type other){
			ptr[posY][posX][posZ] += other;
			return *this;
		}
		/**
 		* Operatore +
 		* 
 		* @param other iteratore che punta al valore da sommare
 		*/
		iterator& operator+= (const iterator &other){
			ptr[posY][posX][posZ] += other.ptr[other.posY][other.posX][other.posZ];
			return *this;
		}

		/**
 		* Operatore -=
 		* 
 		* @param other valore da sottrarre
 		*/
		iterator& operator-= (const value_type other){
			ptr[posY][posX][posZ] -= other;
			return *this;
		}
		/**
 		* Operatore -=
 		* 
 		* @param other iteratore che punta al valore da sottrarre
 		*/
		iterator& operator-= (const iterator &other){
			ptr[posY][posX][posZ] -= other.ptr[other.posY][other.posX][other.posZ];
			return *this;
		}
	

	private:
		value_type ***ptr;
		size_type altezza, larghezza, profondita, posY, posX, posZ; //le pos mi danno la posizione corrente

		/**
 		* Costruttore di inizializzazione. Sarà usato anche da myarrayT.
 		* 
 		* @param p puntatore ai dati 
		* @param y altezza del cubo
		* @param x larghezza del cubo 
		* @param z profondita del cubo
		* @param pY altezza attuale
		* @param pX larghezza attuale 
		* @param pZ profondita attuale
 		*/
		explicit iterator(value_type ***p, size_type y, size_type x, size_type z, size_type pY, size_type pX, size_type pZ) :
					ptr(p), altezza(y), larghezza(x), profondita(z), posY(pY), posX(pX), posZ(pZ){}
	};

	//SOLO read
	class const_iterator{
		friend class iterator;
		friend class Grid3d;
	public:
		//traits per renderlo usabile con la STL
		typedef std::random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;


		//costruttore di default
		const_iterator() : ptr(0), altezza(0), larghezza(0), profondita(0){}


		//costruttore di copia
		const_iterator(const const_iterator &other) : ptr(other.ptr), altezza(other.altezza), larghezza(other.larghezza), profondita(other.profondita),
								posY(other.posY), posX(other.posX), posZ(other.posZ) {}

		
		const_iterator& operator= (const const_iterator &other){
			if(this != &other){
				ptr = other.ptr;
				altezza = other.altezza;
				larghezza = other.larghezza;
				profondita = other.profondita;
				posY = other.posY;
				posX = other.posX;
				posZ = other.posZ;
			}
			return *this;
		}


		/**
 		* Costruttore di conversione. 
 		* No explicit perchè vogliamo permettere le conversioni di tipo implicite
 		* 
 		* @param other iterator da convertire
 		*/
		const_iterator(const iterator &other) : ptr(other.ptr), altezza(other.altezza), larghezza(other.larghezza), profondita(other.profondita),
								posY(other.posY), posX(other.posX), posZ(other.posZ){}

		~const_iterator(){}

		//op. di deferenziamento(ritorna il dato), read
		const value_type &operator*() const{
			return ptr[posY][posX][posZ];
		}

		//op. di freccia(ritorna il puntatore al dato), read
		const value_type *operator->() const {
			return &ptr[posY][posX][posZ];
		}

		//op. ++(preincremento)
		const_iterator &operator++(){
			if(posZ < profondita-1)
				++posZ;
			else if(posX < larghezza-1){
				++posX;
				posZ = 0;
			}
			else if(posY < altezza-1){
				++posY;
				posZ = 0;
				posX = 0;
			}
			return *this;
		}
		//postincremento
		const_iterator operator++(int){
			const_iterator tmp(ptr, altezza, larghezza, profondita, posY, posX, posZ);
			if(posZ < profondita-1)
				++posZ;
			else if(posX < larghezza-1){
				++posX;
				posZ = 0;
			}
			else if(posY < altezza-1){
				++posY;
				posZ = 0;
				posX = 0;
			}
			return tmp;
		}

		//op. di confronto(true se puntano allo stesso dato)
		bool operator==(const const_iterator &other) const{
			return ptr == other.ptr && posY==other.posY && posX==other.posX && posZ==other.posZ;
		}
		bool operator!=(const const_iterator &other) const{
			return !(*this == other);
		}

		//op. <
		bool operator<(const value_type other) const{
			return ptr[posY][posX][posZ] < other;
		}
		bool operator<(const const_iterator &other) const{
			return ptr[posY][posX][posZ] < other.ptr[other.posY][other.posX][other.posZ];
		}

		//op. <=
		bool operator<=(const value_type other) const{
			return ptr[posY][posX][posZ] <= other;
		}
		bool operator<=(const const_iterator &other) const{
			return ptr[posY][posX][posZ] <= other.ptr[other.posY][other.posX][other.posZ];
		}

		//op. >
		bool operator>(const value_type other) const{
			return ptr[posY][posX][posZ] > other;
		}
		bool operator>(const const_iterator &other) const{
			return ptr[posY][posX][posZ] > other.ptr[other.posY][other.posX][other.posZ];
		}
		
		//op. >=
		bool operator>=(const value_type other) const{
			return ptr[posY][posX][posZ] >= other;
		}
		bool operator>=(const const_iterator &other) const{
			return ptr[posY][posX][posZ] >= other.ptr[other.posY][other.posX][other.posZ];
		}
			

	private:
		value_type ***ptr;
		const size_type altezza, larghezza, profondita;
		size_type posY, posX, posZ; //le pos mi danno la posizione corrente

		//costruttore di inizializzazione
		explicit const_iterator(value_type ***p, const size_type y, const size_type x, const size_type z, size_type pY, size_type pX, size_type pZ) :
					ptr(p), altezza(y), larghezza(x), profondita(z), posY(pY), posX(pX), posZ(pZ){}
	};


	iterator begin(){
		return iterator(_ptr, _altezza, _larghezza, _profondita, 0, 0, 0);
	}
	iterator end(){
		return iterator(_ptr, _altezza, _larghezza, _profondita, _altezza-1, _larghezza-1, _profondita-1);
	}
	const_iterator begin() const{
		return const_iterator(_ptr, _altezza, _larghezza, _profondita, 0, 0, 0);
	}
	const_iterator end() const{
		return const_iterator(_ptr, _altezza, _larghezza, _profondita, _altezza-1, _larghezza-1, _profondita-1);
	}

private:
	size_type _altezza, _larghezza, _profondita;
	value_type ***_ptr;
};






//operatore di stream
template<typename T>
std::ostream &operator<<(std::ostream &os, const Grid3d<T>& a) {
  
	for(int i=0; i<a.altezza(); ++i)
		for(int j=0; j<a.larghezza(); ++j)
			for(int y=0; y<a.profondita(); ++y)
				std::cout<<a.get_value(i, j, y)<<" ";
	return os;
}




struct NomeCognome{
	std::string nome, cognome;
};
struct StartWith{
	std::string cognome;
	bool operator()(const NomeCognome &x){
		if(x.cognome == cognome)
			return true;
		return false;
	}
};

template<typename T>
struct Intervallo{
	T a, b;
};
template<typename T>
struct IsInRange{
	Intervallo<T> intervallo;
	bool operator()(const T &x){
		if(x>=intervallo.a && x<=intervallo.b)
			return true;
		return false;
	}
};

template<typename T, typename P>
Grid3d<bool> evaluate(const Grid3d<T> A, P &cmp){
	Grid3d<bool> B(A.altezza(), A.larghezza(), A.profondita()); //da returnare
		
	for(int i=0; i<B.altezza(); ++i)
		for(int j=0; j<B.larghezza(); ++j)
			for(int y=0; y<B.profondita(); ++y){
				if(cmp(A.get_value(i, j, y)))
					B(i, j, y) = true;
				else
					B(i, j, y) = false;
			}
	return B;
}



#endif
