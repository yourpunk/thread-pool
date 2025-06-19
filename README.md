# 🧵 Minimal Thread Pool in C++

A clean, minimal, header-only thread pool implementation in modern C++ — built from scratch to fully understand threading, task scheduling, and synchronization primitives without relying on external abstractions.

This is not a black-box. Every line is written to be read, tweaked, and understood.

---

## ✨ Features

- 🧵 **Fixed-size thread pool** with scoped lifetime
- ⛓️ **Thread-safe task queue** using `std::mutex` + `std::condition_variable`
- 🧼 **RAII-based cleanup** — ensures graceful thread shutdown on destruction
- ⚡ **Lock-guarded enqueue logic** — no race conditions
- 💼 **Header-only** — just drop in and go

---

## 🧠 Why I Built This

I wanted to demystify multithreading and build something I could reason about:
- ✅ How to manage multiple threads without leaks or races
- ✅ How to queue and dispatch tasks efficiently
- ✅ How to handle lifetime, shutdown, and exceptions safely
- ✅ How to **understand** every part of a working thread pool

---

## ⚙️ Usage Example

```cpp
#include "ThreadPool.h"
#include <iostream>

int main() {
    ThreadPool pool(4); // 4 worker threads

    for (int i = 0; i < 10; ++i) {
        pool.enqueue([i] {
            std::cout << "Running task " << i << std::endl;
        });
    }

    // Threads join automatically on destruction
}
```

---

## 📁 Project Layout
```plaintext
.
├── ThreadPool.h   # Full implementation (header-only)
├── LICENSE.txt
├── README.md 
└── main.cpp       # Minimal usage demo
```

---

## 🛡️ Thread Safety
- Task queue is protected by std::mutex and condition variables
- All enqueue operations are thread-safe
- Pool shutdown is synchronized and exception-safe
- Ensures no dangling threads or invalid memory access

---

## 📬 Want to Extend?
This thread pool can be extended with:
- 🧭 Work stealing / dynamic load balancing
- 🔄 Bounded queue + backpressure
- 🧪 Task prioritization logic
- 🗂️ Metrics / tracing support
- ⚙️ Futures + result passing

---

## 🔖 License
MIT — use it, learn from it, extend it. Just don’t pretend you wrote it.

---

## 👤 Author
🦾 Crafted by Aleksandra Kenig (aka [yourpunk](https://github.com/yourpunk)) — game developer, systems nerd, multithreading enjoyer.

> If you read all this, we should probably work together.
