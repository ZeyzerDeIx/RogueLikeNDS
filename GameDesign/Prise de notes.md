# Prise de notes

## Session du 8/IX



* boucle qui se réinitialise quand le nombre de pas est atteint
* différentes strates

  * une qui sert un peu d'onboarding
  * 2 autres qui viennent s'ajouter quand on a réussi à maîtriser un peu la première
  * malgré tout, besoin de revenir à la première, les 3 spnt à égalité pour le lore (pas d'ordre d'importance)
  * quand tu meurs dans une strate, tu respawn dedans (pas à la strate 1)
* déterminisme : pas de système de chance, on peut tout contrôler
* on spawn tjrs dans la même pièce
* facteur spatial important dans l'apparition des pièces
* but du jeu = plus de limite de pas / sortir du lieu
* certaines actions modifient le nombre de pas qu'on peut faire
* on a pas de compteur de pas, mais un feedback visuel qui t'informe du nombre qu'il te reste (sauf action bloquant le feedback)
* système de graph conditionnel pour l'invocation des pièces les unes par rapport aux autres (Lucas cook un tool de tardbâ)
* changement de strates : un moyen obvious, mais des moyens cachés qui permettent d'accéder à d'autres zones des strats (+ déclencher le waaaah ou le aaaargh des joueureuses)
* moyen de téléportation entre les pièces + possibilité de se téléporter dans une pièce qui se crée au moment de la téléportation sans être reliée au reste de la map dans la boucle
* quand on entre dans une pièce, les emplacements des couloirs existent déjà mais juste avec des effets d'ombre ; quand tu passes le couloir, il s'illumine et tu vois la pièce derrière

  * s'il y a un conflit entre 2 couloirs de 2 pièces différentes qui mènent au même endroit, le joueur.euse franchit le premier couloir selon son choix, et le deuxième disparaît / est condamné (possibilité de faire apparaître une pièce secrète dans une configuration justement pensée impossible)
  * possibilité d'un croisement de couloirs si il n'y a pas de conflit de génération de pièces
  * génération de la rotation des pièces dépendant du couloir (coordonnées locales)
* système de carte : à voir mais ça risque d'être compliqué



* mécaniques dans les pièces

  * ah force à moi pour trouver des idées hein
  * levier / bouton à activer ou désactiver
  * curseurs à positionner
  * objets à déplacer (ex : le prendre sur une étagère et le mettre sur une table) (ça risque d'être chaud selon le scope qu'on vise) (mais ça peut pousser à faire des allers retours donc être fun pour les galères de nombre pas)
  * texte à lire / image à voir qui donnent des conseils ou des indices sur le fonctionnement du labyrinthe et le lore
  * objet à consommer ; possibles effets : - influence sur le nombre de pas / - permet au joueureuse de voir des trucs qu'iel pouvait pas voir avant
  * téléportation



* 3 scopes :

  * gros scope : les strates, plus de pièces, des trucs qui bougent
  * moyens scope :
  * petit scope : pas de strates, 10 pièces, toutes les pièces fixes et les couloirs régus



* répartition des tâches :

  * Audrey :

    * règles de génération des pièces
    * mécaniques de jeux (comme pour les règles de génération je mets chez moi mais on peut faire à 2 stv !!!)
  * Lucas :

    * programmation
    * tool pour les règles des pièces



* Concrètement ça ressemble à quoi sur les écrans ? (ma prise de note est atroce j'me parle à moi même pitié juge pas je mettrai au propre dans le GD Doc, c'est pour garder une trace de ma réflexion)

  * ##### &#x20;1

    * écran du haut : le labyrinthe et le personnage qui évolue dedans ; commandes du personnage avec la croix directionnelle
    * écran du bas : interaction tactile avec les mécaniques de jeu
    * utilisation des boutons L, R et ABXY ? franchement on peut abandonner L et R ;)))
  * ##### &#x20;**2**

    * écran du bas : personnage dans le labyrinthe, déplacement tactile ou via la croix directionnelle + interaction avec certaines des mécaniques (genre appuyer sur A quand ton perso est collé à un objet pour
    * écran du haut : interaction avec les mécaniques de jeux via la croix directionnelle ou ABXY (jcrois je boycott L et R parce que ces boutons se cassaient tjrs sur nos ds quand on était enfant donc je les déteste, donc en vrai est ce qu'on a vraiment besoin de leur trouver une utilité ?)

*voir les images dans RogueLikeNDS\\GameDesign\\Schémas*

*faut que je trouve plus de refs mais ça arrive fort, là je est trop fatiguée pour les trouver*

