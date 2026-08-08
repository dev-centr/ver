<a id="readme-top"></a>
<div align="center">
  <a href="https://github.com/dev-centr/ver/graphs/contributors"><img src="https://img.shields.io/github/contributors/dev-centr/ver.svg?style=for-the-badge" alt="Contributors"></a>
  <a href="https://github.com/dev-centr/ver/network/members"><img src="https://img.shields.io/github/forks/dev-centr/ver.svg?style=for-the-badge" alt="Forks"></a>
  <a href="https://github.com/dev-centr/ver/stargazers"><img src="https://img.shields.io/github/stars/dev-centr/ver.svg?style=for-the-badge" alt="Stargazers"></a>
  <a href="https://github.com/dev-centr/ver/issues"><img src="https://img.shields.io/github/issues/dev-centr/ver.svg?style=for-the-badge" alt="Issues"></a>
  <a href="https://github.com/dev-centr/ver/blob/main/LICENSE"><img src="https://img.shields.io/github/license/dev-centr/ver.svg?style=for-the-badge" alt="License"></a>

  <h1>Ver</h1>
  <p>Universal Version Control Interface — library, CLI, and Qt GUI with one command model across Git, SVN, and Mercurial.</p>
  <p>
    <a href="https://docs.devcentr.org/ver/"><strong>Explore the docs</strong></a>
    &middot;
    <a href="https://github.com/dev-centr/ver/issues">Report Bug</a>
    &middot;
    <a href="https://github.com/dev-centr/ver/issues">Request Feature</a>
  </p>
</div>

## About The Project

Ver maps a small, consistent vocabulary (`status`, `save`, `sync`, …) onto the nearest VCS root. Use it as a **CLI**, embed the **D library**, or open the **Qt GUI**.

Legacy C sources under `src/` remain for reference; the supported build is DUB (`source/`).

### Built With

* **D** — library + CLI + GUI
* **DUB** — package / build
* **Qt 6 + dqt** — optional GUI (`dlang-supplemental/dqt`)

## Installation

```bash
dub build --config=cli --build=release
dub build --config=library --build=release
# optional:
dub build --config=gui --build=release
```

As a dependency (DUB registry after first publish, or git):

```json
"dependencies": {
  "ver": {
    "repository": "git+https://github.com/dev-centr/ver.git",
    "version": "~main"
  }
}
```

## Usage

```bash
ver status
ver save "message"
ver sync
ver-gui
```

## Changelog

See [CHANGELOG.adoc](./CHANGELOG.adoc).

## License

Boost Software License 1.0 — see [LICENSE](./LICENSE).

## Contact

Dev-Centr — support@devcentr.org

Project Link: https://github.com/dev-centr/ver
