class t_attr():
  def __init__(self):
    self.__dict__["content"] = {}
  
  def __setattr__(self, name, value):
    self.content[name] = value
    print('add:' + str(self.content[name]))
  def __getattr__(self, name):
    print('get:' + str(self.content[name]))
    return self.content[name]

def main():
  t = t_attr()
  t.xiaofei = 'xiaofei'
  t.xiaofei1 = 'xiaofei1'
  print(t.content)  


if __name__ == '__main__':
  main()
