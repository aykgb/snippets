.PHONY:cc_snippets_test,run,clear,clean

cc_snippets_test: clear
	cd cc/src && mkdir -p build; \
		cd build && cmake .. && make -j 4;
	ln -sf cc/build/cc_snipets_test cc_snippets_test

run:
	./cc_snippets_test

clear:
	rm -f cc_snippets_test

clean: clear
	rm -r src/cc/build
