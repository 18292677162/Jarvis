src = $(wildcard *.cc)
obj = $(patsubst %.cc, %, $(src))

targe1 = jsonDeserial
targe2 = jsonSerial

all:$(targe1) $(targe2)

$(targe1):jsonDeserial.cc
	g++ $^ -o $@ -ljsoncpp -std=c++11

$(targe2):jsonSerial.cc
	g++ $^ -o $@ -ljsoncpp -std=c++11

clean:
	rm -rf $(obj)

.PHONY:clean all
