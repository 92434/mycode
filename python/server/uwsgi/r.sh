#!/bin/sh

uwsgi --http :8001 --wsgi-file test.py
