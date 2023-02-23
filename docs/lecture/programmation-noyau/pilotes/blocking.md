---
title: "Opérations bloquantes"
---

## Opérations d'entrées/sorties bloquantes

Quand une opération de lecture ou d'écriture doit attendre sur des données ou
sur la disponibilité du périphérique, le pilote doit bloquer le thread jusqu'à ce
que les données soient disponibles. Ceci peut être réalisé en le mettant en
sommeil jusqu'à ce que la requête puisse être satisfaite à l'aide de waitqueues.

Déclaration de la _waitqueue_ (`<linux/wait.h>`, `<linux/sched.h>`) et d'un
fanion de signalisation

```c
wait_queue_head_t my_queue;
int request_can_be_processed = 0;
```

Initialisation de la _waitqueue_

```c
init_waitqueue_head (&my_queue);
```

Attendre jusqu'à ce que l'opération puisse être satisfaite (read/write)

```c
wait_event_interruptible(&my_queue, request_can_be_processed != 0);
```

Lorsque les ressources sont disponibles, le pilote peut réveiller le thread et lui
signaler que la requête peut finalement être traitée

```c
request_can_be_processed = 1;
wake_up_interruptible(&my_queue);
```

L'application en espace utilisateur sera notifiée s'il utilise un des appels
système pour la scrutation (`select`, `poll` ou `epoll`). Ces opérations
utilisent l'opération `poll` du pilote de périphérique dont les services sont
disponibles dans l'interface `<linux/poll.h>`.

Cette méthode permet d'attendre sur les ressources en mode non bloquant et
peut être pour de opérations de lecture comme d'écriture

```c
static unsigned int skeleton_poll (struct file *f,
                                   poll_table *wait) {
    unsigned mask = 0;
    poll_wait (f, &my_queue, wait);
    if (request_can_be_processed != 0)
    mask |= POLLIN | POLLRDNORM; // read operation
 // mask |= POLLOUT | POLLWRNORM; // write operation
    return mask;
}
```