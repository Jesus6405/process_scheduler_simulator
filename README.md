# Process Scheduler Simulator

Un simulador interactivo de planificación de procesos del sistema operativo, desarrollado en **C++** utilizando el framework **Qt**. Esta aplicación permite visualizar y entender cómo diferentes algoritmos de planificación gestionan y asignan el tiempo de CPU a múltiples procesos.

## Características

- **Simulación en tiempo real:** Visualiza cómo se ejecutan, esperan y finalizan los procesos.
- **Creación manual de procesos:** Interfaz para agregar procesos personalizados especificando tiempo de llegada, ráfaga de CPU y prioridad.
- **Generador de procesos:** Herramienta incorporada para generar ráfagas de procesos aleatorios y probar los algoritmos bajo diferentes cargas de trabajo.
- **Interfaz gráfica de usuario (GUI):** Diseñada de forma intuitiva con Qt Widgets para una experiencia de usuario fluida.

## Algoritmos de Planificación Implementados

El simulador soporta los siguientes algoritmos clásicos de planificación de CPU:

1. **FCFS (First-Come, First-Served):** El primer proceso en llegar es el primero en ser ejecutado.
2. **SJF (Shortest Job First):** Ejecuta el proceso con la ráfaga de CPU más corta (No expropiativo).
3. **SRTF (Shortest Remaining Time First):** Versión expropiativa de SJF. Si llega un nuevo proceso con un tiempo restante menor al del proceso actual, se le asigna la CPU.
4. **Round Robin (RR):** Asigna un *quantum* o bloque de tiempo de CPU a cada proceso de manera cíclica.
5. **Priority Scheduler (No expropiativo):** Ejecuta el proceso con la mayor prioridad. Si un proceso de mayor prioridad llega mientras otro se ejecuta, espera su turno.
6. **Priority Preemptive Scheduler:** Versión expropiativa basada en prioridad. Un proceso de mayor prioridad interrumpe al que se está ejecutando actualmente.
7. **Random Scheduler:** Selecciona el siguiente proceso a ejecutar de forma aleatoria entre los que están listos.

## Requisitos del Sistema

- **Compilador C++:** Soporte para el estándar `C++17` o superior.
- **Qt Framework:** Versión 5.x o superior (compatible con Qt 6).

## Estructura del Proyecto

El código fuente está organizado en las siguientes carpetas:

- `Headers/`: Archivos de cabecera (`.h`) con las declaraciones de clases de algoritmos, formularios y la lógica base del simulador.
- `Sources/`: Archivos de implementación (`.cpp`).
- `Forms/`: Archivos de diseño de interfaz de usuario de Qt (`.ui`) para la ventana principal, el generador y la creación manual de procesos.

## Cómo compilar y ejecutar

Puedes abrir y compilar el proyecto fácilmente usando **Qt Creator**:

1. Abre **Qt Creator**.
2. Selecciona *Abrir Proyecto* y busca el archivo `process_scheduler_simulator.pro`.
3. Configura el proyecto con tu kit de compilación de escritorio predeterminado (por ejemplo, MinGW o GCC dependiendo de tu SO).
4. Haz clic en el botón de **Ejecutar** (Run) o presiona `Ctrl+R`.

### Compilación por línea de comandos (qmake)

Si prefieres usar la terminal:

```bash
# 1. Navega a la carpeta del proyecto
cd process_scheduler_simulator

# 2. Crea un directorio de compilación
mkdir build && cd build

# 3. Genera los Makefiles con qmake
qmake ../process_scheduler_simulator.pro

# 4. Compila el proyecto
make   # (o mingw32-make en Windows)

# 5. Ejecuta el binario resultante
./process_scheduler_simulator
```

## Contribuciones
¡Las contribuciones y sugerencias son bienvenidas! Si deseas agregar un nuevo algoritmo o mejorar la interfaz gráfica, siéntete libre de experimentar en el código.
