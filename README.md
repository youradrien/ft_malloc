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

## 🧱 architecture

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

* block are returned to the free list of corresponding page
* no immediate `munmap` for TINY/SMALL zones
* `munmap` is used only for LARGE allocations

---

## performance notes

* Avoids touching unused memory pages (lazy paging friendly)
* Minimizes page reclaims by limiting memory writes
* Reduces fragmentation with aligned allocations
* Efficient reuse of freed blocks

---

## 🧪 Testing

Example test:

```c
while (i < 1024)
{
    addr = malloc(1024);
    addr[0] = 42;
    free(addr);
    i++;
}
```

Expected behavior:

* Stable memory usage
* Minimal page reclaims (~baseline + small overhead)

---

## 🛠 Build

```bash
make
```

this produces:

```
libft_malloc.dylib (macOS)
libft_malloc.so    (Linux)
```

---

## ▶️ usage

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

### test my malloc performance
```bash
/urs/bin/time -l ./your_test
```

---

## debug

inspect allocations with:

```c
show_alloc_mem();
```

---

## ⚠️ limitations

* No advanced coalescing of adjacent free blocks (optional improvement)
* Basic fragmentation handling
* Simplified allocator compared to production systems

---

## possible improvements

* block-coalescing
* Better fit strategies (best-fit / segregated lists)

---

