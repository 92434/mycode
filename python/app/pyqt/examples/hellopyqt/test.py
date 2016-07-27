import ctypes

pdll = ctypes.CDLL('./out/lib/libxiaofei.so')
print str(dir(pdll))
pdll.test(1)
pdll.main(1, 'xiaofei')
