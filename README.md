# 🧠 ZeroStore

> A minimalist, zero-dependency in-memory key-value store written in C.  
> Built from scratch to practice manual memory management, pointer logic, and basic TCP networking.

---

## 📌 Features

- No external libraries — built using standard C and POSIX sockets.
- Supports basic key-value operations over TCP:
  - `SET key value`
  - `GET key`
  - `DEL key`
- In-memory hash map with manual memory allocation.
- Command-line TCP server.

---

## ✅ Usage
- Compile with `make`.  
- Run the server using `./zerostore`.  
- Connect with `nc localhost 12345`
- Use commands like `SET foo bar`.