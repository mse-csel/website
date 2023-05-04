---
title: Control Groups (CGroups)
---

## Introduction

La gestion des ressources des microprocesseur pour les différentes tâches que le système
doit assurer est un problème récurrent sur les systèmes embarqués.

Les groupes de contrôle (_CGroups_) offrent un mécanisme puissant pour
limiter, compter et isoler les ressources du microprocesseur, par exemple les cœurs, la mémoire,
utilisation des I/O.

Les _CGroups_ permettent de partitionner le système en différentes activités.
Ces dernières seront ensuite agrégées dans des groupes hiérarchiques avec
des comportements spécialisés.

<figure markdown>
![](img/cgroups.drawio.svg)
</figure>

## Subsystems

Un sous-système représente une ressource du microprocesseur, par exemple le temps CPU ou la
capacité d'une mémoire.

Les _CGroups_ proposent :

- `cpuset` : pour assigner des CPU individuels (multi-cœurs) et des nœuds de mémoire à
  des tâches dans un groupe de contrôle
- `cpu` : pour fournir aux tâches des groupes de contrôle accès au CPU
- `memory` : pour établir les limites d'utilisation de la mémoire par les tâches d'un groupe
  de contrôle et pour générer des rapports automatiques sur les ressources mémoire
  utilisées
- `blkio` : pour établir des limites sur l'accès des entrées/sorties à partir et depuis des
  périphériques blocs tels que des lecteurs physiques (disques, lecteurs USB, etc.)
- `cpuacct` : pour générer des rapports automatiques sur les ressources CPU utilisées
- `devices` : pour autoriser ou refuser l'accès aux périphériques
- `freezer` : pour suspendre ou réactiver les tâches dans un groupe de contrôle
- `net_cls` : pour repérer les paquets réseau avec un identifiant de classe (_classid_) qui
  permet au contrôleur de trafic Linux (_tc_) d'identifier les paquets provenant d'une tâche
  particulière d'un groupe de contrôle
- `ns` : le sous-système namespace

Documentation: `<Linux sources>/Documentation/cgroups`

## Configuration du noyau Linux

Pour utiliser les _CGroups_, les _flags_ suivants doivent être mis en place dans la
configuration du noyau Linux (commande : `make linux-menuconfig`):

```text

    General setup --->
      [*] Control Group support ---> 
        [*]   Memory controller
        [*]   IO controller
        [*]   CPU controller
        [*]   PIDs controller
        [*] Cpuset controller
        [*]   Include legacy /proc/<pid>/cpuset file
        [*] Device controller
        [*] Simple CPU accounting controller 
        [*] Perf controller  
      [*] Automatic process group scheduling
```

## Utilisation des CGroups

Les _CGroups_ sont disponible en espace utilisateur. Pour cela il suffit de les
monter dans l'arborescence du _rootfs_.

```bash
mount -t tmpfs none /sys/fs/cgroup
mkdir /sys/fs/cgroup/set
mount -t cgroup -o memory,cpu,cpuset cgroups /sys/fs/cgroup/set
```

L'option `-o` permet de choisir les sous-systèmes que l'on souhaite exporter. Si
aucune option n'est spécifiée, l'ensemble des sous-systèmes seront visibles.

Ensuite il suffit de créer la hiérarchie souhaitée, de configurer les ressources
allouées à ces différents groupes de contrôle et de placer les différents
processus dans cette hiérarchie.

**Exemple pour un programme**

```bash
mkdir /sys/fs/cgroup/set/program

echo "2-3" > /sys/fs/cgroup/set/program/cpuset.cpus
echo "0" > /sys/fs/cgroup/set/program/cpuset.mems

echo "20M" > /sys/fs/cgroup/set/program/memory.limit_in_bytes

echo PID(high-program) > /sys/fs/cgroup/set/program/tasks
```