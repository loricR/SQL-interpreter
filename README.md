Créer une branche pour faire des modifications.
PUSH les modifications sur cette branche.
Une fois que c'est fini, PUSH une dernière fois sur cette branche puis MERGE à la branche main et faire un MERGE REQUEST depuis gitlab.
Faire un FETCH régulièrement (c'est juste une image du distant donc aucun risque).

Avant tout, pour avoir le projet en local :
	git clone https://gitlab.com/projetlp25/projetlp25.git
	
Savoir où on en est :
	git status

PUSH (envoyer sur le serveur distant) :
	(se déplacer dans le dossier du projet)
	git add [les fichiers modifiés ou . pour tous]
	git commit -m "Je dis ici les modifs effectuée"
	git push -u origin [nom de la branche où on veut push]
	
PULL (récupérer tous les fichiers du serveur distant) :
	git fetch
	git pull
	(équivalent à git fetch et git merge)

Créer BRANCH :
	git branch [nom de la branche qu'on veut créer]
	(sans argument ça donne la liste des branches existantes)
	
Se déplacer dans les BRANCH :	
	git checkout [nom de la branche où on veut aller]
	ATTENTION: faire un commit avant sinon toutes les modifs en cours sur la branche se retrouve dans l'autre (pas besoin de commit si on veut bien déplacer les modifs)
	
MERGE :
	git fetch
	git merge [nom de la branche a merge]
	Il faudra peut-être gérer des conflits
	
MERGE REQUEST :
	Faire depuis gitlab quand on a fini de bosser sur une branche après un MERGE.
	Si le MERGE à été fait avant, il n'y a pas de conflit alors ça se fait direct.
	
Enregistrer user et mdp (pas mettre un mdp important, c'est stocké en clair sur le disque) :
	git config --global credential.helper store
