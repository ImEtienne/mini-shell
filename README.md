# Shell Minimaliste

Ce projet présente un shell minimaliste développé en langage C. Il permet à l'utilisateur d'exécuter des commandes et d'interagir avec le système de manière simplifiée. Ce shell met en œuvre diverses fonctionnalités clés telles que la gestion des processus, la redirection de flux, le parsing de commandes et la gestion de signaux.

## Table des matières

- [Fonctionnalités](#fonctionnalités)
- [Prérequis](#prérequis)
- [Compilation](#compilation)
- [Exécution](#exécution)
- [Utilisation](#utilisation)
- [Organisation du Code](#organisation-du-code)
- [Licence](#licence)

## Fonctionnalités

- **Interactivité :**  
  Le shell permet d’interagir directement avec l’utilisateur en acceptant des commandes sans arguments.

- **Gestion des Processus :**  
  Utilisation des fonctions système telles que `fork(2)`, `exec(3)`, `wait(2)` et `exit(3)` pour la création et la gestion des processus.

- **Parsing des Commandes :**  
  Développement d’une fonction de parsing capable de découper les chaînes de commande et de gérer jusqu’à 16 arguments.

- **Gestion des Arguments Supplémentaires :**  
  Prise en charge des arguments supplémentaires fournis sur l’entrée standard après chaque commande.

- **Redirection de la Sortie Standard :**  
  Implémentation de la redirection de la sortie standard vers un fichier grâce aux fonctions `open(2)`, `dup2(2)` et `close(2)`.

- **Implémentation Simplifiée des Pipes :**  
  Mise en place d’une version simplifiée de la chaîne de commandes (pipes) en utilisant `dup2(2)` et `pipe(2)`.

- **Gestion des Signaux :**  
  Le shell ignore le signal `SIGINT` (Ctrl+C) pour éviter une interruption accidentelle, tout en laissant les programmes lancés gérer ce signal normalement.

- **Utilisation Efficace des Appels Système :**  
  Mise en œuvre des fonctions de manipulation de fichiers et de processus en langage C pour garantir la stabilité et la performance du shell.

## Prérequis

Avant de compiler et d'exécuter ce shell, assurez-vous de disposer des éléments suivants :

- **Un compilateur C** (par exemple, GCC).
- **Un environnement Unix/Linux** qui supporte les appels système utilisés.
- **Les droits nécessaires** pour exécuter des opérations système telles que la redirection de flux et la gestion des signaux.

## Compilation

Pour compiler le projet, exécutez la commande suivante dans un terminal :

```bash
gcc -Wall -Werror -std=gnu99 shell.c -o shell

```

## Exécution

Une fois compilé, lancez le shell avec la commande suivante :

```bash  
 ./shell

```

## Utilisation

- **Saisie de Commande :**  
  Tapez une commande puis appuyez sur `Entrée` pour l'exécuter.

- **Arguments :**  
  Le shell gère jusqu'à 16 arguments par commande.

- **Redirection de Sortie :**  
  La sortie standard peut être redirigée vers un fichier grâce aux mécanismes implémentés.

- **Utilisation des Pipes :**  
  Une version simplifiée de la chaîne de commandes (pipes) est disponible pour enchaîner l’exécution   de plusieurs commandes.

- **Gestion du Signal SIGINT :**  
  Le shell ignore le signal `SIGINT` pour éviter une interruption intempestive, tandis que les   programmes lancés reçoivent ce signal et le gèrent normalement.

## Organisation du Code

### `shell.c`

Ce fichier contient l'implémentation principale du shell, incluant :

- **Gestion des processus :**  
  Utilisation des fonctions `fork()`, `exec()`, `wait()` et `exit()` pour créer et gérer les processus.

- **Parsing des commandes :**  
  Une fonction de parsing pour découper la commande et gérer les arguments.

- **Redirection de la sortie standard :**  
  Mise en place de la redirection de la sortie standard avec `open()`, `dup2()` et `close()`.

- **Implémentation simplifiée des pipes :**  
  Utilisation de `pipe()` et `dup2()` pour enchaîner l’exécution de plusieurs commandes via des pipes.

- **Gestion des signaux :**  
  Le shell ignore le signal `SIGINT` dans son propre contexte, permettant aux programmes lancés de recevoir et gérer ce signal normalement.

## Licence

Ce projet est sous licence MIT. Consultez le fichier [LICENSE](https://github.com/ImEtienne/mini-shell?tab=MIT-1-ov-file#) pour plus de détails.

## Capture d'écran

![image](https://private-user-images.githubusercontent.com/86003446/412590894-206db5ea-3dfa-4533-85c2-12c2c589d05a.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MzkzOTA0NDksIm5iZiI6MTczOTM5MDE0OSwicGF0aCI6Ii84NjAwMzQ0Ni80MTI1OTA4OTQtMjA2ZGI1ZWEtM2RmYS00NTMzLTg1YzItMTJjMmM1ODlkMDVhLnBuZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNTAyMTIlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjUwMjEyVDE5NTU0OVomWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTMzZDJkZWM1NWY1NzMyNGJhMjBjNWU5MTFjNjhiMmU5OWFjZDUxZDM3N2ZkY2JjNDg5ZDkxMDQxZDk2OWU4OGYmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0In0.J8t6yptkYQkWG3sG0pZHeJBa4U3MN6jOOMVXOsA-ggs)
