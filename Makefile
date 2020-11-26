.PHONY:rocksdb,brpc,cc_snippets_test,run,clear,clean

rocksdb:
	cd third_party/rocksdb && make -j 8 static_lib

brpc:
	cd third_party/brpc && mkdir -p build && cmake build && make -j 8 -C build
	cd third_party/brpc && cp -ruf build/output/* .

cc_snippets_test: clear
	cd cxx && mkdir -p build; \
		cd build && cmake .. && make -j 4;
	ln -sf cxx/build/cc_snippets_test cc_snippets_test

run:
	./cc_snippets_test

lint:
	@git ls-files | egrep '.*\.(h|cc)' | grep -v trash | xargs cpplint --filter=-legal/copyright,-build/c++11,-build/header_guard

format:
	@git ls-files | egrep '.*\.(h|cc)' | grep -v trash | xargs clang-format -style=google -i

clear:
	rm -f cc_snippets_test

clean: clear
	rm -r cxx/build
