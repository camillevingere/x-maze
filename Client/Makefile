#
# Makefile pour l'application de construction de labyrinthe
#

SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
BIBLIOTHEQUES = -L ../lib_game/ -lgame -L ../lib_lab/ -llab -lm -L ../Graphique -lgraph -lSDL2_gfx -lSDL2 -lm -L ../lib_network -lnet -L ../lib_util/ -lutil -L ../lib_thread/ -lthread -pthread
CFLAGS += -std=c99 -D_DEFAULT_SOURCE -I ../Graphique -I ../lib_lab/ -I ../lib_game/ -I ../lib_network -I ../lib_util -I ../lib_thread
CIBLE = client

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
# Dependances pour l'executable
#

$(CIBLE): $(OBJETS)
	$(CC) -o $@ $^ $(BIBLIOTHEQUES)