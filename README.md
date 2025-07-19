# ⚙️ Algorithm Optimization Project (Time & Space)

This C-based project focuses on the **efficient implementation of an algorithm for managing a network of car-sharing stations**, optimizing both **execution time** and **memory usage**. It includes route planning, station and vehicle management, and is designed with minimal dynamic allocation where possible.

---

## 📚 Description

The system simulates a set of **car-sharing stations**, where each station can hold cars with different maximum distances. Users can:

- Add or demolish stations
- Add or scrap cars at a station
- Plan optimized routes based on car availability and range

The core focus of the implementation is to avoid excessive heap allocation and reuse memory efficiently while ensuring good computational performance.

---

## 💡 Key Features

- Doubly-linked list to represent stations
- Optimized path planning using array-based support structures instead of dynamic memory
- No use of external libraries—pure C and standard I/O
- Efficient backward and forward traversal for route computation

---

## 🚀 How to Run

### Compile:
```bash
gcc -o main main.c

Sample commands

aggiungi-stazione 1000 3
80 100 120
aggiungi-stazione 2000 2
90 110
aggiungi-auto 1000 150
rottama-auto 1000 100
pianifica-percorso 1000 2000
demolisci-stazione 1000

Sample outputs

aggiunta
aggiunta
aggiunta
rottamata
1000 2000
demolita
