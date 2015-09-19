#coding:utf-8
from django.contrib import admin
from .models import Person, Article, Comment

#class ChoiceInline(admin.StackedInline):
class ChoiceInline(admin.TabularInline):
	model = Comment
	extra = 0

class AritcleAdmin(admin.ModelAdmin):
	fieldsets = [
		('标题', {'fields': ['name']}),
		('内容', {'fields': ['content']}),
		('标签', {'fields': ['labels']}),
		(
			'Date information',
			{
				'fields': ['pub_date'],
				'classes': ['collapse']
			}
		),
	]
	#list_display = ('question_text', 'pub_date')
	list_display = ('name', 'pub_date', 'was_published_recently')
	list_filter = ['pub_date']
	inlines = [ChoiceInline]
	search_fields = ['name']

# Register your models here.
admin.site.register(Person)
admin.site.register(Article, AritcleAdmin)
