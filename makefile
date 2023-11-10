ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
endif

.PHONY: clean
.PHONY: clean_obj
.PHONY: clean_exe
.PHONY: clean_txt
.PHONY: test
.PHONY: run

PATHU = unity/src/
PATHS = src/
PATHT = test/
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHR = build/results/
PATHBAT = bat/

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)*.c)

COMPILE=gcc -c
LINK=gcc
DEPEND=gcc -MM -MG -MF
CFLAGS=-I. -I$(PATHU) -I$(PATHS) -DTEST

RESULTS = $(patsubst $(PATHT)Test%.c,$(PATHR)Test%.txt,$(SRCT) )

PASSED = `grep -s PASS $(PATHR)*.txt`
FAIL = `grep -s FAIL $(PATHR)*.txt`
IGNORE = `grep -s IGNORE $(PATHR)*.txt`

OBJ_FILES = $(wildcard $(PATHO)*.o)
EXE_FILES = $(wildcard $(PATHB)*.$(TARGET_EXTENSION))
TXT_FILES = $(wildcard $(PATHR)*.txt)

RUN_SRC_FILES = $(wildcard $(PATHS)*.c)
RUN_OBJ_FILES = $(patsubst $(PATHS)%.c, $(PATHO)%.o, $(RUN_SRC_FILES))

test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

$(PATHR)%.txt: $(PATHB)%.$(TARGET_EXTENSION)
	$(subst /,\, -./$< > $@ 2>&1)

$(PATHB)Test%.$(TARGET_EXTENSION): $(PATHO)Test%.o $(PATHO)%.o $(PATHO)unity.o #$(PATHD)Test%.d
	$(LINK) -o $@ $^

$(PATHO)%.o:: $(PATHT)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHS)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHU)%.c $(PATHU)%.h
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHD)%.d:: $(PATHT)%.c
	$(DEPEND) $@ $<

$(PATHB):
	$(MKDIR) $(subst /,\, ./$(PATHB))

$(PATHD):
	$(MKDIR) $(subst /,\, ./$(PATHD))

$(PATHO):
	$(MKDIR) $(subst /,\, ./$(PATHO))

$(PATHR):
	$(MKDIR) $(subst /,\, ./$(PATHR))

clean_obj:
	$(CLEANUP) $(subst /,\, $(OBJ_FILES))

clean_exe:
	$(CLEANUP) $(subst /,\, $(EXE_FILES))

clean_txt:
	$(CLEANUP) $(subst /,\, $(TXT_FILES))

clean: clean_obj clean_exe clean_txt

run:
	$(COMPILE) -o $(PATHO)main.o $(PATHS)main.c
	$(LINK) -o $(PATHB)main.$(TARGET_EXTENSION) $(RUN_OBJ_FILES)
	cls
	@echo "======== main.$(TARGET_EXTENSION) OUTPUT: ========"
	$(PATHB)main.$(TARGET_EXTENSION)

clear:
	$(subst /,\, .\$(PATHBAT)bat_clear.bat)


.PRECIOUS: $(PATHB)Test%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHD)%.d
.PRECIOUS: $(PATHO)%.o
.PRECIOUS: $(PATHR)%.txt
