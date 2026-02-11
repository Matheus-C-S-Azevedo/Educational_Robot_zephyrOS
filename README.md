# Educational Robot with Zephyr OS (ESP32)

[![Zephyr](https://img.shields.io/badge/Zephyr-RTOS-6DB33F?logo=zephyrproject&logoColor=white)](#)
[![Platform](https://img.shields.io/badge/Platform-ESP32-E7352C?logo=espressif&logoColor=white)](#)
[![Language](https://img.shields.io/badge/Language-C-00599C?logo=c&logoColor=white)](#)
[![Build System](https://img.shields.io/badge/Build-CMake%20%2B%20west-064F8C)](#)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE)

Firmware educacional para robótica embarcada com **ESP32** e **Zephyr RTOS**, organizado para aprendizado progressivo de arquitetura de software embarcado, integração de periféricos e boas práticas de desenvolvimento.

---

## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Architecture](#architecture)
- 
---

## Overview

Este projeto implementa uma base de **robô educacional** sobre Zephyr, com foco em:

- Fundamentos de **RTOS** (threads, timers, sincronização)
- Organização modular do firmware
- Integração incremental de sensores, atuadores e conectividade


## Project Structure

```text
Educational_Robot_zephyrOS/
├── boards/                # Board support e ajustes específicos de hardware
├── docs/                  # Documentação técnica e guias
├── dts/
│   └── bindings/          # Bindings DeviceTree customizados
├── include/               # Headers públicos do projeto
├── src/                   # Código-fonte principal
├── CMakeLists.txt         # Configuração de build (CMake)
├── prj.conf               # Opções Kconfig do Zephyr
└── sample.yaml            # Metadados de sample/testes (Zephyr/Twister)
```

## architecture

```text
+--------------------------------------------------+
|                   Application                    |
|  (robot behavior / state machine / control loop) |
+-------------------------+------------------------+
                          |
                          v
+--------------------------------------------------+
|                 Service Layer                    |
|    motion control | sensing | telemetry | power  |
+-------------------------+------------------------+
                          |
                          v
+--------------------------------------------------+
|              Hardware Abstraction                |
|   GPIO / PWM / I2C / SPI / ADC / UART / BLE     |
+-------------------------+------------------------+
                          |
                          v
+--------------------------------------------------+
|                 Zephyr Kernel                    |
| threads | timers | queues | semaphores | logging |
+--------------------------------------------------+
```



