include $(default_rules)

CC := gcc

LDFLAGS += -lpthread
LDFLAGS += -ldl

staticd_lib_c_files := sqlite-amalgamation/sqlite3.c
$(staticd_lib_c_files) : sqlite-amalgamation
staticd_lib_name := sqlite3
include $(BUILD_LIB)

shared_lib_c_files := sqlite-amalgamation/sqlite3.c
$(shared_lib_c_files) : sqlite-amalgamation
shared_lib_name := sqlite3
include $(BUILD_SO)

c_files := sqlite-amalgamation/shell.c
sqlite-amalgamation/shell.c.LOCAL_LDFLAGS := -L$(out_dir)/lib
sqlite-amalgamation/shell.c.LOCAL_LDFLAGS += -static
sqlite-amalgamation/shell.c.LOCAL_LDFLAGS += -lsqlite3
include $(BUILD_BINS)

LOCAL_LDFLAGS := -L$(out_dir)/lib
LOCAL_LDFLAGS += -static
LOCAL_LDFLAGS += -lsqlite3
app_c_files := sqlite-amalgamation/shell.c
$(app_c_files) : sqlite-amalgamation
app_name := sqlite3
include $(BUILD_APP)

LOCAL_LDFLAGS :=
app_c_files := sqlite-amalgamation/sqlite3.c
app_c_files += sqlite-amalgamation/shell.c
$(app_c_files) : sqlite-amalgamation
app_name := sqlite3-raw
include $(BUILD_APP)

include $(ADD_TARGET)

sqlite-amalgamation : sqlite-amalgamation.zip
	unzip -d $@ $<
sqlite-amalgamation.zip :
	wget -O $@ http://www.sqlite.org/snapshot/sqlite-amalgamation-201503251735.zip

clean : clean_src
clean_src:
	rm -rf sqlite-amalgamation
test_db:
	-rm ./test.db
	./out/bin/sqlite3 test.db ""
	./out/bin/sqlite3 test.db "create table mytable(id integer primary key, value text)"
	./out/bin/sqlite3 test.db "insert into mytable(id, value) values(1, 'Micheal')"
	./out/bin/sqlite3 test.db "insert into mytable(id, value) values(2, 'Jenny')"
	./out/bin/sqlite3 test.db "insert into mytable(value) values('Francis')"
	./out/bin/sqlite3 test.db "insert into mytable(value) values('Kerk')"
	./out/bin/sqlite3 test.db "select * from mytable"
	./out/bin/sqlite3 -list test.db "select * from mytable"
	./out/bin/sqlite3 -html test.db "select * from mytable"
	./out/bin/sqlite3 -column test.db "select * from mytable"
	./out/bin/sqlite3 -header test.db "select * from mytable"
#	./out/bin/sqlite3 test.db ".mode column"
#	./out/bin/sqlite3 test.db ".header on"
#	./out/bin/sqlite3 test.db "select * from mytable"
	./out/bin/sqlite3 -batch test.db ".mode column" ".header on" "select * from mytable"
	./out/bin/sqlite3 test.db "alter table mytable add column email text not null default 'no email' collate nocase"
	./out/bin/sqlite3 -header test.db "select * from mytable"
	./out/bin/sqlite3 test.db "create view nameview as select * from mytable"
	./out/bin/sqlite3 test.db "create index test_idx on mytable(value)"
	./out/bin/sqlite3 test.db ".schema"
	./out/bin/sqlite3 test.db ".tables"
	./out/bin/sqlite3 test.db ".indices"
#	./out/bin/sqlite3 test.db ".output dump.sql"
#	./out/bin/sqlite3 test.db ".dump"
#	./out/bin/sqlite3 test.db ".output stdout"
	./out/bin/sqlite3 test.db ".dump" >dump.sql
	-rm test.db
	./out/bin/sqlite3 test.db ".read dump.sql"
	-rm test.db
	./out/bin/sqlite3 test.db < dump.sql
#	./out/bin/sqlite3 test.db ".output data.cvs"
#	./out/bin/sqlite3 test.db ".separator"
#	./out/bin/sqlite3 test.db "select * from nameview"
#	./out/bin/sqlite3 test.db ".output stdout"
	./out/bin/sqlite3 -separator "*" test.db "select * from nameview" >dump.cvs
	./out/bin/sqlite3 test.db "create table newtable ( id integer primary key, value text, email text not null )"
	./out/bin/sqlite3 -separator "*" test.db ".import dump.cvs newtable"

mydb:
	./out/bin/sqlite3 mydata "create table if not exists maintable(domain text not null, username text not null, key text not null)"
	./out/bin/sqlite3 mydata "delete from maintable where domain=='www.qq.com' and username=='897388305' and key=='xf22001'"
	./out/bin/sqlite3 mydata "insert into maintable(domain, username, key) values('www.qq.com', '897388305', 'xf22001')"
	./out/bin/sqlite3 mydata "delete from maintable where domain=='www.qq.com' and username=='553514546' and key=='xf22001'"
	./out/bin/sqlite3 mydata "insert into maintable(domain, username, key) values('www.qq.com', '553514546', 'xf22001')"
	./out/bin/sqlite3 -batch mydata ".mode column" ".header on" "select * from maintable"
	./out/bin/sqlite3 -batch mydata ".mode column" ".header on" ".dump"
	./out/bin/sqlite3 -batch mydata "create temporary table if not exists xiaofei(domain text not null, username text not null, key text not null)" ".tables"
	./out/bin/sqlite3 -batch mydata "create temporary table if not exists xixi(domain text not null, username text not null, key text not null)" ".tables"
	./out/bin/sqlite3 -batch mydata "create table if not exists xf22001(domain text not null, username text not null, key text not null)" ".tables"
	./out/bin/sqlite3 -batch mydata "update maintable set username='897388305' where username='897388305'"
	./out/bin/sqlite3 -batch mydata ".mode column" ".header on" "select * from maintable where username like '89%'"
