---
title: "Travaux pratiques"
week: 5
---

{% set exno = namespace(no=0) %}
{% set subno = namespace(no=0) %}
{% macro ex() -%}
{% set exno.no = exno.no + 1 %}**Exercice #{{ exno.no }}**{% set subno.no = 0 %}
{%- endmacro %}
{% macro exx() -%}
**{% set subno.no = subno.no + 1 %}Exercice #{{ exno.no }}.{{ subno.no }}**
{%- endmacro %}

# Pilotes de périphériques / Travaux pratiques

## Pilotes orientés mémoire

{{ ex() }}: Réaliser un pilote orienté mémoire permettant de mapper en espace utilisateur les registres du µP en utilisant le fichier virtuel `/dev/mem`. Ce pilote permettra de lire l'identification du µP (Chip-ID aux adresses `0x01c1'4200` à `0x01c1'420c`) décrit dans l'exercice "Accès aux entrées/sorties" du cours sur la programmation de modules noyau.

{% if solution >= page.meta.week %}
??? success "Solution"
    ```makefile title="Makefile"
    {!docs/programmation-noyau/pilotes/src/exercice01/Makefile!>}
    ```
    ```c title="main.c"
    {!docs/programmation-noyau/pilotes/src/exercice01/main.c!>}
    ```
{% endif %}

## Pilotes orientés caractère

{{ ex() }}: Implémenter un pilote de périphérique orienté caractère. Ce pilote sera capable de stocker dans une variable globale au module les données reçues par l'opération write et de les restituer par l'opération `read`. Pour tester le module, on utilisera les commandes `echo` et `cat`.

{% if solution >= page.meta.week %}
??? success "Solution"
    ```makefile title="Makefile"
    {!docs/programmation-noyau/pilotes/src/exercice02/Makefile!>}
    ```
    ```makefile title="../../buildroot_path"
    {!docs/programmation-noyau/pilotes/src/exercice02/buildroot_path!>}
    ```
    ```text title="../../kernel_settings"
    {!docs/programmation-noyau/pilotes/src/exercice02/kernel_settings!>}
    ```
    ```c title="skeleton.c"
    {!docs/programmation-noyau/pilotes/src/exercice02/skeleton.c!>}
    ```
{% endif %}

{{ ex() }}: Etendre la fonctionnalité du pilote de l'exercice précédent afin que l'on puisse à l'aide d'un paramètre module spécifier le nombre d'instances. Pour chaque instance, on créera une variable unique permettant de stocker les données échangées avec l'application en espace utilisateur.

{% if solution >= page.meta.week %}
??? success "Solution"
    ```c title="skeleton.c"
    {!docs/programmation-noyau/pilotes/src/exercice03/skeleton.c!>}
    ```
{% endif %}

{{ ex() }}: Développer une petite application en espace utilisateur permettant d'accéder à ces pilotes orientés caractère. L'application devra écrire un texte dans le pilote et le relire.

{% if solution >= page.meta.week %}
??? success "Solution"
    ```c title="main.c"
    {!docs/programmation-noyau/pilotes/src/exercice04/main.c!>}
    ```
{% endif %}

## sysfs

{{ ex() }}: Développer un pilote de périphérique orienté caractère permettant de valider la fonctionnalité du sysfs. Le pilote offrira quelques attributs pouvant être lus et écrites avec les commandes `echo` et `cat`. Ces attributs seront disponibles sous l'arborescence `/sys/class/…`.

Dans un premier temps, implémentez juste ce qu'il faut pour créer une nouvelle classe (par exemple : `my_sysfs_class`)

{% if solution >= page.meta.week %}
??? success "Solution"
    ```c title="skeleton.c" hl_lines="115"
    {!docs/programmation-noyau/pilotes/src/exercice05/skeleton.c!>}
    ```
{% endif %}


{{ exx() }} : Ajoutez maintenant les opérations sur les fichiers définies à l'exercice #3. Vous pouvez définir une classe
comme dans l'exercice précédent, ou vous pouvez utiliser un `platform_device`, ou encore un `miscdevice`.

{% if solution >= page.meta.week+1 %}
??? success "Solution"
    ```c title="skeleton.c" hl_lines="157 198 209"
    {!docs/programmation-noyau/pilotes/src/exercice051/skeleton.c!>}
    ```
{% endif %}

## Device Tree _(optionel)_

{{ ex() }}:
Adapter l'implémentation de l'exercice #3 ci-dessus afin que celui-ci utilise un _device tree_ (DT) pour décrire le nombre de périphériques à mettre en œuvre. Le DT sera externe à l'arborescence des sources du noyaux Linux. La structure `struct miscdevice` peut être utilisée pour instancier les _devices_ et les fichiers d'accès (`/dev/…`).

{% if solution >= page.meta.week %}
??? success "Solution"
    ```makefile title="Makefile"
    {!docs/programmation-noyau/pilotes/src/exercice06/Makefile!>}
    ```
    ```c title="skeleton.c"
    {!docs/programmation-noyau/pilotes/src/exercice06/skeleton.c!>}
    ```
    ```text title="boot.cmd"
    {!docs/programmation-noyau/pilotes/src/exercice06/boot.cmd!>}
    ```
{% endif %}


## Opérations bloquantes

{{ ex() }}: Développer un pilote et une application utilisant les entrées/sorties bloquantes pour signaler une interruption matérielle provenant de l'un des switches de la carte d'extension du NanoPI. L'application utilisera le service select pour compter le nombre d'interruptions.

!!! info "Remarque"
    les switches n'ont pas d'anti-rebonds, par conséquent il est fort probable que vous comptiez un peu trop
    d'impulsions; effet à ignorer.

{% if solution >= page.meta.week %}
??? success "Solution"
    **Device Driver :**
    ```c title="drv/skeleton.c"
    {!docs/programmation-noyau/pilotes/src/exercice07/drv/skeleton.c!>}
    ```

    **Application :**
    ```makefile title="app/Makefile"
    {!docs/programmation-noyau/pilotes/src/exercice07/app/Makefile!>}
    ```
    ```c title="app/main.c"
    {!docs/programmation-noyau/pilotes/src/exercice07/app/main.c!>}
    ```
{% endif %}

## Pilotes orientés mémoire _(optionel)_

{{ ex() }}: Sur la base de l'exercice 1, développer un pilote orienté caractère permettant de mapper en espace utilisateur ces registres (implémentation de l'opération de fichier « mmap »). 
Le driver orienté mémoire sera ensuite adapté à cette nouvelle interface.

!!! info "Remarque"
    à effectuer après les exercices des pilotes orientés caractère.

{% if solution >= page.meta.week %}
??? success "Solution"
    **Device Driver :**
    ```c title="drv/skeleton.c"
    {!docs/programmation-noyau/pilotes/src/exercice08/drv/skeleton.c!>}
    ```

    **Application :**
    ```c title="app/main.c"
    {!docs/programmation-noyau/pilotes/src/exercice08/app/main.c!>}
    ```
{% endif %}

## Ioctl _(optionel)_

{{ ex() }}: Implémenter à l'intérieur d'un pilote de périphérique l'opération `ioctl` afin de pouvoir:

- Envoyer une commande
- Ecrire et lire une valeur entière
- Ecrire et lire un bloc de configuration de plus de 50 octets

Afin de valider le pilote, développer une petite application permettant d'effectuer ces opérations et de les valider. 

{% if solution >= page.meta.week %}
??? success "Solution"
    **Device Driver :**
    ```c title="drv/skeleton.h"
    {!docs/programmation-noyau/pilotes/src/exercice09/drv/skeleton.h!>}
    ```
    ```c title="drv/skeleton.c"
    {!docs/programmation-noyau/pilotes/src/exercice09/drv/skeleton.c!>}
    ```

    **Application :**
    ```c title="app/main.c"
    {!docs/programmation-noyau/pilotes/src/exercice09/app/main.c!>}
    ```
{% endif %}

## `procfs` _(optionel)_

{{ ex() }}: Implémenter à l'intérieur d'un pilote de périphérique les opérations nécessaires afin de pouvoir lire un bloc de configuration et de pouvoir modifier le contenu de la valeur entière par `procfs`. Seules les commandes `echo` et `cat` doivent être nécessaires pour manipuler ces attributs.

{% if solution >= page.meta.week %}
??? success "Solution"
    ```c title="skeleton.c"
    {!docs/programmation-noyau/pilotes/src/exercice10/skeleton.c!>}
    ```
{% endif %}

---

!!! note "Archives 2021/2022"
    - [Exercices](assets/sp.04.2_mas_csel_noyau_pilotes_exercices.pdf)
