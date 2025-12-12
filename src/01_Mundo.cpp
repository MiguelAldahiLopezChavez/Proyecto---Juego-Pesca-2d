// 01_Mundo.cpp
// Archivo principal del juego: inicializa recursos, gestiona la ventana,
// el bucle principal y las transiciones entre pantallas.
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "audio.hpp"
#include "peces.hpp"
#include "fonts.hpp"
#include "pescador.hpp"
using namespace sf;


// Función principal: crea la ventana, carga recursos, y ejecuta el loop principal.
int main()
{
    // Textura para el sprite del pez usado en la mecánica (imagen pequeña/placeholder)
    Texture pez_mec;
         if (!pez_mec.loadFromFile("assets/imagen/nuevaimagen/iamgenpez_mecanicajuego.png")) 
     {
    
          return -1;
     }

     Sprite spritePezMec(pez_mec);
     float escalaPezMec = 0.55f;
     spritePezMec.setScale(escalaPezMec, escalaPezMec);

    // Crear rectángulo verde (más alargado en Y)
    // Este rectángulo actúa como la "barra" o área que debe tocar el pez
    // para sumar tiempo de contacto en el sistema de reto.
    RectangleShape cuadradoVerdeShape(Vector2f(90, 120)); // Más alto que ancho
    cuadradoVerdeShape.setFillColor(Color::Green);
    cuadradoVerdeShape.setOrigin(45, 60); // Centrar el origen (mitad del tamaño)

        // Textura que contiene la mecánica principal (panel donde se mueve el pez)
        Texture mecanica_juego;
         if (!mecanica_juego.loadFromFile("assets/imagen/nuevaimagen/mecanica_juego_principal_2.png")) 
     {
    
          return -1;
     }

     Sprite spriteMecanicaJuego(mecanica_juego);
     
    // Variables de control generales: timers y flags para controlar
    // la secuencia de animaciones y la aparición de la mecánica.
     Clock timerAnimacion;
     Clock timerInstrucciones;
     bool animacionTerminada = false;
     bool mostrarPezFisico = false;
     bool mostrarInstrucciones = false;
     float tiempoEspera = 2.0f; // 2 segundos después de terminar animación
     float tiempoInstrucciones = 7.0f; // 8 segundos para mostrar instrucciones
     


     // Fondos: `fondo` usado para la pantalla inicial/menu, `fondo2` para la escena de juego
     Texture fondo;
       if (!fondo.loadFromFile("assets/imagen/Background.png")) 
    {

        return -1;
    }   

        Texture fondo2;
         if (!fondo2.loadFromFile("assets/imagen/nuevaimagen/background.png")) 
     {
    
          return -1;
     }

     // Textura de la sombra del pescador (decorativa, se dibuja bajo el sprite)
     Texture sombrapescador;
        if (!sombrapescador.loadFromFile("assets/imagen/nuevaimagen/sombra.png")) 
        {
        
            return -1;
        }

    


    // Determinar tamaño de ventana a partir del fondo principal
    Vector2u size = fondo.getSize();
    unsigned int anchoVentana = size.x;
    unsigned int altoVentana = size.y;


    // Posicionar la mecánica de juego: centrada en Y, a la izquierda en X
    Vector2u sizeMecanica = mecanica_juego.getSize();
    spriteMecanicaJuego.setPosition(60, (altoVentana - sizeMecanica.y) / 2);

    Sprite spriteSombra(sombrapescador);
    spriteSombra.setPosition(505, 460);
    
    // Variables para el cuadrado verde (con gravedad)
    // Controlan posición, velocidad, y límites para la física simple.
    float cuadradoX = 0;
    float cuadradoY = 0;
    float cuadradoVelocidadY = 0; // Velocidad actual de caída (+ = abajo, - = arriba)
    float cuadradoGravedad = 1500.0f; // Aceleración hacia ABAJO (píxeles/s²) 
    float cuadradoFuerzaSalto = -750.0f; // Impulso hacia ARRIBA (negativo)
    float cuadradoVelocidadMaxima = 850.0f; // Velocidad máxima de caída
    float cuadradoLimiteSup = 0;
    float cuadradoLimiteInf = 0;
    bool cuadradoInicializado = false;
    
    // Variables para movimiento simple del pez
    // `pezPosX/pezPosY` determinan la posición, `pezVelocidadY` controla la velocidad vertical,
    // `pezDireccion` indica sentido (1 abajo, -1 arriba) y valores min/max limitan la velocidad.
    float pezPosX = 0.0f;
    float pezPosY = 0.0f;
    // Velocidades del pez en píxeles/segundo (con movimiento escalado por dt)
    float pezVelocidadY = 200.0f; // Velocidad inicial
    float pezDireccion = 1.0f; // 1 = abajo, -1 = arriba
    float pezVelocidadCambio = 100.0f; // Magnitud del cambio aleatorio (se multiplica por dt)
    float pezVelocidadMin = 200.0f;
    float pezVelocidadMax = 1000.0f;
    
    // Variables para control del salto
    bool mousePresionado = false; 


    // Hoja de sprites del pescador: usada por la clase `Pescador` para animaciones
    Texture pescadoranim;
    if (!pescadoranim.loadFromFile("assets/imagen/nuevaimagen/cast bobbin Sheet.png")) {
        return -1;
    }
    Pescador pescador(pescadoranim);
    float escala = 1.5f;  
    pescador.obtenerSprite().setScale(escala, escala);
    pescador.obtenerSprite().setPosition(anchoVentana/2-120, altoVentana/2-195);
    bool animacionBucleIniciada = false;
    bool bucleFinalIniciado = false;
    bool sfxCatchPlayedForThisAnim = false; // reproducir sfx desde frame 6 una sola vez por animación

    // Sistema de reto: tiempo total para acumular cierta cantidad de contacto
    // entre el cuadrado verde y el pez. Controla victoria/fallo por intento.
    bool retoActivo = false;
    bool retoGanado = false;
    float tiempoRetoTotal = 8.0f;
    float tiempoRetoRestante = 0.0f;
    float tiempoContactoAcumulado = 0.0f;
    float tiempoContactoNecesario = 5.0f;
    Clock relojReto;
    
    // Timers para mostrar mensajes automáticos (victoria/derrota breves)
    Clock timerVictoria;
    Clock timerDerrota;
    float tiempoMostrarMensaje = 3.0f; // 3 segundos para mostrar mensajes
    
    // Sistema de juego: intentos limitados, fallos permitidos y objetivo de puntos
    int intentosRealizados = 0;
    int intentosMaximos = 5;
    int fallosAcumulados = 0;
    int fallosMaximos = 3;
    int puntosAcumulados = 0;
    int puntosMeta = 1000;
    bool juegoTerminado = false;

    // Cargar fuente con `controlTexto` (wrapper simple sobre `sf::Font`)
    controlTexto texto;
    texto.loadFont("assets/Letras/opcion 1/Bear Days.ttf");

    Text mostrarTexto;
    Text mostrarTexto1;
    Text textoInstrucciones;
    Text hudRetoTiempo;       
    Text hudContactoTiempo;   
    Text mensajeReto;         // Mensaje grande de resultado (ganado/fallido)
    // Recompensas
    RewardManager recompensaManager;
    const Reward* recompensaSeleccionada = nullptr;
    // Carga directa alternativa por ruta
    Texture recompensaTexture;
    Sprite recompensaSprite;
    Text textoRecompensaNombre;
    Text textoRecompensaPeso;
    Text textoRecompensaPuntos;
    Text textoRecompensaMultiplicador;
    Text hudIntentos;        // HUD arriba del pescador "X/10"
    Text hudPuntosTotales;   // HUD esquina sup. derecha "X/1000"

    // Reloj auxiliar para parpadeo de textos en pantalla
    Clock reloj;
    bool mostrarTexto1Visible = true;
    float intervaloparpadeo =0.3f;
    bool animacionCortaMostrada = false;
    

    // Preparar textos para menú, instrucciones y HUD
    mostrarTexto.setFont(texto.getFont());
    mostrarTexto.setString("Pesca2D");
    mostrarTexto.setCharacterSize(200); 
    mostrarTexto.setFillColor(Color::White);
    mostrarTexto.setPosition(anchoVentana/2 -350, altoVentana/2-200);

    mostrarTexto1.setFont(texto.getFont());
    mostrarTexto1.setString("(Presiona Enter para iniciar)");
    mostrarTexto1.setCharacterSize(30);
    mostrarTexto1.setFillColor(Color::White);
    mostrarTexto1.setPosition(anchoVentana/2 -180, altoVentana/2 + 70);

    hudRetoTiempo.setFont(texto.getFont());
    hudRetoTiempo.setCharacterSize(40);
    hudRetoTiempo.setFillColor(Color::White);
    hudRetoTiempo.setPosition(anchoVentana - 375, 320);

    hudContactoTiempo.setFont(texto.getFont());
    hudContactoTiempo.setCharacterSize(50);
    hudContactoTiempo.setFillColor(Color::White);
    hudContactoTiempo.setPosition(anchoVentana - 400, altoVentana / 2 - 14);

    mensajeReto.setFont(texto.getFont());
    mensajeReto.setCharacterSize(72);
    mensajeReto.setFillColor(Color::White);
    mensajeReto.setString("");

    textoInstrucciones.setFont(texto.getFont());
    textoInstrucciones.setString("Instrucciones:\n\nControles: SPACE para lanzar\nUP/clic para controlar la barra.\n\nMeta: 1000 pts en 5 lanzamientos.\nSolo 3 fallos permitidos.\n\n\t\t\t\tBuena suerte!");
    textoInstrucciones.setCharacterSize(30);
    textoInstrucciones.setFillColor(Color::White);
    textoInstrucciones.setPosition((anchoVentana/2) - 300, 200);
    
    // Configurar HUD de intentos (arriba del pescador)
    hudIntentos.setFont(texto.getFont());
    hudIntentos.setCharacterSize(36);
    hudIntentos.setFillColor(Color::White);
    hudIntentos.setPosition(anchoVentana/2 - 40, 50);
    
    // Configurar HUD de puntos totales (esquina superior derecha)
    hudPuntosTotales.setFont(texto.getFont());
    hudPuntosTotales.setCharacterSize(40);
    hudPuntosTotales.setFillColor(Color(255, 215, 0)); // Dorado
    hudPuntosTotales.setPosition(anchoVentana - 200, 30);

    // Crear ventana con el tamaño extraído del fondo y comenzar la música del menú
    RenderWindow window(VideoMode(anchoVentana, altoVentana), "Juego de Pesca 2D");
    iniciarMusicaJuego("assets/Musica/Troubadeck 54 Infinity.ogg");
    
    // Efectos de sonido (SFX): cargar buffers y asociarlos a `Sound` para su reproducción
    SoundBuffer sfxCatchBuf, sfxTran1hBuf, sfxTran2Buf,sfxEsfuerzoBuf,sfxEfectojuegoBuf,sfxpanpescasteBuf,sfxrecompensaBuf,sfxvidamenosBuf,sfxjuegoperdidoBuf,sfxreinicioBuf,sfxruidoaguaBuf,sfxvictoriaBuf;
    Sound sfxCatch, sfxTran1, sfxTran2,sfxEsfuerzo,sfxEfectojuego,sfxpanpescaste,sfxrecompensa,sfxvidamenos,sfxjuegoperdido, sfxreinicio,sfxruidoagua,sfxvictoria;

    // Cargar cada archivo de efecto en su buffer correspondiente
    sfxCatchBuf.loadFromFile("assets/efectosonido/DSGNMisc_CAST-Bubblewrap_HY_PC-001.wav");
    sfxTran1hBuf.loadFromFile("assets/efectosonido/DSGNMisc_INTERFACE-Phasey Swipe_HY_PC-006.wav");
    sfxTran2Buf.loadFromFile("assets/efectosonido/MAGSpel_CAST-Underwater_HY_PC-004.wav");
    sfxEsfuerzoBuf.loadFromFile("assets/efectosonido/Voice_Male_V2_Effort_Mono_06.wav");
    sfxEfectojuegoBuf.loadFromFile("assets/efectosonido/Vehicle_Car_Button_Mono_02.wav"); 
    sfxpanpescasteBuf.loadFromFile("assets/efectosonido/MAGSpel_CAST-Bubbly Gather_HY_PC-005.wav");
    sfxrecompensaBuf.loadFromFile("assets/efectosonido/DSGNTonl_USABLE-Whimsy Coin_HY_PC-003.wav");
    sfxvidamenosBuf.loadFromFile("assets/efectosonido/DSGNMisc_HIT-Bit Kick_HY_PC-002.wav");
    sfxjuegoperdidoBuf.loadFromFile("assets/efectosonido/DSGNMisc_SKILL IMPACT-Bubbly Zaps_HY_PC-003.wav");
    sfxreinicioBuf.loadFromFile("assets/efectosonido/DSGNMisc_INTERFACE-Phasey Swipe_HY_PC-006.wav");
    sfxruidoaguaBuf.loadFromFile("assets/efectosonido/Ambiance_Sea_Loop_Stereo.wav");
    sfxvictoriaBuf.loadFromFile("assets/efectosonido/winning-82808.wav");



    // Asociar los buffers cargados a objetos `Sound` y ajustar volúmenes por defecto
    sfxCatch.setBuffer(sfxCatchBuf);
    sfxTran1.setBuffer(sfxTran1hBuf);
    sfxTran2.setBuffer(sfxTran2Buf);
    sfxEsfuerzo.setBuffer(sfxEsfuerzoBuf);
    sfxEfectojuego.setBuffer(sfxEfectojuegoBuf);
    sfxpanpescaste.setBuffer(sfxpanpescasteBuf);
    sfxrecompensa.setBuffer(sfxrecompensaBuf);
    sfxvidamenos.setBuffer(sfxvidamenosBuf);
    sfxjuegoperdido.setBuffer(sfxjuegoperdidoBuf);
    sfxreinicio.setBuffer(sfxreinicioBuf);
    sfxruidoagua.setBuffer(sfxruidoaguaBuf);
    sfxvictoria.setBuffer(sfxvictoriaBuf);

    sfxCatch.setVolume(25.f);
    sfxTran1.setVolume(25.f);
    sfxTran2.setVolume(25.f);
    sfxEsfuerzo.setVolume(100.f);
    sfxEfectojuego.setVolume(85.f);
    sfxpanpescaste.setVolume(40.f);
    sfxrecompensa.setVolume(50.f);
    sfxvidamenos.setVolume(60.f);
    sfxjuegoperdido.setVolume(70.f);
    sfxreinicio.setVolume(25.f);
    sfxruidoagua.setVolume(30.f);
    sfxvictoria.setVolume(70.f);

    // Sprites para los fondos (pantalla inicial y escena de juego)
    Sprite spriteFondo(fondo);
    Sprite spriteFondo2(fondo2);

    // Estados de pantalla que controla el flujo del juego
    enum EstadoPantalla { INICIO, TRANSICION, INSTRUCCIONES, TRANSICION_FINAL, JUEGO, VICTORIA, DERROTA, RECOMPENSAS, REINICIO_TRANSICION };
    EstadoPantalla estado = INICIO;

    int opacidad = 0;
    RectangleShape fadeRect(Vector2f(anchoVentana, altoVentana));
    fadeRect.setFillColor(Color(0, 0, 0, 0));


    // Reloj persistente para calcular `dt` (delta time) y mantener físicas independientes del FPS
    Clock relojDelta;
    while (window.isOpen())
    {
        float dt = relojDelta.restart().asSeconds();

        // Procesamiento de eventos (entrada de usuario)
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (estado == INICIO && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                estado = TRANSICION;
                opacidad = 0;
                sfxTran1.play();
            }
            // Reiniciar reto con Enter cuando el reto terminó (ganado o perdido) y estamos en JUEGO
            if (estado == JUEGO && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                if (!retoActivo && mostrarPezFisico) {
                    // SFX de interfaz al confirmar con Enter (no reinicio)
                    sfxTran1.play();
                    retoActivo = true;
                    retoGanado = false;
                    tiempoRetoRestante = tiempoRetoTotal;
                    tiempoContactoAcumulado = 0.0f;
                    mensajeReto.setString("");
                }
            }
            // Salir de pantalla de recompensas con Enter: detener efectos, evaluar condiciones finales
            if (estado == RECOMPENSAS && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                // Detener el bucle del SFX de recompensa al salir de recompensas
                sfxrecompensa.setLoop(false);
                sfxrecompensa.stop();
                // SFX de interfaz al confirmar con Enter (no reinicio)
                sfxTran1.play();
                recompensaSeleccionada = nullptr;
                mensajeReto.setString("");
                
                // Verificar condiciones de fin de juego
                if (puntosAcumulados >= puntosMeta) {
                    // ¡Victoria! Alcanzó la meta de puntos - ir directo a pantalla final
                    juegoTerminado = true;
                    estado = VICTORIA;
                    // Detener ambiente de agua al finalizar el juego
                    sfxruidoagua.setLoop(false);
                    sfxruidoagua.stop();
                    // SFX de victoria final del juego
                    sfxvictoria.play();
                    // SFX de interfaz para entrada a pantalla final
                    sfxreinicio.play();
                } else if (intentosRealizados >= intentosMaximos) {
                    // Se acabaron los intentos sin alcanzar la meta - ir directo a pantalla final
                    juegoTerminado = true;
                    estado = DERROTA;
                    // Detener ambiente de agua al finalizar el juego
                    sfxruidoagua.setLoop(false);
                    sfxruidoagua.stop();
                    // Reproducir SFX de juego perdido una sola vez
                    sfxjuegoperdido.setLoop(false);
                    sfxjuegoperdido.play();
                    // SFX de interfaz para entrada a pantalla final
                    sfxreinicio.play();
                } else {
                    // Continuar jugando
                    estado = JUEGO;
                    // Resetear animación del pescador para volver al inicio
                    animacionTerminada = false;
                    mostrarPezFisico = false;
                    bucleFinalIniciado = false;
                    animacionBucleIniciada = false;
                    pescador.currentFrame = 3;
                    // Limpiar variables del reto
                    retoActivo = false;
                    retoGanado = false;
                    tiempoRetoRestante = 0.0f;
                    tiempoContactoAcumulado = 0.0f;
                    cuadradoInicializado = false;
                    // Reanudar ambiente de agua al volver al estado de juego
                    sfxruidoagua.setLoop(true);
                    sfxruidoagua.play();
                }
                // Importante: evitar que este mismo evento Enter dispare el reinicio inmediato
                // en las pantallas finales (VICTORIA/DERROTA). Pasar al siguiente evento.
                continue;
            }
            // Iniciar transición antes de reiniciar desde pantallas finales de VICTORIA/DERROTA
            if ((estado == VICTORIA || estado == DERROTA) && juegoTerminado && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                opacidad = 0;
                estado = REINICIO_TRANSICION;
                // Reproducir SFX de interfaz al reiniciar solo si es derrota final
                if (estado == REINICIO_TRANSICION && juegoTerminado) {
                    // Como estamos saliendo desde DERROTA (pantalla final), disparar el swipe
                    sfxTran1.play();
                }
            }
            // Control de animación del pescador (SPACE): inicia la animación completa
            if (estado == JUEGO && event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                if (!animacionTerminada || !mostrarPezFisico) {
                    animacionTerminada = false;
                    mostrarPezFisico = false;
                    bucleFinalIniciado = false;
                    animacionBucleIniciada = false;
                    sfxCatchPlayedForThisAnim = false;
                    sfxEsfuerzo.play();
                    pescador.currentFrame = 3;
                    pescador.iniciarAnimacion(false);
                }
            }
            // Control de salto del cuadrado verde (UP arrow y mouse click): aplica impulso hacia arriba
            if (estado == JUEGO && ((event.type == Event::KeyPressed && event.key.code == Keyboard::Up) ||
                (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left))) {
                if (animacionTerminada && mostrarPezFisico && cuadradoInicializado) {
                    cuadradoVelocidadY = cuadradoFuerzaSalto;
                }
            }
        }

        if(reloj.getElapsedTime().asSeconds() > intervaloparpadeo)
        {
            mostrarTexto1Visible = !mostrarTexto1Visible;
            reloj.restart();
        }
        // Dibujado inicial: limpiar pantalla y dibujar el fondo por defecto
        window.clear();
        window.draw(spriteFondo);




        // Lógica por estados: menú inicial, juego, recompensas, victoria y derrota
        if (estado == INICIO) {
            // Pantalla normal de inicio
            window.draw(mostrarTexto);
            if (mostrarTexto1Visible) {
                window.draw(mostrarTexto1);
            }

        } else if (estado == JUEGO) {
            // Estado principal de juego: actualizar animaciones, físicas simples,
            // detectar contacto pez-cuadrado y gestionar el sistema de reto.
            window.clear();
            window.draw(spriteFondo2);
            // Inicia el bucle de los primeros 3 frames solo si no hay animación activa
            if (!pescador.isAnimacionActiva() && pescador.getFrameActual() < 3) {
                pescador.iniciarAnimacion(true); // true = bucle (primeros 3 frames)
            }
            pescador.update();
            window.draw(pescador.obtenerSprite());

            // Reproducir efecto de sonido cuando la animación alcance el frame 6
            if (!sfxCatchPlayedForThisAnim && pescador.getFrameActual() >= 7) {
                sfxCatch.play();
                sfxCatchPlayedForThisAnim = true;
            }
            
            // Las físicas simples del pez se manejan directamente cuando aparece
            
            // Detectar cuando termina la animación completa del pescador
            // La animación se considera "terminada" cuando entra en el bucle final (frames 8-10)
            if (!animacionTerminada && pescador.isEnBucleFinal()) {
                animacionTerminada = true;
                timerAnimacion.restart();
            }
                        
            // Mostrar mecánica de juego y activar sistemas cuando termina la animación
            // Cuando la animación ha terminado, inicializar y mostrar la mecánica del reto
            if (animacionTerminada) {
                // Inicializar sistemas la primera vez que se detecta el fin de animación
                if (!mostrarPezFisico) {
                    // Obtener límites de la mecánica de juego
                    Vector2f posMecanica = spriteMecanicaJuego.getPosition();
                    FloatRect boundsMecanica = spriteMecanicaJuego.getGlobalBounds();
                    
                    // Inicializar posición del pez (movimiento simple)
                    pezPosX = posMecanica.x + 143; // Centro entre 208 y 310
                    pezPosY = posMecanica.y + 50;
                    
                    // Inicializar cuadrado verde (movimiento directo)
                    cuadradoX = posMecanica.x + (((208 + 310) / 2.0f) + 1); // Centro entre 208 y 310
                    cuadradoLimiteSup = posMecanica.y + 90;  // Límite superior
                    cuadradoLimiteInf = posMecanica.y + boundsMecanica.height - 90;  // Límite inferior
                    cuadradoY = cuadradoLimiteSup + 150; // Comenzar BIEN ABAJO del límite superior para caer visiblemente
                    cuadradoVelocidadY = 0; // Empezar sin velocidad inicial
                    
                    
                    // Activar banderas que permiten que las físicas y el HUD se dibujen
                    cuadradoInicializado = true;
                    mostrarPezFisico = true;
                    // Iniciar reto al mostrar la mecánica y entidades
                    retoActivo = true;
                    retoGanado = false;
                    tiempoRetoRestante = tiempoRetoTotal;
                    tiempoContactoAcumulado = 0.0f;
                    relojReto.restart();
                    // Iniciar SFX de mecánicas en bucle mientras están activas
                    sfxEfectojuego.setLoop(true);
                    sfxEfectojuego.play();
                }
                
                // Mostrar mecánica de juego
                window.draw(spriteMecanicaJuego);
                
                if (cuadradoInicializado) {
                    // === MOVIMIENTO DEL PEZ Y FÍSICAS DEL CUADRADO ===
                    // === MOVIMIENTO SIMPLE DEL PEZ ===
                    // Obtener límites para el pez
                    Vector2f posMecanica = spriteMecanicaJuego.getPosition();
                    FloatRect boundsMecanica = spriteMecanicaJuego.getGlobalBounds();
                    float pezLimiteSup = posMecanica.y + 40;
                    float pezLimiteInf = posMecanica.y + boundsMecanica.height - 110;
                    float pezZonaMedia = (pezLimiteSup + pezLimiteInf) * 0.5f; // Punto medio
                    float pezZonaMediaMargen = 12.0f; // Margen para detectar paso por la zona media
                    
                    // Cambiar velocidad gradualmente para movimiento impredecible (escalado por dt)
                    if (retoActivo) {
                        pezVelocidadY += ((rand() % 10 - 5) * pezVelocidadCambio) * dt;
                        if (pezVelocidadY > pezVelocidadMax) pezVelocidadY = pezVelocidadMax;
                        if (pezVelocidadY < pezVelocidadMin) pezVelocidadY = pezVelocidadMin;
                    }
                    
                    // Mover pez arriba y abajo
                    if (retoActivo) {
                        // Movimiento del pez escalado por dt para independencia del framerate
                        pezPosY += (pezVelocidadY * pezDireccion) * dt;
                    }

                    // Al cruzar por la zona media, existe probabilidad de cambiar velocidad y/o dirección
                    if (retoActivo && std::abs(pezPosY - pezZonaMedia) <= pezZonaMediaMargen) {
                        // Probabilidad de cambiar velocidad (30%)
                        if ((rand() % 100) < 5) {
                            float r = static_cast<float>(rand() % 100) / 100.0f; // 0..1
                            pezVelocidadY = pezVelocidadMin + r * (pezVelocidadMax - pezVelocidadMin);
                        }
                        // Probabilidad de invertir dirección (20%)
                        if ((rand() % 100) < 2) {
                            pezDireccion = -pezDireccion;
                        }
                    }
                    
                    // Cambiar dirección al tocar límites
                    if (pezPosY <= pezLimiteSup) {
                        pezDireccion = 1.0f; // Hacia abajo
                        pezPosY = pezLimiteSup;
                    } else if (pezPosY >= pezLimiteInf) {
                        pezDireccion = -1.0f; // Hacia arriba
                        pezPosY = pezLimiteInf;
                    }
                    
                    // Dibujar pez en su nueva posición
                    spritePezMec.setPosition(pezPosX - 25, pezPosY - 15);
                    
                    // === FÍSICA DE GRAVEDAD DEL CUADRADO VERDE ===
                    // Aplicar gravedad constantemente
                    if (retoActivo) {
                        cuadradoVelocidadY += cuadradoGravedad * dt; // dt es delta time
                    }
                    
                    // Limitar velocidad máxima
                    if (cuadradoVelocidadY > cuadradoVelocidadMaxima) {
                        cuadradoVelocidadY = cuadradoVelocidadMaxima;
                    }
                    
                    // Actualizar posición Y con la velocidad
                    if (retoActivo) {
                        cuadradoY += cuadradoVelocidadY * dt;
                    }
                    
                    // Verificar límites
                    if (cuadradoY < cuadradoLimiteSup) {
                        cuadradoY = cuadradoLimiteSup;
                        if (cuadradoVelocidadY < 0) { // Solo si va hacia arriba
                            cuadradoVelocidadY = 0; // Parar el movimiento ascendente
                        }
                    } else if (cuadradoY > cuadradoLimiteInf) {
                        cuadradoY = cuadradoLimiteInf;
                        cuadradoVelocidadY = 0; // Detener completamente en el límite inferior
                    }
                    
                    // Dibujar cuadrado verde en su posición actual (barrita que detecta contacto)
                    cuadradoVerdeShape.setPosition(cuadradoX, cuadradoY);
                    window.draw(cuadradoVerdeShape);
                    window.draw(spritePezMec);
                    

                    // === DETECCIÓN DE CONTACTO Y LÓGICA DE RETO ===
                    if (retoActivo && !retoGanado) {
                        // Actualizar tiempo restante del reto
                        tiempoRetoRestante -= dt;
                        if (tiempoRetoRestante < 0) tiempoRetoRestante = 0;

                        // Detectar intersección usando bounds
                        FloatRect rectCuadrado = cuadradoVerdeShape.getGlobalBounds();
                        FloatRect rectPez = spritePezMec.getGlobalBounds();
                        bool enContacto = rectCuadrado.intersects(rectPez);

                        // Si hay intersección entre el rectángulo y el pez acumulamos tiempo de contacto
                        if (enContacto) {
                            tiempoContactoAcumulado += dt;
                        }

                        // Comprobar victoria/derrota y preparar mensaje (cuando se cumple tiempo de contacto o se agota el tiempo)
                        if (tiempoContactoAcumulado >= tiempoContactoNecesario) {
                            retoGanado = true;
                            retoActivo = false;
                            // Detener sonido de mecánicas al ganar
                            sfxEfectojuego.setLoop(false);
                            sfxEfectojuego.stop();
                            sfxCatch.play();
                            
                            // Contar intento exitoso y agregar puntos
                            intentosRealizados++;
                            
                            // Preparar recompensa y calcular puntos para mostrar en la pantalla de recompensa
                            recompensaManager.loadDefaults();
                            const Reward& r = recompensaManager.pickRandom();
                            recompensaSeleccionada = &r;
                            
                            // Agregar puntos del pez capturado
                            puntosAcumulados += r.finalPoints;
                            
                            // Asegurar que esta es victoria individual (no final)
                            // juegoTerminado se verifica después en recompensas
                            
                            // Ir a pantalla de victoria primero (individual, no final)
                            estado = VICTORIA;
                            timerVictoria.restart();
                            // Quitar reproducción de recompensa aquí; solo en RECOMPENSAS
                            // Intentar carga directa por ruta para evitar problemas de sprite interno
                            if (!r.imagePath.empty()) {
                                if (recompensaTexture.loadFromFile(r.imagePath)) {
                                    recompensaSprite.setTexture(recompensaTexture);
                                }
                            }
                            // Configurar texto de nombre
                            textoRecompensaNombre.setFont(texto.getFont());
                            textoRecompensaNombre.setCharacterSize(48);
                            textoRecompensaNombre.setFillColor(Color::White);
                            textoRecompensaNombre.setString(r.displayName);
                            // Centrar el nombre
                            FloatRect nameBounds = textoRecompensaNombre.getLocalBounds();
                            textoRecompensaNombre.setOrigin(nameBounds.left + nameBounds.width/2.f, nameBounds.top + nameBounds.height/2.f);
                            
                            // Configurar texto de peso
                            textoRecompensaPeso.setFont(texto.getFont());
                            textoRecompensaPeso.setCharacterSize(32);
                            textoRecompensaPeso.setFillColor(Color(200, 200, 255)); // Azul claro
                            char pesoBuffer[128];
                            std::snprintf(pesoBuffer, sizeof(pesoBuffer), "Peso: %.1f kg (Rango: %.0f-%.0f kg)", 
                                         r.actualWeight, r.minWeight, r.maxWeight);
                            textoRecompensaPeso.setString(pesoBuffer);
                            FloatRect pesoBounds = textoRecompensaPeso.getLocalBounds();
                            textoRecompensaPeso.setOrigin(pesoBounds.left + pesoBounds.width/2.f, pesoBounds.top + pesoBounds.height/2.f);
                            
                            // Configurar texto del multiplicador y demás textos de la tarjeta de recompensa
                            textoRecompensaMultiplicador.setFont(texto.getFont());
                            textoRecompensaMultiplicador.setCharacterSize(36);
                            textoRecompensaMultiplicador.setFillColor(Color(255, 150, 50)); // Naranja
                            char multBuffer[128];
                            std::snprintf(multBuffer, sizeof(multBuffer), "%s de %.1fkg! x%.2f = %dpts", 
                                         r.displayName.c_str(), r.actualWeight, r.multiplier, r.finalPoints);
                            textoRecompensaMultiplicador.setString(multBuffer);
                            FloatRect multBounds = textoRecompensaMultiplicador.getLocalBounds();
                            textoRecompensaMultiplicador.setOrigin(multBounds.left + multBounds.width/2.f, multBounds.top + multBounds.height/2.f);
                            
                            // Configurar texto de puntos base
                            textoRecompensaPuntos.setFont(texto.getFont());
                            textoRecompensaPuntos.setCharacterSize(28);
                            textoRecompensaPuntos.setFillColor(Color(180, 180, 180)); // Gris claro
                            char puntosBuffer[64];
                            std::snprintf(puntosBuffer, sizeof(puntosBuffer), "(Base: %d pts)", r.basePoints);
                            textoRecompensaPuntos.setString(puntosBuffer);
                            FloatRect puntosBounds = textoRecompensaPuntos.getLocalBounds();
                            textoRecompensaPuntos.setOrigin(puntosBounds.left + puntosBounds.width/2.f, puntosBounds.top + puntosBounds.height/2.f);
                        } else if (tiempoRetoRestante <= 0) {
                            // Se acabó el tiempo del reto: fallo en el intento
                            retoActivo = false;
                            retoGanado = false;
                            // Detener sonido de mecánicas al perder
                            sfxEfectojuego.setLoop(false);
                            sfxEfectojuego.stop();
                            
                            // Contar fallo e intento
                            fallosAcumulados++;
                            intentosRealizados++;
                            // SFX para indicar que perdiste una vida/fallo
                            sfxvidamenos.play();
                            
                            // Verificar si se acabaron los fallos permitidos
                            if (fallosAcumulados >= fallosMaximos) {
                                juegoTerminado = true;
                                estado = DERROTA;
                                // Detener ambiente de agua al finalizar el juego por exceso de fallos
                                sfxruidoagua.setLoop(false);
                                sfxruidoagua.stop();
                                // Reproducir SFX de juego perdido una sola vez
                                sfxjuegoperdido.setLoop(false);
                                sfxjuegoperdido.play();
                            } else {
                                // Continuar jugando, mostrar mensaje de fallo temporal
                                estado = DERROTA;
                                // Detener ambiente de agua mientras se muestra el mensaje de fallo
                                sfxruidoagua.setLoop(false);
                                sfxruidoagua.stop();
                                timerDerrota.restart();
                            }
                        }

                        // Actualizar HUD en pantalla (tiempo restante, tiempo de contacto)
                        char buffer1[128];
                        std::snprintf(buffer1, sizeof(buffer1), "Tiempo: %.1fs", tiempoRetoRestante);
                        hudRetoTiempo.setString(buffer1);

                        char buffer2[128];
                        std::snprintf(buffer2, sizeof(buffer2), "Contacto: %.1fs", tiempoContactoAcumulado);
                        hudContactoTiempo.setString(buffer2);
                        // Color del HUD según contacto
                        if (enContacto) {
                            hudRetoTiempo.setFillColor(Color(0, 200, 0));
                            hudContactoTiempo.setFillColor(Color(0, 200, 0));
                        } else {
                            hudRetoTiempo.setFillColor(Color::White);
                            hudContactoTiempo.setFillColor(Color::White);
                        }
                        
                    }

                    // Dibujar HUD cuando el reto está activo o ha terminado
                    if (mostrarPezFisico) {
                        window.draw(hudRetoTiempo);
                        window.draw(hudContactoTiempo);
                        if (!retoActivo && mensajeReto.getString().getSize() > 0) {
                            // Centrar el mensaje en pantalla
                            FloatRect bounds = mensajeReto.getLocalBounds();
                            float x = (anchoVentana - bounds.width) * 0.5f - bounds.left;
                            float y = (altoVentana - bounds.height) * 0.5f - bounds.top;
                            mensajeReto.setPosition(x, y);
                            window.draw(mensajeReto);
                        }
                    }
                }
            }
            
            // Mostrar HUD de intentos y puntos siempre en el juego (superpuestos a la mecánica)
            char intentosBuffer[32];
            std::snprintf(intentosBuffer, sizeof(intentosBuffer), "%d/%d", intentosRealizados, intentosMaximos);
            hudIntentos.setString(intentosBuffer);
            
            char puntosBuffer[32];
            std::snprintf(puntosBuffer, sizeof(puntosBuffer), "%d/%d", puntosAcumulados, puntosMeta);
            hudPuntosTotales.setString(puntosBuffer);
            
            window.draw(hudIntentos);
            window.draw(hudPuntosTotales);
            
            // Sombra del pescador dibujada bajo el sprite para profundidad
            window.draw(spriteSombra);
        } else if (estado == RECOMPENSAS) {
            // Pantalla de recompensas solo con fondo negro
            window.clear(Color::Black);
            if (recompensaSeleccionada) {
                // Preferir sprite cargado directamente si está disponible
                Sprite sp = (!recompensaTexture.getSize().x ? recompensaSeleccionada->sprite : recompensaSprite);
                Vector2u texSize = (!recompensaTexture.getSize().x ? recompensaSeleccionada->texture.getSize() : recompensaTexture.getSize());
                // Caja objetivo dentro de la tarjeta de recompensa
                float maxW = 450.0f;
                float maxH = 400.0f;
                float scaleX = (texSize.x > 0) ? (maxW / static_cast<float>(texSize.x)) : 1.f;
                float scaleY = (texSize.y > 0) ? (maxH / static_cast<float>(texSize.y)) : 1.f;
                float scale = std::min(scaleX, scaleY);
                sp.setScale(scale, scale);
                // Posicionar centrado
                FloatRect sb = sp.getGlobalBounds();
                float cx = (anchoVentana - sb.width) * 0.5f;
                float cy = (altoVentana - sb.height) * 0.5f - 100;
                sp.setPosition(cx, cy);
                window.draw(sp);
                // Posicionar textos centrados bajo el sprite
                textoRecompensaNombre.setPosition(anchoVentana/2.f, cy + sb.height + 20);
                textoRecompensaMultiplicador.setPosition(anchoVentana/2.f, cy + sb.height + 170);
                textoRecompensaPeso.setPosition(anchoVentana/2.f, cy + sb.height + 80);
                textoRecompensaPuntos.setPosition(anchoVentana/2.f, cy + sb.height + 120);
                
                window.draw(textoRecompensaNombre);
                window.draw(textoRecompensaMultiplicador);
                window.draw(textoRecompensaPeso);
                window.draw(textoRecompensaPuntos);
            }
            // Tip: Presionar Enter para volver (con parpadeo)
            if (mostrarTexto1Visible) {  // Usar la misma variable de parpadeo que el menú inicial
                Text hint;
                hint.setFont(texto.getFont());
                hint.setCharacterSize(24);
                hint.setFillColor(Color::White);
                hint.setString("Presiona Enter para continuar");
                FloatRect hb = hint.getLocalBounds();
                hint.setOrigin(hb.left + hb.width/2.f, hb.top + hb.height/2.f);
                hint.setPosition(anchoVentana/2.f, altoVentana - 675);
                window.draw(hint);
            }
        } else if (estado == VICTORIA) {
            // Pantalla negra con mensaje de victoria
            window.clear(Color::Black);
            
            if (juegoTerminado) {
                // Victoria final del juego
                Text victoriaTexto;
                victoriaTexto.setFont(texto.getFont());
                victoriaTexto.setString("FELICIDADES!\nAlcanzaste la meta!");
                victoriaTexto.setCharacterSize(60);
                victoriaTexto.setFillColor(Color(0, 255, 0)); // Verde
                FloatRect vBounds = victoriaTexto.getLocalBounds();
                victoriaTexto.setOrigin(vBounds.left + vBounds.width/2.f, vBounds.top + vBounds.height/2.f);
                victoriaTexto.setPosition(anchoVentana/2.f, altoVentana/2.f - 50);
                window.draw(victoriaTexto);
                
                // Mostrar estadísticas
                Text statsTexto;
                statsTexto.setFont(texto.getFont());
                char statsBuffer[256];
                std::snprintf(statsBuffer, sizeof(statsBuffer), 
                             "Puntos obtenidos: %d/%d\nIntentos usados: %d/%d\nFallos: %d/%d", 
                             puntosAcumulados, puntosMeta, intentosRealizados, intentosMaximos, fallosAcumulados, fallosMaximos);
                statsTexto.setString(statsBuffer);
                statsTexto.setCharacterSize(36);
                statsTexto.setFillColor(Color::White);
                FloatRect sBounds = statsTexto.getLocalBounds();
                statsTexto.setOrigin(sBounds.left + sBounds.width/2.f, sBounds.top + sBounds.height/2.f);
                statsTexto.setPosition(anchoVentana/2.f, altoVentana/2.f + 80);
                window.draw(statsTexto);
                
                // Mensaje de reinicio (con parpadeo)
                if (mostrarTexto1Visible) {
                    Text reiniciarTexto;
                    reiniciarTexto.setFont(texto.getFont());
                    reiniciarTexto.setString("Presiona Enter para reiniciar");
                    reiniciarTexto.setCharacterSize(28);
                    reiniciarTexto.setFillColor(Color::White);
                    FloatRect rBounds = reiniciarTexto.getLocalBounds();
                    reiniciarTexto.setOrigin(rBounds.left + rBounds.width/2.f, rBounds.top + rBounds.height/2.f);
                    reiniciarTexto.setPosition(anchoVentana/2.f, altoVentana - 100);
                    window.draw(reiniciarTexto);
                }
            } else {
                // Victoria de un solo pez (juegoTerminado = false)
                Text victoriaTexto;
                victoriaTexto.setFont(texto.getFont());
                victoriaTexto.setString("Has pescado!");
                victoriaTexto.setCharacterSize(72);
                victoriaTexto.setFillColor(Color(0, 255, 0)); // Verde
                FloatRect vBounds = victoriaTexto.getLocalBounds();
                victoriaTexto.setOrigin(vBounds.left + vBounds.width/2.f, vBounds.top + vBounds.height/2.f);
                victoriaTexto.setPosition(anchoVentana/2.f, altoVentana/2.f);
                window.draw(victoriaTexto);
                
                // Transición automática después de 3 segundos solo para victorias individuales
                if (!juegoTerminado && timerVictoria.getElapsedTime().asSeconds() >= tiempoMostrarMensaje) {
                    estado = RECOMPENSAS;
                    // Detener ambiente de agua al entrar a la pantalla de recompensa
                    sfxruidoagua.setLoop(false);
                    sfxruidoagua.stop();
                    // Reproducir SFX de recompensa una sola vez al entrar a la tarjeta
                    sfxrecompensa.setLoop(false);
                    sfxrecompensa.play();
                }
            }
        } else if (estado == DERROTA) {
            // Pantalla negra con mensaje de derrota
            window.clear(Color::Black);
            
            if (juegoTerminado) {
                // Derrota final del juego
                Text derrotaTexto;
                derrotaTexto.setFont(texto.getFont());
                if (fallosAcumulados >= fallosMaximos) {
                    derrotaTexto.setString("JUEGO TERMINADO!\nDemasiados fallos");
                } else {
                    derrotaTexto.setString("JUEGO TERMINADO!\nNo alcanzaste la meta");
                }
                derrotaTexto.setCharacterSize(50);
                derrotaTexto.setFillColor(Color(220, 60, 60)); // Rojo
                FloatRect dBounds = derrotaTexto.getLocalBounds();
                derrotaTexto.setOrigin(dBounds.left + dBounds.width/2.f, dBounds.top + dBounds.height/2.f);
                derrotaTexto.setPosition(anchoVentana/2.f, altoVentana/2.f - 50);
                window.draw(derrotaTexto);
                
                // Mostrar estadísticas finales
                Text statsTexto;
                statsTexto.setFont(texto.getFont());
                char statsBuffer[256];
                std::snprintf(statsBuffer, sizeof(statsBuffer), 
                             "Puntos obtenidos: %d/%d\nIntentos usados: %d/%d\nFallos: %d/%d", 
                             puntosAcumulados, puntosMeta, intentosRealizados, intentosMaximos, fallosAcumulados, fallosMaximos);
                statsTexto.setString(statsBuffer);
                statsTexto.setCharacterSize(32);
                statsTexto.setFillColor(Color::White);
                FloatRect sBounds = statsTexto.getLocalBounds();
                statsTexto.setOrigin(sBounds.left + sBounds.width/2.f, sBounds.top + sBounds.height/2.f);
                statsTexto.setPosition(anchoVentana/2.f, altoVentana/2.f + 60);
                window.draw(statsTexto);
                
                // Mensaje de reinicio (con parpadeo)
                if (mostrarTexto1Visible) {
                    Text reiniciarTexto;
                    reiniciarTexto.setFont(texto.getFont());
                    reiniciarTexto.setString("Presiona Enter para reiniciar");
                    reiniciarTexto.setCharacterSize(28);
                    reiniciarTexto.setFillColor(Color::White);
                    FloatRect rBounds = reiniciarTexto.getLocalBounds();
                    reiniciarTexto.setOrigin(rBounds.left + rBounds.width/2.f, rBounds.top + rBounds.height/2.f);
                    reiniciarTexto.setPosition(anchoVentana/2.f, altoVentana - 100);
                    window.draw(reiniciarTexto);
                }
            } else {
                // Fallo individual (juegoTerminado = false)
                Text derrotaTexto;
                derrotaTexto.setFont(texto.getFont());
                char falloBuffer[128];
                std::snprintf(falloBuffer, sizeof(falloBuffer), "No pescaste nada\nFallos: %d/%d", fallosAcumulados, fallosMaximos);
                derrotaTexto.setString(falloBuffer);
                derrotaTexto.setCharacterSize(60);
                derrotaTexto.setFillColor(Color(220, 60, 60)); // Rojo
                FloatRect dBounds = derrotaTexto.getLocalBounds();
                derrotaTexto.setOrigin(dBounds.left + dBounds.width/2.f, dBounds.top + dBounds.height/2.f);
                derrotaTexto.setPosition(anchoVentana/2.f, altoVentana/2.f);
                window.draw(derrotaTexto);
                
                // Transición automática después de 3 segundos solo para derrotas individuales
                if (!juegoTerminado && timerDerrota.getElapsedTime().asSeconds() >= tiempoMostrarMensaje) {
                    // Volver directamente al juego (pescador esperando)
                    estado = JUEGO;
                    // Resetear animación del pescador para volver al inicio
                    animacionTerminada = false;
                    mostrarPezFisico = false;
                    bucleFinalIniciado = false;
                    animacionBucleIniciada = false;
                    pescador.currentFrame = 3;
                    // Limpiar variables del reto
                    retoActivo = false;
                    retoGanado = false;
                    tiempoRetoRestante = 0.0f;
                    tiempoContactoAcumulado = 0.0f;
                    cuadradoInicializado = false;
                    mensajeReto.setString("");
                    // Reanudar ambiente de agua al volver al estado de juego
                    sfxruidoagua.setLoop(true);
                    sfxruidoagua.play();
                }
            }
        }

        // Primera transición de fundido (INICIO -> INSTRUCCIONES)
        if (estado == TRANSICION) {
            if (opacidad < 255) {
                opacidad += 1;
                if (opacidad > 255) opacidad = 255;
                fadeRect.setFillColor(Color(0, 0, 0, opacidad));
                window.draw(fadeRect);
            } else {
                // Cuando termine la primera transición, ir a instrucciones
                fadeRect.setFillColor(Color(0, 0, 0, 255));
                window.draw(fadeRect);
                window.display();
                sf::sleep(sf::milliseconds(250));
                estado = INSTRUCCIONES;
                timerInstrucciones.restart();
                continue;
            }
        }
        
        // Estado de instrucciones: mostrar texto y esperar tiempo para pasar a la siguiente transición
        else if (estado == INSTRUCCIONES) {
            detenerMusicaJuego();
            window.clear(Color::Black);
            window.draw(textoInstrucciones);
            
            // Cuando termine el tiempo de instrucciones, ir a segunda transición
            if (timerInstrucciones.getElapsedTime().asSeconds() >= tiempoInstrucciones) {
                estado = TRANSICION_FINAL;
                opacidad = 0;
                // Iniciar SFX en bucle durante la transición para que se note
                sfxTran2.setLoop(true);
                sfxTran2.play();
            }
        }
        
        // Segunda transición de fundido (INSTRUCCIONES -> JUEGO)
        else if (estado == TRANSICION_FINAL) {
            if (opacidad < 255) {
                opacidad += 1;
                if (opacidad > 255) opacidad = 255;
                fadeRect.setFillColor(Color(0, 0, 0, opacidad));
                window.draw(fadeRect);
            } else {
                // Cuando termine la segunda transición, ir al juego
                fadeRect.setFillColor(Color(0, 0, 0, 255));
                window.draw(fadeRect);
                window.display();
                sf::sleep(sf::milliseconds(250));
                // Detener el bucle del SFX al finalizar la transición
                sfxTran2.setLoop(false);
                estado = JUEGO;
                // Iniciar ambiente de agua en bucle durante el estado de juego
                sfxruidoagua.setLoop(true);
                sfxruidoagua.play();
                animacionCortaMostrada = false;
                fadeRect.setFillColor(Color(0, 0, 0, 0));
                // Preparar flag para evitar repetición temprana
                sfxCatchPlayedForThisAnim = false;
                continue;
            }
        }
        // Transición de reinicio desde pantallas finales hacia INICIO
        else if (estado == REINICIO_TRANSICION) {
            if (opacidad < 255) {
                opacidad += 1; // transición lenta como la inicial
                if (opacidad > 255) opacidad = 255;
                fadeRect.setFillColor(Color(0, 0, 0, opacidad));
                window.draw(fadeRect);
            } else {
                // Al concluir el fundido, efectuar el reinicio
                // Resetear todas las variables del juego
                intentosRealizados = 0;
                fallosAcumulados = 0;
                puntosAcumulados = 0;
                juegoTerminado = false;
                retoActivo = false;
                retoGanado = false;
                // Asegurar que el SFX de mecánicas esté detenido
                sfxEfectojuego.setLoop(false);
                sfxEfectojuego.stop();
                // Detener SFX de juego perdido si estaba en bucle
                sfxjuegoperdido.setLoop(false);
                sfxjuegoperdido.stop();
                // Detener ambiente de agua si estaba activo
                sfxruidoagua.setLoop(false);
                sfxruidoagua.stop();
                animacionTerminada = false;
                mostrarPezFisico = false;
                bucleFinalIniciado = false;
                animacionBucleIniciada = false;
                pescador.currentFrame = 3;
                tiempoRetoRestante = 0.0f;
                tiempoContactoAcumulado = 0.0f;
                cuadradoInicializado = false;
                mensajeReto.setString("");
                recompensaSeleccionada = nullptr;
                // Ir a inicio y limpiar fundido
                estado = INICIO;
                // Reproducir música del menú solo en pantalla de inicio
                iniciarMusicaJuego("assets/Musica/Troubadeck 54 Infinity.ogg");
                fadeRect.setFillColor(Color(0, 0, 0, 0));
                continue;
            }
        }
        window.display();
        
        }

        // Ya no necesitamos limpiar físicas Box2D - movimiento directo
        
        detenerMusicaJuego();
        return 0;
    }