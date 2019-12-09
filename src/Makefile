# --------------------------------------------------------------------------
# VERBOSE Compile Information
ifdef VERBOSE
        VERBOSE = true
        VERBOSE_ECHO = @ echo
        VERBOSE_SHOW =
        QUIET_ECHO = @ echo > /dev/null
else
        VERBOSE = false
        VERBOSE_ECHO = @ echo > /dev/null
        VERBOSE_SHOW = @
        QUIET_ECHO = @ echo
endif


# --------------------------------------------------------------------------
# BUILD directory
ifndef BUILD
    ifdef DEBUG
        BUILD := build-debug
    else
        BUILD := build
    endif
endif

# --------------------------------------------------------------------------
# These CFLAGS assume a GNU compiler.  For other compilers, write a script
# which converts these arguments into their equivalent for that particular
# compiler.

CFLAGS := std=c++11 -fvisibility=hidden

CFLAGS += -O2

LDFLAGS = -lpthread -fopenmp

# --------------------------------------------------------------------------
# Default targets are everything

.PHONY: all
all: kruskal_seq

# --------------------------------------------------------------------------
# Compile target patterns

$(BUILD)/obj/%.o: mst/%.cpp
	$(QUIET_ECHO) $@: Compiling object
	@ mkdir -p $(dir $(BUILD)/dep/$<)
	@ g++ $(CFLAGS) -M -MG -MQ $@ -DCOMPILINGDEPENDENCIES \
        -o $(BUILD)/dep/$(<:%.cpp=%.d) -c $<
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ $(CFLAGS) -o $@ -c $<

$(BUILD)/obj/%.o: utils/%.cpp
	$(QUIET_ECHO) $@: Compiling object
	@ mkdir -p $(dir $(BUILD)/dep/$<)
	@ g++ $(CFLAGS) -M -MG -MQ $@ -DCOMPILINGDEPENDENCIES \
       -o $(BUILD)/dep/$(<:%.cpp=%.d) -c $<
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ $(CFLAGS) -o $@ -c $<

# --------------------------------------------------------------------------
# kruskal_seq targets

.PHONY: kruskal_seq
kruskal_seq: $(BUILD)/bin/kruskal_seq

KRUSKAL_SEQ_OBJS = $(BUILD)/obj/kruskal_seq.o \
				   $(BUILD)/obj/disjoint_set.o \
				   $(BUILD)/obj/graph.o
#You can append other objects

$(BUILD)/bin/kruskal_seq: $(KRUSKAL_SEQ_OBJS)
	$(QUIET_ECHO) $@: Building executable
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ -o $@ $^ $(LDFLAGS)
