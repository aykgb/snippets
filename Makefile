.PHONY:init,cc_snippets_test,run,clear,clean

cc_snippets_test: clear
	cd cxx && mkdir -p build; \
		cd build && cmake .. && make -j 4;
	ln -sf cxx/build/cc_snippets_test cc_snippets_test

run:
	./cc_snippets_test

clear:
	rm -f cc_snippets_test

clean: clear
	rm -r cxx/build
