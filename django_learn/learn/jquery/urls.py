from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^([^/]+)/$', 'jquery.views.index', name='jquery'),
    url(r'^serverlist/servlet/([^/]+)$', 'jquery.views.index', name='serverlet'),
]
