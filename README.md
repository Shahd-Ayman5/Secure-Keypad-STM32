
# Secure Keypad System (STM32)

## Overview

This project implements a **secure keypad-based locking system** using STM32.
It integrates multiple hardware peripherals with a **Mealy State Machine** to manage system behavior in real-time.

The system supports password-based unlocking, failure tracking, alarm triggering, and asynchronous interrupt handling for external events.

---

## Demo

## Features

*  4x4 Matrix Keypad for password input
*  Emergency Reset (High-priority interrupt)
*  Doorbell (Interrupt-based, non-blocking)
*  7-Segment Display for failed attempts
*  LED Bar for password progress visualization
*  Success LED for unlocked state
*  Alarm LED for lockout state
*  Mealy State Machine-based logic

---

## 🔄 State Machine Design

### 🔒 LOCKED (Initial State)

*  Valid input → update LED bar progress
*  Sequence complete → transition to UNLOCKED
*  Invalid input → reset progress + increment failure counter
*  Lockout threshold reached → transition to ALARM
*  Doorbell → pulse indicator (no state change)

---

### 🔓 UNLOCKED

*  Success LED ON
*  Lock command (`#`) → return to LOCKED
*  Ignore all keypad inputs

---

### 🚨 ALARM

*  Alarm LED ON
*  Ignore all inputs
*  Emergency Reset → return to LOCKED

---

## 🔌 Hardware Requirements

* STM32 Microcontroller
* 4x4 Matrix Keypad
* Push Buttons (Emergency & Doorbell)
* 7-Segment Display
* LEDs (Progress, Success, Alarm, Doorbell)
* Resistors

---

## How It Works

1. User enters password via keypad
2. Each correct digit lights one LED (progress bar)
3. If full sequence matches → system unlocks
4. Incorrect attempts are counted and displayed
5. After threshold → system enters ALARM state
6. Emergency button resets the system instantly
7. Doorbell works independently using interrupts

---

## Key Concepts

* Mealy State Machine
* Interrupt Handling (EXTI + NVIC)
* GPIO Interfacing
* Embedded Modular Design

