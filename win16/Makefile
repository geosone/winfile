# Native Win16 port of the Microsoft sources, not the Pascal drive demo.
WATCOM ?= /opt/watcom
OUT ?= bld/win16
JWLINK ?= jwlink
CC := $(WATCOM)/binl64/wcc
CFLAGS := -q -bt=windows -ml -2 -zW -os -s -zq -w4 -we -wcd202 -dCALMIRA_WIN16 \
  -i=$(WATCOM)/h -i=$(WATCOM)/h/win
OBJS := $(OUT)/lfnmisc.obj $(OUT)/wfmem.obj $(OUT)/numfmt.obj $(OUT)/memory.obj
APP_SOURCES := src/winfile.c $(filter-out src/winfile.c,$(wildcard src/*.c))
APP_OBJS := $(patsubst src/%.c,$(OUT)/%.obj,$(APP_SOURCES)) $(OUT)/wfgoto.obj $(OUT)/memory.obj

.PHONY: core core-test application
core: $(OUT)/WINFILE.LIB

$(OUT):
	mkdir -p $@

$(OUT)/%.obj: src/%.c win16/platform.h src/wfmem.h | $(OUT)
	$(CC) $(CFLAGS) -fr=$(@:.obj=.err) -fo=$@ $<

$(OUT)/memory.obj: win16/memory.c win16/platform.h | $(OUT)
	$(CC) $(CFLAGS) -fr=$(@:.obj=.err) -fo=$@ $<

$(OUT)/WINFILE.LIB: $(OBJS)
	$(WATCOM)/binl64/wlib -q -b -n $@ $(addprefix +,$(OBJS))

$(OUT)/core_test.obj: win16/core_test.c win16/platform.h src/wfmem.h | $(OUT)
	$(CC) $(CFLAGS) -fr=$(@:.obj=.err) -fo=$@ $<

$(OUT)/core_test.res: win16/core_test.rc src/res.h | $(OUT)
	$(WATCOM)/binl64/wrc -q -bt=windows -i=$(WATCOM)/h/win -r -fo=$@ $<

core-test: $(OUT)/WFCORE.EXE

$(OUT)/WFCORE.EXE: $(OUT)/core_test.obj $(OUT)/core_test.res $(OUT)/WINFILE.LIB
	$(JWLINK) option quiet format windows name $@ file $(OUT)/core_test.obj \
		library $(OUT)/WINFILE.LIB libpath $(WATCOM)/lib286/win \
		libpath $(WATCOM)/lib286 library windows,clibl \
		option stack=8k option heapsize=8k option version=3.10
	$(WATCOM)/binl64/wrc -q -bt=windows $(OUT)/core_test.res $@

# Missing APIs are build errors, never successful no-op stubs or a substitute
# demonstration application. This target is expected to fail until the
# remaining original modules and resources have been ported.
application: $(OUT)/WINFILE.EXE

$(OUT)/wfgoto.obj: src/wfgoto.cpp | $(OUT)
	$(WATCOM)/binl64/wpp $(CFLAGS) -fr=$(@:.obj=.err) -fo=$@ $<

$(OUT)/winfile.res: src/res.rc src/res.h | $(OUT)
	$(WATCOM)/binl64/wrc -q -bt=windows -i=$(WATCOM)/h/win -i=src \
		-r -fo=$@ src/res.rc

$(OUT)/WINFILE.EXE: $(APP_OBJS) $(OUT)/winfile.res
	$(JWLINK) option quiet format windows name $@ $(addprefix file ,$(APP_OBJS)) \
		libpath $(WATCOM)/lib286/win libpath $(WATCOM)/lib286 \
		library windows,shell,commdlg,clibl option stack=8k option version=3.10
	$(WATCOM)/binl64/wrc -q -bt=windows $(OUT)/winfile.res $@
