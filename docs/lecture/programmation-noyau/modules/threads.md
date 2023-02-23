---
title: Threads dans le noyau
---

## Création de threads dans le noyau

Certaines applications nécessitent l'usage de plusieurs _threads_.
L'interface `<linux/kthread.h>` propose des services simplifiés pour
leur création et leur destruction.

- La macro `kthread_run` permet de créer simplement un _thread_
  ``` c
  struct task_struct* kthread_run(
    int (*threadfn)(void *data), data, namefmt,...);
  ```
    - `threadfn` est la fonction implémentant le corps du _thread_
    - `data` est un pointeur sur des données passées au thread
    - `namefmt` est le nom du thread passé sous la forme d'un printf
- Pour stopper un _thread_, il suffit d'utiliser la fonction `kthread_stop`
  ``` c
  int kthread_stop (struct task_struct *k);
  ```
- La fonction `kthread_should_stop` offre un service au _thread_ pour
  tester périodiquement s'il doit s'arrêter.
  ``` c
  int kthread_should_stop (void);
  ```

 Le corps d'un thread créé avec l'interface `kthread.h` prend la forme suivante :

``` c
int thread (void* data)
{
    while (!kthread_should_stop()) {
        /* do something... */
    }
    return 0;
}
```

La commande `ps` permet de lister tous les threads et processus

La commande `cat /proc/<pid>/stat` permet d'afficher des informations
sur l'état du processus

On trouve facilement sur Internet de petits programmes offrant une
représentation plus conviviale, par exemple :
http://www.brokestream.com/procstat.html