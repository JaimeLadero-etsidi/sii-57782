#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "DatosMemCompartida.h"

int main() {
    // Abro el fichero de memoria compartida creado por el tenis
    int fd = open("/tmp/tenis_memoria", O_RDWR);
    if (fd == -1) {
        std::cerr << "Error al abrir la memoria compartida en el bot" << std::endl;
        return 1;
    }

    // Proyectamos el fichero en memoria
    DatosMemCompartida *datos = (DatosMemCompartida *) mmap(
        NULL,
        sizeof(DatosMemCompartida),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (datos == MAP_FAILED) {
        perror("Error en mmap del bot");
        close(fd);
        return 1;
    }

    // Cierre el descriptor de fichero
    close(fd);

    // Bucle infinito del bot
    while (true) {
        // Calcular el centro de la raqueta usando y1 e y2
        float centroRaquetaY = (datos->raqueta1.y1 + datos->raqueta1.y2) / 2.0;


        // Comparar la coordenada Y de la esfera (centro.y) con la de la raqueta
        if (datos->esfera.centro.y < centroRaquetaY) {
            datos->accion = 1;  // Se mueve arriba
        } else if (datos->esfera.centro.y > centroRaquetaY) {
            datos->accion = -1; // se mueve abajo
        } else {
            datos->accion = 0;  // se queda quieto
        }

        // Se suspende el proceso durante 25 ms
        usleep(25000);
    }

    return 0;
}
