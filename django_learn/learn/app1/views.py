#coding:utf-8

# Create your views here.

from django.shortcuts import render
from django.http import HttpResponse
import random
import json
import logging

# Get an instance of a logger
logger = logging.getLogger('django.request')
 
def index(request):
	logger.error('Something went wrong!')
	return HttpResponse(u"欢迎光临 自强学堂!!!")

def home(request):
	TutorialList = ["HTML", "CSS", "jQuery", "Python", "Django"]
	def get_list():
		return TutorialList
		#return None
	List = ['自强学堂', '渲染Json到模板']
	Dict = {'site': '自强学堂', 'author': '涂伟忠'}
	return render(request, 'home.html',
			{
			'TutorialList': get_list,
		'var':random.uniform(0,100),
				'List': json.dumps(List),
				'Dict': json.dumps(Dict)
		}
	)

def add(request, a, b):
	logger.info(request.body)
	return HttpResponse(str(int(a) + int(b)))
