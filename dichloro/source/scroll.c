#include "variables.h"
#include "video.h"
#include "text.h"
#include "math.h"
#include "graphics.h"
#include "programs.h"

// CREDITS FOR ARTICZERA (MADE ORIGINAL FOR NOVA OS)
void _start()
{
    const char* message = "DichloroOS program!!!";
    int messageLength = 27;
    int initialX = WSCREEN;

    while (initialX > -(27 * 8 * 3))
    {
        clearscren();

        for (int i = 0; i < messageLength; i++)
        {
            int charX = initialX + (i * 8);
            int yOffset = (int)(WAVE_AMPLITUDE * sin((charX + WSCREEN) / WAVE_FREQUENCY));

            SetCursorX(charX);
            SetCursorY((HSCREEN / 2) + yOffset);

            printout(message[i], 0x20 + i);
        }

        initialX -= SCROLL_SPEED;

        for (volatile int i = 0; i < 100000; i++);
    }
}