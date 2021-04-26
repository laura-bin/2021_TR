# ----------------------------------------------------------------------
# Fichier d'exemple du livre "Solutions Temps-Reel sous Linux"
# (C) 2012 - Christophe BLAESS
# http://christophe.blaess.fr
# 
# https://www.blaess.fr/christophe/livres/solutions-temps-reel-sous-linux/
# ----------------------------------------------------------------------

CROSS_COMPILE ?=
CC      = $(CROSS_COMPILE)gcc
CFLAGS  = -Wall -g -pthread
LDFLAGS = -pthread

EXEMPLES=                                \
	exemple-taux-cpu                 \
	exemple-nice-threads             \


.PHONY: all
all : ${EXEMPLES}

.PHONY: clean
clean : 
	@rm -f core *.o *.out *.bb *.bbg *.gcov *.da *~
	@rm -f ${EXEMPLES}

