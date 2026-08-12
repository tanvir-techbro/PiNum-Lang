# PiNum Language Support

Syntax highlighting for the [PiNum](https://github.com/tanvir-techbro/PiNum-Lang) programming language (`.pn` files).

## Features

- Syntax highlighting for keywords, types, strings, numbers, comments, and directives
- Automatic comment toggling with `#`
- Auto-closing brackets and quotes

## Install

Install the `.vsix` from the command line:

```bash
code --install-extension pinum-lang-1.0.1.vsix
```

Or use the **Extensions** panel → `...` → **Install from VSIX...** and pick the file.

> Note: `.vsix` files cannot be installed on VS Code for the Web (vscode.dev). Use the desktop app.

## Usage

Any file ending in `.pn` is automatically recognized as PiNum. If highlighting does not appear:

1. Run the command palette → `Developer: Inspect Editor Tokens and Scopes` and check the scope is `source.pn`.
2. If the scope shows plain text, reload the window (`Developer: Reload Window`).

## Release Notes

### 1.0.1

- Fixed package metadata (icon, README, repository) so the extension installs and can be published
- Highlighted `break` / `continue` and single-quoted characters

### 1.0.0

- Initial release
