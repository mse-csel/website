---
title: Surveillance de changements dans le système de fichiers
---

## Inotify - Introduction

Pour certaines applications, il peut être très utile de surveiller les
changements apportés à certains fichiers ou répertoires.

Par exemple, un système de backup peut être intéressé à connaitre tous les
fichiers qui ont été ajoutés, enlevés ou modifiés. Cette même surveillance
peut s'avérer très pratique pour des processus _daemon_ utilisant des fichiers
de configuration. Elle lui permettrait d'adapter sa paramétrisation aux
nouvelles modifications.

La solution proposée par Linux s'appelle `inotify`.

`inotify` permet de surveiller 12 événements distincts :

| Event            | Description                                          |
|------------------|------------------------------------------------------|
| IN_ACCESS        | File was accessed (`read()`)                         |
| IN_ATTRIB        | File metadata changed                                |
| IN_CLOSE_WRITE   | File opened for writing was closed                   |
| IN_CLOSE_NOWRITE | File opened read-only was closed                     |
| IN_CREATE        | File/directory created inside watched directory      |
| IN_DELETE        | File/directory deleted from inside watched directory |
| IN_DELETE_SELF   | Watched file/directory was itself deleted            |
| IN_MODIFY        | File was modified                                    |
| IN_MOVE_SELF     | Watched file/directory was itself moved              |
| IN_MOVED_FROM    | File moved out of watched directory                  |
| IN_MOVED_TO      | File moved into watched directory                    |
| IN_OPEN          | File was opened                                      |

## Inotify - Opérations

Le mécanisme _inotify_ de Linux propose divers services pour la surveillance de
fichiers ou de répertoires. Il est intéressant de noter que ceux-ci peuvent être
bloquants ou non bloquants. Dans le cas de services bloquants, les services
de multiplexage (par exemple `epoll`) peuvent être mis en œuvre pour attendre sur
des événements.

| Opération                              | syscall             |
|----------------------------------------|---------------------|
| Créer une instance de surveillance     | `inotify_init1`     |
| Ajouter un nouvel article à surveiller | `inotify_add_watch` |
| Éliminer un article de la surveillance | `inotify_rm_watch`  |
| Lire les événements survenus           | `read`              |
| Fermer une instance de surveillance    | `close`             |

## Créer une instance de surveillance

Pour créer une instance de surveillance, Linux propose l'appel système
`inotify_init1()`.

```c
#include <sys/inotify.h>
int inotify_init1(int flags);
```

**Exemple**

```c
int ifd = inotify_init1(0);
if (ifd == -1)
    /* error */
```

**Comportement**

- La fonction `inotify_init1()` crée une nouvelle instance de surveillance. La
  méthode retourne un descripteur de fichier. En cas d'erreur, la valeur `-1` est
  retournée.
- Si l'on souhaite un service non bloquant, il suffit de passer `IN_NONBLOCK` dans
  l'argument `flags`.

## Ajouter un article à surveiller

Pour ajouter un nouvel article dans l'instance de surveillance, Linux propose
l'appel système `inotify_add_watch()`.

```c
#include <sys/inotify.h>
int inotify_add_watch (int fd, const char* pathname, uint32_t mask);
```

**Exemple**

```c
int wd = inotify_add_watch(ifd, "/path/to/file_dir", IN_ALL_EVENTS);
if (wd == -1)
    /* error */
```

**Comportement**

- La méthode `inotify_add_watch()` ajoute à l'instance de surveillance `fd` un
  nouvel article (fichier ou répertoire) spécifié par le 2^e^ argument `pathname`. Les
  événements que l'on souhaite surveiller sont indiqués par le 3^e^ argument `mask`.
- La méthode retourne un descripteur correspondant à l'article. Ce dernier sera
  associé à chaque événement de ce même article. En cas d'erreur, la valeur `-1`
  est retournée.
- Le processus doit naturellement disposer des droits pour surveiller un fichier ou
  un répertoire.

# Lire les événements

La lecture des événements se réalise simplement avec la méthode `read()`.

```c
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
ssize_t read(int fd, void *buf, size_t len);
```

**Exemple**

```c
char buff[sizeof(struct inotify_event)+NAME_MAX+1];
ssize_t len = read (ifd, buff, sizeof(buff));
if (len == -1)
    /* error */
char* p = buff;
while (len > 0) {
    struct inotify_event* event = (struct inotify_event*)p;
    /* process event... */
    len -= sizeof(struct inotify_event) + event->len;
    p += sizeof(struct inotify_event) + event->len;
}
```

**Comportement**

- La méthode `read()` retourne dans le `buff` une liste d'événements

**Arguments**

- La `struct inotify_event` fournit les informations suivantes
  ```c
  struct inotfy_event {
    int wd; // watch descriptor
    uint32_t mask;   // maks of events
    uint32_t cookie; // unique cookie associating related events -> rename(2)
    uint32_t len;    // size of name field
    char name[];     // optional null-terminated name
  };
  ```
- `wd` identifie l'article surveillé pour lequel l'événement a été levé.
- `mask` contient les bits décrivant l'événement
- `cookie` est une valeur entière unique permettant de mettre en relation deux
  événements. Ceci arrive si l'on renomme un fichier ou un répertoire. Dans les
  autres cas, `cookie` vaut `0`.
- `name` identifie le nom relatif du fichier ou du répertoire pour lequel l'événement
  a été levé.
- `len` décrit le nombre de caractères contenus dans `name` incluant le
  caractère `0` terminant le _string_.