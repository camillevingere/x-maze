#
# Makefile pour la bibliotheque graphique
#

SOURCES = $(wildcard *.c)
INCDIR = -I ../lib_lab/ -I ../Graphique

CFLAGS += -std=c99 -D_DEFAULT_SOURCE
OBJETS = $(SOURCES:.c=.o)
CIBLE = libgame.a

#
# Nom de la cible principale
#

all: $(CIBLE)

#
# Cible de nettoyage
#

clean: 
	rm -f core *.o $(CIBLE)

#
# Dependances pour la bibliotheque
#

$(OBJETS): $(SOURCES)
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

$(CIBLE): $(OBJETS)
	$(AR) rs $@ $?

#$(CIBLE:.a=).o: $(CIBLE:.a=).c $(CIBLE:.a=).h