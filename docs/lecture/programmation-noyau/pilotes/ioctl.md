---
title: "ioctl : input/output control"
---

## Principe

_ioctl_ (_Input/Output Control_) a été introduit dans les systèmes Unix vers la fin
des années 1970. Elle est supportée par la plupart des systèmes Unix, dont
Linux et Mac OS X. Windows fournit une interface similaire connue sous le
nom de _DeviceIoControl_.

_ioctl_ fournit au niveau du mode utilisateur la méthode `ioctl` avec le
prototype suivant:

```c
int ioctl (int fd, unsigned long cmd, ...);
```

- `fd` = descripteur de fichier obtenu lors de son ouverture
- `cmd` = commande/opération devant être exécutée
- `...` = paramètres optionnels
- _return_ : valeur de retour si positif, sinon erreur
  `-EINVAL` (invalid argument)

La fonction `unlocked_ioctl` est appelée quand l'application en espace utilisateur utilise
la méthode `ioctl()` sur le fichier correspondant au pilote

```c
long (*unlocked_ioctl) (struct file *f, unsigned int cmd, unsigned long arg)
```

- Elle permet d'échanger les données de configuration/états avec le pilote du
  périphérique sans bloquer le noyau
- `f` est le pointeur sur la structure de fichier qui a été passé lors de l'opération `open()`
- `cmd` correspond au mot/numéro de commande passé par l'application
- `arg` est le paramètre optionnel de la commande `ioctl()` en espace utilisateur. Dans
  le cas où celui-ci n'est pas spécifié par l'application lors de l'appel, son contenu est
  indéterminé. 

## Mot de commande

Les mots de commande `cmd` sont des nombres uniques, codés sur 32 bits,
permettant d'identifier les opérations que le driver devra exécuter.

Le mot de commande a la structure suivante:

- `type` : nombre magique unique (magic number) codé sur 8 bits, lequel doit est
  défini après consultation de la liste `Documentation/userspace-api/ioctl/ioctl-number.rst` fournie dans
  la documentation de Linux
- `number` : numéro de la commande/opération codé sur 8 bits.
- `direction` : définit le type d'opération devant être exécutée
    - `__IOC_NONE` : pour une commande
    - `__IOC_READ` : pour une lecture de données du pilote vers l'application
    - `__IOC_WRITE` : pour une écriture de données de l'application vers le pilote
    - `__IOC_READ | __IOC_WRITE` : pour une écriture et lecture
- `size` : taille des données de l'utilisateur impliquées dans l'opération (13 ou 14 bits)

L'interface` <linux/ioctl.h>` fournit une série de macros facilitant la
définition des numéros de commande

- `__IO (type, nr)` pour une commande
- `__IOR (type, nr, datatype)` pour une opération de lecture
- `__IOW (type, nr, datatype)` pour une opération d'écriture
- `__IOWR (type, nr, datatype)` pour une opération d'écriture et lecture

Pour décoder une commande, l'interface fournit des macros

- `__IOC_DIR (cmd)` pour la direction
- `__IOC_TYPE (cmd)` pour le type (magic number)
- `__IOC_NR (cmd)` pour le numéro de la commande/opération
- `__IOC_SIZE (cmd)` pour la taille des données

Exemples :

```c
struct SKELETON_RW { char str[100]; };
#define SKELETON_IOMAGIC 'g'
#define SKELETON_IO_RESET  _IO  (SKELETON_IOMAGIC, 0)
#define SKELETON_IO_WR_REF _IOW (SKELETON_IOMAGIC, 1, struct RW)
#define SKELETON_IO_RD_REF _IOR (SKELETON_IOMAGIC, 2, struct RW)
#define SKELETON_IO_WR_VAL _IOW (SKELETON_IOMAGIC, 3, int)
```

## Paramètre optionnel

Le paramètre optionnel `arg` permet à l'application d'échanger, selon les
architectures, jusqu'à 16K octets de données avec le pilote.

Le paramètre est passé au pilote sous la forme d'un `unsigned long`. Celui-ci
peut aussi bien représenter une valeur entière qu'un pointeur.

Si les données sont passées par référence (pointeur), celles-ci doivent être
copiées à l'aide des méthodes définies dans l'interface `<linux/uaccess.h>`

- `copy_from_user`
- `copy_to_user`

Il existe également d'autres méthodes pour effectuer le transfert:

- `acces_ok` : pour vérifier la validité de l'adresse (impératif)
- `put_user` : pour transférer des données du pilote vers l'utilisateur
- `get_user` : pour transférer des données de l'utilisateur vers le pilote
