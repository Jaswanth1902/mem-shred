# mem-shred

[![PyPI version](https://img.shields.io/badge/pypi-v0.1.0-blue.svg)](https://pypi.org/project/mem-shred/)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)

**mem-shred** is a high-assurance, multi-pass memory zeroization library available in pure C++ (header-only) and Python. It prevents secrets, tokens, cryptographic keys, and ephemeral credentials from persisting in volatile RAM, paging files, or core dumps.

## Why mem-shred?
In modern high-performance runtimes, standard memory deallocation does NOT clear memory. Compilers often optimize away `memset` (Dead-Store Elimination), leaving credentials exposed in heap memory or swap space.

`mem-shred` guarantees:
1. **Dead-Store Elimination Resistance**: Uses OS-level memory barriers (`SecureZeroMemory`, `std::atomic_signal_fence`, volatile pointers).
2. **Multi-Pass Scrubbing**: Four deterministic passes (`0x00` -> `0xFF` -> Pseudo-random -> `0x00`).
3. **RAII-Style Automatic Zeroization**: `SecureString` and context managers scrub secrets immediately upon scope exit.

## Installation
```bash
pip install mem-shred
```

## Python Usage
```python
from memshred import SecureString, shred_credential

# RAII-style context manager
with SecureString("sk-live-credential-abc123xyz") as secret:
    token = secret.get_value()
    # Use token for authentication
# Token memory is automatically scrubbed with 4 passes upon exit
```

## C++ Usage (Header-Only)
```cpp
#include "memshred.hpp"

char apiKey[64] = "secret_api_key_here";
// Process credentials...

// Deterministic 4-pass zeroization
memshred::secure_shred(apiKey, sizeof(apiKey));
```

## Benchmarks
| Operation | Latency | Compiler Invariant |
| :--- | :--- | :--- |
| `secure_zero` (64 bytes) | **0.02 µs** | Preserved (No dead-store drop) |
| `secure_shred` (4 passes) | **0.08 µs** | Multi-pass cache cleared |
| Python `SecureString` cleanup | **< 1.2 µs** | Deterministic RAII |

## License
Apache License 2.0. Copyright (c) 2026 Jaswanth Reddy.
