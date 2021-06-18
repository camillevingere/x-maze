#
# Makefile pour le projet labyrinthe
#

#
# Constantes pour la compilation
#

CC = gcc
LD = gcc
AR = ar
export CFLAGS += -g -Wall #-L../lib_network/ -lnet -pthread
MAKE = make 

#
# Constantes du projet
#

DIRS=Graphique lib_lab lib_game lib_network lib_thread lib_util Serveur Client

#
# Cible principale
#

all: $(patsubst %, _dir_%, $(DIRS))

$(patsubst %,_dir_%,$(DIRS)):
	cd $(patsubst _dir_%,%,$@) && $(MAKE)

#
# Cible de deverminage
#

debug: CFLAGS += -DDEVERMINE
debug: all

#
# Cible de nettoyage
#

clean: $(patsubst %, _clean_%, $(DIRS))

$(patsubst %,_clean_%,$(DIRS)):
	cd $(patsubst _clean_%,%,$@) && $(MAKE) clean

test: clean all

	Serveur/./serveur
