#
#  Makefile for doxygen documentation 
#

GH_PAGES_DIR = ../../../../ev3rt-doc-gh-pages
DOC_FOLDER = ev3rt-doc-cxx-api-en

docs:
	doxygen Doxyfile-CXX-API-en

clean:
	rm -rf ev3rt-doc-cxx-api-en

all: clean \
	docs 

copy:
	cp -rv $(DOC_FOLDER)/ $(GH_PAGES_DIR)/$(DOC_FOLDER)/
	echo "\nDocs path: $(GH_PAGES_DIR)\n"

gh-pages: docs copy clean
