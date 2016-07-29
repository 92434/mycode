class c1(dict):
  def __init__(self):
    self['attr1'] = 'attr1'
    self['attr2'] = 'attr2'

def test_c1():
  tc1 = c1()
  print(dir(tc1))

#test_c1()


class c2():
  def __init__(self):
    self.__dict__['attr1'] = 'attr1'
    #self.xiaofei = 'xiaofei'

#  def __setattr__(self, name, value):
#    print("__setattr__" + ' ' + name + ' ' + value)
#    #self.__dict__[name] = value
#    self.name = value

def test_c2():
  tc2 = c2()
  tc2.xiaofei="xiaofei"
  print(dir(tc2))
  import traceback
  traceback.print_exc()
  import inspect
  line = inspect.stack()[1]
  line_info = '[%s:%d:%s]' %(line[1], line[2], line[3])
  print(line_info)


test_c2()
