# DriveSyncV2
*[Lire en français](README.fr.md)*

> ⚠️ Work in progress — some features are still experimental.

DriveSyncV2 is a real-time synchronizer between a local folder and Google Drive, written in C++. It watches a local directory and automatically reflects every file/folder creation or deletion on Google Drive.

## Features

- Real-time filesystem event detection via `inotify`
- Automatic upload of locally created files and folders to Google Drive
- Automatic deletion on Google Drive when a local file/folder is removed
- Automatic OAuth2 token refresh on expiration (401)
- Local persistence of Drive IDs in `~/.config/drivesync/elementId.json`

## Architecture

```
DriveSyncV2/
├── app/           → Entry point (main)
├── cloud/         → Google Drive API interface (OAuth2, upload, deletion)
├── config/        → OAuth2 credentials (git-ignored)
├── fileManager/   → Local Drive ID management (LocalSettings)
├── sync/          → Sync orchestration (SyncManager)
├── watcher/       → Local folder watcher (inotify)
└── utils/         → Logger
```

## Requirements

- Linux (uses `inotify`, Linux only)
- CMake >= 3.10
- [libcurl](https://curl.se/libcurl/)
- [nlohmann/json](https://github.com/nlohmann/json) (system-installed)

```bash
# Ubuntu/Debian
sudo apt install libcurl4-openssl-dev nlohmann-json3-dev
```

## Build

```bash
git clone https://github.com/Ferdinaelectro1/DriveSyncV2.git
cd DriveSyncV2
mkdir build && cd build
cmake ..
make
```

## Configuration

Create a `cloud/key.h` file (git-ignored) with your Google OAuth2 credentials:

```cpp
#pragma once
#include <string>

std::string _access_token  = "ya29.xxx";          // expires after 1h, auto-refreshed
const std::string _refresh_token  = "1//xxx";
const std::string client_id       = "xxx.apps.googleusercontent.com";
const std::string client_secret   = "GOCSPX-xxx";

void set_token(const std::string& token) {
    _access_token = token;
}
```

See [Getting OAuth2 credentials](#getting-oauth2-credentials) for setup instructions.

## Usage

Run the executable from the folder you want to sync:

```bash
cd /path/to/your/folder
/path/to/build/driveSync
```

> ℹ️ The app watches the current working directory it is launched from. Configurable folder paths are planned for a future release.

Stop with `Ctrl+C`.

## Getting OAuth2 credentials

1. Create a project on [Google Cloud Console](https://console.cloud.google.com)
2. Enable the Google Drive API
3. Create an OAuth Client ID of type **Desktop app**
4. Publish the app (OAuth consent screen → "Publish app")
5. Generate a refresh token via the OAuth2 flow:

```bash
# 1. Open this URL in your browser
https://accounts.google.com/o/oauth2/v2/auth?client_id=YOUR_CLIENT_ID&redirect_uri=http://localhost&response_type=code&scope=https://www.googleapis.com/auth/drive&access_type=offline&prompt=consent

# 2. Grab the code from the redirect URL, then:
curl -X POST https://oauth2.googleapis.com/token \
  -d client_id=YOUR_CLIENT_ID \
  -d client_secret=YOUR_CLIENT_SECRET \
  -d code=THE_CODE \
  -d redirect_uri=http://localhost \
  -d grant_type=authorization_code
```

## Known Limitations

- The watched folder is the current working directory (not yet configurable)
- No offline support yet — files created without internet connection are not queued
- OAuth2 token is not yet persisted locally between restarts
- No bidirectional sync (Drive → local)

## Roadmap

- [ ] Persist OAuth2 token between restarts
- [ ] Offline queue — sync deferred changes when connection is restored
- [ ] Configurable folder path support
- [ ] systemd integration for auto-start on boot
- [ ] Bidirectional synchronization

## License

This project is licensed under the [Business Source License 1.1](LICENSE).
Free for personal and non-commercial use. Commercial use requires a license from the author.
Starting from **2029-01-01**, this project will be released under the MIT License.