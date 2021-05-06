O2:
	g++ -O2 -g -DNDEBUG main.cpp /home/tndnrd/Projects/Education-MIPT/Onegin/foperations.cpp HashTable.cpp List.cpp hashfuncs.cpp HashTableMemoryManager.cpp parse.cpp -msse4 -mavx2 -o run
pg:
	g++ -pg main.cpp -g -O2 -DNDEBUG /home/tndnrd/Projects/Education-MIPT/Onegin/foperations.cpp HashTable.cpp List.cpp hashfuncs.cpp HashTableMemoryManager.cpp parse.cpp -msse4 -o run
	cat test.txt | ./run > /dev/null
	cat test.txt | gprof ./run > profile
	gprof2dot ./profile | dot -Tsvg -o output.svg
O0:
	g++ main.cpp -O0 /home/tndnrd/Projects/Education-MIPT/Onegin/foperations.cpp HashTable.cpp List.cpp parse.cpp hashfuncs.cpp -o run

csv:
	g++ csv_gen.cpp -O2 /home/tndnrd/Projects/Education-MIPT/Onegin/foperations.cpp HashTable.cpp List.cpp parse.cpp hashfuncs.cpp HashTableMemoryManager.cpp -o csv
perf:
	rm -f callgrind.out.*
	valgrind --tool=callgrind ./run
acsv:
	cd Datasets/ded-dict/;   time python ded-dict-to-ACSV.py
	cd Datasets/400k-words/; time python 400k-words-to-ACSV.py

