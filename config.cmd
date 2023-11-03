@echo off
rem usage: config [icc|wcc|bcc|emx]

copy confdir\os2\%1\make_lib.cmd .
copy confdir\os2\%1\make.src     src
copy confdir\os2\%1\make.pro     prog
copy confdir\os2\resource\leda.* prog\demo
copy confdir\os2\resource\leda.* prog\window

