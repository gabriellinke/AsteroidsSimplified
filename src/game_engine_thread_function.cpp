#include "game_engine_thread.h"
#include "constants.h"

#include <stdio.h>
#include <math.h>
#include "game_logic/Space.h"
#include <vector>

extern "C" void game_engine_thread_function(void);

int calculateSpaceshipAimAngle(UINT coord);
std::vector<int> getInputs();

int recordScore;

void game_engine_thread_function(void)
{
    ULONG status, flag;
    Space space;
    std::vector<int> objects;
    std::vector<int> inputs;

    tx_event_flags_set(&event_flags, FLAG1, TX_OR);

    while (1)
    {
        status = tx_event_flags_get(&event_flags, FLAG0, TX_OR_CLEAR, &flag, TX_WAIT_FOREVER);
        if(TX_SUCCESS != status) __BKPT(0);

        // Pega inputs e passa para o espaço
        inputs = getInputs();
        space.setInputs(inputs);

        // Atualiza o próximo frame do jogo
        objects = space.update();

        // Verifica se o jogo acabou
        if(space.getGameOver()) {
            // Verifica quantos pontos fez, se precisar atualiza o recorde
            // Manda para a tela inicial
            if(space.getScore() > recordScore)
                recordScore = space.getScore();
            tx_event_flags_set(&event_flags, FLAG2, TX_OR); // Seta flag para voltar para o Menu
        }

        // Envia objetos para a fila para que sejam desenhados
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            int message = *it;
            status = tx_queue_send(&graphic_queue, &message, TX_NO_WAIT);
            if(TX_SUCCESS != status) __BKPT(0);
        }

        tx_event_flags_set(&event_flags, FLAG1, TX_OR);
    }
}

int calculateSpaceshipAimAngle(UINT coord) {
    // Extract the values from the bits
    int x = ((coord & MASK_COORDS_X) >> SHIFT_COORDS_X);
    int y = Y_MAX - ((coord & MASK_COORDS_Y) >> SHIFT_COORDS_Y); // Invert Y axis to calculate angle properly
    int diff_x = x-X_MAX/2;
    int diff_y = y-Y_MAX/2;
    double angle_rad = atan2(diff_y, diff_x);
    double angle_deg = angle_rad * 180.0 / PI;
    return (360 + 90-(INT)(angle_deg))%360;
}

std::vector<int> getInputs() {
    std::vector<int> inputs;
    ULONG message, status;

    status = tx_queue_receive(&control_queue, &message, TX_NO_WAIT);
    while(TX_SUCCESS == status) {

        if(message >> SHIFT_INPUT == SHOT_MESSAGE) {
            inputs.push_back(message);
        } else {
            int angle = calculateSpaceshipAimAngle(message);
            message = TOUCH_MESSAGE << SHIFT_INPUT | angle;
            inputs.push_back(message);
        }
        status = tx_queue_receive(&control_queue, &message, TX_NO_WAIT);
    }

    if(TX_QUEUE_EMPTY != status) __BKPT(0);

    return inputs;
}
