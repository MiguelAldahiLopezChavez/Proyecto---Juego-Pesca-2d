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
    RectangleShape cuadradoVerdeShape(Vector2f(90, 180)); // Más alto que ancho
    cuadradoVerdeShape.setFillColor(Color::Green);
    cuadradoVerdeShape.setOrigin(45, 90); // Centrar el origen (mitad del tamaño)

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
    
        Texture mecanica;
        if (!mecanica.loadFromFile("assets/imagen/nuevaimagen/mecanica_juego_principal.png")) 
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
    float pezVelocidadY = 0.5f; // Velocidad inicial
    float pezDireccion = 1.0f; // 1 = abajo, -1 = arriba
    float pezVelocidadCambio = 0.04f; // Qué tan rápido cambia la velocidad
    float pezVelocidadMin = 0.5f;
    float pezVelocidadMax = 1.7f;
    
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
    float tiempoRetoTotal = 15.0f;
    float tiempoRetoRestante = 0.0f;
    float tiempoContactoAcumulado = 0.0f;
    float tiempoContactoNecesario = 3.0f;
    Clock relojReto;


    controlTexto texto;
    texto.loadFont("assets/Letras/opcion 1/Bear Days.ttf");

    Text mostrarTexto;
    Text mostrarTexto1;
    // HUD del reto
    Text hudRetoTiempo;       // Esquina superior derecha: tiempo restante
    Text hudContactoTiempo;   // Mitad lado derecho: tiempo de contacto acumulado

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

    // Inicializar HUD del reto con la misma fuente
    hudRetoTiempo.setFont(texto.getFont());
    hudRetoTiempo.setCharacterSize(40);
    hudRetoTiempo.setFillColor(Color::White);
    // Posición: esquina superior derecha con margen
    hudRetoTiempo.setPosition(anchoVentana - 325, 320);

    hudContactoTiempo.setFont(texto.getFont());
    hudContactoTiempo.setCharacterSize(50);
    hudContactoTiempo.setFillColor(Color::White);
    // Posición: medio del lado derecho
    hudContactoTiempo.setPosition(anchoVentana - 400, altoVentana / 2 - 14);

    RenderWindow window(VideoMode(anchoVentana, altoVentana), "Juego de Pesca 2D");
    iniciarMusicaJuego("assets/Musica/Troubadeck 25 Deep Dark Sea.ogg");

    //Pez pez1("assets/imagen/op3/peces/nemo/nemo.png", 100, 100, 1, 0);
    Sprite spriteFondo(fondo);
    Sprite spriteFondo2(fondo2);

    enum EstadoPantalla { INICIO, TRANSICION, JUEGO };
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
            // Control de animación del pescador (solo SPACE)
            if (estado == JUEGO && event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                std::cout << "SPACE PRESIONADO - Control pescador!" << std::endl;
                if (!animacionTerminada || !mostrarPezFisico) {
                    // Si no está en la mecánica, resetear/iniciar animación
                    std::cout << "Reseteando/iniciando animación del pescador" << std::endl;
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
                std::cout << "SALTO CUADRADO DETECTADO!" << std::endl;
                if (animacionTerminada && mostrarPezFisico && cuadradoInicializado) {
                    // Aplicar impulso hacia arriba (contrarrestar gravedad)
                    cuadradoVelocidadY = cuadradoFuerzaSalto;
                    std::cout << "¡¡¡SALTO APLICADO!!! Nueva velocidad Y: " << cuadradoVelocidadY << std::endl;
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
            
            // Las físicas simples del pez se manejan directamente cuando aparece
            
            // Detectar cuando termina la animación completa del pescador
            // La animación termina cuando está en el bucle final (frames 8-10)
            if (!animacionTerminada && pescador.isEnBucleFinal()) {
                animacionTerminada = true;
                timerAnimacion.restart();
                std::cout << "Animacion terminada, iniciando timer" << std::endl;
            }
            
            // Debug: mostrar estado actual
            if (!animacionTerminada) {
                std::cout << "Frame actual: " << pescador.getFrameActual() << ", Activa: " << pescador.isAnimacionActiva() << std::endl;
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
                    cuadradoLimiteSup = posMecanica.y + 125;  // Límite superior
                    cuadradoLimiteInf = posMecanica.y + boundsMecanica.height - 125;  // Límite inferior
                    cuadradoY = cuadradoLimiteSup + 150; // Comenzar BIEN ABAJO del límite superior para caer visiblemente
                    cuadradoVelocidadY = 0; // Empezar sin velocidad inicial
                    
                    std::cout << "Cuadrado inicializado en: (" << cuadradoX << ", " << cuadradoY << ")" << std::endl;
                    std::cout << "Límites Y: " << cuadradoLimiteSup << " - " << cuadradoLimiteInf << std::endl;
                    
                    cuadradoInicializado = true;
                    mostrarPezFisico = true;
                    // Iniciar reto al mostrar la mecánica y entidades
                    retoActivo = true;
                    retoGanado = false;
                    tiempoRetoRestante = tiempoRetoTotal;
                    tiempoContactoAcumulado = 0.0f;
                    relojReto.restart();
                    std::cout << "Sistemas inicializados: Pez simple + Cuadrado verde movimiento directo" << std::endl;
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
                    
                    // Cambiar velocidad gradualmente para movimiento impredecible
                    if (!retoGanado) {
                        pezVelocidadY += (rand() % 10 - 5) * pezVelocidadCambio;
                        if (pezVelocidadY > pezVelocidadMax) pezVelocidadY = pezVelocidadMax;
                        if (pezVelocidadY < pezVelocidadMin) pezVelocidadY = pezVelocidadMin;
                    }
                    
                    // Mover pez arriba y abajo
                    if (!retoGanado) {
                        pezPosY += pezVelocidadY * pezDireccion;
                    }

                    // Al cruzar por la zona media, existe probabilidad de cambiar velocidad y/o dirección
                    if (!retoGanado && std::abs(pezPosY - pezZonaMedia) <= pezZonaMediaMargen) {
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
                    if (!retoGanado) {
                        cuadradoVelocidadY += cuadradoGravedad * dt; // dt es delta time
                    }
                    
                    // Limitar velocidad máxima
                    if (cuadradoVelocidadY > cuadradoVelocidadMaxima) {
                        cuadradoVelocidadY = cuadradoVelocidadMaxima;
                    }
                    
                    // Actualizar posición Y con la velocidad
                    if (!retoGanado) {
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
                    
                    // Debug
                    std::cout << "Pez: (" << pezPosX << ", " << pezPosY << ") Vel: " << pezVelocidadY * pezDireccion;
                    std::cout << " | Cuadrado: (" << cuadradoX << ", " << cuadradoY << ") VelY: " << cuadradoVelocidadY << std::endl;

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

                        // Comprobar victoria
                        if (tiempoContactoAcumulado >= tiempoContactoNecesario) {
                            retoGanado = true;
                            retoActivo = false;
                            std::cout << "\n=== RETO SUPERADO ===\n";
                            std::cout << "Contacto acumulado: " << tiempoContactoAcumulado << "s de " << tiempoContactoNecesario << "s" << std::endl;
                        } else if (tiempoRetoRestante <= 0) {
                            // Fin del reto sin victoria
                            retoActivo = false;
                            std::cout << "\n=== RETO FALLIDO ===\n";
                            std::cout << "Contacto acumulado: " << tiempoContactoAcumulado << "s de " << tiempoContactoNecesario << "s" << std::endl;
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
                        
                        // Mostrar estado simple en consola
                        std::cout << "Reto: restante=" << tiempoRetoRestante << "s, contacto=" << tiempoContactoAcumulado << "s" << (enContacto ? " [CONTACTO]" : "") << std::endl;
                    }

                    // Dibujar HUD cuando el reto está activo o ganado/terminado
                    if (mostrarPezFisico) {
                        window.draw(hudRetoTiempo);
                        window.draw(hudContactoTiempo);
                    }
                }
            }
            window.draw(spriteSombra);
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

