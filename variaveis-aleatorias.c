#include<stdlib.h>
#include<math.h>

#define PI 3.14159265359

double uniforme(double a, double b)
{
    double uniforme_padrao = (double)rand() / (double)RAND_MAX;

    return a + uniforme_padrao * (b - a);
}

double normal(double media, double desvio_padrao) { // Amostra uma variável aleatória normal pelo algoritmo de box muller (https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform)
    static int tem_guardado = 0;
    static double guardado;
    
    double u1, u2;
    double r, theta;

    if (tem_guardado) {
        tem_guardado = 0;
        return guardado;
    }

    do {
        u1 = uniforme(0, 1);
    } while (u1 <= 0);

    u2 = uniforme(0, 1);

    r = sqrt(-2 * log(u1)) * desvio_padrao;
    theta = 2 * PI * u2;

    tem_guardado = 1;
    guardado = r * cos(theta) + media;

    return r * sin(theta) + media;
}