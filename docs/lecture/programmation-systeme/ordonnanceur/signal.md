---
title: Traitement des signaux
---

## Signaux - Introduction

Les signaux sont des interruptions logicielles permettant de traiter des
événements asynchrones au bon déroulement du programme

Les événements à l'origine de la levée des signaux sont souvent externes au
système, par excemple pression sur la touche ++ctrl+c++ sur un clavier

Un processus peut envoyer un signal à un autre processus ou groupe de
processus, offrant ainsi une forme primitive de communication
interprocessus

Lorsqu'un signal est levé (généré ou envoyé), le noyau effectue une des
actions suivantes:

- **Ignorer le signal** :
  Le noyau ignore le signal et aucune action n'est entreprise.
- **Capturer et traiter le signal** :
  Le noyau arrête l'exécution du processus et traite le signal en appelant une
  fonction ayant été enregistrée préalablement par le processus en cours.
- **Effectuer une action par défaut** :
  Le noyau arrête l'exécution du processus et exécute une action par défaut
  implémentée par le noyau lui-même et dépendante du signal. Celle-ci consiste
  souvent à terminer le processus et générer un coredump.

## Liste

Les signaux supportés par Linux sont disponibles dans le fichier `signal.h`

Chaque signal est identifié par une valeur symbolique avec le préfix `SIG`,
laquelle représente une valeur entière positive non nulle

Signaux disponibles sous Linux (`kill -l`):


| Signal    | No | Description                                  |
|-----------|----|----------------------------------------------|
| SIGHUP    | 1  | Hangup                                       |
| SIGINT    | 2  | Terminal interrupt (Ctrl-C)                  |
| SIGQUIT   | 3  | Terminal quit (Ctrl-\\)                      |
| SIGILL    | 4  | Illegal instruction                          |
| SIGTRAP   | 5  | Trace trap                                   |
| SIGABRT   | 6  | Abort                                        |
| SIGBUS    | 7  | BUS error                                    |
| SIGFPE    | 8  | Floating point exception                     |
| SIGKILL   | 9  | Kill (can't be caught or ignored)            |
| SIGUSR1   | 10 | User defined signal 1                        |
| SIGSEGV   | 11 | Invalid memory segment access                |
| SIGUSR2   | 12 | User defined signal 2                        |
| SIGPIPE   | 13 | Write on a pipe with no reader, Broken pipe  |
| SIGALRM   | 14 | Alarm clock                                  |
| SIGTERM   | 15 | Termination                                  |
| SIGSTKFLT | 16 | Stack fault                                  |
| SIGCHLD   | 17 | Child process has stopped or exited, changed |
| SIGCONT   | 18 | Continue executing, if stopped               |
| SIGSTOP   | 19 | Stop executing (can't be caught or ignored)  |
| SIGTSTP   | 20 | Terminal stop signal (Ctrl-Z)                |
| SIGTTIN   | 21 | Background process trying to read, from TTY  |
| SIGTTOU   | 22 | Background process trying to write, to TTY   |
| SIGURG    | 23 | Urgent condition on socket                   |
| SIGXCPU   | 24 | CPU limit exceeded                           |
| SIGXFSZ   | 25 | File size limit exceeded                     |
| SIGVTALRM | 26 | Virtual alarm clock                          |
| SIGPROF   | 27 | Profiling alarm clock                        |
| SIGWINCH  | 28 | Window size change                           |
| SIGIO     | 29 | I/O now possible                             |
| SIGPWR    | 30 | Power failure restart                        |

Les signaux `SIGSTOP` (19) et `SIGKILL` (9) ne peuvent être ni capturés et ni ignorés.

## Opérations

Pour le traitement des signaux, Linux propose des services permettant
d'attacher une méthode spécifique à chaque signal respectivement de lever
un signal pour un processus ou un groupe de processus.
4 Opérations
q Capturer un signal (syscall: sigaction)
q Lever un signal (syscall: kill)
q Attendre sur un signal (syscall: pause)