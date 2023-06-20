#include "game_engine_thread.h"
#include <stdio.h>
#include <math.h>

#include "MyClass.h"
#include <vector>

#define x_0 120
#define y_0 160
#define y_max 320
#define x_max 240
#define PI 3.14159

#define FLAG0 (1 << 0)
#define FLAG1 (1 << 1)

ULONG points = 0;
INT angle = 0;
INT x1 = 20, x2 = 200, y1 = 20, y2 = 280, x3 = 0, y3 = 0;

int counter = 1000;

extern "C" void game_engine_thread_function(void);

void getInputs();
void updateGame();

void updateScoreGraphics();
void updateSpaceshipGraphics();
void updateAsteroidsGraphics();
void calculateSpaceshipAimAngle(UINT coord);

void game_engine_thread_function(void)
{
    ULONG message, status, flag;
    updateScoreGraphics();
    updateSpaceshipGraphics();
    updateAsteroidsGraphics();
    tx_event_flags_set(&event_flags, FLAG1, TX_OR);

    while (1)
    {
        status = tx_event_flags_get(&event_flags, FLAG0, TX_OR_CLEAR, &flag, TX_WAIT_FOREVER);
        if(TX_SUCCESS != status) __BKPT(0);

        // TODO: Lembrar de passar inputs para a classe Space
        getInputs();
        updateGame();

        tx_event_flags_set(&event_flags, FLAG1, TX_OR);
    }
}


void updateScoreGraphics() {
    ULONG update_score_message = 4 << 18 | points;
    ULONG status = tx_queue_send(&graphic_queue, &update_score_message, TX_NO_WAIT);
    if(TX_SUCCESS != status) __BKPT(0);
}

void updateSpaceshipGraphics() {
    ULONG update_spaceship_message = 0 << 18 | angle;
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
    int x = (coord >> 15) & 0x7FFF;
    int y = y_max - (coord & 0x7FFF);
    int diff_x = x-x_0;
    int diff_y = y-y_0;
    double angle_rad = atan2(diff_y, diff_x);
    double angle_deg = angle_rad * 180.0 / PI;
    angle = (360 + 90-(INT)(angle_deg))%360;
}

void getInputs() {
    ULONG message, status;
    status = tx_queue_receive(&control_queue, &message, TX_NO_WAIT);
    while(TX_SUCCESS == status) {
        if(message >> 30 == 0x0) {
            points += 10;
            updateScoreGraphics();
        } else {
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
        message = 1 << 21 | 2 << 18 | x1 << 9 | y1;
        status = tx_queue_send(&graphic_queue, &message, TX_NO_WAIT);
        if(TX_SUCCESS != status) __BKPT(0);
    }
    if(counter > 300) {
        message = 2 << 21 | 3 << 18 | x2 << 9 | y2;
        status = tx_queue_send(&graphic_queue, &message, TX_NO_WAIT);
        if(TX_SUCCESS != status) __BKPT(0);
    }
    if(counter > 0) {
        message = 3 << 21 | 1 << 18 | x3 << 9 | y3;
        status = tx_queue_send(&graphic_queue, &message, TX_NO_WAIT);
        if(TX_SUCCESS != status) __BKPT(0);
    }
    updateSpaceshipGraphics();

    if(x1 > 240) x1 = 0;
    if(x2 > 240) x2 = 0;
    if(x3 > 240) x3 = 0;
    if(y1 > 320) y1 = 0;
    if(y2 > 320) y2 = 0;
    if(y3 > 320) y3 = 0;
    if(x1 < 0) x1 = 240;
    if(x2 < 0) x2 = 240;
    if(x3 < 0) x3 = 240;
    if(y1 < 0) y1 = 320;
    if(y2 < 0) y2 = 320;
    if(y3 < 0) y3 = 320;
}
