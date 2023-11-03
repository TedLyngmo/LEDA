@echo off
rem usage: config [ztc|wcc|bcc|emx]

copy confdir\msdos\%1\make_lib.bat .
copy confdir\msdos\%1\make.src  src
copy confdir\msdos\%1\make.pro  prog

