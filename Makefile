# use pkg-config for getting CFLAGS and LDLIBS
FFMPEG_LIBS=    libavdevice                        \
                libavformat                        \
                libavfilter                        \
                libavcodec                         \
                libswresample                      \
                libswscale                         \
                libavutil                          \

CFLAGS += -g
CFLAGS := $(shell pkg-config --cflags $(FFMPEG_LIBS)) $(CFLAGS)
LDLIBS := $(shell pkg-config --libs $(FFMPEG_LIBS)) $(LDLIBS)

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

	cp entry.cpp build/
compile:
	g++ -o build/out  $(CFLAGS) $(LDLIBS) -I./build $(SOURCES)
	mv build/out out
build:
	make -B copysrc
	make -B compile

clean-test:
	$(RM) test*.pgm test.h264 test.mp2 test.sw test.mpg

clean: clean-test
	$(RM) $(EXAMPLES) $(OBJS)
