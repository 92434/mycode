from django.contrib import admin
from .models import Article

# Register your models here.

class AritcleAdmin(admin.ModelAdmin):
	list_display = ('title', 'was_published_recently', 'pub_date', 'update_time')
	list_filter = ['pub_date']
	search_fields = ['title']

admin.site.register(Article, AritcleAdmin)
