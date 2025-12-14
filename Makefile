bin/testBSTreeDict: DIct.h BSTreeDict.h BSNode.h TableEntry.h testBSTreeDict.cpp 
	
	mkdir -p bin	
	g++ -o bin/testBSTreeDict testBSTreeDict.cpp

clean:
	rm -rf *.o *.gch bin
