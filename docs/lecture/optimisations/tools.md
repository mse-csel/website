---
title: Outils
---

## Aperçu

Linux propose une large palette d'outils pour la mesure de performances :

<figure markdown>
![](img/linux_perf_tools_full.png)
</figure>

## strace

`strace` est un outil permettant de surveiller les appels système utilisés par une
application sous Linux

```text
# strace -tt ./clock 1

01:43:36.802013 execve("./clock", ["./clock", "1"], 0xffffc8c547e0 /* 16 vars */) = 0
01:43:36.804217 brk(NULL) = 0x89f2000
01:43:36.804476 mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xffffb05d .
01:43:36.804795 faccessat(AT_FDCWD, "/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file or directo .
01:43:36.805084 openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or .
01:43:36.805252 openat(AT_FDCWD, "/lib/tls/aarch64/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No su .
01:43:36.805444 newfstatat(AT_FDCWD, "/lib/tls/aarch64", 0xfffff027b800, 0) = -1 ENOENT (No such fil ..
01:43:36.805662 openat(AT_FDCWD, "/lib/tls/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file ..
01:43:36.805840 newfstatat(AT_FDCWD, "/lib/tls", 0xfffff027b800, 0) = -1 ENOENT (No such file or ...
01:43:36.805965 openat(AT_FDCWD, "/lib/aarch64/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such ...
01:43:36.806093 newfstatat(AT_FDCWD, "/lib/aarch64", 0xfffff027b800, 0) = -1 ENOENT (No such file ...
01:43:36.806288 openat(AT_FDCWD, "/lib/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
01:43:36.806450 read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0\267\0\1\0\0\0\350\365\1\0\0\0\0\0"..., ...
01:43:36.806634 fstat(3, {st_mode=S_IFREG|0755, st_size=1283888, ...}) = 0
01:43:36.806778 mmap(NULL, 1356392, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0xffff ...
01:43:36.806937 mprotect(0xffffb0591000, 61440, PROT_NONE) = 0
......
01:43:36.809857 write(1, "time=0'000'001 ns\n", 18time=0'000'001 ns
) = 18
01:43:36.810015 write(1, "292\n", 4292
) = 4
01:43:36.810183 exit_group(0) = ?
01:43:36.810781 +++ exited with 0 +++
# 
```

Il est important de noter que strace influence énormément le comportement de
l'application

`strace` propose un grand choix d'options, ici une autre façon de mesurer le temps

```text
# strace -c ./clock 100000
...
% time     seconds  usecs/call     calls    errors syscall                      
------ ----------- ----------- --------- --------- ----------------             
100.00    1.643296          16    100002           write                        
  0.00    0.000000           0         1           ioctl                        
  0.00    0.000000           0         1         1 faccessat                    
  0.00    0.000000           0         5         4 openat                       
  0.00    0.000000           0         1           close                        
  0.00    0.000000           0         1           read                         
  0.00    0.000000           0         3         3 newfstatat                   
  0.00    0.000000           0         2           fstat                        
  0.00    0.000000           0         1           sched_setaffinity            
  0.00    0.000000           0         3           brk                          
  0.00    0.000000           0         1           execve                       
  0.00    0.000000           0         4           mmap                         
  0.00    0.000000           0         4           mprotect                     
------ ----------- ----------- --------- --------- ----------------             
100.00    1.643296                100029         8 total                        
```

## ltrace

`ltrace` est un outil permettant de surveiller les appels faits par une application à
des méthodes de bibliothèques partagées (_shared libraries_)

```text
# ltrace -tt ./clock 1

00:04:31.538829 __libc_start_main([ "./clock", "1" ] <unfinished ...>
00:04:31.549903 sched_setaffinity(0, 128, 0xfffff2218f08, 0xfffff2218f08) = 0
00:04:31.550871 printf("clocks_per_sec=%ld\n", 1000000clocks_per_sec=1000000
)                          = 23
00:04:31.552707 atol(0xfffff2219e96, 0, 1, 0) = 1
00:04:31.553544 clock_getres(4, 0xfffff2218ef8, 10, 0xfffff2219e97) = 0
00:04:31.554208 printf("time=%ld'%03ld'%03ld ns\n", 0, 0, 1time=0'000'001 ns
)                     = 18
00:04:31.555774 clock_gettime(4, 0xfffff2218eb0, 1, 0) = 0
00:04:31.556448 clock_gettime(4, 0xfffff2218eb0, 0x20f342eff9be1b74, 0x20f3bd10726438f8) = 0
00:04:31.557182 clock_gettime(4, 0xfffff2218eb0, 0x20f342eff9be1b74, 0x20f3bd10726438f8) = 0
00:04:31.557924 clock_gettime(4, 0xfffff2218ea0, 0x20f342eff9be1b74, 0x20f3bd10726438f8) = 0
00:04:31.558599 printf("%lld\n", 678875678875
)                                         = 7
00:04:31.560006 +++ exited (status 0) +++
```

Là également, il est important de noter que `ltrace` influence passablement le
comportement de l'application

`ltrace` propose aussi un grand choix d'options, ici une autre façon de mesurer
le temps

```text
# ltrace -c ./clock 1000 > /dev/null 

% time     seconds  usecs/call     calls      function
------ ----------- ----------- --------- --------------------
 57.09    0.910034      910034         1 __libc_start_main
 28.43    0.453122         226      2002 clock_gettime
 14.40    0.229538         229      1002 printf
  0.03    0.000452         452         1 exit_group
  0.02    0.000384         384         1 sched_setaffinity
  0.02    0.000245         245         1 atol
  0.01    0.000238         238         1 clock_getres
------ ----------- ----------- --------- --------------------
100.00    1.594013                  3009 total
```

## ftrace

`ftrace` est un outil pour l'analyse d'un système mettant en œuvre une
instrumentation au niveau noyau Linux

`ftrace` permet de tracer à la µs les événements suivants :

- Appels système
- Fonctions de traitement d'interruptions
- Fonctions d'ordonnancement
- Piles de protocoles réseau

Une large documentation est disponible dans le noyau

- https://www.kernel.org/doc/Documentation/trace/ftrace.txt
- https://www.kernel.org/doc/Documentation/trace/events.txt

Avant d'utiliser `ftrace` il faut préalablement configurer le noyau Linux

```text
make linux-menuconfig

--> kernel-hacking
    --> Tracers
        --> Kernel Function Tracer
        --> Trace syscalls
```

## ftrace - Utilisation

L'utilisation de `ftrace` passe par les étapes suivantes :

- Sélectionner un traceur indiquant la fonctionnalité à tracer dans le noyau
- Définir un filtre pour limiter les traces produites
  (exemple: le nom de la fonction à tracer)
- Activer la _trace_ en effectuant la commande :
  ```bash
  echo 1 > /proc/sys/kernel/ftrace_enabled
  ```
- Après enregistrement, désactiver la _trace_ afin de limiter la taille des données par :
  ```bash
  echo 0 > /proc/sys/kernel/ftrace_enabled
  ```
- Exploiter les résultats

L'utilisation de `ftrace` nécessite d'activer la fonctionnalité _debugfs_

```bash
mount -t debugfs none /sys/kernel/debug
```

On dispose alors du répertoire `/sys/kernel/debug/tracing` contenant
entre-autres le fichier trace qui contient les résultats de l'instrumentation.

Les autres principaux fichiers de ce répertoire sont donnés ci-après :

- `available_tracers` contient les différents traceurs disponibles, soit `nop`,
  `function`, `function_graph`, ...
- `current_tracer` contient le traceur courant, donc une valeur de la liste
  précédente
- `tracing_on` permet d'activer/désactiver les traces en écrivant les valeurs `1` ou `0`
- `available_events` contient les évènements traçables comme `sched_wakeup`.
  Ces évènements correspondent à des points de trace (ou _Tracepoints_) statiques
  ajoutés au noyau Linux, voir `Documentation/trace/events.txt`
- `set_ftrace_pid` permet de tracer un processus donné par son PID

## OProfile

OProfile est également un outil de profilage de code sous Linux avec une très
faible empreinte :

- Il permet de profiler aussi bien un système complet qu'un sous ensemble, par exemple
  les routines d'interruption, les pilotes de périphériques ou un processus
- Pour effectuer l'instrumentation du système, il utilise les horloges système
- Il est également capable d'utiliser les compteurs matériels si le processeur en
  disposant

Avec `perf`, `OProfile` est probablement un des outils de monitoring les plus
utilisés sous Linux

La documentation est disponible sur http://oprofile.sourceforge.net
