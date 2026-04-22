# 🧠 ft_malloc

custom memory allocator written in C, inspired by the behavior of the libc `malloc`, `free`, and `realloc`.

it uses dynamic memory management using low-level system calls such as `mmap` and `munmap`, with a focus on performance && memory efficiency.

---

## 🚀 Features

* custom implementations of:
  * `malloc`
  * `free`
  * `realloc`
* memory zones:
  * **TINY** allocations
  * **SMALL** allocations
  * **LARGE** allocations
* page-based memory management using `mmap`
* lazy allocation strategy (no full pre-initialization)
* thread safety w/ `pthread_mutex`

---

## architecture

memory is divided into zones:

| Type  | Size Range                 | Storage Strategy          |
| ----- | -----------------------    | ------------------------- |
| TINY  | small allocations <= 128   | pre-mapped zones          |
| SMALL | medium allocations <= 1024 | pre-mapped zones          |
| LARGE | large allocations  > 1024  | one `mmap` per allocation |

---

### 📦 zone layout

each zone contains:

```
[t_page][t_block][data][t_block][data]...
```
* `t_page` → metadata: whole zone
* `t_block` → metadata: for each allocation
* `data` → user raw memory

---

## ⚙️ allocation Strategy

### 🔹 lazy initialization

instead of pre-creating all blocks:
* 1 large free block is created initially, blocks are split on demand during each allocation 

significantly reduces:
* page reclaims
* memory overhead
* initialization cost
* & avoids touching unused memory pages (lazy paging)

---

### 🔹 block Splitting

allocating:
```
[FREE BLOCK]
      ↓ split
[ALLOCATED][FREE REMAINDER]
```

---

### 🔹 free Strategy

* block are returned to the free list of corresponding page ->  minimizes page reclaims
* no immediate `munmap` for TINY/SMALL zones
* `munmap` is used only for LARGE allocations

---

## 🧪 testing

example test:

```c
while (i < 1024)
{
    addr = malloc(1024);
    addr[0] = 42;
    free(addr);
    i++;
}
```

```c
	char *addr1; 
	char *addr3; 

	addr1 = (char*)malloc(16*M); 
	strcpy(addr1, "Bonjours\n"); 
	print(addr1); 
	addr3 = (char*)realloc(addr1, 128*M); 
	addr3[127*M] = 42; 
	print(addr3); 
```

---

## 🛠 Build

```bash
make
```

this produces (library):

```
libft_malloc.dylib (macOS)
libft_malloc.so    (Linux)
```

---

## ▶️  USAGE

### macOS

```bash
export DYLD_INSERT_LIBRARIES=./libft_malloc.dylib
export DYLD_FORCE_FLAT_NAMESPACE=1
./your_program
```

### linux

```bash
export LD_PRELOAD=./libft_malloc.so
./your_program
```


---

## debug

inspect allocations with:

```c
show_alloc_mem();
```

test my malloc performance with:
```bash
/urs/bin/time -l ./your_test
```

---

## possible improvements

* block-coalescing (no advanced coalescing of adjacent free blocks)
* Better fit strategies (best-fit / segregated lists)

---

