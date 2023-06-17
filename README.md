#Documentação auxiliar para os desenvolvedores

### EVENTOS NA FILA DO BOTÃO:
Único evento, de pressionamento do botão. É enviado o inteiro 1 para identificar o evento.
Acredito que esse evento poderia ser uma event flag.

### EVENTOS NA FILA DE CONTROLE:
Os eventos poderão ser de dois tipos: SHOT ou MOVE. Os dois primeiros bits serão usados para a identificação do evento e os bits restantes para transmitir o payload de cada eventos

1. SHOT:
 - Bits de identificação (b31, b30): 00
 - Payload (b29, b0): não tem
 
2. MOVE:
 - Bits de identificação (b31, b30): 10
 - Payload (b29, b0): 
  - (b29, b14) - 15 bits para identificar a coordenada x do local pressionado
  - (b14, b0) - 15 bits para identificar a coordenada y do local pressionado
  
Exemplo:
#include <stdio.h>

int main() {
    int x = 123;  // Example x-coordinate
    int y = 456;  // Example y-coordinate

    // Prepare the bits
    int coord = (0 << 31) | (0 << 30) | (x << 15) | y;

    // Extract the values from the bits
    int extracted_x = (coord >> 15) & 0x7FFF;
    int extracted_y = coord & 0x7FFF;

    printf("Original x: %d\n", x);
    printf("Original y: %d\n", y);
    printf("Extracted x: %d\n", extracted_x);
    printf("Extracted y: %d\n", extracted_y);

    return 0;
}


