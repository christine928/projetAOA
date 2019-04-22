# projetAOA
Le projet sous sa forme initiale sera lancé.
Pour essayer les autres fichiers, il faut modifier le makefile et changer driver.o en driver2.o pour exécuter le fichier driver2.c au lieu de driver.c par exemple.
Ainsi :
->driver2.c avec kernel.c permet d obtenir des valeurs pour calculer repw. Il faut appeler ./baseline <taille> <repm> <nbre_repw>, et appelera la fonction initiale de  ./baseline <taille> 0 <repm> à ./baseline <taille> <nbre_repw-1> <repm>
->driver3.c avec kernel.c fonctionne de la meme maniere pour repm. il s appelle avec ./baseline <taille> <repw> <nbre_repm>
->driverInv.c avec kernelInv.c : permet d'utiliser le code avec la structure de tableaux plutot que les tableaux de structure.
->driverSoAOpenMP.c avec kernelSoAOpenMP.c : permet d'utiliser le code avec la structure de tableaux en parallele via openMP
