"""mem-shred: Zero-overhead anti-forensic memory sanitizer for Python."""
import ctypes
from contextlib import contextmanager
from typing import Union

__version__ = "0.1.0"


def shred_buffer(target: Union[bytearray, memoryview, ctypes.Array]) -> None:
    """
    Overwrites the underlying memory of a mutable buffer in-place with 3 passes:
    0xFF -> 0xAA -> 0x00.
    """
    if isinstance(target, bytearray):
        buf = memoryview(target)
    elif isinstance(target, memoryview):
        buf = target
    else:
        try:
            buf = memoryview(target)
        except TypeError:
            raise TypeError("Target must be a mutable buffer (bytearray, memoryview, or ctypes array)")

    n = len(buf)
    if n == 0:
        return

    # Pass 1: 0xFF
    buf[:] = b"\xFF" * n
    # Pass 2: 0xAA
    buf[:] = b"\xAA" * n
    # Pass 3: 0x00
    buf[:] = b"\x00" * n


@contextmanager
def secure_memory_guard(buffer: Union[bytearray, memoryview]):
    """Context manager ensuring memory is shredded upon exit even if an exception occurs."""
    try:
        yield buffer
    finally:
        shred_buffer(buffer)
