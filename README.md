# Android Backup Extractor
Android backup extractor (based on Qt)

##Dependencies
####zib
Download [zlib] and unpack it to C:\Qt\zlib-1.2.8 (or any path you prefer).

####Make zlib
	cd C:\Qt\zlib-1.2.8
	mingw32-make -f win32/Makefile.gcc

####.pro files
If you've changed zlib directory path, you also need to fix this two lines in .pro file.

	INCLUDEPATH += C:/Qt/zlib-1.2.8
	LIBS += -LC:/Qt/zlib-1.2.8 -lz
	
##Todo
- Background data processing
- Support for **not** compressed backup files

##License
**MIT**


[zlib]:http://zlib.net/zlib128.zip
