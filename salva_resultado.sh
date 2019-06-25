#!/bin/bash

(./mmu ../traces/bzip.trace -lru; ./mmu ../traces/gcc.trace -lru; ./mmu ../traces/sixpack.trace -lru; ./mmu ../traces/swim.trace -lru) > ../lru.txt
(./mmu ../traces/bzip.trace -sc; ./mmu ../traces/gcc.trace -sc; ./mmu ../traces/sixpack.trace -sc; ./mmu ../traces/swim.trace -sc) > ../sc.txt 