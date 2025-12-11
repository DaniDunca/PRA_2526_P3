bin/testHashTable: DIct.h HashTable.h TableEntry.h testHashTable.cpp
	 mkdir -p bin
	 g++ -o bin/testHashTable testHashTable.cpp

clean:
	rm -rf *.o *.gch bin
