# Minesweeper-on-Terminal

A fully-featured, terminal-based Minesweeper game written entirely in C++. 

This project goes beyond the standard console game by including an in-depth statistics tracker, a highly customizable color/UI settings menu, and a **custom-built, templated Matrix library** developed completely from scratch to handle the underlying grid logic.

## Features

### Gameplay
* **Classic Minesweeper Mechanics:** Includes recursive empty-cell revealing, flagging, and win/loss detection.
* **Multiple Difficulties:** * Easy (8x8, 10 mines)
    * Medium (16x16, 40 mines)
    * Hard (16x30, 99 mines)
    * Custom (Specify your own grid size and mine count).
* **Detailed Statistics Tracking:** Automatically tracks and saves games played, win rate, fastest win times, win streaks, and total time spent playing.

### Customization
* **RGB Color Customization:** Fully customize the terminal colors for hidden cells, flagged cells, blank cells, numbered cells, mine cells, and standard text using RGB values.
* **UI Toggles:** Show or hide the active timer and the dig/flag counters.
* **Preset Management:** Save your custom aesthetic configurations as presets, load them, or revert to default settings at any time.

### Technical Highlights
* **Custom Matrix Library (`Matrix.cpp`):** Features a fully templated matrix data structure with support for matrix math (addition, multiplication, transposing), scalar operations, proxy classes for 2D array subscripting (`[][]`), and memory management using the Rule of Five.
* **Memory Efficient Grid:** Each cell in the Minesweeper grid is packed into a single 8-bit `unsigned char`. 
    * The lowest 2 bits store the cell state (Hidden, Flagged, Revealed).
    * The upper bits store the cell value (Adjacent mine count, or a mine indicator).


## Directory Structure

**Important:** For the game to save settings, statistics, and presets correctly, ensure your directory contains a `game data` folder in the same directory as the executable.

```text
├── main.cpp                 # Main game loop, UI rendering, and logic
├── libraries/
│   └── Matrix.cpp           # Custom templated Matrix library
├── game data/
│   ├── settings.csv         # Stores current color and UI preferences
│   ├── stats.csv            # Stores player statistics
│   ├── user_presets.csv     # Stores saved user configuration presets
│   └── default_settings.csv # Base configurations to restore defaults
└── README.md
```

## Installation and Compilation

### Prerequisites

* A C++ compiler that supports **C++17** or higher (required for `if constexpr` used within the Matrix library).
* A terminal emulator that supports ANSI escape codes for RGB coloring.

### Build Instructions

Just download the whole repository, compile `main.cpp`, and run the exe file.


## How to Play

Once you launch the game and select a difficulty, you will be presented with the grid. The game is driven by entering coordinates.

1. **Enter Row and Column:** The game will prompt you sequentially for the row number, then the column number.
2. **Switching Modes:** By default, you are in **Dig** mode. When prompted for a row or column, type `-1` to toggle between **Dig** and **Flag** modes.
3. **Quitting:** Type `0` at any coordinate prompt to safely quit the current game and return to the main menu.


## About the Matrix Library

Included in `libraries/Matrix.cpp` is a standalone, reusable matrix mathematics and 2D grid structure. It supports:

* Dynamic resizing and memory management.
* Mathematical operator overloading (`+`, `-`, `*`, `/`, `+=`, etc.) for both scalars and other matrices.
* Bitwise and Boolean operator overloading.
* Helper generations (`zeros_matrix`, `ones_matrix`, `identity_matrix`, `random_matrix`).
* Functional applications mapping over matrix data (`apply()`).

Feel free to extract and use `Matrix.cpp` in your other C++ projects!
