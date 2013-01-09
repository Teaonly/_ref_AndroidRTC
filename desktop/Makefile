TALK_INC_DIR = ./
EXPAT_INC_DIR = /usr/include
SSL_INC_DIR =  /usr/include/openssl
OBJS_DIR = ./obj

AR = ar
CC = g++
CFLAGS = -Wall -fno-rtti -fno-exceptions -fvisibility=hidden -ffunction-sections -fdata-sections -gdwarf-2 -g3 -O0 -DHAMMER_TIME=1 -DLOGGING=1 -DFEATURE_ENABLE_SSL -DHASHNAMESPACE=__gnu_cxx -DPOSIX -DDISABLE_DYNAMIC_CAST -DHAVE_OPENSSL_SSL_H=1 -D_REENTRANT -DPOSOIX -D_DEBUG -DEXPAT_RELATIVE_PATH -DLINUX -I$(TALK_INC_DIR) -I$(EXPAT_INC_DIR) -I$(SSL_INC_DIR)
LDFLAGS = -lexpat -lssl -lcrypto -lpthread -lrt

LIB_SRCS =
include libjingle_build.mk
#LIB_OBJS = ${LIB_SRCS:.cc=.o}
LIB_OBJS = ${LIB_SRCS:%.cc=$(OBJS_DIR)/%.o}


APP_SRCS =
include app_build.mk
APP_OBJS_ = ${APP_SRCS:%.cpp=$(OBJS_DIR)/%.o}
APP_OBJS = ${APP_OBJS_:%.cc=$(OBJS_DIR)/%.o}

LIB = libjingle.a
TARGET = SimpleRTC

$(OBJS_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS_DIR)/%.o : %.cc
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(LIB_OBJS)
	$(AR) -cr $@ $(LIB_OBJS)

$(TARGET): $(LIB) $(APP_OBJS)
	$(CC) -o $@ $(APP_OBJS) $(LIB) $(LDFLAGS)

.PHONY: clean all
all: $(TARGET)
clean:
	rm -f $(TARGET)
	rm -f $(LIB)
	rm -rf $(OBJS_DIR)
