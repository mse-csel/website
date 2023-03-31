---
title: "procfs: process file system"
---

## Principe

_procfs_ (_process file system_), accessible sous le répertoire `/proc`, est un
pseudo-système de fichiers, créé sous Linux pour accéder, au moyen
d'utilitaires très simple, tels que `ls`, `cat` ou `echo`, aux informations du
noyau. Par exemple :

- `cat /proc/devices` : retourne la liste des pilotes installés, le type et le major number
- `cat /proc/modules` : retourne la liste des modules installés dans le noyau

Les pilotes désirant échanger des données avec des applications, le font
simplement en exportant des fichiers virtuels sous `/proc`. Ceux-ci permettent
l'échange de ces informations sous forme _ascii_.

Cependant aujourd'hui, il est plutôt recommandé d'implémenter cette
fonctionnalité sous `sysfs` que sous `procfs`.

## Méthodes d'accès et leur installation

Au niveau du noyau, l'interface `<linux/proc_fs.h>` offre une série de
méthodes permettant au pilote d'instancier des méthodes de lecture et d'écriture.

Depuis la version 3.10 du noyau, _procfs_ utilise les `fops` employés par les
méthodes d'accès aux pilotes de périphériques devront être utilisées.

Pour installer les méthodes d'accès dans _procfs_, on utilisera la fonction

```c
struct proc_dir_entry *proc_create (const char *name, mode_t mode,
    struct proc_dir_entry *parent, struct proc_ops *fops);
```

Si l'on désire créer des sous-répertoires, on utilisera la méthode

```c
struct proc_dir_entry *proc_mkdir (const char *name,
    struct proc_dir_entry *parent);
```

Pour éliminer une entrée dans procfs, on utilisera la méthode

```c
void remove_proc_entry (const char *name,
   struct proc_dir_entry *parent);
```

