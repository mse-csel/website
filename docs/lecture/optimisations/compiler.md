---
title: Compilateur
---

## Introduction

Les compilateurs disposent d'options pour le profiling

Ces options vont ajouter des instructions dans le fichier exécutable compilé

Lors de l'exécution de l'application spécialement compilée, des données de
profiling seront enregistrées dans des fichiers

Des outils spécifiques permettront ensuite de traiter ces données

Avantages :

- Simple à utiliser (option de compilation)

Inconvénients :

- Application plus lente
- Comportement de l'application peut se trouver modifié

## Gcov

_Gcov_ est intégré au compilateur GCC de GNU


!!! warning "Info"
    Ne fonctionne pas sur la cible, doit impérativement être testé sur le container docker.

Il permet de mesurer la couverture de code :

- Combien de fois chaque ligne de code est exécutée ?
- Et donc, quelles lignes ne sont jamais exécutées ?

Pour l'activer il suffit de compiler l'application avec les options
`-fprofile-arcs` et `-ftest-coverage`

Lors de l'exécution de l'application, les données seront enregistrées dans des
fichiers `.data`

L'analyse des données se fait avec l'application `gcov` qui permet d'avoir une
vue annotée du code source


## Gcov - Exemple

Les sources se trouvent dans le dépôt git (`src/06_optimization/gcov`)

Pour générer l'application, l'exécuter et évaluer le résultat, utilisez les
commandes ci-dessous

```bash
make
make gcov-generate
make gcov-read
```

<figure markdown>
![](img/compiler.drawio.svg)
</figure>

## Gprof

Gprof est intégré au compilateur GCC de GNU

Il fournit des informations plus détaillées que `gcov`, mais au niveau des
fonctions

- Temps d'exécution de chaque fonction
- Nombre d'appels
- Peut générer un arbre d'appels (_call-graph_) des fonctions

Pour l'activer, il suffit de compiler et linker l'application avec l'option `-pg`

## Gprof - Exemple

Les sources se trouvent dans le dépôt git (`src/06_optimization/gprof`)

Pour générer l'application, l'exécuter et évaluer le résultat, utilisez les
commandes ci-dessous

```bash
make
make gprof-generate
make gprof-read
```

```c
void func1(void)
{
    for(int i=0; i<0xfffffff; i++); // wait...
}

void func2(void)
{
    for(int i=0;i<0xfffffff;i++); // wait...
    func1();
}

int main(void)
{
    for(int i=0;i<0xfffffff;i++);
    func1();
    func2();
    return 0;
}
```

Résultat :

```text
Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 50.21      1.19     1.19        2     0.59     0.59  func1
 24.89      1.78     0.59        1     0.59     1.19  func2
 24.89      2.37     0.59                             main
```

