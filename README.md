# 🧵 Minimal Thread Pool in C++

Wrote this header-only thread pool from scratch while learning how threads and synchronization really work in C++.  
No tutorials copied, no ChatGPT regurgitation — just me, a lot of debugging, and pure stubbornness.

> **Is it production-ready?** <br> Probably not.<br> **Did I write it myself to actually understand how the hell threading works?**<br> Yes.<br> **Is it better than copying from StackOverflow?** <br>  100%.<br>

If you're looking for a clean, minimal, modern C++ thread pool you can actually understand — you're in the right place.

---

## ✨ Features

- Fixed-size thread pool
- Safe task queue (mutex + condition_variable)
- Automatic cleanup (RAII-style)
- No external dependencies
- Header-only, easy to drop in

## 📁 Files
- `ThreadPool.h` — the whole thing lives here. My baby.
- `main.cpp` — just a tiny demo. Nothing fancy.

## ⚙️ Usage

```cpp
#include "ThreadPool.h"
#include <iostream>

int main() {
    ThreadPool pool(4); // 4 threads

    for (int i = 0; i < 10; ++i) {
        pool.enqueue([i] {
            std::cout << "Running task " << i << std::endl;
        });
    }

    // Destructor waits for all tasks to finish
}
```

## 🤘 Why I Did This
I was tired of treating multithreading like a black box.
This was my hands-on exercise to learn:
- how to manage multiple threads safely
- how to queue and schedule tasks
- how to avoid deadlocks (the fun part™)
- and how to clean it all up properly


## 🪵 Notes
- Only `ThreadPool.h` is written by me — I added main.cpp just as a basic demo.
- You're free to use it however you want (**MIT** license).
- No, it's not production-ready — but it does what it says on the tin.
- And yeah, I’m proud of it.


## 🧷 License
**MIT** — use, break, fix, improve. Just don’t claim you wrote it, okay?

## 👤 Author
🦾 Crafted by Aleksandra Kenig (aka [yourpunk](https://github.com/yourpunk)).


## 📬 Wanna say hi?
If you're curious, hiring, or just nerding out over C++ —
feel free to ping me. I’m always building.
