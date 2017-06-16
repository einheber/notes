all: pistache pistache-lib rapidjson rapidjson-lib nserver

nserver: main.cpp handler.cpp
	g++ main.cpp handler.cpp -std=c++11 -Lpistache/build/src -lnet -o nserver

pistache:
	git clone https://github.com/oktal/pistache.git
	git submodule update --init

pistache-lib: pistache/build/src/libnet.so

pistache/build/src/libnet.so:
	cd pistache; mkdir -p build; cd build; cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..; make

rapidjson:
	git clone https://github.com/miloyip/rapidjson.git
	git submodule update --init

rapidjson-lib: rapidjson/build/bin/capitalize

rapidjson/build/bin/capitalize:
	cd rapidjson; mkdir -p build; cd build; cmake ..; make
clean:
	rm -f nserver
	rm -rf pistache
	rm -rf rapidjson
