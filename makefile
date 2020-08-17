# Make

.PHONY = clean debug
KEYS :=
TMP  :=
TMPL :=
LIBS_G :=
LIBS :=
CC := gcc	

tree_of_processes : tree_of_processes.o
		$(CC) $(TMP) $(KEYS) tree_of_processes.o\
							 -o tree_of_processes $(TMPL) $(LIBS)

tree_of_processes.o : tree_of_processes.c
		$(CC) $(TMP) $(KEYS) -c tree_of_processes.c\
							 -o tree_of_processes.o $(TMPL) $(LIBS_G)


debug : tree_of_processes_g.o
		$(eval KEYS:=-g)
		$(CC) $(TMP) $(KEYS) tree_of_processes_g.o\
							 -o tree_of_processes_g $(TMPL) $(LIBS_G)

tree_of_processes_g.o : tree_of_processes.c
		$(eval KEYS:=-g)
		$(CC) $(TMP) $(KEYS) -c tree_of_processes.c\
							 -o tree_of_processes_g.o $(TMPL) $(LIBS_G)

clean :
		rm -f *.o *.gch tree_of_processes tree_of_processes_g a.out
