#!/bin/bash

# Lista de nombres de procesos que quieres matar
procesos=("kalmanFilter" "controller" "pid_process" "motors")

# Itera sobre la lista y mata cada proceso
for proceso in "${procesos[@]}"; do
    pid=$(pgrep -f "$proceso")
    if [ -n "$pid" ]; then
        echo "Matando el proceso $proceso (PID: $pid)"
        kill "$pid"
    else
        echo "No se encontró el proceso $proceso en ejecución."
    fi
done