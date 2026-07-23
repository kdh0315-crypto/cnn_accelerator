#include <stdio.h>
#include <stdint.h>
#include "parameter.h"

// module
#include "module/conv.h"


// print function to print
#include "debug/debug_print.h"


int main(void)
{
    uint8_t conv_en = 1;
    

    LayerProbe_t probe1;
    LayerProbe_Init(&probe1, FILTERED_IMG_WIDTH1, CONV_IMG_WIDTH1);

    int i = 0;
    printf("Line Buffer & Convolution layer Test Start\n");
    for (int cycle = 0; cycle < TOTAL_CYCLES; cycle++)
    {
        
        


        i++;
        if (i >= INPUT_IMG_SIZE)
        {
            i = INPUT_IMG_SIZE;
            conv_en = 0;
        }
    }

    LayerProbe_Print(&probe1, "Layer1", FILTERED_IMG_HEIGHT1, CONV_IMG_HEIGHT1);
    LayerProbe_Free(&probe1);

    return 0;
}
