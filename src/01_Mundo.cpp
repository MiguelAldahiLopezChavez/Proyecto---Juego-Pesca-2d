#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "audio.hpp"
#include "peces.hpp"
#include "fonts.hpp"
#include "pescador.hpp"
using namespace sf;


int main()
{
    Texture pez_mec;
         if (!pez_mec.loadFromFile("assets/imagen/nuevaimagen/iamgenpez_mecanicajuego.png")) 
     {
    
          return -1;
     }

     Sprite spritePezMec(pez_mec);
     float escalaPezMec = 0.55f;
     spritePezMec.setScale(escalaPezMec, escalaPezMec);

    // Crear rectángulo verde (más alargado en Y)
    RectangleShape cuadradoVerdeShape(Vector2f(90, 120)); // Más alto que ancho
    cuadradoVerdeShape.setFillColor(Color::Green);
    cuadradoVerdeShape.setOrigin(45, 60); // Centrar el origen (mitad del tamaño)

    Texture mecanica_juego;
         if (!mecanica_juego.loadFromFile("assets/imagen/nuevaimagen/mecanica_juego_principal_2.png")) 
     {
    
          return -1;
     }

     Sprite spriteMecanicaJuego(mecanica_juego);
     
     // Variables de control
     Clock timerAnimacion;
     bool animacionTerminada = false;
     bool mostrarPezFisico = false;
     float tiempoEspera = 2.0f; // 2 segundos después de terminar animación
     


    Texture fondo;
       if (!fondo.loadFromFile("assets/Background.png")) 
    {

        return -1;
    }   

    Texture fondo2;
         if (!fondo2.loadFromFile("assets/imagen/nuevaimagen/background.png")) 
     {
    
          return -1;
     }

     Texture sombrapescador;
        if (!sombrapescador.loadFromFile("assets/imagen/nuevaimagen/sombra.png")) 
        {
        
            return -1;
        }

    


    Vector2u size = fondo.getSize();
    unsigned int anchoVentana = size.x;
    unsigned int altoVentana = size.y;


    // Posicionar la mecánica de juego: centrada en Y, a la izquierda en X
    Vector2u sizeMecanica = mecanica_juego.getSize();
    spriteMecanicaJuego.setPosition(60, (altoVentana - sizeMecanica.y) / 2);

    Sprite spriteSombra(sombrapescador);
    spriteSombra.setPosition(505, 460);
    
    // Variables para el cuadrado verde (con gravedad)
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

    // Sistema de reto: 15s para acumular 3s de contacto pez-rectángulo
    bool retoActivo = false;
    bool retoGanado = false;
    float tiempoRetoTotal = 10.0f;
    float tiempoRetoRestante = 0.0f;
    float tiempoContactoAcumulado = 0.0f;
    float tiempoContactoNecesario = 5.0f;
    Clock relojReto;


    controlTexto texto;
    texto.loadFont("assets/Letras/opcion 1/Bear Days.ttf");

    Text mostrarTexto;
    Text mostrarTexto1;
    Text hudRetoTiempo;       
    Text hudContactoTiempo;   
    Text infoJuego;
    Text mensajeReto;         // Mensaje grande de resultado (ganado/fallido)
    // Recompensas
    RewardManager recompensaManager;
    const Reward* recompensaSeleccionada = nullptr;
    // Carga directa alternativa por ruta
    Texture recompensaTexture;
    Sprite recompensaSprite;
    Text textoRecompensaNombre;

    Clock reloj;
    bool mostrarTexto1Visible = true;
    float intervaloparpadeo =0.3f;
    bool animacionCortaMostrada = false;
    

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
    
    infoJuego.setFont(texto.getFont());
    infoJuego.setString("\t\t\t\t\tUsa SPACE para pescar\nUsa UP o clic izquierdo para mover rectangulo verde");
    infoJuego.setCharacterSize(20);
    infoJuego.setFillColor(Color::White);
    infoJuego.setPosition((anchoVentana/2) - 275, 50);

    mensajeReto.setFont(texto.getFont());
    mensajeReto.setCharacterSize(72);
    mensajeReto.setFillColor(Color::White);
    mensajeReto.setString("");

    RenderWindow window(VideoMode(anchoVentana, altoVentana), "Juego de Pesca 2D");
    iniciarMusicaJuego("assets/Musica/Troubadeck 25 Deep Dark Sea.ogg");

    Sprite spriteFondo(fondo);
    Sprite spriteFondo2(fondo2);

    enum EstadoPantalla { INICIO, TRANSICION, JUEGO, VICTORIA, DERROTA, RECOMPENSAS };
    EstadoPantalla estado = INICIO;

    int opacidad = 0;
    RectangleShape fadeRect(Vector2f(anchoVentana, altoVentana));
    fadeRect.setFillColor(Color(0, 0, 0, 0));


    // Reloj persistente para delta time correcto
    Clock relojDelta;
    while (window.isOpen())
    {
        float dt = relojDelta.restart().asSeconds();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (estado == INICIO && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                estado = TRANSICION;
                opacidad = 0; // Inicializa opacidad solo al entrar en transición
            }
            // Reiniciar reto con Enter cuando el reto terminó (ganado o perdido) y estamos en JUEGO
            if (estado == JUEGO && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                if (!retoActivo && mostrarPezFisico) {
                    retoActivo = true;
                    retoGanado = false;
                    tiempoRetoRestante = tiempoRetoTotal;
                    tiempoContactoAcumulado = 0.0f;
                    mensajeReto.setString("");
                }
            }
            // Salir de pantalla de recompensas con Enter
            if (estado == RECOMPENSAS && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                // Iniciar transición de vuelta al juego
                estado = TRANSICION;
                opacidad = 0; // Reiniciar transición
                recompensaSeleccionada = nullptr;
                mensajeReto.setString("");
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
            }
            // Continuar desde VICTORIA a RECOMPENSAS
            if (estado == VICTORIA && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                estado = RECOMPENSAS;
            }
            // Continuar desde DERROTA de vuelta al juego
            if (estado == DERROTA && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                // Resetear al estado inicial del juego
                estado = TRANSICION;
                opacidad = 0;
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
            }
            // Control de animación del pescador (solo SPACE)
            if (estado == JUEGO && event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                if (!animacionTerminada || !mostrarPezFisico) {
                    animacionTerminada = false;
                    mostrarPezFisico = false;
                    bucleFinalIniciado = false;
                    animacionBucleIniciada = false;
                    pescador.currentFrame = 3;
                    pescador.iniciarAnimacion(false);
                }
            }
            // Control de salto del cuadrado verde (UP arrow y mouse click)
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
        window.clear();
        window.draw(spriteFondo);




if (estado == INICIO) {
            window.draw(mostrarTexto);
            if (mostrarTexto1Visible) {
                window.draw(mostrarTexto1);
            }

        } else if (estado == JUEGO) {
            window.clear();
            window.draw(spriteFondo2);
            // Inicia el bucle de los primeros 3 frames solo si no hay animación activa
            if (!pescador.isAnimacionActiva() && pescador.getFrameActual() < 3) {
                pescador.iniciarAnimacion(true); // true = bucle (primeros 3 frames)
            }
            pescador.update();
            window.draw(pescador.obtenerSprite());
            // Mostrar infoJuego mientras aún no termina la animación del pescador
            if (!animacionTerminada) {
                window.draw(infoJuego);
            }
            
            // Las físicas simples del pez se manejan directamente cuando aparece
            
            // Detectar cuando termina la animación completa del pescador
            // La animación termina cuando está en el bucle final (frames 8-10)
            if (!animacionTerminada && pescador.isEnBucleFinal()) {
                animacionTerminada = true;
                timerAnimacion.restart();
            }
                        
            // Mostrar mecánica de juego y activar sistemas cuando termina la animación
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
                    
                    
                    cuadradoInicializado = true;
                    mostrarPezFisico = true;
                    // Iniciar reto al mostrar la mecánica y entidades
                    retoActivo = true;
                    retoGanado = false;
                    tiempoRetoRestante = tiempoRetoTotal;
                    tiempoContactoAcumulado = 0.0f;
                    relojReto.restart();
                }
                
                // Mostrar mecánica de juego
                window.draw(spriteMecanicaJuego);
                
                if (cuadradoInicializado) {
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
                    
                    // Dibujar cuadrado verde en su posición actual
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

                        if (enContacto) {
                            tiempoContactoAcumulado += dt;
                        }

                        // Comprobar victoria/derrota y preparar mensaje
                        if (tiempoContactoAcumulado >= tiempoContactoNecesario) {
                            retoGanado = true;
                            retoActivo = false;
                            // Ir a pantalla de victoria primero
                            estado = VICTORIA;
                            // Preparar recompensa para después
                            recompensaManager.loadDefaults();
                            const Reward& r = recompensaManager.pickRandom();
                            recompensaSeleccionada = &r;
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
                            // Centrar el nombre bajo el sprite
                            FloatRect nameBounds = textoRecompensaNombre.getLocalBounds();
                            textoRecompensaNombre.setOrigin(nameBounds.left + nameBounds.width/2.f, nameBounds.top + nameBounds.height/2.f);
                        } else if (tiempoRetoRestante <= 0) {
                            retoActivo = false;
                            retoGanado = false;
                            // Ir a pantalla de derrota
                            estado = DERROTA;
                        }

                        // Actualizar HUD en pantalla (esquina sup. derecha y medio lado derecho)
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
                float cy = (altoVentana - sb.height) * 0.5f - 20;
                sp.setPosition(cx, cy);
                window.draw(sp);
                // Posicionar nombre centrado bajo el sprite
                textoRecompensaNombre.setPosition(anchoVentana/2.f, cy + sb.height + 50);
                window.draw(textoRecompensaNombre);
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
                hint.setPosition(anchoVentana/2.f, altoVentana - 70);
                window.draw(hint);
            }
        } else if (estado == VICTORIA) {
            // Pantalla negra con mensaje de victoria
            window.clear(Color::Black);
            // Mensaje principal de victoria
            Text victoriaTexto;
            victoriaTexto.setFont(texto.getFont());
            victoriaTexto.setString("¡Has pescado!");
            victoriaTexto.setCharacterSize(72);
            victoriaTexto.setFillColor(Color(0, 255, 0)); // Verde
            FloatRect vBounds = victoriaTexto.getLocalBounds();
            victoriaTexto.setOrigin(vBounds.left + vBounds.width/2.f, vBounds.top + vBounds.height/2.f);
            victoriaTexto.setPosition(anchoVentana/2.f, altoVentana/2.f - 50);
            window.draw(victoriaTexto);
            // Mensaje de continuar (con parpadeo)
            if (mostrarTexto1Visible) {
                Text continuar;
                continuar.setFont(texto.getFont());
                continuar.setString("Presiona Enter para descubrir tu recompensa");
                continuar.setCharacterSize(30);
                continuar.setFillColor(Color::White);
                FloatRect cBounds = continuar.getLocalBounds();
                continuar.setOrigin(cBounds.left + cBounds.width/2.f, cBounds.top + cBounds.height/2.f);
                continuar.setPosition(anchoVentana/2.f, altoVentana/2.f + 50);
                window.draw(continuar);
            }
        } else if (estado == DERROTA) {
            // Pantalla negra con mensaje de derrota
            window.clear(Color::Black);
            // Mensaje principal de derrota
            Text derrotaTexto;
            derrotaTexto.setFont(texto.getFont());
            derrotaTexto.setString("Reto fallido\nIntenta de nuevo");
            derrotaTexto.setCharacterSize(72);
            derrotaTexto.setFillColor(Color(220, 60, 60)); // Rojo
            FloatRect dBounds = derrotaTexto.getLocalBounds();
            derrotaTexto.setOrigin(dBounds.left + dBounds.width/2.f, dBounds.top + dBounds.height/2.f);
            derrotaTexto.setPosition(anchoVentana/2.f, altoVentana/2.f - 50);
            window.draw(derrotaTexto);
            // Mensaje de continuar (con parpadeo)
            if (mostrarTexto1Visible) {
                Text continuar;
                continuar.setFont(texto.getFont());
                continuar.setString("Presiona Enter para volver a pescar");
                continuar.setCharacterSize(30);
                continuar.setFillColor(Color::White);
                FloatRect cBounds = continuar.getLocalBounds();
                continuar.setOrigin(cBounds.left + cBounds.width/2.f, cBounds.top + cBounds.height/2.f);
                continuar.setPosition(anchoVentana/2.f, altoVentana/2.f + 50);
                window.draw(continuar);
            }
        }

             // Transición de fundido
        if (estado == TRANSICION) {
            if (opacidad < 255) {
                opacidad += 1; // velocidad del fundido
                if (opacidad > 255) opacidad = 255;
                fadeRect.setFillColor(Color(0, 0, 0, opacidad));
                window.draw(fadeRect);
            } else {
                // Cuando opacidad llega a 255, se mantiene el frame negro un ciclo
                fadeRect.setFillColor(Color(0, 0, 0, 255));
                window.draw(fadeRect);
                window.display();
                sf::sleep(sf::milliseconds(250)); // Pausa breve para el efecto
                estado = JUEGO;
                animacionCortaMostrada = false;
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