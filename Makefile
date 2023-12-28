# use pkg-config for getting CFLAGS and LDLIBS
LIBS=opencv4

CFLAGS += -g
CFLAGS := $(shell pkg-config --cflags $(LIBS)) $(CFLAGS)
LDLIBS := $(shell pkg-config --libs $(LIBS)) $(LDLIBS)

SOURCEDIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))/build
SOURCES   := $(shell find $(SOURCEDIR) -name '*.cpp')

# the following examples make explicit use of the math library
avcodec:           LDLIBS += -lm
encode_audio:      LDLIBS += -lm
mux:               LDLIBS += -lm
resample_audio:    LDLIBS += -lm

.phony: all clean-test clean

copysrc:
	- [ -e build/ ] && rm -rf build/
	mkdir -p build/

	cp -r libs/wsserver/wsserver build/
	cp -r libs/wsvideo/wsvideo build/
	cp -r wsvideo-ffmpeg build/

	cp $(EXAMPLE) build/
compile:
	objects=""; \
	for file in $(SOURCES); do \
		echo $$file; \
		echo "$${file%.cpp}.o"; \
		$(COMPILER) -o "$${file%.cpp}.o" -c -x c++ -I./build $(CFLAGS) $$file; \
		objects="$$objects $${file%.cpp}.o"; \
	done; \
	$(COMPILER) $$objects $(LDLIBS) -o build/out
	mv build/out out
build:
	echo $(LDLIBS)
	echo $(CFLAGS)
	make -B copysrc EXAMPLE="$(EXAMPLE)"
	make -B compile COMPILER="$(COMPILER)"

clean-test:
	$(RM) test*.pgm test.h264 test.mp2 test.sw test.mpg

clean: clean-test
	$(RM) $(EXAMPLES) $(OBJS)
