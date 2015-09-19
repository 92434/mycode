from django.db import models
from fields import *
from django.utils import timezone
import datetime

# Create your models here.
class Person(models.Model):
	name = models.CharField(max_length=30)
	age = models.IntegerField()

	def __str__(self):
		return self.name.encode('utf-8')

class Article(models.Model):
	name = models.CharField(max_length=100)
	content = CompressedTextField()
	pub_date = models.DateTimeField('date published')
	labels = ListField()

	def __str__(self):
		return self.name.encode('utf-8')

	def was_published_recently(self):
		#return self.pub_date >= timezone.now() - datetime.timedelta(days=1)
		return timezone.now() - datetime.timedelta(days=1) <= self.pub_date <= timezone.now()
	was_published_recently.admin_order_field = 'pub_date'
	was_published_recently.boolean = True
	was_published_recently.short_description = 'Published recently?'

class Comment(models.Model):
	article = models.ForeignKey(Article)
	content = CompressedTextField()
	votes = models.IntegerField(default=0)

	def __str__(self):# __unicode__ on Python 2
		return self.content.encode('utf-8')
