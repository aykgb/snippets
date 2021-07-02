.PHONY:all

all:cc_snippets_test run

rocksdb:
	@cd third_party/rocksdb && make -j 8 static_lib

BRPC_TARGET=brpc-shared
brpc:
	@cd third_party/brpc && mkdir -p build && cd build && cmake .. -DWITH_GLOG=ON && make -j 8 ${BRPC_TARGET}

dependency: rocksdb brpc

gen_proto:
	@cd cxx && cp -ur ../proto . && cd proto && protoc *.proto --proto_path=. --cpp_out=.

cc_snippets_test: clear
	@cd cxx && mkdir -p build && cd build && cmake .. && make -j 8
	@ln -sf cxx/build/cc_snippets_test cc_snippets_test

clear:
	@rm -f cc_snippets_test

run:
	./cc_snippets_test

lint:
	@git ls-files | egrep '.*\.(h|cc)' | grep -v trash | xargs cpplint --filter=-legal/copyright,-build/c++11,-build/header_guard

format:
	@git ls-files | egrep '.*\.(h|cc)' | grep -v trash | xargs clang-format -style=google -i

clear:
	rm -f cc_snippets_test

.PHONY:clean
clean:
	@rm -r cxx/build
