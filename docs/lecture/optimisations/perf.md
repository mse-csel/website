---
title: Perf
---

## Introduction

Depuis la version 2.6.31, perf est intégré au noyau Linux

Il se compose de :

- Back-end _perf_events_ intégré au noyau
- Une application `perf` s'exécutant en espace utilisateur

Le code source de l'application est dans les sources de Linux :

- `linux/tools/perf`
- Documentation sous `linux/tools/perf/Documentation/examples.txt`

Il permet d'enregistrer des événements du _PMU_, mais également des
_tracepoints_ du noyau

## Fonctionnalités

Perf offre les fonctionnalités suivantes :

- Génération d'arbres d'appel (_call graph_)
- Collecte d'événements à partir des compteurs hardware disponible dans le _PMU_
- Collecte de tracepoints du noyau Linux
- Collecte d'événements en temps réel
- Annotation de code source / instructions assembleur
- 
Une documentation détaillée est disponible sur le [wiki officiel](https://perf.wiki.kernel.org)

## Tracepoints du noyau

Un _tracepoint_ est un appel de fonction placé à un endroit stratégique dans le
code du noyau Linux

_Perf_ est capable d'activer les tracepoints :

- Si le _tracepoint_ est désactivé, celui-ci ne fera rien (si ce n'est gaspiller quelques
  cycles pour tester s'il est activé et utiliser quelques bytes dans l'exécutable)
- S'il est activé, le `tracepoint` notifie celui qui l'a configuré

Les _tracepoints_ permettent de tracer :

- Les appels système
- Les événements TCP/IP
- Les opérations du file system
- Les changements de contexte de l'ordonnanceur
- ...

## Application utilisateur

L'application _perf_ se décompose en plusieurs sous-commandes

La liste complète peut être obtenue en invoquant `perf` sans argument :

- `perf list` : affiche la liste de tous les événements disponibles
- `perf stat` : exécute une application et affiche des statistiques des compteurs
  d'événements
- `perf top` : outil de profiling en temps réel similaire à la commande `top`,
  affichant les données des compteurs matériels
- `perf record` : exécute une commande et enregistre des données de compteur
  dans un fichier de profiling nommé `perf.data`
- `perf report` : lit un fichier `perf.data` créé avec la commande `record` et affiche les
  informations de profiling

## Perf List

```text
> perf list
List of pre-defined events (to be used in -e):
  cpu-cycles OR cycles                               [Hardware event]
  instructions                                       [Hardware event]
  cache-references                                   [Hardware event]
  cache-misses                                       [Hardware event]
  branch-instructions OR branches                    [Hardware event]
  branch-misses                                      [Hardware event]
  bus-cycles                                         [Hardware event]
  stalled-cycles-frontend OR idle-cycles-frontend    [Hardware event]
  ref-cycles                                         [Hardware event]

  cpu-clock                                          [Software event]
  task-clock                                         [Software event]
  ...
```

## Perf Stat

```text
> perf stat ./sudoku-resolver
Performance counter stats for './sudoku-resolver':

      17200.968507 task-clock (msec)         #    0.999 CPUs utilized          
             2,019 context-switches          #    0.117 K/sec                  
                21 cpu-migrations            #    0.001 K/sec                  
               150 page-faults               #    0.009 K/sec                  
    66,111,521,094 cycles                    #    3.843 GHz                    
    13,661,886,259 stalled-cycles-frontend   #   20.66% frontend cycles idle   
   <not supported> stalled-cycles-backend  
   166,918,528,666 instructions              #    2.52  insns per cycle        
                                             #    0.08  stalled cycles per insn
    12,592,498,548 branches                  #  732.081 M/sec                  
       368,812,217 branch-misses             #    2.93% of all branches        

      17.222153960 seconds time elapsed
```


## Perf  Record et Report

```text
> perf record ./sudoku-resolver
[ perf record: Woken up 11 times to write data ]
[ perf record: Captured and wrote 2.669 MB perf.data (~116626 samples) ]
```

```text
> perf report
Samples: 69K of event 'cycles', Event count (approx.): 66377473404
 84.68%  sudoku-resolver  sudoku-resolver    [.] isAvailable
 15.02%  sudoku-resolver  sudoku-resolver    [.] fillSudoku
  0.08%  sudoku-resolver  [i915]             [k] gen6_read32
  0.05%  sudoku-resolver  [kernel.kallsyms]  [k] _raw_spin_lock_irqsave
```

## Perf Top

```text
> perf top -e cpu-clock
PerfTop: 1579 irqs/sec  kernel:99.9% exact: 0.0% [4000Hz cpu-clock], (all, 4 CPUs)
------------------------------------------------------------------------------------

    99.73%  [kernel]          [k] arch_cpu_idle
     0.05%  [kernel]          [k] _raw_spin_unlock_irqrestore
     0.04%  [kernel]          [k] _raw_spin_unlock_irq
     0.03%  [kernel]          [k] tick_nohz_idle_exit
     0.01%  [kernel]          [k] __softirqentry_text_start
     0.01%  [kernel]          [k] copy_page
     0.01%  [kernel]          [k] tcp_write_timer_handler
     0.01%  [kernel]          [k] rcu_idle_exit
     0.01%  [kernel]          [k] __local_bh_enable_ip
     0.01%  [kernel]          [k] preempt_count_sub
     0.00%  [vdso]            [.] __kernel_clock_gettime
     0.00%  [kernel]          [k] put_ctx
     0.00%  perf              [.] 0x000000000008d2ac
     0.00%  [kernel]          [k] __schedule
     0.00%  [kernel]          [k] timespec64_add_safe
     0.00%  [kernel]          [k] el0_svc_common
     0.00%  libc-2.28.so      [.] __vsnprintf_chk
     0.00%  [kernel]          [k] unwind_frame
```

## Résolution des symboles

_Perf_ est capable de donner les informations de performances de manière
symbolique, pour ceci il procède de la façon suivante

- Les compteurs hardware donnent l'adresse de l'instruction exécutée
- _Perf_ associe ces adresses aux lignes de code correspondantes
- Les symboles de debug contenus dans l'exécutable sont utilisés
- Il faut donc que notre application soit compilée avec les symboles de debug
  (option `-g` du compilateur)
- _Perf_ peut également résoudre les symboles au niveau du noyau

## Note sur les compteurs

Les compteurs matériels affichés par `perf list` sont des compteurs génériques.

Cependant chaque processeur dispose d'une liste de compteurs propre à son
architecture.

_Perf_ effectue un _mapping_ entre les compteurs d'une architecture donnée vers
ses compteurs génériques.

Par exemple, le compteur _cycles_ est mappé :

- Processeur Intel: `UNHALTED_CORE_CYCLES` (voir `arch/x86/kernel/cpu/perf_event.c`)
- Processeur AMD: `CPU_CLK_UNHALTED`
- Processeur ARM: voir `arch/arm/kernel/perf_event_v7.c`

## Note sur la précision des événements

Perf est basé sur un échantillonnage par événement _event-based sampling_

- Cela signifie que seulement une partie des événements (et donc des
  instructions) sont enregistrés (dès que le compteur génère une interruption)
- On peut configurer l'échantillonnage de deux manières, en spécifiant:
    - le nombre d'occurrences d'événements (période) --> option `-c`
    - le nombre de échantillons / secondes (fréquence) --> option `-F`
- À cause de l'échantillonnage, on peut avoir des cas spéciaux ou notre application
  se trouve synchronisée avec la collecte d'événements de `perf`
    - On aura ainsi des résultats faussés, ce sera toujours la même instruction qui
      sera enregistrée au moment de l'interruption du compteur.

De plus, le nombre de compteurs est limité par l'architecture

- Si on configure plus d'événements (option `-e`) que de compteurs, `perf` 
  fait du multiplexage entre les différents événements.

Plus d'informations sur le [wiki de perf](https://perf.wiki.kernel.org/index.php/Tutorial#Sampling_with_perf_record)
