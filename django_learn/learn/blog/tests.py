#coding:utf-8
from django.test import TestCase

#from blog.models import Blog
#b = Blog(name='Beatles Blog', tagline='All the latest Beatles news.')
#b.save()
#
## 方法 1
#Author.objects.create(name="WeizhongTu", email="tuweizhong@163.com")
## 方法 2
#twz = Author(name="WeizhongTu", email="tuweizhong@163.com")
#twz.save()
## 方法 3
#twz = Author()
#twz.name="WeizhongTu"
#twz.email="tuweizhong@163.com"
# 
## 方法 4，首先尝试获取，不存在就创建，可以防止重复
#Author.objects.get_or_create(name="WeizhongTu", email="tuweizhong@163.com")
## 返回值(object, True/False)
#
#from blog.models import Entry
#entry = Entry.objects.get(pk=1)
#cheese_blog = Blog.objects.get(name="Cheddar Talk")
#entry.blog = cheese_blog
#entry.save()
#
## Create your tests here.
#Person.objects.all() # 查询所有
#Person.objects.all()[:10] 切片操作，获取10个人，不支持负索引，切片可以节约内存，不支持负索引，后面有相应解决办法，第7条
#Person.objects.get(name="WeizhongTu") # 名称为 WeizhongTu 的一条，多条会报错
# 
#get是用来获取一个对象的，如果需要获取满足条件的一些人，就要用到filter
#Person.objects.filter(name="abc") # 等于Person.objects.filter(name__exact="abc") 名称严格等于 "abc" 的人
#Person.objects.filter(name__iexact="abc") # 名称为 abc 但是不区分大小写，可以找到 ABC, Abc, aBC，这些都符合条件
# 
#Person.objects.filter(name__contains="abc") # 名称中包含 "abc"的人
#Person.objects.filter(name__icontains="abc") #名称中包含 "abc"，且abc不区分大小写
# 
#Person.objects.filter(name__regex="^abc") # 正则表达式查询
#Person.objects.filter(name__iregex="^abc")# 正则表达式不区分大小写
# 
## filter是找出满足条件的，当然也有排除符合某条件的
#Person.objects.exclude(name__contains="WZ") # 排除包含 WZ 的Person对象
#Person.objects.filter(name__contains="abc").exclude(age=23) # 找出名称含有abc, 但是排除年龄是23岁的
#
#import pickle
#
#query = pickle.loads(s)	 # Assuming 's' is the pickled string.
#qs = MyModel.objects.all()
#qs.query = query# Restore the original 'query'.
#
#Author.objects.all().order_by('name')
#Author.objects.all().order_by('-name') # 在 column name 前加一个负号，可以实现倒序
#
#Author.objects.filter(name__contains="WeizhongTu").filter(email="tuweizhong@163.com")
#Author.objects.filter(name__contains="Wei").exclude(email="tuweizhong@163.com")
# 
## 找出名称含有abc, 但是排除年龄是23岁的
#Person.objects.filter(name__contains="abc").exclude(age=23)
#
#Person.objects.all()[:10] 切片操作，前10条
#Person.objects.all().reverse()[:2] # 最后两条
#Person.objects.all().reverse()[0] # 最后一条
# 
## 使用 order_by，在栏目名（column name）前加一个负号
#Author.objects.order_by('-id')[:20] # id最大的20条
#
#qs1 = Pathway.objects.filter(label__name='x')
#qs2 = Pathway.objects.filter(reaction__name='A + B >> C')
#qs3 = Pathway.objects.filter(inputer__name='WeizhongTu')
# 
## 合并到一起
#qs = qs1 | qs2 | qs3
#这个时候就有可能出现重复的
# 
## 去重方法
#qs = qs.distinct()
