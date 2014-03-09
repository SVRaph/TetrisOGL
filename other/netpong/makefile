CC=g++

INCLUDE_PATH=-I./
LDFLAGS= #-L/usr/lib/ -L/usr/local/lib/
LIBRARY= -lboost_system -lboost_thread -lpthread
LIBRARY_GL= -lglut -lGLU -lGL
WARN = -Wno-deprecated 
CPPFLAGS= ${WARN} ${INCLUDE_PATH}

all:	
	${CC} ${CPPFLAGS} serveur.cpp -o pong_serveur ${LDFLAGS} ${LIBRARY}
	${CC} ${CPPFLAGS} client.cpp -o pong_client  ${LDFLAGS} ${LIBRARY} ${LIBRARY_GL}	
clean:
	/bin/rm -rfv *.o *.exe *~ output/* pong_serveur pong_client
