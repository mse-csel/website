---
title: "Démons (Daemons)"
---

# Daemons - Introduction

Les démons (_daemons_) sont des processus qui fonctionnent en arrière-plan.
Les démons ont comme processus parent le processus `init`. Ils sont
généralement lancés au démarrage de la machine et s'exécutent avec les
privilèges « root » ou tout autre utilisateur spécial.

Marche à suivre pour qu'un processus se transforme en démon:

1. Créer un nouveau processus: `fork()` et terminer le processus parent: `exit()`
1. Créer une nouvelle session pour le nouveau processus: `setsid()`
1. Créer le processus démon: `fork()` et terminer le processus parent: `exit()`
1. Capturer les signaux souhaités: `sigaction ()`
1. Mettre à jour le masque pour la création de fichiers: `umask()`
1. Mettre à jour le masque pour la création de fichiers: `chdir()`
1. Fermer tous les descripteurs de fichiers: c`lose()`
1. Rediriger `stdin`, `stdout` et `stderr` vers `/dev/null`: `open()` et `dup2()`
1. Option: ouvrir un fichier de _logging_, par exemple sous syslog: `openlog()`
1. Option: chercher l'_ID_ de l'utilisateur et du groupe avec moins de privilèges
1. Option: changer le répertoire root vers un avec moins de visibilité: `chroot()`
1. Option: changer l'_ID_ de l'utilisateur et du groupe: `seteuid()` et `setegid()`
1. Implémenter le corps du démon...

## Alternatives

Sous Unix System V, les systèmes s'initialisent grâce au daemon `init`. Ce
programme est chargé de lire le fichier `/etc/inittab` et de lancer les
différentes applications sous forme de daemon.

Ce système a été tout naturellement repris sous Linux. Cependant il existe
tout une série d'alternatives:

- **busybox-init**: version simplifiée d'_init_ de Unix System V
- **systemd**: le remplaçant d'_init_ de Unix System V
- **upstart**: version Ubuntu (obsolète)
- ...
