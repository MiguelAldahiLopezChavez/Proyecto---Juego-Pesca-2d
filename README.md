# 🎣 Juego de Pesca 2D

![Juego](assets/imagen/GrabacionJuego.gif)

---

## 📝 Descripción del Proyecto

Mini-juego de pesca 2D con físicas en C++/SFML, sistema de recompensas por captura y puntuación basada en el peso del pez.

---

## 🎯 Objetivo del Juego
- Captura peces y acumula puntos según su peso y sus puntos base (basado en su rareza)
- Alcanzar 1000 puntos en 5 intentos
- Máximo 3 fallos

---

## 🎮 Controles
Lista de los controles:

- `Space`: Lanzar caña.
- `Up` o `Mouse Izquierdo`: Control de barra / salto del cuadrado.
- `Enter`: Continuar / Confirmar / Reiniciar.

---

## ⚙️ Mecánicas

- Reto de contacto: mantén el cuadrado en contacto con el pez por un tiempo para capturarlo.
- Recompensas por pez: cada captura muestra nombre, peso y puntos.
- Puntuación con multiplicador: depende de la posición del peso en su rango.
- Transiciones: instrucciones, juego, recompensas y pantalla final con reinicio.

---

## 🏆 Características
- HUD de intentos y puntos totales
- Sistema de peces con base, rango de peso y multiplicador
- Transiciones suaves entre estados

---

## 👥 Equipo
- Integrante 1: Miguel Aldahi López Chávez (@MiguelAldahiLopezChavez)
- Integrante 2: Gilberto Ibrahim Salcedo Lepe (@usuario-github)

---

## 🛠️ Tecnologías
- Motor/Framework: SFML 2.5+
- Lenguaje: C++
- Librerías adicionales: std::random, etc.

---

## 📜 Créditos
- Assets de terceros utilizados: [indica autor/fuente si aplica]
- Referencias o inspiraciones: [lista de juegos, artículos, etc.]
- Agradecimientos: [profesor, compañeros, recursos]

---

## 📁 Assets
- Pixel Combat asset pack: [HeltonYan](https://heltonyan.itch.io/pixelcombat) - Licencia CC BY 4.0

---

## 🏗️ Makefile y Ejecución
Tareas disponibles en `makefile`:

- `make clean`: limpia binarios/artefactos
- `make`: compila el proyecto
- `make run`: compila (si hace falta) y ejecuta

Uso en PowerShell (Windows):
```powershell
make clean
make
make run
```

Ejecución manual:
```powershell
make
.\bin\JuegoPesca2D.exe
```
