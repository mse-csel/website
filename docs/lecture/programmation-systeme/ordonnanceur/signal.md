---
title: Traitement des signaux
---

## Signaux – Introduction

Les signaux sont des interruptions logicielles permettant de traiter des
événements asynchrones au bon déroulement du programme

Les événements à l’origine de la levée des signaux sont souvent externes au
système, par excemple pression sur la touche ++ctrl+c++ sur un clavier

Un processus peut envoyer un signal à un autre processus ou groupe de
processus, offrant ainsi une forme primitive de communication
interprocessus

Lorsqu’un signal est levé (généré ou envoyé), le noyau effectue une des
actions suivantes:

- **Ignorer le signal** :
  Le noyau ignore le signal et aucune action n’est entreprise.
- **Capturer et traiter le signal** :
  Le noyau arrête l’exécution du processus et traite le signal en appelant une
  fonction ayant été enregistrée préalablement par le processus en cours.
- **Effectuer une action par défaut** :
  Le noyau arrête l’exécution du processus et exécute une action par défaut
  implémentée par le noyau lui-même et dépendante du signal. Celle-ci consiste
  souvent à terminer le processus et générer un coredump.
