# -*- coding: utf-8 -*-
from django.http import HttpResponse, Http404
from django.views.decorators.csrf import csrf_exempt

import os
import json
import urllib
import base64

import config
from utils import *

@csrf_exempt
def controller(request):
	"""获取ueditor的后端URL地址	"""

	action = request.GET.get("action","")
	if not action:
		return  HttpResponse(json.dumps(get_http_response_state(info = 'NO ACTION')), content_type='application/json')

	reponse_action = {
		"config": action_config,
		"uploadimage": action_upload,
		"uploadscrawl": action_upload,
		"uploadvideo": action_upload,
		"uploadfile": action_upload,
		"catchimage": action_upload,
		"listimage": action_list,
		"listfile": action_list,
	}
	return reponse_action.get(action)(request, action)

def action_config(request, action):
	return HttpResponse(json.dumps(config.ueditor_php_config, ensure_ascii=False), content_type="application/javascript")

def action_upload(request, action):
	"""上传文件"""

	state = "SUCCESS"

	#上传文件
	upload_action = {
		"uploadimage": {
			'config': {
				'action_field_name' : 'imageFieldName',
				'action_path_format': 'imagePathFormat',
				'action_max_size': 'imageMaxSize',
				'action_allow_files' : 'imageAllowFiles',
				"oriName": "",
			},
			"base64": "upload",
		},
		'uploadscrawl': {
			'config': {
				'action_field_name' : 'scrawlFieldName',
				'action_path_format': 'scrawlPathFormat',
				'action_max_size': 'scrawlMaxSize',
				'action_allow_files' : 'scrawlAllowFiles',
				"oriName": "scrawl.png",
			},
			"base64": "base64",
		},
		"uploadvideo": {
			'config': {
				'action_field_name' : 'videoFieldName',
				'action_path_format': 'videoPathFormat',
				'action_max_size': 'videoMaxSize',
				'action_allow_files' : 'videoAllowFiles',
				"oriName": "",
			},
			"base64": "upload",
		},
		"uploadfile": {
			'config': {
				'action_field_name' : 'fileFieldName',
				'action_path_format': 'filePathFormat',
				'action_max_size': 'fileMaxSize',
				'action_allow_files' : 'fileAllowFiles',
				"oriName": "",
			},
			"base64": "upload",
		},
		"catchimage": {
			'config': {
				'action_field_name' : 'catcherFieldName',
				'action_path_format': 'catcherPathFormat',
				'action_max_size': 'catcherMaxSize',
				'action_allow_files' : 'catcherAllowFiles',
				"oriName": "",
			},
			"base64": "remote",
		},
	}

	return do_upload(request, upload_action.get(action))

def do_upload(request, action_info):
	base64_action = {
		'remote': catcher_remote_image,
		'base64': up_base64,
		'upload': upload,
	}

	base64 = action_info.get('base64')
	upload_config = action_info.get('config')

	return base64_action[base64](request, upload_config)

def catcher_remote_image(request, upload_config):
	"""远程抓图，当catchRemoteImageEnable:true时，
		如果前端插入图片地址与当前web不在同一个域，则由本函数从远程下载图片到本地
	"""
	state = "SUCCESS"

	if not request.method == "POST":
		return  HttpResponse(json.dumps(get_http_response_state(info = '请求方法错误：不是POST')), content_type='application/json')
	
	action_field_name = upload_config.get('action_field_name')
	field_name = request.GET.get(action_field_name, config.ueditor_php_config.get(action_field_name))
	remote_urls = request.POST.getlist(field_name + "[]", [])

	catcher_infos = []

	for remote_url in remote_urls:
		#http开头验证
		if not remote_url.startswith('http'):
			state = 'ERROR_HTTP_LINK'
			return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')

		action_path_format = upload_config.get('action_path_format')
		config_path_format = config.ueditor_php_config.get(action_path_format)


		remote_file_name = os.path.basename(remote_url)

		remote_original_name, remote_original_ext = os.path.splitext(remote_file_name)

		action_allow_files = upload_config.get('action_allow_files')
		allow_files = request.GET.get(action_allow_files, config.ueditor_php_config.get(action_allow_files))
		if not remote_original_ext in allow_files:
			state = 'ERROR_TYPE_NOT_ALLOWED'
			return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')

		path_format = get_time_format()
		path_format.update(
			{
				"basename":remote_original_name,
				"extname":remote_original_ext[1:],
				"filename":remote_original_name
			}
		)

		base_dir = os.path.join(config.django_settings.BASE_DIR, config.ueditor_php_config.get('static_dir', config.django_settings.STATIC_URL).lstrip('/'))
		path, local_path, local_file_path = get_local_file_path(base_dir, config_path_format, path_format, remote_file_name)
		try:
			if not os.path.exists(local_path):
				os.makedirs(local_path)
		except Exception, E:
			state = 'ERROR_CREATE_DIR'
			return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')



		try:
			remote_image = urllib.urlopen(remote_url)
			 #将抓取到的文件写入文件
			try:
				with open(local_file_path, 'wb') as f:
					f.write(remote_image.read())
			except Exception,E:
				state = 'ERROR_WRITE_CONTENT'
				return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')
		except Exception,E:
			state = 'ERROR_FILE_NOT_FOUND'
			return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')

		file_path = os.path.join(path, remote_file_name)
		catcher_info = get_http_response_state(state = state)
		catcher_info.update(
			{
				"url": urllib.basejoin(config.django_settings.STATIC_URL, file_path.lstrip('/')),
				"size": os.path.getsize(local_file_path),
				"title": os.path.basename(remote_file_name),
				"original": remote_file_name,
				"source": remote_url,
			}
		)

		catcher_infos.append(catcher_info)

	return_info={
		"state": "SUCCESS" if len(catcher_infos) > 0 else "NO CATCHER INFO",
		"list": catcher_infos,
	}

	return HttpResponse(json.dumps(return_info, ensure_ascii = False), content_type="application/javascript")

def up_base64(request, upload_config):
	"""上传文件"""

	state = "SUCCESS"

	if not request.method == "POST":
		return  HttpResponse(json.dumps(get_http_response_state(info = '请求方法错误：不是POST')), content_type='application/json')

	action_field_name = upload_config.get('action_field_name')
	field_name = request.GET.get(action_field_name, config.ueditor_php_config.get(action_field_name))

	#上传涂鸦，涂鸦是采用base64编码上传的，需要单独处理
	upload_file_name = upload_config.get('oriName')
	upload_file_size = 0

	#取得上传的文件的原始名称
	upload_original_name, upload_original_ext = os.path.splitext(upload_file_name)
	path_format = get_time_format()
	path_format.update(
		{
			"basename":upload_original_name,
			"extname":upload_original_ext[1:],
			"filename":upload_original_name,
		}
	)

	#检测保存路径是否存在,如果不存在则需要创建
	base_dir = os.path.join(config.django_settings.BASE_DIR, config.ueditor_php_config.get('static_dir', config.django_settings.STATIC_URL).lstrip('/'))
	action_path_format = upload_config.get('action_path_format')
	config_path_format = config.ueditor_php_config.get(action_path_format)
	#取得输出文件的路径
	path, local_path, local_file_path = get_local_file_path(base_dir, config_path_format, path_format, upload_file_name)
	try:
		if not os.path.exists(local_path):
			os.makedirs(local_path)
	except Exception, E:
		state = 'ERROR_CREATE_DIR'
		return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')

	#所有检测完成后写入文件
	content = request.POST.get(field_name)

	try:
		with open(local_file_path, 'wb') as f:
			f.write(base64.decodestring(content))
	except Exception,E:
		state = "ERROR_WRITE_CONTENT"
		return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')

	#返回数据
	file_path = os.path.join(path, upload_file_name)
	return_info = get_http_response_state(state = state)
	return_info.update(
		{
			'url': urllib.basejoin(config.django_settings.STATIC_URL, file_path.lstrip('/')),#保存后的文件名称
			'original': upload_file_name,#原始文件名
			'type': upload_original_ext,
			'size': upload_file_size
		}
	)

	return HttpResponse(json.dumps(return_info, ensure_ascii = False), content_type = "application/javascript")

def upload(request, upload_config):
	"""上传文件"""

	state = "SUCCESS"

	if not request.method == "POST":
		return  HttpResponse(json.dumps(get_http_response_state(info = '请求方法错误：不是POST')), content_type='application/json')

	action_field_name = upload_config.get('action_field_name')
	field_name = request.GET.get(action_field_name, config.ueditor_php_config.get(action_field_name))

	#取得上传的文件
	file = request.FILES.get(field_name)
	if file is None:
		state = 'NO_FILES'
		return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type = 'application/json')
	upload_file_name = file.name
	upload_file_size = file.size

	#取得上传的文件的原始名称
	upload_original_name, upload_original_ext = os.path.splitext(upload_file_name)
	path_format = get_time_format()
	path_format.update(
		{
			"basename":upload_original_name,
			"extname":upload_original_ext[1:],
			"filename":upload_original_name,
		}
	)

	#文件类型检验
	action_allow_files = upload_config.get('action_allow_files')
	allow_files = request.GET.get(action_allow_files, config.ueditor_php_config.get(action_allow_files))

	if not upload_original_ext in allow_files:
		state = 'ERROR_TYPE_NOT_ALLOWED'
		return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')

	action_max_size = upload_config.get('action_max_size')
	max_size = request.GET.get(action_max_size, config.ueditor_php_config.get(action_max_size))

	#大小检验
	if upload_file_size != 0:
		MF = file_size(max_size)
		if upload_file_size > MF.size:
			state = 'MAX_FILE_SIZE_EXCEED'
			return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')


	#检测保存路径是否存在,如果不存在则需要创建
	base_dir = os.path.join(config.django_settings.BASE_DIR, config.ueditor_php_config.get('static_dir', config.django_settings.STATIC_URL).lstrip('/'))
	action_path_format = upload_config.get('action_path_format')
	config_path_format = config.ueditor_php_config.get(action_path_format)
	#取得输出文件的路径
	path, local_path, local_file_path = get_local_file_path(base_dir, config_path_format, path_format, upload_file_name)
	try:
		if not os.path.exists(local_path):
			os.makedirs(local_path)
	except Exception, E:
		state = 'ERROR_CREATE_DIR'
		return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')

	#所有检测完成后写入文件
	try:
		with open(local_file_path, 'wb') as f:
			for chunk in file.chunks():
				f.write(chunk)
	except Exception,E:
		state = "ERROR_WRITE_CONTENT"
		return  HttpResponse(json.dumps(get_http_response_state(state = state)), content_type='application/json')

	#返回数据
	file_path = os.path.join(path, upload_file_name)
	return_info = get_http_response_state(state = state)
	return_info.update(
		{
			'url': urllib.basejoin(config.django_settings.STATIC_URL, file_path.lstrip('/')),#保存后的文件名称
			'original': upload_file_name,#原始文件名
			'type': upload_original_ext,
			'size': upload_file_size
		}
	)

	return HttpResponse(json.dumps(return_info, ensure_ascii = False), content_type = "application/javascript")

def action_list(request, action):
	"""列出文件"""

	list_action = {
		"listfile": {
			'config': {
				'action_manager_list_size' : 'fileManagerListSize',
				'action_manager_list_path' : 'fileManagerListPath',
				'action_manager_allow_files': 'fileManagerAllowFiles',
			},
		},
		'listimage': {
			'config': {
				'action_manager_list_size': 'imageManagerListSize',
				'action_manager_list_path': 'imageManagerListPath',
				'action_manager_allow_files': 'imageManagerAllowFiles',
			},
		},
	}

	return do_list(request, list_action.get(action))

def do_list(request, action_info):

	state = "SUCCESS"
	if not request.method == "GET":
		return  HttpResponse(json.dumps(get_http_response_state(info = '请求方法错误：不是GET')), content_type='application/json')

	list_config = action_info.get('config')

	#取得动作
	action_manager_list_size = list_config.get('action_manager_list_size')
	list_size = request.GET.get(action_manager_list_size, config.ueditor_php_config.get(action_manager_list_size))
	action_manager_list_path = list_config.get('action_manager_list_path')
	list_path = request.GET.get(action_manager_list_path, config.ueditor_php_config.get(action_manager_list_path))
	action_manager_allow_files = list_config.get('action_manager_allow_files')
	allow_files = request.GET.get(action_manager_list_path, config.ueditor_php_config.get(action_manager_allow_files))

	#取得参数
	list_size = long(request.GET.get("size", list_size))
	list_start = long(request.GET.get("start", 0))

	
	base_dir = os.path.join(config.django_settings.BASE_DIR, config.ueditor_php_config.get('static_dir', config.django_settings.STATIC_URL).lstrip('/'))
	root_path = os.path.join(base_dir, list_path.lstrip('/'))
	files = list_get_files(base_dir, list_path, allow_files)

	#返回数据
	if len(files) == 0:
		return_info = get_http_response_state(state = 'ERROR_FILE_NOT_FOUND')
		return_info.update(
			{
				"list": [],
				"start": list_start,
				"total": 0,
			}
		)
	else:
		return_info = get_http_response_state(state = 'SUCCESS')
		return_info.update(
			{
				"list": files[list_start : list_start + list_size],
				"start": list_start,
				"total": len(files)
			}
		)

	
	return HttpResponse(json.dumps(return_info), content_type="application/javascript")
