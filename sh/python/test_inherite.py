class A():
	def __init__(self):
		print 'A::__init__'
	def call_func(self):
		print 'A::call_func'
		self.func()
	def func(self):
		print 'A::func'


class B(A):
	def __init__(self):
		A.__init__(self)
		print 'B::__init__'
	def func(self):
		print 'B::func'

def main():
	b = B()
	b.call_func()
	a = A()
	a.call_func()

if __name__ == '__main__':
	main()
