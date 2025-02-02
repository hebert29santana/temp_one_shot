/*
    Hebert Costa Vaz Santana
    TIC370101235
    Grupo 7

    Temporizador One Shot
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Definição dos pinos
#define BUTTON_GPIO 5
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13

// Estados dos LEDs
typedef enum {
    STATE_ALL_ON,
    STATE_TWO_ON,
    STATE_ONE_ON,
    STATE_ALL_OFF
} LedState;

// Variáveis globais
volatile LedState current_state = STATE_ALL_OFF;
volatile bool button_pressed = false;
volatile bool timer_running = false;

// Função para configurar os GPIOs
void setup_gpio() {
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_init(LED_RED);
    gpio_init(BUTTON_GPIO);

    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(BUTTON_GPIO, GPIO_IN);

    gpio_pull_up(BUTTON_GPIO); // Habilita pull-up no botão
}

// Função para atualizar os LEDs com base no estado atual
void update_leds() {
    switch (current_state) {
        case STATE_ALL_ON:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_BLUE, 1);
            gpio_put(LED_RED, 1);
            break;
        case STATE_TWO_ON:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_BLUE, 1);
            gpio_put(LED_RED, 0);
            break;
        case STATE_ONE_ON:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_BLUE, 0);
            gpio_put(LED_RED, 0);
            break;
        case STATE_ALL_OFF:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_BLUE, 0);
            gpio_put(LED_RED, 0);
            break;
    }
}

// Callback do temporizador
int64_t timer_callback(alarm_id_t id, void *user_data) {
    if (current_state == STATE_ALL_ON) {
        current_state = STATE_TWO_ON;
    } else if (current_state == STATE_TWO_ON) {
        current_state = STATE_ONE_ON;
    } else if (current_state == STATE_ONE_ON) {
        current_state = STATE_ALL_OFF;
        timer_running = false; // Finaliza a temporização
    }

    update_leds(); // Atualiza os LEDs

    if (current_state != STATE_ALL_OFF) {
        // Agenda o próximo alarme
        add_alarm_in_ms(3000, timer_callback, NULL, false);
    }

    return 0;
}

// Função de debounce para o botão
bool debounce_button() {
    static uint32_t last_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time - last_time > 50) { // Debounce de 50ms
        last_time = current_time;
        return !gpio_get(BUTTON_GPIO); // Retorna true se o botão estiver pressionado
    }
    return false;
}

// Loop principal
int main() {
    stdio_init_all();
    setup_gpio();

    while (1) {
        if (debounce_button() && !timer_running) {
            button_pressed = true;
            current_state = STATE_ALL_ON;
            timer_running = true;
            update_leds();
            add_alarm_in_ms(3000, timer_callback, NULL, false); // Inicia a temporização
        }

        sleep_ms(10); // Pequeno delay para evitar uso excessivo da CPU
    }

    return 0;
}