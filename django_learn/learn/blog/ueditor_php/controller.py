# -*- coding: utf-8 -*-
from django.http import HttpResponse, Http404

from . import config

@csrf_exempt
def controller(request):
	"""获取ueditor的后端URL地址	"""

	action=request.GET.get("action","")
	reponseAction={
		"config": action_config,
		"uploadimage": action_upload,
		"uploadscrawl": UploadFile,
		"uploadvideo": UploadFile,
		"uploadfile": UploadFile,
		"catchimage": catcher_remote_image,
		"listimage": list_files,
		"listfile": list_files
	}
	return reponseAction[action](request)

def action_config(request):
	return HttpResponse(json.dumps(config.ueditor_php_config, ensure_ascii=False), content_type="application/javascript")

def action_upload(request):
	"""上传文件"""
	if not request.method=="POST":
		return  HttpResponse(json.dumps(u"{'state:'request method is not POST'}"),content_type="application/javascript")

	state="SUCCESS"
	action=request.GET.get("action")

	#上传文件
	upload_action={
		"uploadimage": {
			'field_name': config.ueditor_php_config['imageFieldName'],
			'config': {

				"pathFormat": config.ueditor_php_config['imagePathFormat'],
				"maxSize": config.ueditor_php_config['imageMaxSize'],
				"allowFiles": config.ueditor_php_config['imageAllowFiles'],
				"oriName": "", 
			},
			"base64": "upload",
		},
		'uploadscrawl': {
			'field_name': config.ueditor_php_config['scrawlFieldName'],
			'config': {

				"pathFormat": config.ueditor_php_config['scrawlPathFormat'],
				"maxSize": config.ueditor_php_config['scrawlMaxSize'],
				"allowFiles": config.ueditor_php_config['scrawlAllowFiles'],
				"oriName": "scrawl.png",
			},
			"base64": "base64",
		},
		"uploadvideo": {
			'field_name': config.ueditor_php_config['videoFieldName'],
			'config': {

				"pathFormat": config.ueditor_php_config['videoPathFormat'],
				"maxSize": config.ueditor_php_config['videoMaxSize'],
				"allowFiles": config.ueditor_php_config['videoAllowFiles'],
				"oriName": "",
			},
			"base64": "upload",
		},
		"uploadfile": {
			'field_name': config.ueditor_php_config['fileFieldName'],
			'config': {

				"pathFormat": config.ueditor_php_config['filePathFormat'],
				"maxSize": config.ueditor_php_config['fileMaxSize'],
				"allowFiles": config.ueditor_php_config['fileAllowFiles'],
				"oriName": "scrawl.png",
			},
			"base64": "upload",
		},
		"catchimage": {
			#'field_name': config.ueditor_php_config['catcherFieldName'],
			'field_name': config.ueditor_php_config['catcherFieldName'],
			'config': {

				"pathFormat": config.ueditor_php_config['catcherPathFormat'],
				"maxSize": config.ueditor_php_config['catcherMaxSize'],
				"allowFiles": config.ueditor_php_config['catcherAllowFiles'],
				"oriName": "",
			},
			"base64": "remote",
		},
	}

	action_info = upload_action[action]

	return do_upload(request, action_info)

def do_upload(request, action_info):
	base64_action = {
		'remote': save_remote,
		'base64': up_base64,
		'upload': upload,
	}

	field_name = action_info['field_name']
	config = action_info['config']
	base64 = action_info['base64']
	return base64_action[base64](request, field_name, config)
