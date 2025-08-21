# Optimization & RTC Playground ⚙️🕒

This project explores **performance optimization** on STM32 (faster GPIO/shift-register driving)
and a minimal **RTC-driven 4-digit clock** with editing and brightness control.  
Inspired by a lab exercise, documented here as a standalone project.

---

## 🎯 Features

### Part A — Optimization (Shift Register / 74HC595)
- **Baseline timing** measurement using a spare GPIO as a scope/logic-analyzer marker.
- **Compiler tuning:** compare `-Os` vs `-Ofast` (code size vs speed).
- **Direct register I/O:** push bits via GPIO **BSRR** instead of high-level toggles.
- **Loop unrolling** for the 8-bit shift (analyze generated assembly).
- Optional: revert too-aggressive toggling if it violates device timing.

### Part B — RTC (4-Digit Clock)
- Display **HH:MM** on a 4-digit 7-segment (multiplexed).
- **Edit modes** via buttons: increment ones/tens of minutes and hours.
- **Blinking colon** (1 Hz) and optional seconds on LEDs (BCD or bar).
- **Brightness control** (fixed value, time-based “night dimming”, or ADC/LDR auto).

---

## 🛠 Hardware

- STM32 Nucleo (e.g., STM32F4xx)
- Breadboard:
  - 4-digit 7-segment (common anode) + driver transistors
  - **74HC595** shift register for segment lines
  - Potentiometer or LDR (optional) for brightness input
  - 8 LEDs + resistors (for seconds/diagnostics)
- Optional logic analyzer (e.g., PulseView)

---

## 💻 Software

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- Mbed-style project layout
- (Optional) PulseView / Saleae for measurements
- Recommended build profiles to compare: **-Os** and **-Ofast**

---

## 📂 Project Structure


---

## 🚀 Getting Started

1. Clone the repository
   ```bash
   git clone https://github.com/Yassinejabnoun/Optimization-RTC-Playground.git
   cd Optimization-RTC-Playground
2. Import into STM32CubeIDE.
3. Wire hardware as in docs/circuit.png
4. Build & flash. Open PuTTY @ 115200 baud on the STLink VCOM.
5. Build & flash (first with -Os, then -Ofast for comparison)

---

## 🎥 Demo

(Add your YouTube link here – thumbnail will be generated automatically)

---

## 📜 License

MIT License – free to use for personal or professional projects.
