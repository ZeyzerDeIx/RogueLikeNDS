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

  * 



* 3 scopes :

  * gros scope : les strates, plus de pièces, des trucs qui bougent
  * moyens scope : 
  * petit scope : pas de strates, 10 pièces, toutes les pièces fixes et les couloirs régus



* répartition des tâches :

  * Audrey :

    * règles de génération des pièces
  * Lucas :

    * programmation
    * tool pour les règles des pièces

