#include "hal.h"
#include <iostream>

unsigned int leds_num[8] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
                           GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9,
                           GPIO_PIN_11, GPIO_PIN_12};
unsigned int sw_num[4] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};

bool sw[4] = {false, false, false, false};

void change_signal(int i){
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
    switch(i){
        case 0:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
            break;
    }
}

void read_and_set(bool set){
    for(int i = 0; i<4; i++) {
        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
        if(set){
            HAL_GPIO_WritePin(GPIOD, leds_num[i], state);
        }
        if (state == GPIO_PIN_SET) sw[i] = 1; else sw[i] = 0;
    }
}

void check_button(){
    GPIO_PinState state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);
    if(state == GPIO_PIN_RESET){
        change_signal(1);
        HAL_Delay(500);
        state = GPIO_PIN_SET;
        while(state == GPIO_PIN_SET && sw[0] && !sw[1] && !sw[2] &&sw[3]){
            state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);
            read_and_set(false);
        }
        HAL_Delay(600);
        change_signal(0);
    }
}

void animate_step(int s1, int s2, int s3, int s4){
    HAL_GPIO_WritePin(GPIOD, leds_num[s1], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, leds_num[s2], GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, leds_num[s3], GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, leds_num[s4], GPIO_PIN_SET);
    HAL_Delay(500);
    check_button();
    read_and_set(false);
}

void animate(){
    HAL_GPIO_WritePin(GPIOD, leds_num[0], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, leds_num[3], GPIO_PIN_RESET);
    change_signal(0);
    while(1){
        animate_step(3, 0, 1, 2);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(0, 1, 2, 3);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(1, 2, 3, 4);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(2, 3, 4, 5);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(3, 4, 5, 6);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(4, 5, 6, 7);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(7, 4, 5, 6);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(6, 3, 4, 5);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(5, 2, 3, 4);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
        animate_step(4, 1, 2, 3);
        if(!(sw[0] && !sw[1] && !sw[2] &&sw[3])){
            break;
        }
    }
    for(int i = 0; i < 8; i++){
        HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
    }
    change_signal(2);
}

void check(){
    if(sw[0] && !sw[1] && !sw[2] &&sw[3]){
        animate();
    }
}

int umain()
{
    change_signal(2);
    while(1) {
        read_and_set(true);
        check();
    }
}
