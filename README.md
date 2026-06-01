# Washing Machine Controller Simulation

## Introduction

This project simulates an automatic washing machine controller using the **AT89C51 microcontroller**.

The system is designed for reliability and maintainability by applying a **Finite State Machine (FSM)** architecture to manage washing cycles, user interactions, and safety mechanisms.

The simulation is implemented in **Proteus Design Suite**, while the firmware is written in **C** and compiled into a `.hex` file for execution on the AT89C51.

---

## Features

### 🔹 Wash Mode Selection

Use the **BM button (P3.2)** while the machine is in the **IDLE** state to select the appropriate washing program.

### 🔹 Water Level Selection

Use the **BW button (P3.3)** while in **IDLE** to configure the desired water level.

### 🔹 Automatic Washing Cycle (FSM Control)

Press the **BS button (P3.0)** to start the washing process.

The controller automatically calculates the total cycle time and sequentially executes:

1. Filling
2. Washing
3. Draining
4. Rinsing
5. Spinning

No further user intervention is required.

### 🔹 Pause / Resume Function

The **BP button (P3.1)** allows the cycle to be paused and resumed.

When paused:

* Countdown timer freezes.
* Motor and valve outputs are disabled immediately.
* Status LED blinks.
* LCD displays **"PAUSED"**.

Press the button again to continue from the same point in the cycle.

---

# Washing Modes & Water Levels

## Wash Modes

| Mode      | Display Code | Description                                               |
| --------- | ------------ | --------------------------------------------------------- |
| Cotton    | Ct           | Longest and most intensive wash cycle with 3 rinse cycles |
| Synthetic | Sy           | Medium wash duration with 2 rinse cycles                  |
| Delicate  | De           | Gentle washing for delicate fabrics with 2 rinse cycles   |
| Quick     | Qk           | Shortest cycle with only 1 rinse cycle                    |

## Water Levels

| Water Level | Display Code | Valve Open Time |
| ----------- | ------------ | --------------- |
| Low         | Lo           | 3 seconds       |
| Medium      | Md           | 5 seconds       |
| High        | Hi           | 8 seconds       |

---

# Hardware Architecture

## Microcontroller

* AT89C51

## Display

* LCD 16x2

  * Remaining time
  * Wash mode
  * Water level
  * Pause status

## Actuators

### DC Motor (MO)

* Alternates direction every second during washing.
* Spins at high speed during spinning mode.

### Water Valve (VA)

Used for:

* Water filling
* Water draining
* Rinsing operations

### Status LEDs

| LED | Function |
| --- | -------- |
| LF  | Filling  |
| LW  | Washing  |
| LR  | Rinsing  |
| LS  | Spinning |

## Inputs

| Pin  | Button | Function              |
| ---- | ------ | --------------------- |
| P3.0 | BS     | Start                 |
| P3.1 | BP     | Pause / Resume        |
| P3.2 | BM     | Mode Selection        |
| P3.3 | BW     | Water Level Selection |

---

# Software Design

The firmware is written in **C (`src.c`)** and compiled into **`src.hex`**.

## Finite State Machine (FSM)

The controller transitions through:

```text
IDLE
  ↓
FILLING
  ↓
WASHING
  ↓
DRAINING
  ↓
RINSING
  ↓
SPINNING
```

State transitions are managed by timers and internal counters.

## Hardware Abstraction

Hardware pin definitions are separated from the application logic to improve readability and maintainability.

## Safety Interlocks

The pause mechanism directly disables motor and valve outputs, ensuring the machine stops immediately when requested.

---

# Installation and Usage

## Requirements

### Software

* Keil C (or any compatible 8051 compiler)
* Proteus Design Suite

---

## Running the Simulation

### Step 1

Open:

```text
doanTKLL.pdsprj
```

in Proteus.

### Step 2

Double-click the **AT89C51** component.

### Step 3

Set the **Program File** field to:

```text
src.hex
```

### Step 4

Press the **Play** button in Proteus to start the simulation.

Optional resources:

* `DEMO.mp4` – Demonstration video
* `presentation.pptx` – Project presentation slides

---

# Operation Guide

### 1. Configure Settings

While in IDLE state:

* Press **BM (P3.2)** to select a wash mode.
* Press **BW (P3.3)** to select a water level.

The selected settings are displayed on the LCD.

### 2. Start Washing

Press:

```text
BS (P3.0)
```

The washing cycle starts automatically.

Monitor:

* LCD display
* LEDs (LF, LW, LR, LS)
* Motor operation
* Water valve activity

### 3. Pause / Resume

During operation:

Press:

```text
BP (P3.1)
```

to pause the machine.

Effects:

* LEDs blink.
* Motor stops.
* Valve closes.
* LCD shows "PAUSED".

Press **BP** again to resume from the same state.

---

# Project Structure

```text
.
├── src.c
├── src.hex
├── doanTKLL.pdsprj
├── DEMO.mp4
├── presentation.pptx
└── README.md
```

---

# Authors

Project developed as part of a microcontroller-based embedded systems course using:

* AT89C51
* Proteus Design Suite
* Embedded C Programming
* Finite State Machine (FSM) Design

