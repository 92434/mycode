#coding:utf-8
from django.db import models
from django.utils import timezone
import datetime

# Create your models here.
class Blog(models.Model):
	name = models.CharField(max_length=100)
	tagline = models.TextField()
 
	def __unicode__(self):  # __str__ on Python 3
		return self.name
 
class Author(models.Model):
	name = models.CharField(max_length=50)
	email = models.EmailField()
 
	def __unicode__(self):  # __str__ on Python 3
		return self.name
 
class Entry(models.Model):
	blog = models.ForeignKey(Blog)
	headline = models.CharField(max_length=255)
	body_text = models.TextField()
	pub_date = models.DateField()
	mod_date = models.DateField()
	authors = models.ManyToManyField(Author)
	n_comments = models.IntegerField()
	n_pingbacks = models.IntegerField()
	rating = models.IntegerField()
 
	def __unicode__(self):  # __str__ on Python 3
		return self.headline

class Article(models.Model):
	title = models.CharField(u'标题', max_length=256)
	content = models.TextField(u'内容')
 
	pub_date = models.DateTimeField(u'发表时间', auto_now_add=True, editable = True)
	update_time = models.DateTimeField(u'更新时间',auto_now=True, null=True)

	def was_published_recently(self):
		#return self.pub_date >= timezone.now() - datetime.timedelta(days=1)
		return timezone.now() - datetime.timedelta(days=1) <= self.pub_date <= timezone.now()
	was_published_recently.admin_order_field = 'pub_date'
	was_published_recently.boolean = True
	was_published_recently.short_description = 'Published recently?'

	def __str__(self):  # __str__ on Python 3
		return self.title.encode('utf-8')
