# PiNum-Lang 🥧🔢

PiNum (represented by the `.pn` extension) is a lightweight, C-inspired programming language currently under development. It features a custom lexer, parser, and code generation pipeline designed for simplicity and potential transpilation or embedding.

## ✨ Features

- **C-Style Syntax:** Familiar data types (`int`, `float`, `char`, `string`, `bool`) and control structures (`if`, `else`, `return`).
- **Standard Library Support:** Includes a modular system using the `@import` directive (e.g., `@import stdlib`, `@import math`).
- **Specialized Modes:** Supports an `ENGINE_MODE` triggered by the `@for engine` directive, allowing for specialized execution contexts.
- **Rich Operator Set:** Includes standard arithmetic, logical, and bitwise operators, as well as pointer-like syntax (`&`).
- **Editor Support:** Built-in syntax highlighting for:
  - **VS Code:** Extension available in `extras/vscode/`.
  - **Neovim:** Lua/Vim syntax files in `extras/nvim/`.

## 🚀 Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`)
- `make`

### Building from Source

To build the `pinum` binary:

```bash
make
```

The compiled binary will be located in the `bin/` directory.

### Running a Program

To execute a `.pn` file:

```bash
./bin/pinum example/helloworld.pn
```

## 📝 Syntax Example (`example/all_tokens.pn`)

```pinum
@import stdlib
@import math
@for engine

int i = 10;
float f = 3.14;
string s = "hello";
bool b = true;

if (i == 10) {
    print("Hello, PiNum!\n");
} else {
    read(i);
}

return 0;
```

## 🛠 Project Structure

- `src/`: Core implementation (Lexer, Parser, Codegen, Helper functions).
- `include/`: Header files defining the language structures and interfaces.
- `bin/`: Compiled binaries.
- `example/`: Sample programs demonstrating language features.
- `extras/`: Editor extensions and syntax highlighting.
- `test/`: Testing suite for language components.

## 🚧 Development Status

PiNum is currently in its early stages:
- [x] Lexer / Tokenizer
- [ ] Parser (In Progress)
- [ ] Code Generation (In Progress)
- [ ] Runtime Library

## 📄 License

This project is licensed under the [MIT License](LICENSE).
