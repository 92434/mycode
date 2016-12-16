libqrencode/.install: libqrencode/Makefile
	make -C ./$(^:/Makefile=)
	make -C ./$(^:/Makefile=) install

libqrencode/Makefile: libqrencode/configure
	cd ./$(^:/configure=); \
	sh configure --prefix=$$(pwd)/.install;

libqrencode/configure: libqrencode
	cd ./$^; \
	sh autogen.sh;

libqrencode: libqrencode.7z
libqrencode.7z:
	git clone git@github.com:fukuchi/libqrencode.git
	cp -a ./$(@:.7z=) ./$(@:.7z=).bak
	rm ./$(@:.7z=)/.git -rf
	7z a libqrencode.7z ./$(@:.7z=)
	rm ./$(@:.7z=) -rf
	mv ./$(@:.7z=).bak ./$(@:.7z=)

clean:
	-make -C ./libqrencode distclean
	-rm -rf ./libqrencode/.install
