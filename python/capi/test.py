import ctypes
def test_ctypes():
	dll = ctypes.CDLL('./out/lib/libtest.so')

	pchar_array = ctypes.c_char_p * 4
	array = pchar_array()
	for i in range(0, len(array)):
		array[i] = 'arg' + str(i)
	print(type(array))
	dll.test_args(4, array)

	class st_t(ctypes.Structure):
		_fields_ = [("name", ctypes.c_char_p), ("age", ctypes.c_int)]
	dll.test_char_p.restype = ctypes.POINTER(st_t)
	ret = dll.test_char_p();
	print(ret.contents.name)
	print(ret.contents.age)

def test_struct_union():
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

if __name__ == "__main__":
	test_ctypes()
	test_struct_union()
