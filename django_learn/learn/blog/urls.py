from django.conf.urls import url
from learn import settings
import os

urlpatterns = [
	#url(r'^static/(?P<path>.*)$', 'django.views.static.serve', {'document_root': settings.STATIC_URL}),
	#url(r'^ueditor/(?P<path>.*)$', 'django.views.static.serve', {'document_root': os.path.join(settings.BASE_DIR, 'blog/static/ueditor')}),
	url(r'^$', 'blog.views.index', name = 'index'),
	url(r'^controller', 'blog.views.get_ueditor_controller', name = 'controller'),
	url(r'^static/(?P<path>.*)$', 'django.views.static.serve', {'document_root': os.path.join(settings.BASE_DIR, 'blog/static')}),
]
