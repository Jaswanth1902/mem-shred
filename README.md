<div align="center">

# 🛡️ mem-shred
### Header-Only C++20 Cryptographic Memory Sanitization & Compiler Barrier

[![C++20](https://img.shields.io/badge/Language-C%2B%2B20-00599C?style=flat-square&logo=c%2B%2B)](https://github.com/Jaswanth1902/mem-shred)
[![Security: Anti-Forensic](https://img.shields.io/badge/Security-Compiler--Resistant%20Zeroization-red?style=flat-square)]()
[![Architecture: Header-Only](https://img.shields.io/badge/Architecture-Header--Only-brightgreen?style=flat-square)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=flat-square)](LICENSE)

**Guarantee that sensitive cryptographic keys, passwords, and tokens are actually scrubbed from RAM.**  
Standard `memset()` calls are frequently optimized away by optimizing compilers (`clang -O3`, `gcc -O3`) as "dead stores". `mem-shred` enforces physical memory wiping using atomic hardware barriers and OS-native zeroing primitives.

[🔬 The Compiler Optimization Trap](#trap) • [🚀 Usage](#usage) • [📐 Verified Assembly](#assembly)

</div>

---

### 🔬 The Compiler Optimization Trap

```cpp
// ❌ VULNERABLE: Compiler dead-store elimination removes this memset under -O3!
void handle_key() {
    char key[64] = "secret_key_12345";
    process(key);
    memset(key, 0, sizeof(key)); // SILENTLY DELETED BY COMPILER!
}

// ✅ SECURE: Guaranteed zeroization with compiler barriers
#include "memshred.hpp"
void handle_key_secure() {
    char key[64] = "secret_key_12345";
    process(key);
    memshred::zeroize(key, sizeof(key)); // PHYSICALLY SCRUBBED
}
```

---

### 🚀 Usage

Include the header directly:
```cpp
#include "memshred.hpp"

std::vector<uint8_t> secret_payload = get_decrypted_token();
// ... use payload ...
memshred::zeroize(secret_payload.data(), secret_payload.size());
```
