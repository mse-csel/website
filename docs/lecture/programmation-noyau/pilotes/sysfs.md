---
title: "sysfs : system file system"
---

## Principe

sysfs (system filesystem), accessible sous `/sys`, est un système de fichiers
virtuels créés pour rendre le débogage de pilotes de périphériques plus
simples. Aujourd'hui, sysfs va bien au-delà et est utilisé pour représenter
l'architecture et l'état d'un système dans l'espace utilisateur.

sysfs permet de représenter des objets du noyau Linux, leurs attributs et leurs
relations les uns envers les autres, comme suit

| Interne au noyau       | Espace utilisateur |
| ---------------------- | ------------------ |
| Objets du noyau        | Répertoires        |
| Attributs des objets   | Fichiers           |
| Relations entre objets | Liens symboliques  |

Des outils très simple, tels que `ls`, `cat` ou `echo`, offrent un moyen pour
accéder aux informations stockées dans le sysfs sous forme ascii, par exemple :

```shell
cat /sys/class/tty/ttyS0/dev
```

retourne le device number de la première interface série du NanoPi

sysfs est construit sous forme d'arborescence

``` text
/sys/
|-- block
|-- bus
|-- class
|-- dev
|-- devices
|-- firmware
|-- fs
|-- kernel
|-- module
|-- power
```

Ceci permet de voir le système sous différents points de vue, par exemple

- Depuis les périphériques existants dans le système `/sys/devices`
- Depuis la structure du bus système `/sys/bus`
- Depuis les pilotes disponibles `/sys/module`
- Depuis différentes "classes" de périphériques `/sys/class`

La documentation est disponible dans les sources du noyau sous
`Documentation/filesystems/sysfs.txt`

L'interface` <include/device.h>` fournit des services facilitant la représentation
et la gestion des pilotes et de leurs périphériques dans le _sysfs_

<figure markdown>
![](img/sysfs.drawio.svg)
</figure>

- La structure `struct device_driver` et les fonctions `driver_register` et
  `driver_unregister` permettent de créer et d'instancier un pilote de périphérique
  dans le _sysfs_. Le pilote et ses attributs seront ainsi visible sous `/sys/module`.
- La structure `struct device` et les fonctions `device_register` et
  `device_unregister` permettent de créer et d'instancier un périphérique dans le
  _sysfs_. Le périphérique et ses attributs seront ainsi visible sous `/sys/devices`.


Cependant, avant de commencer le développement d'un pilote, il est important de
savoir dans quel bus le pilote doit être inséré (i2c, pci, usb, …) et, le cas échéant,
utiliser les structures et services spécifiques fournis par ces bus.

## Attributs du pilote (driver)

La structure `struct driver_attribute` permet de spécifier des méthodes
d'accès (lecture et/ou écriture) pour la échange d'information avec le pilote
d'un périphérique.

```c
struct driver_attribute {
    struct attribute attr;
    ssize_t (*show) (struct device_driver *drv, char *buf);
    ssize_t (*store) (struct device_driver *drv,
                      const char *buf, size_t count);
};
```

- Il est impératif de créer/instancier, pour chaque attribut du pilote, une telle
structure avec des méthodes d'accès propre.
- Les valeurs de l'attribut sont passées sous forme _ascii_ par l'intermédiaire de
l'argument `buf`.

La macro `DRIVER_ATTR` permet d'instancier très simplement cette structure

```c
DRIVER_ATTR (name, mode, show, store);
```

## Installation des méthodes d'accès (driver)


Pour installer les méthodes d'accès d'un attribut du pilote dans _sysfs_, on
utilisera la fonction

```c
int driver_create_file (struct device_driver *drv,
                        struct driver_attribute *attr);
```

Pour éliminer une entrée dans _sysfs_, on utilisera la méthode

```c
void driver_remove_file (struct device_driver *drv,
                         struct driver_attribute *attr);
```
## Attributs d'un périphérique (device)

La structure `struct device_attribute` permet de spécifier des méthodes
d'accès (lecture et/ou écriture) pour l'échange d'information avec le
périphérique.

```c
struct device_attribute {
    struct attribute attr;
    ssize_t (*show)  (struct device *dev,
                      struct device_attribute *attr, char *buf);
    ssize_t (*store) (struct device *dev,
                      struct device_attribute *attr,
                      const char *buf, size_t count);
};
```

- Il est impératif de créer/instancier, pour chaque attribut d'un périphérique, une
telle structure avec des méthodes d'accès propre.

- Les valeurs de l'attribut sont passées sous forme _ascii_ par l'intermédiaire de
l'argument `buf`.

La macro `DEVICE_ATTR` permet d'instancier très simplement cette structure

```c
DEVICE_ATTR (name, mode, show, store);
```

## Installation des méthodes d'accès (device)

Pour installer les méthodes d'accès d'un attribut du périphérique dans _sysfs_,
on utilisera la fonction

```c
int device_create_file (struct device * dev,
                        struct device_attribute * attr);
```

Pour éliminer une entrée dans _sysfs_, on utilisera la méthode

```c
void device_remove_file (struct device * dev,
                         struct device_attribute * attr);
```

## Création d'un device sous le répertoire class

Les attributs d'un pilote (driver) ou d'un périphérique (device) peuvent être
accessible sous différents répertoires de l'arborscence sysfs. La bibliothèque
`platform_device` permet de créer assez facilement une interface pour y
accéder.

4 Les méthodes ci-dessous sa propre `class` à laquelle on pourra ensuite
attacher les fichiers d'accès aux attributs d'un device.

- Création d'une nouvelle `class`
  ```c
  struct class * class_create (struct module * owner, // THIS_MODULE
                               const char * name);
  ```
- Destruction de la `class`
  ```c
  void class_destroy (struct class * cls);
  ```
- Création d'un `device`
  ```c
  struct device * device_create (struct class * class,
                                 struct device * parent,
                                 dev_t devt,
                                 const char * fmt, ...);
  ```
- Destruction du `device`
  ```c
  void device_destroy (struct class * class, dev_t devt);
  ```
