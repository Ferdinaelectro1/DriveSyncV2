# DriveSyncV2
*[Read in English](README.md)*
> ⚠️ Projet en cours de développement — certaines fonctionnalités sont encore expérimentales.

DriveSyncV2 est un synchronisateur en temps réel entre un dossier local et Google Drive, écrit en C++. Il surveille un dossier local et reflète automatiquement chaque création ou suppression de fichier/dossier sur Google Drive.

## Fonctionnalités

- Détection en temps réel des événements filesystem via `inotify`
- Upload automatique des fichiers et dossiers créés localement vers Google Drive
- Suppression automatique sur Google Drive quand un fichier/dossier est supprimé localement
- Refresh automatique du token OAuth2 en cas d'expiration (401)
- Persistance locale des IDs Drive dans `~/.config/drivesync/elementId.json`

## Architecture

```
DriveSyncV2/
├── app/           → Point d'entrée (main)
├── cloud/         → Interface Google Drive API (OAuth2, upload, suppression)
├── config/        → Credentials OAuth2 (ignorés par git)
├── fileManager/   → Gestion locale des IDs Drive (LocalSettings)
├── sync/          → Orchestration de la synchronisation (SyncManager)
├── watcher/       → Surveillance du dossier local (inotify)
└── utils/         → Logger
```

## Prérequis

- Linux (le projet utilise `inotify`, Linux uniquement)
- CMake >= 3.10
- [libcurl](https://curl.se/libcurl/)
- [nlohmann/json](https://github.com/nlohmann/json) (installé système)

```bash
# Ubuntu/Debian
sudo apt install libcurl4-openssl-dev nlohmann-json3-dev
```

## Installation

```bash
git clone https://github.com/Ferdinaelectro1/DriveSyncV2.git
cd DriveSyncV2
mkdir build && cd build
cmake ..
make
```

## Configuration

Crée un fichier `cloud/key.h` (ignoré par git) avec tes credentials OAuth2 Google :

```cpp
#pragma once
#include <string>

std::string _access_token  = "ya29.xxx";          // expire après 1h, régénéré automatiquement
const std::string _refresh_token  = "1//xxx";
const std::string client_id       = "xxx.apps.googleusercontent.com";
const std::string client_secret   = "GOCSPX-xxx";

void set_token(const std::string& token) {
    _access_token = token;
}
```

Pour générer ces credentials, consulte la section [Obtenir des credentials OAuth2](#obtenir-des-credentials-oauth2).

## Utilisation

Lance l'exécutable depuis le dossier que tu veux synchroniser :

```bash
cd /chemin/vers/ton/dossier
/chemin/vers/build/driveSync
```

> ℹ️ L'app surveille le dossier courant depuis lequel elle est exécutée. Le support de chemins configurables est prévu dans une prochaine version.

Arrêt propre avec `Ctrl+C`.

## Obtenir des credentials OAuth2

1. Crée un projet sur [Google Cloud Console](https://console.cloud.google.com)
2. Active l'API Google Drive
3. Crée un OAuth Client ID de type **Application de bureau**
4. Publie l'app (OAuth consent screen → "Publish app")
5. Génère un refresh token via le flow OAuth2 :

```bash
# 1. Ouvre cette URL dans ton navigateur
https://accounts.google.com/o/oauth2/v2/auth?client_id=TON_CLIENT_ID&redirect_uri=http://localhost&response_type=code&scope=https://www.googleapis.com/auth/drive&access_type=offline&prompt=consent

# 2. Récupère le code dans l'URL de redirection, puis :
curl -X POST https://oauth2.googleapis.com/token \
  -d client_id=TON_CLIENT_ID \
  -d client_secret=TON_CLIENT_SECRET \
  -d code=LE_CODE \
  -d redirect_uri=http://localhost \
  -d grant_type=authorization_code
```

## Limitations connues

- Le dossier surveillé est le dossier courant d'exécution (pas encore configurable)
- Pas de gestion offline pour le moment (fichiers créés sans connexion non synchronisés)
- Le token OAuth2 n'est pas encore persisté localement entre les redémarrages
- Pas de synchronisation bidirectionnelle (Drive → local)

## Roadmap

- [ ] Persistance du token OAuth2 entre les redémarrages
- [ ] Queue offline — synchronisation différée au retour de la connexion
- [ ] Support de chemins de dossiers configurables
- [ ] Intégration systemd pour démarrage automatique
- [ ] Synchronisation bidirectionnelle

## License

This project is licensed under the [Business Source License 1.1](LICENSE).
Free for personal and non-commercial use. Commercial use requires a license from the author.
Starting from **2029-01-01**, this project will be released under the MIT License.