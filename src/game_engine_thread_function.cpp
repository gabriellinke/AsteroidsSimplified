#include "game_engine_thread.h"
#include "constants.h"

#include <stdio.h>
#include <math.h>
#include "game_logic/MyClass.h"
#include "game_logic/Space.h"
#include <vector>


/* Variáveis para testes */
ULONG points = 0;
INT angle = 0;
INT x1 = 20, x2 = 200, y1 = 20, y2 = 280, x3 = 0, y3 = 0;

int counter = 1000;
int recordScore;

void updateScoreGraphics();
void updateSpaceshipGraphics();
void updateAsteroidsGraphics();

/*=====================================================================================*/

extern "C" void game_engine_thread_function(void);

void calculateSpaceshipAimAngle(UINT coord);
void getInputs();
void updateGame();

void game_engine_thread_function(void)
{
    ULONG status, flag;
    Space space;
    std::vector<int> objects;
    // TODO: Remover funções
    updateScoreGraphics();
    updateSpaceshipGraphics();
    updateAsteroidsGraphics();
    tx_event_flags_set(&event_flags, FLAG1, TX_OR);

    while (1)
    {
        status = tx_event_flags_get(&event_flags, FLAG0, TX_OR_CLEAR, &flag, TX_WAIT_FOREVER);
        if(TX_SUCCESS != status) __BKPT(0);

        getInputs();
        // space.setInputs()
        objects = space.update();
        if(space.getGameOver()) {
            // Verifica quantos pontos fez, se precisar atualiza o recorde
            // Manda para a tela inicial
            recordScore = points;
            tx_event_flags_set(&event_flags, FLAG2, TX_OR); // Seta flag para voltar para o Menu
        }

        updateGame();

        tx_event_flags_set(&event_flags, FLAG1, TX_OR);
    }
}


void updateScoreGraphics() {
    ULONG update_score_message = SCORE << SHIFT_TYPE | points;
    ULONG status = tx_queue_send(&graphic_queue, &update_score_message, TX_NO_WAIT);
    if(TX_SUCCESS != status) __BKPT(0);
}

void updateSpaceshipGraphics() {
    ULONG update_spaceship_message = SPACESHIP << SHIFT_TYPE | angle;
    ULONG status = tx_queue_send(&graphic_queue, &update_spaceship_message, TX_NO_WAIT);
    if(TX_SUCCESS != status) __BKPT(0);
}

void updateAsteroidsGraphics() {
    // Testando se as classes funcionam
    // Create an instance of MyClass
    std::vector<int> numbers;
    MyClass obj(42);

    numbers.push_back(obj.getNumber());
    numbers.push_back(obj.getNumber());
    numbers.push_back(obj.getNumber());

    int number = 0;
    for (const auto& element : numbers) {
        number += element;
    }
}

void calculateSpaceshipAimAngle(UINT coord) {
    // Extract the values from the bits
    int x = ((coord & MASK_COORDS_X) >> SHIFT_COORDS_X);
    int y = Y_MAX - ((coord & MASK_COORDS_Y) >> SHIFT_COORDS_Y); // Invert Y axis to calculate angle properly
    int diff_x = x-X_MAX/2;
    int diff_y = y-Y_MAX/2;
    double angle_rad = atan2(diff_y, diff_x);
    double angle_deg = angle_rad * 180.0 / PI;
    angle = (360 + 90-(INT)(angle_deg))%360;
}

// TODO: Atirar ao recer mensagem de shot
void getInputs() {
    ULONG message, status;
    status = tx_queue_receive(&control_queue, &message, TX_NO_WAIT);
    while(TX_SUCCESS == status) {

        if(message >> SHIFT_INPUT == SHOT_MESSAGE) {
            points += 10;
            // TODO: Adicionar tiro e remover a parte do score
            updateScoreGraphics();
        } else {
            // TODO: Enviar ângulo da nave para o space
            calculateSpaceshipAimAngle(message);
        }
        status = tx_queue_receive(&control_queue, &message, TX_NO_WAIT);
    }

    if(TX_QUEUE_EMPTY != status) __BKPT(0);
}

void updateGame() {
    x1+=1;
    y1+=1;
    x2-=2;
    y2-=2;
    x3-=3;
    y3-=3;
    counter--;

    ULONG message, status;
    if(counter > 600) {
        message = 1 << SHIFT_ID | BIG_ASTEROID << SHIFT_TYPE | x1 << SHIFT_COORDS_X | y1 << SHIFT_COORDS_Y;
        status = tx_queue_send(&graphic_queue, &message, TX_NO_WAIT);
        if(TX_SUCCESS != status) __BKPT(0);
    }
    if(counter > 300) {
        message = 2 << SHIFT_ID | SMALL_ASTEROID << SHIFT_TYPE | x2 << SHIFT_COORDS_X | y2 << SHIFT_COORDS_Y;
        status = tx_queue_send(&graphic_queue, &message, TX_NO_WAIT);
        if(TX_SUCCESS != status) __BKPT(0);
    }
    if(counter > 0) {
        message = 3 << SHIFT_ID | BULLET << SHIFT_TYPE | x3 << SHIFT_COORDS_X | y3 << SHIFT_COORDS_Y;
        status = tx_queue_send(&graphic_queue, &message, TX_NO_WAIT);
        if(TX_SUCCESS != status) __BKPT(0);
    }
    updateSpaceshipGraphics();

    if(x1 > X_MAX) x1 = 0;
    if(x2 > X_MAX) x2 = 0;
    if(x3 > X_MAX) x3 = 0;
    if(y1 > Y_MAX) y1 = 0;
    if(y2 > Y_MAX) y2 = 0;
    if(y3 > Y_MAX) y3 = 0;
    if(x1 < 0) x1 = X_MAX;
    if(x2 < 0) x2 = X_MAX;
    if(x3 < 0) x3 = X_MAX;
    if(y1 < 0) y1 = Y_MAX;
    if(y2 < 0) y2 = Y_MAX;
    if(y3 < 0) y3 = Y_MAX;
}
