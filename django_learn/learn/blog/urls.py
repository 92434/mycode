from django.conf.urls import url
from learn import settings
import os

urlpatterns = [
	url(r'^$', 'blog.views.index', name = 'index'),
	url(r'^controller', 'blog.ueditor_php.controller.controller', name = 'controller'),
]
