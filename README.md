# ⏰ Alarm Clock on NIOS II – Real-Time Embedded System

This project implements a fully functional alarm clock on an Intel/Altera FPGA using the **NIOS II** soft-core processor, developed as part of the *Operating Systems Principles* course at Tecnológico de Costa Rica. The goal was to design a system that operates without a traditional OS, relying instead on a minimal microarchitecture capable of handling hardware interrupts.

## 📌 Features

- **Time Management**: Tracks and displays the current time with minute-level precision.
- **Alarm Configuration**: Set the alarm time using physical buttons with proper debouncing.
- **Real-Time Interrupt Handling**: Uses timer interrupts (no busy waiting) to keep accurate time without blocking execution.
- **Alarm Trigger**: Activates LEDs and a buzzer when the alarm time is reached, lasting 30 seconds or until manually dismissed.
- **Reset & Mode Switching**: Switches to configuration mode to set the clock or alarm using GPIO inputs.

## 🧠 Technical Overview

- **Microarchitecture**: Based on the NIOS II processor with timer peripheral integrated via Qsys.
- **Interrupts**: Configured via `alt_ic_isr_register()` and `IOWR_ALTERA_AVALON_TIMER_*` macros.
- **Programming Language**: C (bare-metal, no OS).
- **No HDL Required**: All hardware modules were provided; focus was on software and system integration.
- **Peripherals Used**:
  - 7-Segment Displays
  - Push Buttons (for setting time and alarm)
  - LEDs (for animation when alarm is active)
  - Buzzer (for audible alarm)

## 🧪 Deliverables

- `main.c`: Alarm clock logic with interrupt service routines.
- `system.qsys`: Hardware configuration file with timer and GPIO.
- `alarm_clock.sopcinfo`: SOPC system metadata.
- `README.md`: Project documentation and instructions.
- `report.pdf`: Documentation following IEEE Trans style.

## ⚙️ How to Run

1. Load the `system.qsys` in Qsys and generate the system.
2. Compile and upload the software using **NIOS II IDE** or command-line tools.
3. Connect the physical peripherals to the FPGA board.
4. Use buttons to set the time and alarm, then let it run!

## 🏗️ Learning Outcomes

- Gained experience building real-time embedded systems without an OS.
- Understood how to handle timer interrupts and synchronize hardware peripherals.
- Improved design thinking for modular, interrupt-safe embedded applications.

## 👥 Authors

Carlos Andrés Mata Calderón  
Jason Leitón – Professor  
Leonardo Araya – Professor

---

> *This project was part of Task 1 for CE4303 – Operating Systems Principles (August 2023), School of Computer Engineering, Tecnológico de Costa Rica.*
