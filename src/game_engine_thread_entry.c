#include "game_engine_thread.h"
#include <stdio.h>
#include <math.h>

#define x_0 120
#define y_0 160
#define y_max 320
#define x_max 240
#define PI 3.14159

#define FLAG0 (1 << 0)
#define FLAG1 (1 << 1)

ULONG points = 0;
INT angle = 0;
INT x1 = 20, x2 = 200, y1 = 20, y2 = 280;

INT x3, y3, dx, dy, bullet = 0;


void updateScoreGraphics();
void updateSpaceshipGraphics();
void updateAsteroidsGraphics();
void calculateSpaceshipAimAngle(UINT coord);
void getInputs();
void updateGame();
void shotBullet();

/* Game Engine Thread entry function */
void game_engine_thread_entry(void)
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

}

void calculateSpaceshipAimAngle(UINT coord) {
    // Extract the values from the bits
    int x = (coord >> 15) & 0x7FFF;
    int y = y_max - (coord & 0x7FFF);
    int dx = x-x_0;
    int dy = y-y_0;
    double angle_rad = atan2(dy, dx);
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
            shotBullet();
        } else {
            calculateSpaceshipAimAngle(message);
            updateSpaceshipGraphics();
        }
        status = tx_queue_receive(&control_queue, &message, TX_NO_WAIT);
    }

    if(TX_QUEUE_EMPTY != status) __BKPT(0);
}

void updateGame() {
    x1+=1;
    y1+=1;
    x2-=1;
    y2-=1;

    if(bullet) {
        x3 += dx;
        y3 += dy;
    }

    ULONG message, status;
    ULONG test = 2 << 18 | x1 << 9 | y1;
    status = tx_queue_send(&graphic_queue, &test, TX_NO_WAIT);
    if(TX_SUCCESS != status) __BKPT(0);
    test = 3 << 18 | x2 << 9 | y2;
    status = tx_queue_send(&graphic_queue, &test, TX_NO_WAIT);
    if(TX_SUCCESS != status) __BKPT(0);
    test = 1 << 18 | x3 << 9 | y3;
    status = tx_queue_send(&graphic_queue, &test, TX_NO_WAIT);
    if(TX_SUCCESS != status) __BKPT(0);
}

void shotBullet() {
    bullet = 1;
    x3 = x_0 +12;
    y3 = y_0 - 12;
    dx = 0;
    dy = -4;
}
