# Documentación Técnica del Simulador de Planificación

Este documento describe la arquitectura, las clases principales y el flujo de ejecución del **Process Scheduler Simulator**.

## 1. Arquitectura General

El proyecto sigue una arquitectura modular que separa claramente la **lógica de simulación (Backend)** de la **Interfaz Gráfica de Usuario (Frontend/GUI)**.

- **Backend:** Maneja el estado de los procesos, las colas de espera, las métricas y la ejecución de los diferentes algoritmos de planificación.
- **Frontend (GUI):** Construido con Qt, se encarga de mostrar visualmente las colas, el proceso en CPU y recoger el input del usuario.

## 2. Clases Principales (Core)

### `Process` (process.h / process.cpp)
Representa un bloque de control de proceso (PCB). Contiene toda la información vital de un proceso:
- **Atributos base:** `id`, `arrivalTime`, `cpuBurst`, `ioBurst`, `priority`.
- **Estado:** `state` (Nuevo, Listo, Ejecutando, Bloqueado, Terminado).
- **Métricas dinámicas:** `remainingCpuBurst`, `remainingIoBurst`, `waitingTime`, `turnaroundTime`.

### `Simulator` (simulator.h / simulator.cpp)
Es el "Motor" principal de la simulación. Se encarga de coordinar el tiempo (ticks) y el ciclo de vida de todos los procesos.
- **`nextTick()`**: Es el método más importante. Avanza el reloj un instante de tiempo, actualiza los tiempos de espera, maneja los procesos bloqueados (E/S), verifica si nuevos procesos han llegado, y finalmente llama al algoritmo de planificación para decidir qué proceso usar en la CPU.
- **Métricas:** Calcula la utilización del procesador, tiempo de espera promedio y tiempo de retorno promedio.

### `Scheduler` (scheduler.h / scheduler.cpp)
Es una clase base abstracta (Interfaz) para todos los algoritmos de planificación.
- Define el método virtual puro `schedule(readyQueue, currentProcess, currentTick)`, el cual debe ser implementado por cada algoritmo específico para decidir qué proceso entra o sale de la CPU.
- Define el método `isPreemptive()` para saber si el algoritmo soporta expulsión de la CPU.

## 3. Algoritmos de Planificación Implementados

Todos estos algoritmos heredan de la clase base `Scheduler`:

- **`FCFSScheduler`**: Planificación "Primero en llegar, primero en ser servido". Simple y no expulsivo.
- **`SjfScheduler`**: El trabajo más corto (menor ráfaga de CPU) se ejecuta primero. No expulsivo.
- **`SrtfScheduler`**: Versión expulsiva del SJF. Un proceso nuevo con ráfaga menor puede interrumpir al actual.
- **`RoundRobinScheduler`**: Asigna un *Quantum* fijo a cada proceso. Cuando el quantum expira, el proceso se encola al final.
- **`PriorityScheduler`**: Selecciona el proceso con la mayor prioridad (generalmente un valor numérico menor o mayor, según el diseño). No expulsivo.
- **`PriorityPreemptiveScheduler`**: Igual que el anterior, pero si llega un proceso con mayor prioridad al actual en CPU, lo interrumpe.
- **`RandomScheduler`**: Selecciona un proceso al azar de la cola de listos.

## 4. Clases de Interfaz de Usuario (GUI)

- **`MainWindow`**: La ventana principal. Contiene las tablas visuales (`QTableWidget`) que muestran las colas de Listo, Bloqueado y Terminado. Posee un temporizador (`QTimer`) que llama periódicamente a `Simulator::nextTick()` para animar la simulación.
- **`NewProcessForm`**: Un cuadro de diálogo secundario que permite al usuario ingresar manualmente los datos de un proceso nuevo (ráfaga, prioridad, etc.) y añadirlo a la simulación en caliente.
- **`GeneratorForm`**: Un formulario que automatiza la creación de múltiples procesos con características aleatorias dentro de rangos definidos por el usuario, ideal para pruebas de estrés de los algoritmos.

## 5. Ciclo de Ejecución (Flujo del Simulator)

Cada vez que el reloj avanza un pulso (`nextTick()`):
1. **Llegadas:** Se revisan los procesos cuyo `arrivalTime` coincide con el `currentTick` y se mueven de estado "Nuevo" a la cola de "Listos" (`Ready`).
2. **Entrada/Salida:** Se decrementa la ráfaga de I/O de los procesos bloqueados. Si un proceso termina su I/O, vuelve a la cola de "Listos".
3. **Ejecución en CPU:** Se decrementa la ráfaga de CPU del proceso actual en CPU.
4. **Finalización:** Si el proceso en CPU termina su ráfaga, pasa a la cola de "Terminados" (`Terminated`) o "Bloqueados" (si tiene ráfaga de E/S pendiente).
5. **Planificación:** Se invoca el método `schedule()` del algoritmo activo para seleccionar el proceso que debe estar en CPU para el próximo tick (y manejar expropiaciones si las hay).
