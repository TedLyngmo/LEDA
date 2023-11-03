cd src\basic
wmake -u -i -f ../make.src L=libL
cd ..\basic_alg
wmake -u -i -f ../make.src L=libL
cd ..\numbers
wmake -u -i -f ../make.src L=libL
cd ..\dict
wmake -u -i -f ../make.src L=libL
cd ..\prio
wmake -u -i -f ../make.src L=libL
cd ..\graph
wmake -u -i -f ../make.src L=libG
cd ..\graph_alg
wmake -u -i -f ../make.src L=libG
cd ..\plane
wmake -u -i -f ../make.src L=libP
cd ..\plane_alg
wmake -u -i -f ../make.src L=libP
cd ..\window
wmake -u -i -f ../make.src L=libP
cd ..\os2
wmake -u -i -f ../make.src L=libWx
cd ../..
