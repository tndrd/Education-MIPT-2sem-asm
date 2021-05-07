
# Hash table
This is the project of the second semester of "Computer Architecture" held in MIPT. The task includes hash table implementation with subsequent performance analysis and optimization. The entire code is written in C++, excluding single Jupyter notebook and several Python scripts for plotting and data formatting.

Throughout the article the hash table's performance was boosted by ***394%*** with optimization level being ```-O2```.  
	      
## Contents

* [Before optimization](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/README.md#before-optimization)
  * Initial implementation overview
  * Selecting hash-function and hash table size
  * Performance test program
  * Compilation and execution
  * Profiling software
* [First optimization](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/README.md#first-optimization)
  * Analysis
  * Using *crc32* intrinsic
  *  Results
* [Second optimization](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/README.md#second-optimization)
  * Analysis
  * Using local list containers
  *  Results
* [Third optimization](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/README.md#third-optimization)
  * Analysis
  * Transforming datasets' files
  * Using SSE intrinsics to boost *crc32* and *strncmp* functions
  * Results
* Conclusion 

## Before optimization

Hash table is a data structure that can map keys to values. The main idea is that each key is hashed to get an index which points to its corresponding value. This trick provides faster key-value access in comparison with some obvious implementations like searching for a specific key in an array. However, we're not protected from hash collisions, because there may be two keys that have the same hash. There are several methods of collision resolution, the one I've implemented is called separate chaining with linked lists. The scheme below represents the hash table's structure: 

![hash table scheme](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/scheme.png)

The ```HashTable``` structure refers to an array of linked list headers (```list_buffer```) which length equals ```size```. Each list in this array links all the key-value pairs whose keys have the same hash. All the elements of all the lists are stored in buffers, that are allocated by ```HashTableMemoryManager``` class. The ```current_buffer``` is a buffer that is used right now. The field ```n_elements``` represents number of elements in the table.
The ```hash_func``` is a pointer to hash function, which is specified as a parameter when we call the constructor.

Every time we want to add another pair, we hash the desired key. Then we get the reminder of hash by ```size``` division. This would be the index in the ```list_buffer``` array that points to the linked list that corresponds to our key. Then we add our new pair to the end of this linked list by writing it into free cell in ```current_buffer``` and chaining it with this list. If ```current_buffer``` is full, ```HashTableMemoryManager``` allocates new buffer and stores the old one for the purpose of future freeing.

Every time we want to find the value by key, we search for this key in corresponding list by passing through its elements and comparing our key with the ones in the list. 
This fact leads to conclusion that the quantity of elements in each list influences access time: the more elements we have, the more key comparisons we should do.

Hash table v1 source code can be found [here](https://github.com/tndrd/Education-MIPT-2sem-asm/tree/main/HashTable/HashTable_v1).

### Selecting hash-function and hash table size
To improve hash table performance we have to investigate the collision distribution. Let's fill our hash table with [test dictionary dataset](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/Datasets/Raw/ded-dict.txt), using different hash functions. We will use this dataset throughout the article. Also, we will perform the conversions of the raw dataset files into CSV ones.
An average list size will represent an average access performance. The distribution of list sizes' uniformity will represent the access stability (this means that accesses' to different lists performances are comparable).

We will use [this program](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/HashTable_v1/csv_gen.cpp) to fill the tables and save their distributions into CSV files. Then we will plot them using [this notebook](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/HashTable_v1/HT_Distribution.ipynb).

How to run it:
```bash
cd HashTable_v1
make distr
./distr
jupyter notebook
```
That's the result. First, let's have a look at some weird hash-functions: 
1. h(s) = s[0]
  ![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/ascii0.png)
2. h(s) = len(s):
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/hash_len.png) 
3. h(s) = sum(s[i]):
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/hash_sum.png)
4. h(s) = sum(s[i]) / len(s):
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/hash_sum_len.png)
5. h(s)[i+1] = rol(h(s)[i]) xor s[i+1]
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/ded_rol_hash.png) 
7. h(s)[i+1] = ror(h(s)[i]) xor s[i+1]
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/ded_ror_hash.png)

As you may see, using appropriate hash functions is quite important for distribution's uniformity, so it is better to use more complicated ones, like [MurmurHash2](https://ru.wikipedia.org/wiki/MurmurHash2)([🐈‍](https://preview.redd.it/abd1kh5yy3w51.jpg?width=1125&format=pjpg&auto=webp&s=d8eda40ceecf0e8d2efec1c897c885ddf7f72c97)) or [crc32](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) functions:

![murmur](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/murmur.png)
![crc32](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/crc32.png)

The average list size and the dispersion are quite better than the ones in previous distributions, thus I've used ***crc32*** function in hash table's implementation.

The next step is to adjust the hashtable size for an optimal average list size, which is approximately 1.5. Also the size should be a prime number to avoid clusterization of hashes. We can choose the size by using the above-mentioned [filling program](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/HashTable_v1/csv_gen.cpp)  and this [notebook](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/HashTable_v1/HT_Distribution.ipynb). An optimal size for the [dictionary dataset](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/Datasets/Raw/ded-dict.txt) is 131137 with the average list size being approximately 1.2.

### Performance test program

First, our test program will load the [above-mentioned dictionary](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/Datasets/Raw/ded-dict.txt) into the hash table. Then we will open another [400k-words dataset](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/Datasets/Raw/400k-words.txt), search for all its words in the hash table and count the quantity of datasets' intersections. The search operation is similar to addition but without reading the element's value.
To increase the weight of the searching function we will repeat this process several (10) times.

Here are the ```main``` and the ```countWords``` functions. The ```WordList``` is just a structure that contains pointers to the keys in parsed CSV file and their quantity. The ```HashTable::getElement``` function searches for an element with a specified key and returns its address (or 0 if not found).

```cpp
size_t  countWords(const WordList&  wordlist, HashTable&  table)
{
	size_t n_found = 0;
	for (int n_word = 0; n_word < wordlist.quantity; n_word++)
	{
		n_found += !!table.getElement(wordlist.words[n_word]);
	}
	return n_found;
}

int  main()
{
	HashTable table (131137, MeowPurr_hash);
	char* ded_dict = ReadFile("../Datasets/Csv/ded-dict.csv");
	table.readCSV(ded_dict);
	
	char* dict400k = ReadFile("../Datasets/Csv/400k-words.csv");
	WordList words = parseCSV(dict400k);

	int n_read = 0;
	for (int i = 0; i < 10; i ++)
		n_read = countWords(words, table);
	printf("%d\n", n_read);

	free(ded_dict);
	free(dict400k);
	free(words.words);

	return  0;
}
```

### Compilation and execution
Throughout the aricle, the sources will be compiled with GCC. The optimization level key would be ```-O2```.

```bash
$ g++ -O2 -g -DNDEBUG main.cpp HashTable.cpp List.cpp hashfuncs.cpp HashTableMemoryManager.cpp parse.cpp ../../../Education-MIPT/Onegin/foperations.cpp -o run 
```

To build it and run in version 1, use these commands:

```bash
cd HashTable_v1
make
time ./run
```
Building and running the next versions are similar.

The v1's execution result is that the dictionary contains 60403 keys from the 400k-words dataset (in future versions this value will indicate that everything works OK). 
The elapsed user time is 1687 ms.

![v1 time](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/v1_time.png)

Now let's see how we can optimize our program.

### Profiling software

For performance analysis we will use [Callgrind](https://valgrind.org/docs/manual/cl-manual.html) profiler to find bottlenecks that reduce the performance.
For a profile data visualization we will use [KCachegrind](http://kcachegrind.sourceforge.net/html/Home.html).
For a memory access analysis in the third optimization we will use [Intel VTune™](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/vtune-profiler.html#gs.1027no1) profiler.

 ## First optimization

Let's run test program using Callgrind with these commands:


```bash
cd HashTable_v1
make perf
kcachegrind
``` 
Running Callgrind in the next versions is similar.

That's the result:

![v1 task](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/v1_task.png)

As we can see, the ```crc32``` function takes 87% of execution time itself. Let's have a look at its implementation: 

```cpp
typedef u_int64_t hash_t;

hash_t crc32(const char *key)
{   
   int n_char = 0;
   int j = 0;
   unsigned int byte = 0;
   unsigned int crc  = 0;
   unsigned int mask = 0;

   n_char =  0;
   crc    = ~0;
   
   while (key[n_char])
   {
      byte = key[n_char];
      crc  = crc ^ byte;
      
      for (j = 7; j >= 0; j--)
      {   
        mask = -(crc & 1);
        crc  =  (crc >> 1) ^ (0xEDB88320 & mask);
      }
      
      n_char++;
   }
   
   return ~crc;
}
```

We can boost the function by using [crc32 x86 intrinsics](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=crc). There are several intrinsics that perform crc32 algorithm. The difference between them is the amount of data that is hashed in one call. Hashing bigger amounts of data in one call is faster. 

Thus, to make the most of ***crc32 intrinsics*** we should use the function that hashes the biggest amount of data (8 bytes), which is ```_mm_crc32_u64```, and avoid using other ***crc32 intrinsics***. However, this is not a solution for this particular implementation.

The reason is that the keys, which are stored in the file, are not divided into separate blocks. We can not hash the key only with ```_mm_crc32_u64``` and ignore the key length. In this case the hash value may depend on trash values located after the end of the key, so the hash function result will be undefined. 

We can avoid reading trash values by storing the keys separately, as was mentioned earlier. The way is to divide the keys in the input file. Also we can speculatively calculate and store keys' lengths to avoid checking null-chars.  But there are several problems in using this method, that may cause the hash table to be less efficient in some situations. Let's put off this idea [for a while](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/README.md#third-optimization-optional). Now we will use more simple and general solution.

### Using crc32 intrinsic in a simple way

Though we are not able to take the most out of crc32 intrinsics now, we can use them in the common way.

That's the new crc32 implementation:
```cpp
typedef u_int64_t hash_t;

hash_t opt2_crc32(const char* key)
{
    hash_t crc = 0;

    char* current_char = (char*) key;
    
    while (*current_char && *(current_char+1))
    {
        crc = _mm_crc32_u16(crc, *((unsigned short*)(current_char)));
        current_char+=2;
    }

    while (*current_char)
    {
        crc = _mm_crc32_u8(crc, *(current_char++));
    }

    return crc;
}
```

As you may see, this functions firstly tries to hash two bytes in one call. If the end of the string is met, it hashes one last byte if needed. This is a little faster than hashing only one byte in one call.  As it is said earlier, there are another crc32 intrinsics which hash more bytes in one call, but the experiment showed that using them doesn't make sense due to bigger amounts of null-char checks.

### Results

Now test program runs ***128%*** faster. Elapsed user time is 740 ms.

![v2 time](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/v2_time.png)

Hash table v2 source code can be found [here](https://github.com/tndrd/Education-MIPT-2sem-asm/tree/main/HashTable/HashTable_v2).

## Second optimization

Like we did earlier, let's look at the Callgrind's output:

![v2 task](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/v2_task.png)

Although the hash function is still on the top, we've already optimized it, so our next candidate will be ```HashTableList::GetElement```, which self time is 21%. Let's have a look at its source code:
```cpp
ListElement* HashTableList::getElement(const char* key)
{
    register ListElement* current_element = tail;
    
    for (int n_element = 0; n_element < size; n_element++)
    {
        if (!strcmp(key, current_element -> key_)) return current_element;
        current_element = current_element -> next_;
    }

    return nullptr;
}
```

At first glance, it is not obvious what we can optimize here. The hypothesis is that passing through the list entails accessing to memory regions that are far away from each other. This is the example of bad locality, which leads to cache misses that entail slow memory access. 
Let's use VTune profiler memory access performance analysis tool:

![vtune_macc](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/vtune_macc.png)

The function's average read latency is high. In fact, this metric varies from one measurement to another. Let's do several measurements and calculate the average and its inaccuracy.
| Latency, cycles||||||||||
|-----|-----|----|-----|----|-----|----|-----|----|----|
| 138 | 120 | 60 | 100 | 36 | 131 | 54 | 121 | 67 | 90 |

The average load latency is 91 +- 11 (12%) cycles. After the optimization we will repeat these measurements and compare them.

### Using local list containers

To improve data locality let's add a fixed-size array of ```ListElements``` to each ```HashTableList``` structure. It doesn't have to be large; it should be large enough to store an average number of elements. When element is added to the list, we try to store it in ```local_container``` if there is free space there. Otherwise, it should be added to global ```current_buffer```
When we search for a key in the list, we firstly check the ```local_container```. If it's not found there, we check the linked list, starting from ```tail``` .

Here's the way the element is added:
```cpp
int HashTable::addElement(const char* key, Type value)
{
    HashTableList* element_list = getList(key);

    // v3 feature 
    if (element_list -> size < LOCAL_LIST_CAPACITY)
    {
        element_list -> addLocalElement(key, value);
        n_elements++;
        return 0;
    }

    ListElement*   new_element  = mem_manager.getNewElement();
    new_element -> key_         = key;
    new_element -> value_       = value;
	
    element_list -> append(new_element);
    n_elements++;
    return 0;
}
```

Here's the way the element is found in the list:
```cpp
ListElement* HashTableList::getElement(const char* key)
{   
    // v3 feature
    for (int n_element = 0; n_element < LOCAL_LIST_CAPACITY; n_element++)
    {
        if (!strncmp(key, local_container[n_element].key_, MAX_KEY_LENGTH)))
            return &(local_container[n_element]);
    }    

    register ListElement* current_element = tail;
    for (int n_element = local_capacity; n_element < size; n_element++)
    {
        if (!strncmp(key, current_element -> key_, MAX_KEY_LENGTH)) return current_element;
        current_element = current_element -> next_;
    }

    return nullptr;
}
```

### Results

Let the local capacity be two elements. Similar load latency measurements are better now:

| Latency, cycles||||||||||
|-----|-----|----|-----|----|-----|----|-----|----|----|
| 60 | 27 | 39 | 45 | 31 | 56 | 88 | 59 | 50 | 58 |8

Now the average load latency is 51 +- 5 (10%) cycles. Let's look at the performance.
Elapsed user time is 561 ms now. Program runs ***32%*** faster than the previous version.

![v3 time](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/v3_time.png) 

Hash table v3 source code can be found [here](https://github.com/tndrd/Education-MIPT-2sem-asm/tree/main/HashTable/HashTable_v2).

## Third optimization

Again, let's run the Callgrind:

![v3 task](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/v3_task.png)

There are two candidates to be optimized: ***crc32*** and ***strncmp***.

- ```crc32``` can be boosted by using only  ```_mm_crc32_u64``` for all the data. This optimization requires the keys to be stored in separate trash-free blocks to prevent hashing a trash.  

- ```strncmp``` can be boosted with SSE instructions, which use 16-byte aligned data. The possibility is to compare the strings using vector instructions. We should also prevent reading trash values after the end of the keys, like in ```crc32```.

As we can see, ```strncmp``` and ```crc32``` can be boosted in similar way.
The idea is to store the keys in separate, trash-free, 32-byte aligned blocks.

However, this trick entails higher key load time due to necessity of separating and aligning each key. So the effectiveness of this method increases when keys are used much more often than they are loaded.
We can avoid separating and aligning the keys each time we load them by storing them aligned and separated beforehand in a file, if the same datasets are going to be loaded and used multiple times.
However, under different circumstances, this technique may even slow the overall performance.    

Let's create a model situation and pretend that the time spent on separating and aligning the keys is much less than the time spent on working with the hash table. Let's transform our CSV dictionary and 400k-words dataset into above-mentioned format. This allows us to measure the hashtable performance excluding costs of aligning and separating the keys. 

### Transforming datasets' files

Let's store the keys separately from the values. In the file each key should be contained in the beginning of a block which length is a multiple of 16. The remaining space is filled with zeros.  Each block should start at the offset which is a multiple of 16 too.
Also we can speculatively calculate each key's length and store it as a char value at the end of the previous block. Let's call it a "size char".

There are two ([1](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/Datasets/ded-dict-to-ACSV.py), [2](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/Datasets/400k-words-to-ACSV.py)) python scripts which transform our datasets into this format. To run them, execute these commands:
```bash
cd HashTable_v4 
make acsv
```
New files will be located in ```Datasets/Acsv/```.

This is a hex view of [the transformed dictionary dataset](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/Datasets/Acsv/ded-dict.keys)'s part:

![acsv](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/acsv.png)

The next step is to read this file into a buffer allocated with GCC's ```aligned_alloc``` using ```HashTable::readACSV``` function, storing each key's block size. Then we should replace the "size chars" with zeros. Now we can read the keys as ```__m128i*``` and easily operate with them using SSE intrinsics.

### Using SSE intrinsics to boost *crc32* and *strcmp*

Now we can replace ```strcmp``` function with the one that uses SSE intrinsics:

```cpp
inline int SIMD_strcmp(const __m128i* a, size_t a_n_blocks, const __m128i* b, size_t b_n_blocks)
{
    if (a_n_blocks != b_n_blocks) return -1;
    
    int res = ~0;

    for (int n_block = 0; n_block < a_n_blocks; n_block++)
    {
        __m128i first  = _mm_load_si128(a);
        __m128i second = _mm_load_si128(b);
        
        res &= _mm_movemask_epi8(_mm_cmpeq_epi8(first, second));

        if (res != 0xFFFF) return 1;

        a += 1;
        b += 1;
    }
    
    return res != 0xFFFF;
}
```

We compare the keys by checking the equivalence of their whole blocks.  Since we replaced the "size chars" with zeros, the remaining spaces in keys' blocks are both empty. That means that the result of comparison wouldn't depend on undefined values.
This fact allows us to boost our crc32 function. Now we can hash bigger blocks during one iteration unlike [previous crc32 version](https://github.com/tndrd/Education-MIPT-2sem-asm/blob/main/HashTable/README.md#first-optimization). Since we now know the size of each key's corresponding block, zero-char checks are not needed.

Here is the new version of ```crc32```:
```cpp
// typedef u_int64_t hash_t;

hash_t opt3_crc32(const __m128i* key, size_t n_blocks)
{
    hash_t crc = 0;

    hash_t* current_oct = (hash_t*) key;

    for (int n_block = 0; n_block < n_blocks; n_block++)
    {
        crc = _mm_crc32_u64(crc, *(current_oct++));
        crc = _mm_crc32_u64(crc, *(current_oct++));
    }
    return crc;
}
```

### Results

Elapsed user time is 341 ms now. Program runs ***58%*** faster than the previous version.

![v4 time](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/v4_time.png)

That's the output of Callgind:

![last](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/v4_callgr.png)

HashTable v4 source code can be found [here](https://github.com/tndrd/Education-MIPT-2sem-asm/tree/main/HashTable/HashTable_v4).

## Consclusion

Here's the performance comparison chart:
 
 ![results](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/results.png)
 
 Finally, the hash table has been boosted by **394%** relatively to the first version. 
