# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ldr_mqtt)

target_include_directories(app PRIVATE include)

target_sources(app PRIVATE
  src/main.c
  src/wifi_connect.c
  src/connection_manager.c
  src/broker_connect.c
  src/mqtt_publish.c
  src/servo_control.c
)
