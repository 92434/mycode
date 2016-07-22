clean:
	find . -name "*.pyc" | xargs rm;
	rm db.sqlite3 debug.log

run:
	python manage.py runserver 0.0.0.0:8001

shell:
	python manage.py $@
