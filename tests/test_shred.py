from memshred import shred_buffer, secure_memory_guard

def test_shred_bytearray():
    data = bytearray(b"top_secret_credentials_12345")
    shred_buffer(data)
    assert all(b == 0 for b in data)

def test_context_manager():
    secret = bytearray(b"transient_auth_token")
    with secure_memory_guard(secret) as s:
        assert b"transient" in s
    assert all(b == 0 for b in secret)

if __name__ == "__main__":
    test_shred_bytearray()
    test_context_manager()
    print("All mem-shred tests PASSED!")
