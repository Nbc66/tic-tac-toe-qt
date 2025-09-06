# Tic-Tac-Toe Qt

A cross-platform Tic-Tac-Toe game built with **Qt Widgets** and **CMake**.  
Includes AI opponents (Random + Minimax) and unit tests with **QtTest**.

---

## ✨ Features

- 3×3 board with win/draw detection  
- AI agents:
  - **RandomAgent** – random valid move
  - **MinimaxAgent** – optimal play
- Qt Widgets UI
- Unit tests for board, agents, controller (QtTest + CTest)

---

## 🛠 Requirements

- **CMake** ≥ 3.16 (check with `cmake --version`)
- **C++17** (or newer) compiler
- **Qt 5** or **Qt 6**:
  - Modules: **Widgets**, **Test**
- (Windows only) `windeployqt` (comes with Qt)

---

## 🚀 Quick Start (All Platforms)

```bash
# 1) Clone
git clone https://github.com/your-user/tic-tac-toe-qt.git
cd tic-tac-toe-qt

# 2) Configure (Debug)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# If Qt isn't auto-found, pass your Qt path:
# cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/<ver>/<kit>
#   Linux example:  /opt/Qt/6.6.1/gcc_64
#   macOS example:  ~/Qt/6.6.1/macos
#   Windows MSVC:   E:\Qt\6.9.2\msvc2022_64

# 3) Build
cmake --build build --config Debug

# 4) Run the app
./build/tic-tac-toe-qt           # Linux/macOS
build\tic-tac-toe-qt.exe         # Windows
```
---

## 🧪 Run Tests

```bash
# From the build directory:
cd build

# Run all tests
ctest --output-on-failure

# Run a single test
ctest -R AgentTests --output-on-failure
```

---
## 📦 CMake Presets (optional)
```
A CMakePresets.json is included for convenience.

# Configure + build with a preset (example)
cmake --preset x64-debug
cmake --build --preset x64-debug
```

If Qt isn’t found with a preset, 
edit `CMakePresets.json` and set your local Qt path under `CMAKE_PREFIX_PATH`.

---
## 🪟 Notes for Windows

 - This project auto-runs windeployqt after building the app & tests to copy required Qt DLLs alongside the binaries.

 - If you open the folder with Visual Studio 2022 (“Open Folder”), it will detect CMake automatically. Select a preset (e.g., x64-debug) and use Test Explorer or ctest from the build directory.

---
## 🧰 Troubleshooting

 - Qt not found: Pass your Qt install with -DCMAKE_PREFIX_PATH=....
Alternatively set environment variables like Qt6_DIR to your Qt CMake package dir (e.g., <Qt>/6.x.x/gcc_64/lib/cmake/Qt6).

 - Missing QtTest module: Ensure you installed the Test component with Qt (via the Qt Maintenance Tool or your package manager).

 - Plugin/DLL errors on Windows: Run the test/app from the build folder after a successful build (so windeployqt has copied dependencies). If needed, run windeployqt <path-to-exe> manually.

 - Check That you'r CMake Version is ≥ 3.16