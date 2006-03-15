@echo off
attrib -h library\*.*
attrib -h program\*.*
attrib -r *.*
attrib -r library\*.*
attrib -r program\*.*
attrib -r documentation\*.*
attrib -r uni2uk\*.*
del vssver.scc /s
del mssccprj.scc /s
del *.vspscc /s
del *.ncb /s
del *.trc /s
del *.plg /s
del library\debug\*.*
rd library\debug
del library\releasemindependency\*.*
rd library\releasemindependency
del program\debug\*.*
rd program\debug
del program\release\*.*
rd program\release
del library\usemarcon_library.sln
del program\usemarcon.sln
del Makefile /s
del *.obj /s
del *.o /s
del *.lo /s
del autom4te.cache /s
rd autom4te.cache
del library\.deps /s
rd library\.deps
del library\.libs /s
rd library\.libs
del program\.deps /s
rd program\.deps
del program\.libs /s
rd program\.libs
del config.log
del usemarcon-config
del library\libusemarcon.la