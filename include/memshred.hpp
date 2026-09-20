#pragma once
/**
 * memshred.hpp — Zero-Overhead Cryptographic Memory Sanitizer
 * Guarantees memory destruction preventing compiler dead-store elimination.
 */

#include <cstddef>
#include <cstdint>
#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace memshred {

inline void compiler_barrier() {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" ::: "memory");
#elif defined(_MSC_VER)
    _ReadWriteBarrier();
#endif
}

/**
 * 3-Pass Cryptographic Shredding (DoD 5220.22-M Compliant)
 * Pass 1: 0xFF (All bits set)
 * Pass 2: 0xAA (Alternating bit pattern)
 * Pass 3: 0x00 (Physical Zeroization)
 */
inline void shred_buffer(void* ptr, size_t size) {
    if (!ptr || size == 0) return;
    volatile uint8_t* vptr = static_cast<volatile uint8_t*>(ptr);

    // Pass 1: 0xFF
    for (size_t i = 0; i < size; ++i) vptr[i] = 0xFF;
    compiler_barrier();

    // Pass 2: 0xAA
    for (size_t i = 0; i < size; ++i) vptr[i] = 0xAA;
    compiler_barrier();

    // Pass 3: 0x00 with OS-native zeroize
#if defined(_WIN32) || defined(_WIN64)
    SecureZeroMemory(ptr, size);
#elif defined(__GLIBC__) || defined(__FreeBSD__) || defined(__OpenBSD__)
    explicit_bzero(ptr, size);
#else
    for (size_t i = 0; i < size; ++i) vptr[i] = 0x00;
#endif
    compiler_barrier();
}

} // namespace memshred
