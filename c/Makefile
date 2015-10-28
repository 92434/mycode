include $(default_rules)

#local_lib_src := $(top)/utils
#
#CFLAGS += -I$(local_lib_src)
#CFLAGS += -DLOOPER_USES_EPOLL
#LDFLAGS += -L$(local_lib_src)/$(out_dir)/lib
#
#LDFLAGS += -static
#LDFLAGS += -lrt
#LDFLAGS += -lpthread
#LDFLAGS += -lxiaofei
#
#c_files := c/learn_c.cpp
#c_files += thread/thread_test.cpp
#c_files += thread/pthreads_sched_test.cpp
#c_files += io/mypipe.cpp
#c_files += io/aiotest.cpp
#c_files += io/test_string_op.cpp
#c_files += io/simple_server.cpp
#c_files += io/tcp_server_in.cpp
#c_files += io/tcp_client_in.cpp
#c_files += io/udp_server.cpp
#c_files += io/udp_client.cpp
#c_files += io/tcp_server_un.cpp
#c_files += io/tcp_client_un.cpp
#c_files += io/inet_interface.cpp
#c_files += io/test_file_process.cpp
#
#CFLAGS += -I/usr/local/include/libxml2
#CFLAGS += -I/usr/include/libxml2
#LDFLAGS += -L/usr/local/lib
#LDFLAGS += -lxml2 -lz
#c_files += io/xml/CreateXmlFile.cpp
#c_files += io/xml/ParseXmlFile.cpp
##c_files += io/xml/wxb_codeConv.c
#c_files += io/xml/CreateXmlFile_cn.cpp
#c_files += io/xml/tree2.c
#
#c_files += algorithm/rbtree.cpp
#c_files += algorithm/test_sort.cpp
#
#c_files += system/test_getopt.cpp
#c_files += system/test_optparse.cpp
#
#c_files += gfw_dns_resolver/gfw_dns_resolver.c
##c_files += gfw_dns_resolver/my_dns_resolver.cpp
#
#include $(BUILD_BINS)
#
#app_c_files := io/io_epoll_test.cpp
#app_c_files += io/poll_thread.cpp
#app_c_files += io/io_thread.cpp
#app_name := poll_test
#include $(BUILD_APP)
#
#app_c_files := io/net_server_test.cpp
#app_c_files += io/poll_thread.cpp
#app_c_files += io/io_thread.cpp
#app_c_files += io/net_server.cpp
#app_name := net_server_test
#include $(BUILD_APP)
#
#java_files := gfw_dns_resolver/GFWDnsResolver.java
#java_files += io/xml/XmlSaxTest.java
#include $(BUILD_CLASS)
#
#$(target_files) : $(local_lib_src)/$(out_dir)/lib/libxiaofei.a
include $(ADD_TARGET)

SUB_DIRS :=
SUB_DIRS += test_daemon
SUB_DIRS += test_poll
SUB_DIRS += c
SUB_DIRS += io
SUB_DIRS += system
