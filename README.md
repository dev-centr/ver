<a id="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]

<div align="center">
  <h1>Ver</h1>
  <p>Universal Version Control Interface — pluggable system for Git, SVN, and other VCS providers with intuitive syntax.</p>
  <p>
    <a href="https://github.com/AMDphreak/ver/issues">Report Bug</a>
    ·
    <a href="https://github.com/AMDphreak/ver/issues">Request Feature</a>
  </p>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#installation">Installation</a></li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

## About The Project

Ver is a pluggable version control interface that provides a unified, intuitive command-line experience across different version control systems. It aims to eliminate the cognitive overhead of learning multiple VCS commands by providing a consistent mental model and syntax.

### Features

- **Provider Agnostic**: Works with Git, SVN, Mercurial, and custom providers
- **Intuitive Syntax**: Commands like `ver save`, `ver history`, `ver sync`
- **Handler-Based Architecture**: Pluggable system for adding new VCS providers
- **Auto-Detection**: Automatically detects the VCS provider for your project
- **Safety Rails**: Prevents common mistakes with built-in safeguards
- **Cross-Shell Support**: Works in CMD, PowerShell, and Unix shells

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Installation

```bash
# Clone the repository
git clone https://github.com/AMDphreak/ver.git
cd ver

# Build the project
make

# Install system-wide
make install
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage

### Basic commands

```bash
# Initialize a new project (defaults to gitu provider)
ver init

# Save changes
ver save "Fixed login bug"

# View history
ver history

# Sync with remote
ver sync

# Check status
ver status
```

### Architecture

Ver uses a handler-based architecture where:

1. **CLI Dispatcher** (`ver`) - Parses commands and detects providers
2. **Abstraction Layer** - Maps intuitive commands to provider-specific actions
3. **Provider Handlers** - Execute the actual VCS commands

#### Included Handlers

- **gitu-handler** - Uses the improved gitu interface (Git Unfuckified)
- **git-handler** - Standard Git compatibility
- **local-handler** - Local file snapshots without remote

### Command Mapping

| Ver Command | Git Equivalent | SVN Equivalent | Description |
|-------------|---------------|---------------|-------------|
| `ver init` | `git init` | `svn init` | Initialize repository |
| `ver save` | `git commit` | `svn commit` | Save changes |
| `ver stage` | `git add` | `svn add` | Stage files |
| `ver history` | `git log` | `svn log` | View history |
| `ver sync` | `git pull && git push` | `svn update && svn commit` | Synchronize |
| `ver branch` | `git branch` | `svn copy` | Manage branches |
| `ver merge` | `git merge` | `svn merge` | Merge changes |

### Provider Detection

Ver automatically detects the version control provider by looking for:

- `.git/` directory → gitu-handler (preferred) or git-handler
- `.svn/` directory → svn-handler
- `.hg/` directory → hg-handler
- `.ver/` directory → local-handler

### Development

#### Project Structure

```
ver/
├── src/
│   ├── core/           # CLI dispatcher logic
│   ├── handlers/       # Provider handlers
│   └── tui/           # Terminal UI components
├── plugins/           # External handler plugins
├── tests/            # Test suites
└── docs/             # Documentation
```

#### Adding New Handlers

1. Create a handler in `src/handlers/`
2. Implement the required interface functions
3. Register the handler in the core dispatcher
4. Add tests

See `docs/handler-development.md` for details.

### Shell Integration

#### PowerShell

Add to your `$PROFILE`:

```powershell
function ver { & ver.exe $args }
```

#### CMD

Create a doskey macro:

```batch
doskey ver=ver.exe $*
```

#### Unix Shells

Add to your `.bashrc` or `.zshrc`:

```bash
alias ver='ver.exe'
```

### Configuration

Ver stores configuration in:

- **Windows**: `%APPDATA%/ver/config.json`
- **Unix**: `~/.config/ver/config.json`

Example configuration:

```json
{
  "default_provider": "gitu",
  "handlers": {
    "gitu": {
      "path": "./plugins/gitu-handler",
      "priority": 1
    },
    "git": {
      "path": "./plugins/git-handler",
      "priority": 2
    }
  },
  "ui": {
    "tui_enabled": true,
    "ai_mode": false
  }
}
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests
5. Submit a pull request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## License

MIT License — see [LICENSE](LICENSE) for details.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contact

Ryan Johnson — [@amdphreak](https://twitter.com/amdphreak)

Project Link: [https://github.com/AMDphreak/ver](https://github.com/AMDphreak/ver)

Site: [https://ryanjohnson.dev](https://ryanjohnson.dev)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/AMDphreak/ver.svg?style=for-the-badge
[contributors-url]: https://github.com/AMDphreak/ver/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/AMDphreak/ver.svg?style=for-the-badge
[forks-url]: https://github.com/AMDphreak/ver/network/members
[stars-shield]: https://img.shields.io/github/stars/AMDphreak/ver.svg?style=for-the-badge
[stars-url]: https://github.com/AMDphreak/ver/stargazers
[issues-shield]: https://img.shields.io/github/issues/AMDphreak/ver.svg?style=for-the-badge
[issues-url]: https://github.com/AMDphreak/ver/issues
[license-shield]: https://img.shields.io/github/license/AMDphreak/ver.svg?style=for-the-badge
[license-url]: https://github.com/AMDphreak/ver/blob/main/LICENSE
