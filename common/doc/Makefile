#
#  Makefile for doxygen documentation 
#

GH_PAGES_DIR = ../../../../ev3rt-doc-gh-pages

docs:
	doxygen Doxyfile-C-API-en
	doxygen Doxyfile-C-API-jp
	doxygen Doxyfile-CXX-API-en

clean:
	rm -rf ev3rt-doc-c-api-en
	rm -rf ev3rt-doc-c-api-jp
	rm -rf ev3rt-doc-cxx-api-en

all: clean \
	docs 

copy:
	rm -rf $(GH_PAGES_DIR)
	mkdir $(GH_PAGES_DIR)
	cp index.html $(GH_PAGES_DIR)
	cp -rv ev3rt-doc-c-api-en/ $(GH_PAGES_DIR)/ev3rt-doc-c-api-en/
	cp -rv ev3rt-doc-c-api-jp/ $(GH_PAGES_DIR)/ev3rt-doc-c-api-jp/
	cp -rv ev3rt-doc-cxx-api-en/ $(GH_PAGES_DIR)/ev3rt-doc-cxx-api-en/
	@cd $(GH_PAGES_DIR)
	@pwd
	@echo -e "\nDocs path:"
	@cd -
	@echo -e "\n"

gh-pages: docs copy clean
