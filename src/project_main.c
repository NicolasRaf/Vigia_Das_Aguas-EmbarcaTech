#include <stdio.h>
#include "pico/stdlib.h"
#include "init.h"

int reading = 1;

int main() {
    initializeSystem();

    while (true) {

        if (reading) {
            float distance = mensureDistance();
            char distanceText[20];
            
            ledFeedback(distance);

            sprintf(distanceText, "Distancia: %.2f cm\n", distance);
            showText(distanceText, 0, 0, 1, true);
        }
           
        sleep_ms(100);
    }

    return 0;
}
