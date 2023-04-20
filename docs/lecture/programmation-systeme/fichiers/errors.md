---
title: Traitement des erreurs
---

Sous Linux, il est usuel que l'appel à des fonctions système retourne une valeur
entière signée (`int syscall()`). Si la valeur retournée est `0` ou supérieur à `0`,
elle indique le succès de l'opération. Par contre, en cas d'erreur, on obtient
généralement `-1`.

Le détail de l'erreur (sa cause / son type) peut être obtenu par l'intermédiaire de
la variable globale `extern int errno;` déclarée dans le fichier `<errno.h>`,
lequel définit aussi les codes d'erreurs du système, par exemple :

| Code    | Description               | Code    | Description                         |
|---------|---------------------------|---------|-------------------------------------|
| EPERM   | Operation not permitted   | EXDEV   | Cross-device link                   |
| ENOENT  | No such file or directory | ENODEV  | No such device                      |
| ESRCH   | No such process           | ENOTDIR | Not a directory                     |
| EINTR   | Interrupted system call   | EISDIR  | Is a directory                      |
| EIO     | I/O error                 | EINVAL  | Invalid argument                    |
| ENXIO   | No such device or address | ENFILE  | File table overflow                 |
| E2BIG   | Argument list too long    | EMFILE  | Too many open files *               |
| ENOEXEC | Exec format error         | ENOTTY  | Not a typewriter                    |
| EBADF   | Bad file number           | ETXTBSY | Text file busy                      |
| ECHILD  | No child processes        | EFBIG   | File too large                      |
| EAGAIN  | Try again                 | ENOSPC  | No space left on device             |
| ENOMEM  | Out of memory             | ESPIPE  | Illegal seek                        |
| EACCES  | Permission denied         | EROFS   | Read-only file system               |
| EFAULT  | Bad address               | EMLINK  | Too many links                      |
| ENOTBLK | Block device required     | EPIPE   | Broken pipe                         |
| EBUSY   | Device or resource busy   | EDOM    | Math argument out of domain of func |
| EEXIST  | File exists               | ERANGE  | Math result not representable       |

La bibliothèque standard C fournit plusieurs services pour convertir les codes
d'erreurs en une représentation textuelle et les afficher sur la console.

Exemple 1 :

```c
#include <stdio.h>
#include <string.h> 
#include <errno.h> 

int ret = fonction();
if (ret == -1)
    perror("ERROR");
```

Exemple 2 :

```c
#include <stdio.h>
#include <string.h> 
#include <errno.h>

int ret = fonction();
if (ret == -1) {
    char estr[100] = {[0]=0,};
    strerror_r(errno, estr, sizeof(estr)-1);
    fprintf (stderr, "ERROR: %s", estr);
}
```

Remarque :

- La méthode `strerror` n'est pas _thread-safe_. Il est préférable d'utiliser la
  méthode `strerror_r`
- Bien que la variable `errno` soit globale, sous Linux, elle est stockée par _thread_
  et par conséquent sûre.