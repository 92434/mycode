# -*- coding: utf-8 -*-
from django.shortcuts import render
from django.shortcuts import get_object_or_404, render
from django.http import HttpResponse, Http404
import json
import os

from django.conf import settings
from django.views.decorators.csrf import csrf_exempt 

import logging

logger = logging.getLogger('django.request')

class view_handler(object):
	def __init__(self):
		pass

	def handle_view(self, request, view):
			mname = 'do_' + view
			if not hasattr(self, mname):
				#raise Http404('invalid request!')
				return HttpResponse('error request!')
			else:
				method = getattr(self, mname)
				return method(request, view)

	def do_autocomplete(self, request, view):
		l = [
			"ActionScript",
			"AppleScript",
			"Asp",
			"BASIC",
			"C",
			"C++",
			"Clojure",
			"COBOL",
			"ColdFusion",
			"Erlang",
			"Fortran",
			"Groovy",
			"Haskell",
			"Java",
			"JavaScript",
			"Lisp",
			"Perl",
			"PHP",
			"Python",
			"Ruby",
			"Scala",
			"Scheme"
		]
		return render(request, "%s.html" %(view), {'data' : json.dumps(l)})

	def do_gridview(self, request, view):
		return render(request, "%s.html" %(view))

	def do_serverlist(self, request, view):
		logger.debug(str(request.POST))
		logger.debug(str(request.GET))
		return render(request, "%s.html" %(view))
	def do_list(self, request, view):
		logger.debug(str(request.POST))
		logger.debug(str(request.GET))
		result = [
				{"code":"3","name":"3","price":"3"},
			]
		return HttpResponse(json.dumps(result), content_type='application/json')

	def do_commit(self, request, view):
		logger.debug(str(request.POST))
		logger.debug(str(request.GET))
		print request.GET
		result = {'status' : True}
		return HttpResponse(json.dumps(result), content_type='application/json')

	def do_ajaxadd(self, request, view):
		return render(request, "%s.html" %(view))

	def do_ajaxadd_result(self, request, view):
		a = request.GET['a']
		b = request.GET['b']
		try:
			a = int(a)
			b = int(b)
			return HttpResponse(str(a+b))
		except:
			return HttpResponse('error request!')

	def do_ajaxgetpic_result(self, request, view):
		BASE_DIR = settings.BASE_DIR  # 项目目录
		PICS = os.listdir(os.path.join(BASE_DIR, 'jquery/static/pics'))
		logger.debug('PICS:%s' %(PICS))
		result_list = filter(lambda x: x.endswith('png') or x.endswith('PNG'), PICS)
		logger.debug('result_list:%s' %(result_list))
		logger.debug(dir(request.POST))
		return HttpResponse(json.dumps(result_list), content_type='application/json')

	def do_post_select(self, request, view):
		logger.debug(request.GET)
		return HttpResponse(json.dumps(request.GET.getlist('input_post_select')), content_type='application/json')
		


# Create your views here.
@csrf_exempt
def index(request, view):
	handler = view_handler()
	return handler.handle_view(request, view)
	#return render(request, 'home.html', { 'TutorialList': get_list, 'var':random.uniform(0,100), 'List': json.dumps(List), 'Dict': json.dumps(Dict) })
