def test():
  print '__file__ is ' + __file__
  print '__name__ is ' + __name__
  import os
  path = os.path.join(os.getcwd(), '.git', 'repo' + 'main.py')
  print 'path is ' + path
  print 'path.__class__ is ' + str(path.__class__)
  print path.__class__