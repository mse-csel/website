---
title: Plateforme
---

## Introduction

Pour les périphériques, le noyau Linux a développé un bus _plateforme_, lequel
supporte des pilotes pour la gestion de ces périphériques ne faisant pas partie
d'un type de bus avec une détection dynamique.

L'interface `<linux/platform_device.h>` offre des méthodes/services pour
créer simplement des fichiers d'accès aux attributs des pilotes et
périphériques

## les pilotes (driver)

La structure `struct platform_driver` permet de spécifier les méthodes et
services d'un pilote plateforme

```c
struct platform_driver {
    int  (*probe)    (struct platform_device *);
    int  (*remove)   (struct platform_device *);
    void (*shutdown) (struct platform_device *);
    int  (*suspend)  (struct platform_device *, pm_message_t);
    int  (*resume)   (struct platform_device *);
    struct device_driver driver;
    // ...
};
```

- `probe` : méthode pour l'instanciation du périphérique. Cette méthode n'est appelée que
  si lors de l'enregistrement du pilote, celui-ci trouve un string de compatibilité dans le
  _Device Tree_
- `remove` : méthode pour la destruction du périphérique. Cette méthode est appelée lors
  du dé-enregistrement du pilote
- `shutdown` : méthode pour la destruction du périphérique. Cette méthode est appelée
  lorsque le système Linux est éteint (shutdown)
- `suspend` et `resume` sont des méthodes appelées lors de la mise en sommeil et
réveil du pilote.
- `driver` attribut décrivant le pilote
    - `name` attribut pour spécifier le nom du pilote, lequel sera visible sous `/sys`
    - `of_match_table` pointe sur la table des strings de compatibilité

Les deux méthodes ci-dessous permettent d'enregistrer/de libérer le pilote.

```c
int platform_driver_register (struct platform_driver *);
void platform_driver_unregister (struct platform_driver *);
```

Ces méthodes doivent simplement être appelées dans les méthodes `init` et `exit`
du module.

## Les périphériques (device)

Dans le cas d'un périphérique appartenant à la plateforme, on peut utiliser les
méthodes de l'interface `<linux/platform_device.h>`

```c
struct platform_device {
    const char* name;
    int id;
    struct device dev; // -> a release method should be attached
    u32 num_resources;
    struct resource *resource;
};
```

- Cette structure permet de spécifier le nom du périphérique, le numéro d'instance
  (`id`, -1 s'il n'existe qu'une instance), ainsi que les ressources utilisées par le
  périphérique.
- Si le périphérique implémente les services d'un _char device_, le numéro de
  périphérique obtenu lors de l'instanciation du `cdev` devra être assigné à
  l'attribut `.dev.devt`. Cette opération permettra au noyau Linux de créer le fichier
  d'accès dans `/dev`

Les deux méthodes ci-dessous d'enregistrer/de libérer le périphérique.

```c
int platform_device_register (struct platform_device *);
void platform_device_unregister (struct platform_device *);
```

## Miscdevice

La structure `struct miscdevice` simplifie l'instanciation du périphérique avec la
création d'un fichier d'accès sous `/dev`.

```c
struct miscdevice {
    int minor;
    const char *name;
    const struct file_operations *fops;
    // ...
    struct device *this_device;
    umode_t mode;
};
```

- `minor` minor device number du périphérique, utiliser `MISC_DYNAMIC_MINOR`
  pour obtenir dynamiquement un numéro mineur
- `name` nom du périphérique, ce nom sera également le nom du fichier d'accès
- `fops` pointeur sur la structure contenant les opérations sur le fichier d'accès
  (`struct file_operations`)
- `mode` définit les droits sur fichier d'accès sous /dev

Les deux méthodes ci-dessous d'enregistrer/de libérer le périphérique :

```c
int misc_register (struct miscdevice *);
void misc_unregister (struct miscdevice *);
```