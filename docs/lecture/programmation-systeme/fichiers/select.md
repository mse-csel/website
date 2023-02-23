---
title: Multiplexage des entrées/sorties
---

## Introduction

Il est usuel que des applications doivent attendre sur des périphériques
d’entrées/sorties, tels que clavier, souris, interface de communication, etc.

Ces périphériques sont représentés dans l’application par un descripteur de
fichier, lequel offre généralement un accès au périphérique par un des appels
système `read` ou `write`

<figure markdown>
![](img/mux.drawio.svg)
</figure>

Mécanismes à disposition :

- Utilisation des services non bloquants
- Implémentation d’un thread par entrée/sortie
- Utilisation de services offrant un multiplexage des entrées/sorties

## Mécanismes - Services non bloquants

Un chemin possible pour solutionner cette problématique est l’utilisation de
services non bloquants pour scruter chaque entrée/sortie séquentiellement.

Ces services permettent de tester le périphérique si son accès est possible ou
pas. Si l’accès est disponible, alors l’entrée/sortie est traitée.

<figure markdown>
![](img/select.drawio.svg)
</figure>

**Carences**

- Usage excessif du processeur pour la scrutation
- Complexité du logiciel 

## Mécanismes - Multi-threading

Une deuxième voie possible consiste à utiliser les services bloquants et à
créer un _thread_ par entrée/sortie

Cette technique permet d’éviter que l’indisponibilité d’une entrée/sortie
bloque le traitement des autres

<figure markdown>
![](img/multi-threads.drawio.svg)
</figure>

**Carences** :

- Nombre de threads nécessaires pour réaliser l’application
- Complexité du logiciel pour synchroniser les différentes données
- Complexité lors du debugging du logiciel

## Mécanismes - Multiplexage des entrées/sorties

Les systèmes Unix, et plus particulièrement Linux, proposent des services
autorisant le multiplexage des entrées/sorties. Ces services permettent de
sélectionner un catalogue d’entrées/sorties à traiter et de le passer au noyau.
Ce dernier informera le processus sur ceux disponibles pour traitement.

<figure markdown>
![](img/select-mux.drawio.svg)
</figure>

Linux propose 3 services :

- `select()` (compatible avec les systèmes UNIX)
- `poll()` (pas traité ici)
- `epoll()` (Linux spécifique)

