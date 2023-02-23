---
title: Debugging
---

## Debugging d'un module

**Le débogage des pilotes sous Linux pour des systèmes embarqués est relativement malaisé.**

- L'utilisation de debugger, tel que kgdb, peut naturellement est
  utilisé, mais généralement nécessite une infrastructure supplémentaire
  (par exemple probes _JTAG_).
- La manière la plus simple est d'insérer à l'intérieur du code des
  `printf`. Dans le noyau, cette technique peut être réalisée avec la
  méthode `printk`, par exemple :
  ```C
  printk (KERN_DEBUG "Here I am: %s:%s\n", __FILE__, __LINE__);
  ```
- Ces messages sont stockés dans un tampon circulaire (cela évite de
  consommer trop de mémoire si le nombre de messages explose)
- Ces messages peuvent également être affichés sur la console après
  avoir passé un filtre dont le niveau est spécifié par le paramètre du
  noyau `loglevel` ou par `/proc/sys/kernel/printk` (voir
  `Documentation/sysctl/kernel.txt`)
- La commande `dmesg` permet de lire les messages, par exemple
  ```bash
  $ dmesg | tail -n10
  ```
- Il existe 8 niveaux de sévérité, du plus haut (0) au plus bas (7):
  ``` text
  <0> KERN_EMERG   Used for emergency messages, usually those that precede a
                   crash.
  <1> KERN_ALERT   A situation requiring immediate action.
  <2> KERN_CRIT    Critical conditions, often related to serious hardware or
                   software failures.
  <3> KERN_ERR     Used to report error conditions; device drivers often use
                   KERN_ERR to report hardware difficulties.
  <4> KERN_WARNING Warnings about problematic situations that do not, in
                   themselves, create serious problems with the system.
  <5> KERN_NOTICE  Situations that are normal, but still worthy of note.
                   A number of security-related conditions are reported
                   at this level.
  <6> KERN_INFO    Informational messages. Many drivers print information
                   about the hardware they find at startup time at this level.
  <7> KERN_DEBUG   Used for debugging messages.
  ```
- L'usage de la fonction `printk` n'est plus recommandé lors de développement
  de nouveaux modules. On lui préférera les fonctions spécialisées `pr_xxx`
    - `pr_emerg()`, `pr_alert()`, `pr_crit()`, `pr_err()`, `pr_warning()`,
      `pr_notice()`, `pr_info()`
    - `pr_debug()` (le module doit être compilé avec le flag `-DDEBUG`, pour plus de
       détails https://www.kernel.org/doc/local/pr_debug.txt)