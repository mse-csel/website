---
title: Machines virtuelles
---

## Introduction

L'instrumentation d'une application peut s'effectuer à l'aide d'une machine
virtuelle (ou d'un containeur Docker)

- Le fichier exécutable est lancé dans la machine virtuelle
- La machine virtuelle intercepte le code avant qu'il soit exécuté sur le CPU
- Elle le modifie à la volée pour y ajouter des instructions d'instrumentation
- Cela permet de collecter énormément d'informations
    - Allocations mémoires,
    - _Call graph_,
    - ...

Avantages :

- Ne nécessite pas de compilation spéciale
- Permets de récolter beaucoup d'informations

Inconvénients :

- Impact sur les performances très grand
- Le comportement de l'application peut se trouver modifié

## Valgrind

Valgrind est un outil permettant de faire de l'instrumentation de code à la
volée

C'est un framework comportant plusieurs outils

- `memcheck` : détection d'erreurs de mémoire
  (overflow / underflow, undefined variables, memory leaks, ...)
- `callgrind` : pour faire du profiling et compter le nombre d'exécutions
- `cachegrind` : analyse de l'utilisation du cache
- `massif` : analyse de l'utilisation du heap
- `helgrind` : débogage d'application multi-threadée


