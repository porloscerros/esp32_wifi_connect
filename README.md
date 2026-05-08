# ESP32 Project Template & Utils

Este repositorio es una base, modular para proyectos basados en **ESP32**. Diseñado como un **Boilerplate** reutilizable para iniciar desarrollos rápidamente.

This repository is a modular base for **ESP32** projects. Designed as a reusable **Boilerplate** to jumpstart developments quickly.

---

## 🏗️ Filosofía / Philosophy

**[ES]** Cada recurso vive en su propia clase independiente y no bloqueante (sin `delay()`), facilitando la multitarea. El `main.cpp` actúa como orquestador para activar herramientas según sea necesario.

**[EN]** Each resource lives in its own independent, non-blocking class (no `delay()`), enabling real multitasking. `main.cpp` acts as an orchestrator to toggle tools as needed.

## 🧰 Módulos / Modules

| Módulo / Module | Clase / Class | Descripción / Description |
| :--- | :--- | :--- |
| **WiFi Manager** | `WifiManager` | Gestión WiFi y config vía Bluetooth JSON / WiFi & BT JSON config. |
| **Time Service** | `TimeManager` | Sincronización NTP (GMT-3 Ar) / NTP Time Sync. |
| **Relay Control** | `RelayManager` | Soporte Low Trigger / Low Trigger relay support. |
| **Cycle Timer** | `TimerManager` | Temporizador de ciclos (18/6) / Cycle timer logic. |
| **Reset Handler** | `ResetHandler` | Reset de fábrica (Botón BOOT) / Factory Reset (BOOT button). |

## 🚀 Inicio Rápido / Quick Start

1. **Clonar / Clone:** `git clone https://github.com/porloscerros/esp32-utils.git`
2. **Entorno / Environment:** Abrir en VS Code con **PlatformIO**.
3. **Configurar / Setup:** Define pines en `src/main.cpp`. (Default: Relays 26, 27 | Reset 0).

## 🛠️ Especificaciones / Specs

- **Partitions:** `huge_app.csv` (3MB App space for WiFi + BT).
- **Deps:** `ArduinoJson`, `PubSubClient`.
- **Structure:** Standard `src/` & `include/` folders.

## 📡 Configuración Bluetooth / BT Provisioning

```json
{
  "type": "wifi",
  "ssid": "Your_SSID",
  "pass": "Your_Password"
}
```

(use [Serial Bluetooth Terminal](https://github.com/kai-morich/SimpleBluetoothTerminal) or similar)
