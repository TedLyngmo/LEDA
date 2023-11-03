cd src\basic
make -i -f ..\make.src L=libL
cd ..\basic_alg
make -i -f ..\make.src L=libL
cd ..\numbers
make -i -f ..\make.src L=libL
cd ..\dict
make -i -f ..\make.src L=libL
cd ..\prio
make -i -f ..\make.src L=libL
cd ..\graph
make -i -f ..\make.src L=libG
cd ..\graph_alg
make -i -f ..\make.src L=libG
cd ..\plane
make -i -f ..\make.src L=libP
cd ..\plane_alg
make -i -f ..\make.src L=libP
cd ..\window
make -i -f ..\make.src L=libP
cd ..\os2
make -i -f ..\make.src L=libWx
cd ../..
