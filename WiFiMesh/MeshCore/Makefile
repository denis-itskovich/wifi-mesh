SOURCES =			\
	src/Descriptors.c \
    src/List.c \
    src/Log.c \
    src/Packet.c \
    src/Routing.c \
    src/Scheduler.c \
    src/Settings.c \
    src/Simulator.c \
    src/SortedList.c \
    src/Station.c \
    src/Statistics.c \
    src/Status.c \
    src/TimeLine.c
		
OUTDIR = 			$(OUTDIR_$(CONFIG))
INTDIR = 			$(OUTDIR)
DEPS =				${addprefix $(OUTDIR)/,${notdir $(SOURCES:.c=.d)}}
OBJS =				$(DEPS:.d=.o)
LIB_TARGET = 		$(OUTDIR)/libMeshCore.a
TARGET =			$(OUTDIR)/MeshCore.exe

OUTDIR_DEBUG =		Debug
OUTDIR_RELEASE =	Release

CFLAGS_DEBUG = 		-O0 -g3 -ggdb -D_DEBUG
CFLAGS_RELEASE = 	-O3 -D__NO_LOG

LDFLAGS_DEBUG =
LDFLAGS_RELEASE =	"-Wl,-s,-x"


AR =				ar
CC =				gcc
CFLAGS = 			$(CFLAGS_$(CONFIG)) -Wall -Werror -c -fmessage-length=0 -MMD -MP -o"$(__object)" "$(__source)" -MT"$(__dep)" -MF"$(__dep)"
LDFLAGS = 			$(LDFLAGS_$(CONFIG)) $(LIBS) -o $(TARGET)
LIBS =				-lMeshCore -L"$(OUTDIR)"

__source =			$(1)
__object =			$(INTDIR)/${notdir $(__source:.c=.o)}
__dep =				$(__object:.o=.d)

-include $(DEPS)

all : debug release

define Compiler
$(__object) : $(__source)
	@echo ${notdir $(__source)}
	@$(CC) $(CFLAGS)
endef

${foreach src,$(SOURCES),${eval ${call Compiler,$(src)}}}

build_config : $(TARGET)

clean_config :
	@$(RM) $(OBJS) $(DEPS) $(TARGET) $(LIB_TARGET)

start_build:
	@echo Compiling... 
	
$(LIB_TARGET) : start_build $(OBJS)
	@echo Building static library $(LIB_TARGET)
	@$(AR) -rsc $@ $(OBJS)
	
$(TARGET) : $(LIB_TARGET)
	@echo Building executable $(TARGET)
	@$(CC) ${call CFLAGS,src/Main.c}
	@$(CC) $(OUTDIR)/Main.o $(LDFLAGS) -o $(TARGET)

debug :
	@$(MAKE) --no-print-directory build_config CONFIG=DEBUG 
	
release :
	@$(MAKE) --no-print-directory build_config CONFIG=RELEASE
	
clean :
	@$(MAKE) clean_config CONFIG=DEBUG
	@$(MAKE) clean_config CONFIG=RELEASE

.PHONY: release
.PHONY: debug
.PHONY: clean_config

