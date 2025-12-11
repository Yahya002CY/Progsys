# Progsys
# Question 1
Un message de bienvenue et un invite statique, puis se termine immédiatement. 
Démontrer les E/S de base en utilisant "write" vers "stdout".
L'objectif est d'introduire la sortie de bas niveau sans printf et de mettre en place le salut et les bases de l'interface du shell.
# Question 2
Nous avons introduit une boucle infinie pour lire les commandes utilisateur, forker un processus enfant pour les exécuter et attendre leur achèvement.
la boucle tourne indéfiniment jusqu'à interruption.
char command "MAX_SIZE" pour l'entrée ; "pid_t pid", "ssize_t ret" pour la gestion des processus/entrées.
Activer la lecture des commandes depuis "stdin".
Exécuter des programmes externes en utilisant "fork() crée un enfant + execlp(command, ...) recherche dans PATH".
Le parent wait(&status) attend la fin de l'enfant.
# Question 3
Nous voulons rendre le shell convivial avec une terminaison appropriée.
Ajout du Message d'Au Revoir : BYE_MSG
Vérifications de Sortie, après read :
Si ret == 0 (Ctrl+D) : Affiche au revoir et interrompt.
Si l'entrée est exactement "exit\n" vérification strncmp avec longueur 5 : Affiche au revoir et c'est fini.
# Question 4
Fournir un feedback visuel sur le succès/échec des commandes précédentes.
Utilisation des macros de <sys/wait.h> pour analyser le statut du processus enfant.
WIFEXITED(status) : Si vrai, utilise WEXITSTATUS(status) pour le code de sortie ([exit:0]).
WIFSIGNALED(status) : Si vrai, utilise WTERMSIG(status) pour le signal.([sign:9] pour SIGKILL)
