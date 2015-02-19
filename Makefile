build:
	make -C ./dep/LuaBinding

install:
	@mv -v ./dep/LuaBinding/libgen.so /lib64/

start:
	@cd exec && bash run.sh