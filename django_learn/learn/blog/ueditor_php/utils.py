#coding:utf-8

import datetime
import random

def get_time_format(dt = None):
	if not isinstance(dt, datetime.datetime):
		dt = datetime.datetime.now()

	return {
		"year": dt.strftime("%Y"),
		"month": dt.strftime("%m"),
		"day": dt.strftime("%d"),
		"hour": dt.strftime("%H"),
		"min": dt.strftime("%M"),
		"sec": dt.strftime("%S"),
		"microsec": "%06d" % dt.microsecond,
		"date": dt.strftime("%Y%m%d"),

		"time": dt.strftime("%H%M%S"),
		"datetime": dt.strftime("%Y%m%d%H%M%S"),
		"rnd": random.randrange(000,999),
	}


