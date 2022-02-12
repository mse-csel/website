---
title: "02 : Environnement Linux embarqué"
---

## Slides

[02 Environnement Linux embarqué](assets/sp.02.1_mas_csel_environnement_linux_embarque.pdf)

## Exercices / Travaux pratiques

### Objectifs

Ce travail pratique vise les objectifs suivants :

- Mise en œuvre d'un système embarqué sous Linux
- Mise en œuvre de l'environnement de développement de systèmes embarqués sous Linux avec Docker
- Debugging d'applications sous Linux embarqué
- Mise en production d'un système embarqué sous Linux

### Activités

Il comprendra les activités suivantes :

1. Mise œuvre de l'environnement de développement sous Linux (avec Docker)
    1. Installation de la machine hôte
    1. Création de l'espace de travail sur la machine hôte
    1. Génération de l'environnement de développement (toolchain, U-Boot, Linux kernel, rootfs)
1. Mise en œuvre de l'infrastructure (cible, machine hôte)
    1. Mise en place de l'infrastructure
    1. Gravure (burning) de la carte SD pour la cible
    1. Test de l'environnement de production sous carte SD
    1. Test de l'environnement de développement sous tftp/nfs
1. Debugging d'une application simple depuis la machine hôte
    1. Génération d'une application sur la machine hôte
    1. Lancement de l'application sur la cible
    2. Debugging de l'application avec VS-Code et SSH
2. Mise en production
    1. Génération d'une application
    2. Génération du rootfs avec l'application
    3. Test et validation du produit

### Informations pratiques

Ce paragraphe donne quelques informations utiles pour la réalisation de ce travail pratique.

#### Installation de la machine	hôte

Pour les exercices, nous utilisons des conteneurs Docker. Si vous n'avez pas encore installé Docker Desktop,
téléchargez-le depuis le [site officiel de Docker](https://www.docker.com/products/docker-desktop) et installez-le.

Installez aussi [Visual Studio Code](https://code.visualstudio.com/) si ce n'est pas déjà fait.

#### Configuration de l'environnement de développement

Lorsque vous ouvrez le projet avec VSCode, le système vous proposera probablement d'installer des extensions.
Acceptez-les toutes et cliquez sur le bouton "Install".



#### Génération et installation de l'environnement

Si vous effectuez des modifications de la configuration du noyau ou du rootfs, il faut régénérer les
différents packages et recréer les images. Pour cela, il suffit d'effectuer les commandes suivantes :

```bash
cd /buildroot
make
rm -Rf /rootfs/*
tar xf /buildroot/output/images/rootfs.tar -C /rootfs
```

#### Gravure de la carte SD

Avant de pouvoir graver la carte SD, vous devez copier les images dans le répertoire synchronisé avec votre ordinateur.
Vous pouvez utiliser la commande `rsync` :

```bash
rsync -rlt --delete /buildroot/output/images/ /workspace/buildroot-images
```

Pour graver la carte SD, utilisez le logiciel [Balena Etcher](https://www.balena.io/etcher/).
Insérez la carte SD dans votre ordinateur, sélectionnez l'image `buildroot-images/sdcard.img`, sélectionnez le disque qui correspond
à votre carte SD et cliquez sur "Flash!".

## Documentation

- [Bootargs: Linux kernel boot command-line](assets/sp.02.4_mas_cesl_linux_boot_commands_arguments.pdf)
- [Filesystems for embedded systems](assets/sp.02.5_mas_csel_filesystem_considerations_for_embedded_devices.pdf)

---

!!! note "Archives 2021/2022"
    - [Exercices](assets/sp.02.2_mas_csel_environnement_linux_embarque_exercices.pdf)
    - [Code](assets/sp.02.3_mas_csel_examples.tar)
