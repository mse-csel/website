---
title: "Accès concurrents (race condition)"
---

## Introduction

En programmation multitâches, il arrive fréquemment que plusieurs tâches
(processus ou _threads_) cherchent à accéder au même instant à une ressource
commune et que l'une d'entre elles tente de la modifier, par exemple un spooler d'une
imprimante. Dans de cas, on parle alors de situation de compétition (_race
condition_).

<figure markdown>
![](img/race-condition.drawio.svg)
</figure>

Si cette situation est mal gérée, elle peut déboucher à des résultats différents
selon l'ordre dans lequel les différentes tâches vont agir sur le système. Dans
des situations extrêmes cela peut même résulter à des résultats erronés.

Par leur nature aléatoire, les situations de compétition sont des problèmes
excessivement complexes à identifier et à corriger. Il est par conséquent
essentiel d'avoir un excellent design du système avant de le réaliser.

## Exclusion mutuelle

L'exclusion mutuelle est la méthode permettant de protéger les sections
critiques et d'éviter des situations de compétitions.

L'exclusion mutuelle consiste n'autoriser qu'une seule tâche à la fois à
accéder à la ressource partagée et à bloquer toutes les autres.

Sous Linux, il existe différents mécanismes pour réaliser l'exclusion mutuelle :

- Les sémaphores
- Les mutex (version simplifiée des sémaphores et spécialisée pour cette tâche)
- La désactivation des interruptions (au niveau du noyau)
- La désactivation de l'ordonnancement
- Les moniteurs
- Les verrous

Cependant la méthode la plus simple pour éviter des situations de compétition
est de ne pas les créer. Pour cela il suffit de dédier un et un seul processus à
une ressource. Si d'autres acteurs ont besoin d'y accéder, on peut simplement
utiliser les services de communication offerts par Linux.
