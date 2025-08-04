# Name Mangling and ABI Differences Across Compilers

## Overview

This project demonstrates **C++ name mangling** and **Application Binary Interface (ABI)** differences between compilers (GCC, Clang, MSVC) and operating systems (Linux, macOS, Windows). It showcases how type information is encoded into symbol names, what differences arise from compiler internals, and how `nm`, `dumpbin`, and `c++filt` can be used to explore those symbols.

## Problem Description

C++ allows function overloading, namespaces, templates, etc., which means multiple functions can have the same name but different parameters. To make these functions unique at the binary level, compilers use **name mangling**: they encode type and scope information into function names. However, mangling schemes are **not standardized across compilers**, leading to ABI incompatibilities.

This project:

* Defines a class `Demo` with overloaded `print(...)` functions.
* Compiles it with multiple compilers.
* Compares mangled symbol names across GCC, Clang, and MSVC.

## What Name Mangling Reveals About C++ Type Semantics

1. **Name mangling reveals exact parameter types**, even for overloaded functions.
2. **`const`** applied to value arguments (top-level) does not affect mangling or allow overloads:

   * `void print(int)` == `void print(const int)`
3. **`const`** in pointer/reference content (low-level) *does* affect mangling:

   * `int*` vs `const int*` → different mangled names
4. **`const`** on the pointer itself (top-level) does **not** affect mangling:

   * `const int*` vs `const int* const` → same mangled name
5. **References (****`int&`****, ****`const int&`****)** can be overloaded — different mangled names.

---

## Example Output: Cross-Platform Comparison

### Ubuntu

```
_ZN4Demo5printEi      // int
_ZN4Demo5printEPKi    // const int*
_ZN4Demo5printERj     // unsigned int&
_ZN4Demo5printERKj    // const unsigned int&
_ZN4Demo5printEPl     // long*
_ZN4Demo5printEs      // short
_ZN4Demo5printEd      // double
_ZN4Demo5printEb      // bool
_ZN4Demo5printEc      // char
_ZN4Demo5printEPKc    // const char*
_ZN4Demo5printENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE  // std::string
```

### macOS

```
__ZN4Demo5printEi
__ZN4Demo5printEPKi
__ZN4Demo5printERj
__ZN4Demo5printERKj
__ZN4Demo5printEPl
__ZN4Demo5printEs
__ZN4Demo5printEd
__ZN4Demo5printEb
__ZN4Demo5printEc
__ZN4Demo5printEPKc
__ZN4Demo5printENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE
```

### Windows

```
?print@Demo@@QEAAXH@Z               // int
?print@Demo@@QEAAXPEBH@Z           // const int*
?print@Demo@@QEAAXAEAI@Z           // unsigned int&
?print@Demo@@QEAAXAEBI@Z           // const unsigned int&
?print@Demo@@QEAAXPEAJ@Z           // long*
?print@Demo@@QEAAXF@Z              // short
?print@Demo@@QEAAXN@Z              // double
?print@Demo@@QEAAX_N@Z             // bool
?print@Demo@@QEAAXD@Z              // char
?print@Demo@@QEAAXPEBD@Z           // const char*
?print@Demo@@QEAAXV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z  // std::string
```

## Explanation of Output

GCC and Clang use the Itanium ABI (`_Z...`) with readable `nm` output.

MSVC uses its own mangling scheme (`?print@Demo@@...`) and inspects symbols with `dumpbin /symbols`.

Each mangled symbol name encodes the function name, class, and parameter types. For example:

- `_ZN4Demo5printEi`
  - `Z` = mangled name
  - `N...E` = namespaced/class scope (`Demo::print`)
  - `i` = `int`

- `_ZN4Demo5printERKj`
  - `R` = reference
  - `K` = const
  - `j` = `unsigned int`

## Differences Between Systems
| Platform | Compiler | Example Mangled Symbol | Notes |
|----------|----------|------------------------|-------|
| Ubuntu   | GCC      | `_ZN4Demo5printEi`     | Itanium ABI |
| macOS    | Clang    | `__ZN4Demo5printEi`    | Nearly identical to GCC, double underscore prefix |
| Windows  | MSVC     | `?print@Demo@@QEAAXH@Z`| MSVC uses its own ABI and syntax |

- GCC/Clang symbols are compatible with `nm`, or `c++filt` for demangling.
- MSVC symbols must be inspected with `dumpbin /symbols`.


## How to Compile and Run

### 1. Clone the Repository

```bash
git clone https://github.com/LyudmilaKostanyan/Name-Mangling.git
cd Name-Mangling
```

### 2. Build the Project

Ensure you have CMake and a C++ compiler installed.

The project already includes a `CMakeLists.txt` that sets up symbol inspection automatically depending on your platform and compiler. This means when you build the project, the correct `nm` or `dumpbin` command will run automatically through the `inspect_symbols` target.

However, if you'd like to inspect the mangled symbols manually, you can also use the following commands:

```bash
cmake -S . -B build
cmake --build build
````

### 3. Run the Program

```bash
cd build
./main         # On Linux/macOS
main.exe       # On Windows
````

### 4. Inspect Mangled Symbols

To see the mangled names, you can use:

```bash
nm main      # On Linux/macOS
nm main.exe  # On Windows with MinGW
```

To see demangled names (i.e., the original function signatures), add the `-C` flag:

```bash
nm -C main      # Linux/macOS readable output
nm -C main.exe  # Windows readable output (if using MinGW)
```

Or if MSVC:

```bash
dumpbin /symbols main.obj | findstr print
```
