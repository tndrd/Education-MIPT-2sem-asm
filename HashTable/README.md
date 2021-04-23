# Hash table
This is a project of second semester programming course in MIPT, which includes hash table implementation with subsequent performance analysis and optimization. The whole code is written in C++, excluding several Python scripts that convert raw datasets into CSV (or similar) files.

## Contents

* Before optimization:
  * Inital implementation overview
  * Selecting hash-function and hash table size
  * Analyzing performance and searching for bottlenecks

## Before optimization

Hash table is a data structure that can map keys to values. The main idea is that each key is being hashed to get an index which points to the corresponding value. This trick provides faster key-value access in comparison with some obvious implementations like searching for specific key in an array. However, we're not protected from hash collisions, because there may be two keys that have same hash. There are several methods of collision resolution, the one I've implemented is called separate chaining with linked lists. The scheme below represents the hash table's structure: 

![hash table scheme](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/scheme.png)
We have the ***HashTable*** structure, which refers to an array of linked list headers (***list_buffer***) which length equals **size**. Each list in this array links all the key-value pairs whose keys have the same hash. All the elements of all the lists are stored in the ***data_buffer*** which allocated space equals ***capacity***. The field ***n_elements*** represents number of elements in the table.

Every time we want to add another pair, we hash the desired key. Then we get the reminder of hash by ***size*** division to get an index in the ***list_buffer*** array which points to the linked list that corresponds to our key. Then we add our new pair to the end of this linked list by writing it into free cell in ***data_buffer*** and chaining it with this list.

Every time we want to find the value by key, we search this key in corresponding list by going through its elements and comparing our key with ones in list. 
This fact suggests that the quantity of elements in each list influences access time: the more elements we have, the more key comparisons we should do.

### Selecting hash-function and hash table size
To improve hash table performance we have to investigate the collision distribution. Let's fill our hash table using different hash functions with [test dictionary dataset](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/Datasets/Raw/ded-dict.txt), which contains around 150k pairs. Also, we will use this dataset throughout the article.
An average list size will represent an average access performance. The distribution of list sizes will represent the access stability (this means that access to different lists' performance is compared).

Some weird hash-functions: 
1. $h(key) = key_0$:
  ![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/hash_ascii_0.jpg)
2. $h(key) = len(key)$:
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/hash_len.jpg) 
3. $h(key) = \sum_i (key_i)$:
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/hash_sum.jpg)
4. $h(key) = \sum_i (key_{i}) ~/~len(key)$:
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/hash_sum_len.jpg)
5. $h(key)_{i+1} = rol(h(key)_i) ~xor~key_{i+1}$ and
$h(key)_{i+1} = ror(h(key)_i) ~xor~key_{i+1}$ in comparison:
![key_0](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/ded_rol_hash.jpg)

As you may see, appropriate hash functions are quite important for a good distribution, so it is better to use more complicated ones, like [MurmurHash2](https://ru.wikipedia.org/wiki/MurmurHash2) function:

![murmur](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/assets/MeowPurr_hash.jpg)
The average list size and the dispersion are quite better than the ones in previous distributions, so I've used this function in hash table's implementation v1.

The next step is to adjust the hashtable size for an optimal average list size, which is ~1.5. Also the size should be a prime number to avoid clusterization of hashes. An optimal size for the [dictionary dataset](https://raw.githubusercontent.com/tndrd/Education-MIPT-2sem-asm/main/HashTable/Datasets/Raw/ded-dict.txt) is 131137.

### First performance tests

