#
#  Makefile for doxygen documentation 
#

GH_PAGES_DIR = ../../../../ev3rt-doc-gh-pages

docs:
	doxygen Doxyfile-C-API-en
	doxygen Doxyfile-C-API-jp

clean:
	rm -rf ev3rt-doc-c-api-en
	rm -rf ev3rt-doc-c-api-jp

all: clean \
	docs 

copy:
	rm -rf $(GH_PAGES_DIR)
	mkdir $(GH_PAGES_DIR)
	cp index.html $(GH_PAGES_DIR)
	cp -rv ev3rt-doc-c-api-en/ $(GH_PAGES_DIR)/ev3rt-doc-c-api-en/
	cp -rv ev3rt-doc-c-api-jp/ $(GH_PAGES_DIR)/ev3rt-doc-c-api-jp/
	echo "\nDocs path: $(GH_PAGES_DIR)\n"

gh-pages: docs copy clean
