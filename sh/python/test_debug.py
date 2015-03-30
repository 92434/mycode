import inspect

def current_info():
	return 'xiaofei:[' + inspect.currentframe().f_back.f_code.co_filename + ':' + inspect.currentframe().f_back.f_code.co_name + ':' + str(inspect.currentframe().f_back.f_lineno) + ']:'


def test_func():
	print current_info()

test_func()
