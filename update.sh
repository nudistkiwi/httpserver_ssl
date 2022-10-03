git pull
(cd library && cd httpserver_ssl && git pull)
(cd ..)
(cd client-ssl && git pull)
(cd .. && cd ..)
(cd build && make -j4)
