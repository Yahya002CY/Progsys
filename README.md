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
# Question 5
Dans cette étape, nous avons implémenté la mesure du temps réel d'exécution de chaque commande ainsi que la récupération du code de retour du processus enfant.
Utilisation de clock_gettime nous avons utilisé l'horloge CLOCK_MONOTONIC pour mesurer le temps écoulé de manière fiable.
Le temps est capturé juste avant le fork() et juste après le wait(). La différence est ensuite convertie en millisecondes (ms).
WIFEXITED(status) est utilisé pour vérifier si le processus s'est terminé normalement, affichant ainsi le code de sortie via WEXITSTATUS.
WIFSIGNALED(status) est utilisé pour détecter si le processus a été interrompu par un signal (comme un Ctrl+C ou un kill), affichant le numéro du signal via WTERMSIG.
----Extrait de la logique de calcul--------
long elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
----Exemple sur le shell--------
elhouss-VMware-Virtual-Platform:~/Progsys$ ./enseash
Bienvenue dans le Shell ENSEA.
Pour quitter, tapez 'exit'.
enseash % date
Thu Dec 18 02:25:10 PM CET 2025
enseash [exit:0|2ms] % 
------------------------------
# Question 6
L'objectif de cette étape était de permettre au shell d'exécuter des commandes comportant plusieurs arguments.
Nous avons utilisé la fonction strtok pour séparer la chaîne de caractères saisie par l'utilisateur en plusieurs "tokens" (mots), en utilisant l'espace comme délimiteur.
Ces tokens sont stockés dans un tableau de pointeurs (char *args[]). Ce tableau doit impérativement se terminer par un pointeur NULL pour être compatible avec les fonctions système.
Nous avons utilisé désormais execvp(args[0], args).
args[0] contient le nom du programme à exécuter.
args est le tableau complet contenant le programme et tous ses arguments.
Le "v" dans "execvp" signifie "vector" (tableau), et le "p" signifie que la fonction recherche automatiquement le programme dans le PATH du système.
----Exemple sur le shell-------
elhouss-VMware-Virtual-Platform:~/Progsys$ ./enseash
Bienvenue dans le Shell ENSEA.
Pour quitter, tapez 'exit'.
enseash % fortune -s
In the first place, God made idiots; this was for practice; then he made
school boards.
		-- Mark Twain
enseash [exit:0|19ms] % 
-------------------------------
# Question 7
Ici on permet au shell de manipuler les flux de données en utilisant les opérateurs de redirection standards < et >.
Lors de la tokenisation (découpage de la chaîne), le shell recherche spécifiquement les symboles "<" (redirection d'entrée) et ">" (redirection de sortie).
//Redirection de sortie (>) : Le shell récupère le nom du fichier cible situé après le symbole.
--Il utilise open() avec les drapeaux O_CREAT | O_TRUNC | O_WRONLY pour créer ou écraser le fichier.
--L'appel système dup2() est utilisé pour remplacer le descripteur de fichier STDOUT_FILENO (sortie standard) par celui du fichier ouvert.
//Redirection d'entrée (<) :Le shell ouvre le fichier spécifié en mode lecture seule (O_RDONLY).
--dup2() remplace STDIN_FILENO (entrée standard) par le fichier, permettant à la commande de lire le contenu du fichier comme s'il était tapé au clavier.
----Exemple sur Moodle---------
ls > filelist.txt crée un fichier contenant la liste des fichiers du répertoire.
wc -l < filelist.txt lit le fichier précédemment créé pour en compter les lignes.
----Exemple sur le shell--------
elhouss-VMware-Virtual-Platform:~/Progsys$ ./enseash
Bienvenue dans le Shell ENSEA.
Pour quitter, tapez 'exit'.
enseash % ls > filelist.txt                              
enseash [exit:0|5ms] %  wc -l < filelist.txt
4
enseash [exit:0|3ms] %
-------------------------------
