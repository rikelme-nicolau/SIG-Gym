# Makefile — SIG-Gym (C99) — Linux/WSL/Git Bash/MSYS2/MinGW/PowerShell

# --- toolchain ---
CC       := gcc
CFLAGS   := -std=c99 -Wall -Wextra -Wpedantic -O2
CPPFLAGS := -I. -Isrc

# --- dirs & bin ---
SRC_DIR  := src
BUILD    := build
NAME     := sig-gym

# Plataforma
ifeq ($(OS),Windows_NT)
  EXEEXT := .exe
  # funções cross-platform (usadas com $(call ...))
  MKDIR_P = if not exist "$(1)" mkdir "$(1)"
  RM_RF   = if exist "$(1)" rmdir /S /Q "$(1)"
  RM_F    = if exist "$(1)" del /Q "$(1)"
  RUNBIN  = $(BIN)
else
  EXEEXT :=
  MKDIR_P = mkdir -p "$(1)"
  RM_RF   = rm -rf "$(1)"
  RM_F    = rm -f "$(1)"
  RUNBIN  = ./$(BIN)
endif

BIN := $(BUILD)/$(NAME)$(EXEEXT)

# --- recursive wildcard (sem 'find') ---
rwildcard = $(strip $(wildcard $1$2) \
             $(foreach d,$(wildcard $1*),$(call rwildcard,$(if $(findstring /,$d),$d/,$d/),$2)))

SRCS := $(call rwildcard,$(SRC_DIR)/,*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

.PHONY: all run clean rebuild tree check

# alvo padrão
all: $(BIN)
	@echo Build OK: $(BIN)

# link
$(BIN): $(OBJS) | $(BUILD)
	$(CC) $(OBJS) -o $@

# compila e gera dependências .d espelhadas em build/
$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	@$(call MKDIR_P,$(patsubst %/,%,$(dir $@)))
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -MF $(patsubst %.o,%.d,$@) -c $< -o $@

# garante a pasta build
$(BUILD):
	@$(call MKDIR_P,$(BUILD))

# executar
run: all
	$(RUNBIN)

# limpeza (sem 'true' e tolerante a ausência dos arquivos)
clean:
	-@$(call RM_RF,$(BUILD))
	-@$(call RM_F,*.exe)
	-@$(call RM_F,*.gch)

rebuild: clean all

# debug
tree:
	@echo "SRCs: $(words $(SRCS)) | OBJs: $(words $(OBJS))"
	@$(foreach f,$(SRCS),echo SRC  $(f);)
	@$(foreach o,$(OBJS),echo OBJ  $(o);)

check:
	@echo "gcc:" && gcc --version | head -n 1 || true
	@echo "make:" && (make --version || mingw32-make --version) | head -n 1
# inclui dependências de headers
-include $(DEPS)
