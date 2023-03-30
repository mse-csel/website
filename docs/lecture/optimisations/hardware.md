---
title: Matériel et microprocesseur
---

## Introduction

Les processeurs modernes offrent une batterie de compteurs (hardware
events counters) permettant de collecter et capturer de toute une série
d'informations et d'événements de profiling

- Les instructions exécutés
- Les _cache misses_ (data, instruction)
- Les _branch mispredicted_
- Les cycles CPU gaspillés (_stalled_)
- Beaucoup, beaucoup d'autres...

Comme ces informations sont collectées et stockées par le microprocesseur, elles offrent
une solution avec un très faible impact sur les performances globales des
applications

## Performance Monitor Unit (PMU)

_PMU_ est le nom donné au composant permettant de gérer les compteurs
d'événements d'un processeur

Chaque constructeur de processeurs propose un PMU différent, documenté
généralement dans le manuel de l'architecture

- Les processeurs Intel[^intel] intègrent depuis longtemps un PMU
- Les processeurs ARM[^arm] intègrent un PMU dans l'architecture ARMv7/ARMv8

Le PMU fournit une interface pour activer les différents compteurs
d'événements

- Des instructions permettent d'activer les compteurs
- Une fois le compteur activé, celui-ci commence à collecter les événements
- Il suffit ensuite d'aller lire la valeur du compteur
- Sur ARM, le PMU s'accède et se configure via le coprocesseur _CP15_, donc avec
  les instructions `MCR` / `MRC`

[^intel]: https://perfmon-events.intel.com/
[^arm]: [ARM® Architecture Reference Manual ARMv7-A and ARMv7-R edition, Chapitre C12: The Performance Monitors Extension](https://developer.arm.com/documentation/ddi0406/cd)

## Outils

Il existe différents outils pour collecter des informations de profiling en
utilisant les compteurs matériels

Intel fournit un outil Intel® VTune™ Amplifier (payant)

ARM n'a pas (encore) d'équivalent

Le noyau Linux intègre aussi des outils: `perf` et `oprofile`

Ces outils fonctionnent pour les processeurs Intel, AMD et également ARM
