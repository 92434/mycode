import ctypes

dll = ctypes.CDLL('./out/lib/libtest.so')
pchar_array = ctypes.c_char_p * 4
array = pchar_array('arg1', 'arg2', 'arg3', 'arg4')
dll.main(4, array)

class S(ctypes.Structure):
	_fields_ = [("a", ctypes.c_int), ("b", ctypes.c_int)]
class U(ctypes.Union):
	_fields_ = [("a", ctypes.c_int), ("b", ctypes.c_int)]

s = S(1,2)
print s.a, s.b
u = U(1,2)
print u.a, u.b
print ctypes.sizeof(S)
print ctypes.sizeof(U)
