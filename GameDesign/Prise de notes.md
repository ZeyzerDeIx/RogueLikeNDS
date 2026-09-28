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



* où est-ce qu'on met le curseur entre subir et contrôler ? à quel point le joueuse a conscience de l'impact qu'elle a sur les pièces ? partagé entre le besoin de donner du contrôle pour qu'il puisse avoir un impact dans le jeu, mais est ce qu'on s'éloigne pas du bail ?



* mécaniques dans les pièces

  * ah force à moi pour trouver des idées hein
  * levier / bouton à activer ou désactiver
  * curseurs à positionner
  * objets à déplacer (ex : le prendre sur une étagère et le mettre sur une table) (ça risque d'être chaud selon le scope qu'on vise) (mais ça peut pousser à faire des allers retours donc être fun pour les galères de nombre pas)
  * texte à lire / image à voir qui donnent des conseils ou des indices sur le fonctionnement du labyrinthe et le lore
  * objet à consommer ; possibles effets : - influence sur le nombre de pas / - permet au joueuse de voir des trucs qu'elle pouvait pas voir avant (note : les objets doivent aussi apparaître systématiquement aux mêmes endroits pour neutraliser le facteur chance) -> en fait pas d'objet,
  * l'effet de certains consommables changent selon ce qui s'est passé avant (ex : la nourriture dans la pièce verte redonne des pas si elle a été crée depuis la pièce rouge et en fait perdre si elle a été crée depuis une autre pièce)
  * téléportation : pas toujours volontaire
  * des éléments sur lesquels marcher (dalles au sol), et d'autres à pousser dessus pour activer plusieurs dalles
  * certains éléments sont trouvables dans plusieurs pièce ( nourriture pour regagner des pas / téléporteur)



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

*les photos sont nulles parce que mon tel fait une mise à jour du coup j'ai du piquer celui de Léo déso*

*faut que je trouve plus de refs mais ça arrive fort, là je est trop fatiguée pour les trouver*





##### Pièces :

Pièces simples : 13 (ou plus si possible)

Pièces spéciales : 2 (spawn et ivoire)

Total : 15



Spécificité et condition d'apparition (en cours)

* spawn : toujours la pièce dans laquelle on réapparaît ; ne peut pas être invoquée autrement qu'en lançant une run
* verte :
* jaune : est invoquée en tant que première quatrième pièce dans une direction
* rouge :
* bleu :
* rose :
* violet :
* blanc :
* gris : ne peut être invoquée qu'en lançant une téléportation dans le vide
* noir :
* marron : n'apparaît que dans la zone est
* orange :
* turquoise : apparaît systématiquement quand on crée une pièce après avoir activé précisément 6? leviers dans l'ensemble de la run
* beige :



* ivoire : pièce finale ; le nombre de pas de base permet d'y faire quelques pas avant de s'y effondrer ; uniquement une porte sud ? ; conditions d'ouverture de la sortie qui nécessitent de pas mal faire d'aller retour entre les pièces





## Session du 15/IX

* faire un test dessin (32\*64)
* trouver un graph (idées : Milo / Alma / Angèle ou elle peut ptet demander à des potes)
* switch entre les 2 écrans à intégrer dans les mécaniques
* trouver plus de mécaniques compliquées pour le joueur
* config du spawn = config écran haut/bas -> c'est que pour cette pièce que ça a cet effet là (proposition à chaque début de run de la config que la joueuse veut adopter ?)
* éléments existants seulement selon la config d'écran
* créer des "bugs" lors du switch d'écran qui deviennent



* quand en config2, normalement on déplace le bonhomme en cliquant là où on veut qu'il aille ; mais en fait, on peut aussi tracer des pattern au sol pour lui faire activer un pouvoir (téléportation? ou trouver autre chose)
* quand en config1,  :((( trouver une idée
* possible de se déplacer en diagonale





* les boutons L et R sont matérialisés par 2 petites lumières dans les angles de l'écran, qui s'allument quand on appuie dessus
* avec les boutons L et R, en config commande, la joueuse peut alimenter certaines pièces en énergie, ce qui permet en config tactile d'activer les tracés
* toutes les pièces ne sont pas reliées à l'énergie ; des pièces adjacentes se transmettent l'énergie si elle est activée dans l'une d'entre elles, mais la chaîne est rompue si une pièce non conductrice interrompt
* certaines pièces sont énergisables, mais il faut pour cela réaliser une action ; les blocs à déplacer sur le sol
* tracé de patterns : en config tactile, le joueuse peut tracer des patterns avec le stylet pour faire se déplacer le personnage selon le tracé et ainsi activé un pouvoir ( ce qui coûte donc des pas à la joueuse) : faire pivoter les sorties de la pièces et donc changer les possibles apparitions des salles ; faire apparaître des leviers ou éléments cachés dans les salles ; échanger 2 pièces (ça ne téléporte pas la joueuse, c'est la pièce dans laquelle elle se situe qui échange avec une autre pièce) ; transformer la pièce en une autre pièce qui n'existe pas encore dans la run
* en config tactile, les boutons L et R permettent de révéler plus d'informations sur certains documents ; l'effet lumineux des loupiotes font apparaître des données qu'on avait pas forcément



* limiter le nombre de changement de config qu'on a ? avec des moyens de recharger pendant la run (item consommable)



* téléportation : se fait en entrant dans un couloir qui n'aboutira pas ; au début, ça crée un effet de surprise, puis la joueuse peut maîtriser l'effet ; on se retrouve dans la pièce qui aurait du se situer au bout du couloir



* un passeur présent dans certaines pièces, qui permet de glisser en dehors des couloirs ce qui fait économiser des pas, mais contre un coût -> lequel ? il a plusieurs "comptoirs" fixes dans certaines pièces, et la joueuse choisit à laquelle elle veut aller ; le passeur lui fait prendre le chemin le plus court en ignorant les murs, obstacles etc...



* objet qu'on peut récupérer : un objet pour lire les documents écrits ; les blocs à déplacer sur le sol ; les consommables qui redonnent/enlèvent des pas ; les consommables qui redonnent des changements de config

