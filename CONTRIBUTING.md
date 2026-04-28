# Contributing to DriveSyncV2

Thank you for your interest in contributing to DriveSyncV2! All contributions are welcome, whether it's bug fixes, new features, documentation improvements, or ideas.

## Getting Started

1. Fork the repository
2. Clone your fork locally

```bash
git clone https://github.com/YOUR_USERNAME/DriveSyncV2.git
cd DriveSyncV2
```

3. Build the project

```bash
mkdir build && cd build
cmake ..
make
```

4. Create your `cloud/key.h` file with your own Google OAuth2 credentials (see [README](Readme.md#configuration))

## How to Contribute

### Reporting a Bug

Open an [issue](https://github.com/Ferdinaelectro1/DriveSyncV2/issues) and include:
- A clear description of the bug
- Steps to reproduce it
- What you expected vs what actually happened
- Your OS and compiler version

### Suggesting a Feature

Open an [issue](https://github.com/Ferdinaelectro1/DriveSyncV2/issues) with the label `enhancement` and describe:
- What problem it solves
- How you imagine it working

### Submitting a Pull Request

1. Make your changes on your fork
2. Make sure the project still builds without errors
3. Submit a Pull Request to the `main` branch
4. Describe clearly what your PR does and why

## Commit Messages

We follow the **Conventional Commits** standard. Please write clear and descriptive commit messages:

```
<type>: <short description>
```

Common types:

| Type | When to use |
|------|-------------|
| `feat` | Adding a new feature |
| `fix` | Fixing a bug |
| `docs` | Documentation changes |
| `refactor` | Code refactoring without behavior change |
| `test` | Adding or updating tests |
| `chore` | Build system, CI, dependencies |

Examples:

```
feat: add offline queue for unsynced files
fix: handle null HOME environment variable
docs: update README with systemd setup
refactor: move token refresh logic to separate method
```

## Code Guidelines

- Write clean, readable code
- Keep functions focused on a single responsibility
- Comment non-obvious logic
- No credentials, tokens, or secrets in your code

## Need Help?

Feel free to open an issue if you have any questions. All skill levels are welcome!