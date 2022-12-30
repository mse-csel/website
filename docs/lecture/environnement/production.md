---
title: Mise en mode production
---

## Linux Initialisation

Une propriété des systèmes embarqués est autonomie, c.-à-d. leur capacité à
fonctionner sans intervention extérieure.

Pour atteindre cette autonomie, il est essentiel de pouvoir lancer les logiciels
d'application au démarrage de la cible sans aide extérieure. Linux implémente
un processus d'initialisation. Ce processus est choisi dans l'ordre suivant :

1. `/sbin/init` --> l'emplacement préféré et le plus probable pour le processus _init_
2. `/etc/init` --> un autre emplacement probable pour le processus _init_
3. `/bin/init` --> également un emplacement possible pour le processus _init_
4. `/bin/sh` --> l'emplacement du shell _Bourne_

**Sources** :

- Fichier: `init/main.c`
- Routine: `kernel_init`

## BusyBox processus init

Le logiciel _BusyBox_ est un environnement de commandes très populaire dans
les systèmes embarqués, à l'instar des GNU Core Utilities.

Le processus d'initialisation de BusyBox est appelé par le noyau à l'aide d'un
lien symbolique :

``` text
# ls -l /sbin/init
lrwxrwxrwx 1 default default 14 Feb 5 2012 /sbin/init -> ../bin/busybox
```

Dans ce processus, BusyBox analyse le fichier /etc/inittab et exécute les
instructions qu'il contient. Ce fichier était à l'origine le fichier de configuration
des daemons du système Unix V.

BusyBox offre deux possibilités :

- Mettre une entrée dans le fichier `/etc/inittab`.
- Placer un script de lancement dans `/etc/init.d`.
  Le script `/etc/init.d/rcS` exécute dans l'ordre alphabétique
  tous les fichiers `S??*`.

## Fichier `/etc/inittab`

Le fichier `/etc/inittab` contient une liste d'instructions qui sera interprétée par
BusyBox au démarrage du noyau Linux.

Chaque ligne du fichier a la structure suivante:

``` text
id:runlevel:action:process
```

- `id`: nom du périphique tty, peut être laissé vide
- `runlevel`: complètement ignoré par BusyBox (laissé toujours vide)
- `action`: action à appliquer sur le programme, 8 possibilité
    <div class="md-decimal-list">
    1. `sysinit`: script d'initialisation
    1. `respawn`: redémarre le processus/programme chaque fois qu'il se termine
    1. `askfirst`: similaire à `respawn` mais demande à la console s'il doit être réactivé
    1. `wait`: attend que le processus soit terminé pour continuer
    1. `once`: lance le programme une seule fois sans attendre
    1. `ctrlatldel`: lance le programme lorsque les touches ++ctrl+alt+del++ sont pressées
    1. `shutdown`: lance le programme lorsque le système est en _shutdown_
    1. `restart`: processus à lancer lorsque le processus d'init est redémarré
    </div>
- `process`: spécifie le programme (avec son chemin) que l'action doit
  contrôler

**Exemple de fichier tiré du NanoPi NEO Plus2**

``` text
# /etc/inittab
#
# Copyright (C) 2001 Erik Andersen <andersen@codepoet.org>
#
# Note: BusyBox init doesn't support runlevels. The runlevels field is
# completely ignored by BusyBox init. If you want runlevels, use
# sysvinit.
#
# Format for each entry: <id>:<runlevels>:<action>:<process>
#
# id == tty to run on, or empty for /dev/console
# runlevels == ignored
# action == one of sysinit, respawn, askfirst, wait, and once
# process == program to run

# Startup the system
::sysinit:/bin/mount -t proc proc /proc
::sysinit:/bin/mount -o remount,rw /
::sysinit:/bin/mkdir -p /dev/pts /dev/shm
::sysinit:/bin/mount -a
::sysinit:/sbin/swapon -a
null::sysinit:/bin/ln -sf /proc/self/fd /dev/fd
null::sysinit:/bin/ln -sf /proc/self/fd/0 /dev/stdin
null::sysinit:/bin/ln -sf /proc/self/fd/1 /dev/stdout
null::sysinit:/bin/ln -sf /proc/self/fd/2 /dev/stderr
::sysinit:/bin/hostname -F /etc/hostname

# now run any rc scripts
::sysinit:/etc/init.d/rcS

# Put a getty on the serial port
console::respawn:/sbin/getty -L console 0 vt100 # GENERIC_SERIAL

# Stuff to do for the 3-finger salute
#::ctrlaltdel:/sbin/reboot

# Stuff to do before rebooting
::shutdown:/etc/init.d/rcK
::shutdown:/sbin/swapoff -a
::shutdown:/bin/umount -a -r
```

## Fichier `S??*`

Les scripts de lancement des applications sont appelés dans l'ordre
alphabétique. Ils doivent impérativement débuter avec un S majuscule lequel
sera suivi de deux chiffres.

Exemple de fichier: (.../daemon)

``` bash
#!/bin/sh
#
# Application daemon
#

case "$1" in
    start)
    /usr/local/app_a
    ;;
    
    stop)
    killall app_a
    ;;
    
    restart|reload)
    killall app_a
    /usr/local/app_a
    ;;
    
    *)
    echo $"Usage: $0 {start|stop|restart}"
    exit 1
esac

echo "Application daemon launched"
exit $?
```