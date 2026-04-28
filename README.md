<div align="center">

# bsd

### A header-first, dependency-light **binary serialization / deserialization** library for modern C++23.

<br>

[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-%E2%89%A5%203.26-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Composition](https://img.shields.io/badge/composition-headers%20%2B%201%20cpp-blue)](src)

[![Linux](https://img.shields.io/badge/Linux-supported-FCC624?logo=linux&logoColor=black)](#)
[![macOS](https://img.shields.io/badge/macOS-supported-000000?logo=apple&logoColor=white)](#)
[![Windows](https://img.shields.io/badge/Windows-supported-0078D6?logo=windows&logoColor=white)](#)

[![GCC](https://img.shields.io/badge/GCC-%E2%89%A513-A42E2B?logo=gnu&logoColor=white)](#)
[![Clang](https://img.shields.io/badge/Clang-%E2%89%A517-262D3A?logo=llvm&logoColor=white)](#)
[![MSVC](https://img.shields.io/badge/MSVC-%E2%89%A519.37-5C2D91?logo=visualstudio&logoColor=white)](#)

[![Code style: clang-format](https://img.shields.io/badge/code%20style-clang--format-1f425f.svg)](.clang-format)
[![Static analysis: clang-tidy](https://img.shields.io/badge/static%20analysis-clang--tidy-1f425f.svg)](.clang-tidy)
[![Sanitizers: ASan | UBSan | TSan](https://img.shields.io/badge/sanitizers-ASan%20%7C%20UBSan%20%7C%20TSan-orange)](#quality-gates)
[![Tests: GoogleTest](https://img.shields.io/badge/tests-GoogleTest-4285F4?logo=google&logoColor=white)](#quality-gates)
[![Fuzzing: libFuzzer](https://img.shields.io/badge/fuzzing-libFuzzer-262D3A?logo=llvm&logoColor=white)](#quality-gates)
[![Benchmarks: Google Benchmark](https://img.shields.io/badge/benchmarks-Google%20Benchmark-4285F4?logo=google&logoColor=white)](#quality-gates)

</div>

---

## What we are building

`bsd` turns arbitrary C++ values into compact, deterministic byte streams and
back again, **without ever trusting the input it reads**.

The library is designed around four properties:

| | Property | What it means |
|---|---|---|
| **1** | **Deterministic & portable** | Explicit endianness, alignment, and stable encoding for every supported type. The same value produces the same bytes on every platform and every run. |
| **2** | **Safe by construction** | Every read is bounded, every length is validated before use, and all fallible operations report failure through `std::expected<T, serialize_error>` rather than throwing or invoking UB on malformed input. |
| **3** | **Fast** | Hot paths avoid heap allocation, lean on `std::span` / `std::bit_cast` / `std::memcpy`, and provide bulk paths for trivially copyable arrays. Heavyweight features (compression, checksums, framing) are opt-in plug-ins. |
| **4** | **Evolvable** | Schemas can grow over time: types carry version tags, fields can be optional, and unknown fields can be skipped or rejected per call. |

### Functional scope

<details open>
<summary><b>Primitives & encodings</b></summary>

- All fixed-width integers, `bool`, `std::byte`, enumerations, `float`, `double`.
- Variable-length integers (LEB128) and signed varints (ZigZag + LEB128).
- Strings (`std::string`, `std::u8string`, `std::string_view`) with optional UTF-8 validation.

</details>

<details open>
<summary><b>Standard-library types</b></summary>

- Containers: `array`, `vector`, `deque`, `list`, `forward_list`, `set`, `map`, `unordered_*`, `bitset`, `valarray`.
- Sum / product types: `optional`, `variant`, `tuple`, `pair`, `expected`.
- Smart pointers: `unique_ptr`, `shared_ptr` (with optional interning).
- `std::chrono` durations and time points.

</details>

<details open>
<summary><b>User & polymorphic types</b></summary>

- User-defined types via a non-intrusive `tag_invoke` customization point and an aggregate-reflection helper.
- Polymorphic types via a stable, registry-based type-tag system (no RTTI).

</details>

<details open>
<summary><b>Framing & I/O</b></summary>

- Optional framing layer: magic bytes, format/schema versions, flags, payload length, integrity trailer.
- Optional plug-ins: checksums (CRC32C, xxHash3), compression (zstd, lz4), and a transform hook for encryption / HMAC.
- Stream backends: in-memory span, growable buffer, file, memory-mapped file, counting (dry-run sizing) and hashing writers.

</details>

### Out of scope

- Text formats (JSON / YAML / XML) — `bsd` is binary-only.
- RPC or transport.
- Schema IDL / code generation (a possible later add-on).
- Cryptographic primitives (only a hook is provided).

---

## Tech & tools

### Language & standard library

- **C++23** (no extensions). Targets **GCC ≥ 13**, **Clang ≥ 17**, **MSVC ≥ 19.37** (Visual Studio 2022 17.7+).
- Public symbols live in `bsd::inline v1` so the ABI can evolve without breaking already-built consumers.

### Build & packaging

- **CMake ≥ 3.26** with `CMakePresets.json` (`dev-debug`, `dev-release`, `ci`, `ci-msvc`).
- **Ninja** as the default generator on Linux/macOS; **Visual Studio 17 2022** on Windows.
- Mostly header-only: public API lives under `include/bsd/`, plus one small
  compiled unit (`src/error_category.cpp`) so `std::error_category` has a
  single program-wide singleton. Link target `bsd::bsd` (static library).
- Versioning via a single `project(... VERSION ...)` declaration that flows through a configured `include/bsd/version.hpp` — no version constant is hardcoded anywhere else.

### Quality gates

| Concern              | Tool / mechanism                                            |
|----------------------|-------------------------------------------------------------|
| Warnings-as-errors   | Centralised in `cmake/CompilerWarnings.cmake` (GCC / Clang / MSVC) |
| Memory safety        | **AddressSanitizer**, **MemorySanitizer** (Clang)           |
| Undefined behaviour  | **UndefinedBehaviorSanitizer**                              |
| Data races           | **ThreadSanitizer**                                         |
| Static analysis      | **clang-tidy** (`bugprone-*`, `cert-*`, `cppcoreguidelines-*`, `modernize-*`, `performance-*`, …) |
| Style                | **clang-format** (LLVM-based, 100-col, project conventions) |
| Unit tests           | **GoogleTest** + `ctest`                                    |
| Fuzzing              | **libFuzzer** harnesses (and corpora under `fuzz/`)         |
| Benchmarks           | **Google Benchmark**, JSON output archived per commit       |
| Coverage             | **llvm-cov** / `gcov`                                       |
| CI                   | GitHub Actions matrix: Ubuntu (GCC, Clang) × macOS (AppleClang) × Windows (MSVC), Debug + Release, sanitizer jobs |

### Optional dependencies

All third-party libraries are pulled in via `FetchContent` and gated behind a CMake option. **No third-party code is required to build the core library.**

| Dependency  | Used for                                       | Option                |
|-------------|------------------------------------------------|-----------------------|
| **zstd**    | Compression plug-in                            | `BSD_WITH_ZSTD`       |
| **lz4**     | Compression plug-in                            | `BSD_WITH_LZ4`        |
| **xxHash**  | Fast non-cryptographic integrity hashing       | `BSD_WITH_XXHASH`     |

### Repository conventions

- **`.editorconfig`** + **`.gitattributes`** for cross-platform whitespace and line-ending normalisation.
- **`.clang-format`** and **`.clang-tidy`** at the repo root drive both the CI checks and the in-editor (clangd) experience.
- **No hardcoded values** in implementation code: every tunable (limits, magic bytes, version numbers, buffer sizes, defaults) lives in a central configuration header or is injected through a CMake option.
- **Every public header documents its invariants** with `@invariant` comments.

---

## Build

With CMake ≥ 3.26 and Ninja on the `PATH`:

```bash
cmake --preset dev-debug
cmake --build --preset dev-debug
./build/dev-debug/examples/bsd_smoke
```

Available configure presets:

| Preset        | Purpose                                       |
|---------------|-----------------------------------------------|
| `dev-debug`   | Local development, Debug build                |
| `dev-release` | Local development, Release build              |
| `ci`          | RelWithDebInfo, warnings-as-errors            |
| `ci-msvc`     | Visual Studio 2022 multi-config build         |

> Every preset enables `compile_commands.json` for clangd / clang-tidy.

---

## Repository layout

```text
binary-serializer-deserializer/
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE
├── README.md
├── .clang-format
├── .clang-tidy
├── .editorconfig
├── .gitattributes
├── .github/workflows/        # CI matrix
├── cmake/                    # ProjectOptions, CompilerWarnings, ...
├── docs/                     # Design notes, format spec, invariants
├── include/bsd/              # Public headers (inline namespace v1)
├── src/                      # Minimal compiled units (error category, …)
├── tests/                    # GoogleTest suite (`ctest`)
└── examples/                 # Smoke / usage examples
```

---

<div align="center">

Released under the [**MIT License**](LICENSE).

</div>
