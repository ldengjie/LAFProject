# $Id: GNUmakefile,v 1.9 2008/12/05 09:57:30 flei Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module
# --------------------------------------------------------------

name := GdLSExp
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../../..
endif

.PHONY: all
all: lib bin

ifdef G4ANALYSIS_USE_ROOT
  CPPFLAGS += -DG4ANALYSIS_USE_ROOT 
endif

include $(G4INSTALL)/config/architecture.gmk

CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
LDFLAGS  += $(shell $(ROOTSYS)/bin/root-config --glibs)

include $(G4INSTALL)/config/binmake.gmk

anaclean:
	rm -f $(G4WORKDIR)/tmp/$(G4SYSTEM)/$(G4TARGET)/GdLSExpHisto* 
	rm -f $(G4WORKDIR)/tmp/$(G4SYSTEM)/$(G4TARGET)/GdLSExpAnalysis* 
	rm -f $(G4WORKDIR)/tmp/$(G4SYSTEM)/$(G4TARGET)/GdLSExpStepping* 
