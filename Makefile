# Copyright 2025 Ana Oliveira, Humberto Gomes, Mariana Rocha, Sara Lopes
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# CONFIGURATION VARIABLES

CC       := gcc
CPP      := g++
CFLAGS   := -O2 -w -Ilib/include
CPPFLAGS := -Iinclude -std=c++17 -Wall -Wextra -pedantic -Wshadow \
				$(shell pkg-config --cflags glfw3) -DGLFW_INCLUDE_NONE \
				$(shell pkg-config --cflags glm) \
				$(shell pkg-config --cflags gl) \
				-Ilib/include
LIBS := -lm \
		   $(shell pkg-config --libs glfw3) \
		   $(shell pkg-config --libs glm) \
		   $(shell pkg-config --libs gl)

DEBUG_CPPFLAGS   := -O0 -ggdb3
RELEASE_CPPFLAGS := -O2
PROFILE_CPPFLAGS := -O2 -ggdb3

# Note: none of these directories can be the root of the project
# Also, these may need to be synced with the ones in .gitignore
BUILDDIR          := build
ENGINE_EXENAME    := engine
GENERATOR_EXENAME := generator
DEPDIR            := deps
OBJDIR            := obj

# Default installation directory (if PREFIX is not set)
PREFIX ?= $(HOME)/.local

# END OF CONFIGURATION

SOURCES     = $(shell find "src" -name '*.cpp' -type f)
OBJECTS     = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
LIB_SOURCES = $(shell find "lib" -name '*.c' -type f)
LIB_OBJECTS = $(patsubst lib/%.c, $(OBJDIR)/%.o, $(LIB_SOURCES))
HEADERS     = $(shell find "include" -name '*.hpp' -type f)
DEPENDS     = $(patsubst src/%.cpp, $(DEPDIR)/%.d, $(SOURCES))
REPORTS     = $(patsubst reports/%.tex, %.pdf, $(shell find reports -name '*.tex' -type f))

ifeq ($(DEBUG), 1)
	CPPFLAGS += $(DEBUG_CPPFLAGS)
	BUILD_TYPE = DEBUG
else ifeq ($(PROFILE), 1)
	CPPFLAGS += $(PROFILE_CPPFLAGS)
	BUILD_TYPE = PROFILE
else
	CPPFLAGS += $(RELEASE_CPPFLAGS)
	BUILD_TYPE = RELEASE
endif

# Only generate dependencies for tasks that require them
# THIS WILL NOT WORK IF YOU TRY TO MAKE AN INDIVIDUAL FILE
ifeq (, $(MAKECMDGOALS))
	INCLUDE_DEPENDS = Y
else ifneq (, $(filter default, $(MAKECMDGOALS)))
	INCLUDE_DEPENDS = Y
else ifneq (, $(filter all, $(MAKECMDGOALS)))
	INCLUDE_DEPENDS = Y
else ifneq (, $(filter install, $(MAKECMDGOALS)))
	INCLUDE_DEPENDS = Y
else
	INCLUDE_DEPENDS = N
endif

default: $(BUILDDIR)/$(ENGINE_EXENAME) $(BUILDDIR)/$(GENERATOR_EXENAME)
reports: $(REPORTS)
all: $(BUILDDIR)/$(ENGINE_EXENAME) $(BUILDDIR)/$(GENERATOR_EXENAME) $(DOCSDIR) $(REPORTS)

ifeq (Y, $(INCLUDE_DEPENDS))
include $(DEPENDS)
endif

$(DEPDIR)/%.d: src/%.cpp Makefile
	@mkdir -p $$(dirname $@)
	$(CPP) -MM $< -MF $@ $(CPPFLAGS)

$(OBJDIR)/%.o: src/%.cpp Makefile
	@mkdir -p $$(dirname $@)
	$(CPP) -MMD -MF $(patsubst src/%.cpp, $(DEPDIR)/%.d, $<) -c $< -o $@ $(CPPFLAGS)

$(OBJDIR)/%.o: lib/%.c Makefile
	@mkdir -p $$(dirname $@)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/$(ENGINE_EXENAME) $(BUILDDIR)/$(GENERATOR_EXENAME) $(BUILDDIR)/build_type: \
	$(OBJECTS) $(LIB_OBJECTS)
	@mkdir -p $(BUILDDIR)
	@echo $(BUILD_TYPE) > $(BUILDDIR)/build_type
	$(CPP) -o $(BUILDDIR)/cgmain $^ $(LIBS)
	@ln -s cgmain $(BUILDDIR)/$(ENGINE_EXENAME) 2> /dev/null ; true
	@ln -s cgmain $(BUILDDIR)/$(GENERATOR_EXENAME) 2> /dev/null ; true

$(BUILDDIR)/%.pdf: reports/%.tex Makefile
	$(eval TMP_LATEX = $(shell mktemp -d))
	$(eval PDF_NAME := $(patsubst reports/%.tex, %.pdf, $<))

	@mkdir -p $(BUILDDIR)
	cd reports && pdflatex -halt-on-error -output-directory $(TMP_LATEX) $(shell basename $<) < /dev/null
	@cp $(TMP_LATEX)/$(PDF_NAME) $@
	@rm -r $(TMP_LATEX)

.PHONY: clean
clean:
	rm -r $(BUILDDIR) $(DEPDIR) $(OBJDIR) $(ENGINE_EXENAME) $(GENERATOR_EXENAME) \
		2> /dev/null ; true

install: $(BUILDDIR)/$(ENGINE_EXENAME) $(BUILDDIR)/$(GENERATOR_EXENAME)
	install -Dm 755 $(BUILDDIR)/cgmain $(PREFIX)/bin
	ln -s cgmain $(PREFIX)/bin/generator
	ln -s cgmain $(PREFIX)/bin/engine

.PHONY: uninstall
uninstall:
	rm $(PREFIX)/bin/cgmain $(PREFIX)/bin/$(ENGINE_EXENAME) $(PREFIX)/bin/$(GENERATOR_EXENAME)
