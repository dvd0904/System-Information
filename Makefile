CC=gcc
CXX=g++-posix
MING_BASE:=i686-w64-mingw32-
MINGW_HOST="i686-w64-mingw32"

CMAKE_OPTS=-DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=${MING_BASE}${CC} -DCMAKE_CXX_COMPILER=${MING_BASE}${CXX}
WIN_CMAKE_RULES+=win32/sysinfo
WIN_CMAKE_RULES+=win32/shared_modules

SYSINFO=data_provider/
SHARED_MODULES=shared_modules/
EXTERNAL_JSON=external/cJSON/
EXTERNAL_NLOHMANN=external/nlohmann/

AR 		:= ar
LINK	:= ${AR} -rcs
RANLIB  := ranlib
SI_CC = ${MING_BASE}${CC}
SI_CXX = ${MING_BASE}${CXX}
CFLAGS:=-I./ -I$(EXTERNAL_JSON) -I${SHARED_MODULES}common -I${SHARED_MODULES}utils -I${SYSINFO}include -I${EXTERNAL_NLOHMANN}
CFLAGS +=-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Woverloaded-virtual -Wunused -Wcast-align -Wformat=2 -std=c++14 -static -static-libgcc -static-libstdc++
LDFLAGS:= -lpsapi -liphlpapi -lws2_32

# cd data_provider/ && mkdir -p build && cd build && cmake -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++-posix    .. && make

cjson_c := ${EXTERNAL_JSON}cJSON.c
cjson_o := $(cjson_c:.c=.o)

JSON_LIB   	 = $(EXTERNAL_JSON)libcjson.a

$(JSON_LIB): ${cjson_o}
	${LINK} $@ $^
	${RANLIB} $@

${EXTERNAL_JSON}%.o: ${EXTERNAL_JSON}%.c
	${SI_CC} ${CFLAGS} -fPIC -c $^ -o $@

data_provider_src_cc := $(wildcard data_provider/src/*.cc)
data_provider_src_o := $(data_provider_src_cc:.cc=.o)

data_provider/src/%.o: data_provider/src/%.cc 
	${SI_CXX} ${CFLAGS} -fPIC -c $^ -o $@

dai: ${data_provider_src_o}

settings:
	echo ${CFLAGS}

.PHONY: main.exe

main.exe: ${data_provider_src_o} $(JSON_LIB) main.cc libgcc_s_sjlj-1.dll
	${SI_CXX} ${CFLAGS} $^ -o $@ ${LDFLAGS}


libgcc_s_sjlj-1.dll: $(wildcard /usr/lib/gcc/i686-w64-mingw32/*-posix/libgcc_s_sjlj-1.dll)
	cp $< $@

clean:
	rm -rf ${cjson_o}
	rm -rf ${JSON_LIB}
	rm -rf ${data_provider_src_o}
	rm -rf main.exe