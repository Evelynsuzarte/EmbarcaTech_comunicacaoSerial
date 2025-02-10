#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "matriz_leds.h"
#include "inc/ssd1306.h"
#include "inc/font.h"


// Definições de GPIO
#define BOTAO_A 5
#define BOTAO_B 6
#define LED_R 11
#define LED_G 12
#define LED_B 13
#define MATRIZ 7
#define I2C_PORT i2c1
#define DISPLAY_SDA 14
#define DISPLAY_SCL 15
#define I2C_ADDRESS 0x3C



volatile bool botaoA_pressionado = false;
volatile bool botaoB_pressionado = false;
volatile int verde = 0;
volatile int azul = 0;
ssd1306_t display;
volatile char c;
volatile char texto_LED;



bool debounce(uint gpio) {
    sleep_ms(50);
    return gpio_get(gpio) == 0;
}


void init_pin(){
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(DISPLAY_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(DISPLAY_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(DISPLAY_SDA);                                        // Pull up the data line
    gpio_pull_up(DISPLAY_SCL);                                        // Pull up the clock line
    ssd1306_init(&display, WIDTH, HEIGHT, false, I2C_ADDRESS, I2C_PORT); // Inicializa o display
    ssd1306_config(&display);                                         // Configura o display
    ssd1306_send_data(&display);                                      // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels LIGADOS.
    ssd1306_fill(&display, true);
    ssd1306_send_data(&display);


    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);

    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);
    
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);

    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

}


void exibir_texto_oled(char c) {
    ssd1306_fill(&display, 0);
    char texto[2] = {c, '\0'};
    ssd1306_draw_string(&display, texto, 0, 0);
    ssd1306_send_data(&display);
}


void callback_botao(uint gpio, uint32_t events) {
    char texto_LED[20];

    if (debounce(BOTAO_A) && gpio == BOTAO_A) {
        botaoA_pressionado = true;
        gpio_put(LED_G, !gpio_get(LED_G));
        verde = !verde;
        printf("LED verde %s\n", verde ? "ligado" : "desligado");
        
        ssd1306_draw_string(&display, "Verde", 0, 0);
        botaoA_pressionado = false;
    }

    if (debounce(BOTAO_B) && gpio == BOTAO_B) {
        botaoB_pressionado = true;
        gpio_put(LED_B, !gpio_get(LED_B));
        azul = !azul;
        printf("LED azul %s\n", azul ? "ligado" : "desligado");

        ssd1306_draw_string(&display, "Azul", 0, 0);
        botaoB_pressionado = false;
    }
}



void uart_callback() {
    if (uart_is_readable(uart0)) {
        c = uart_getc(uart0);
        printf("Recebido: %c\n", c);
    }
}

void init_oled() {
    ssd1306_init(&display, WIDTH, HEIGHT, false, I2C_ADDRESS, I2C_PORT);
    ssd1306_fill(&display, 0);
    ssd1306_draw_string(&display, "Iniciando...", 0, 0);
    ssd1306_send_data(&display);
}





void numero_0(PIO pio, uint sm){
    Matriz_leds_config zero = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(zero, pio, sm);
}

void numero_1(PIO pio, uint sm){
    Matriz_leds_config um = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(um, pio, sm);
}

void numero_2(PIO pio, uint sm){
    Matriz_leds_config dois = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(dois, pio, sm);
}

void numero_3(PIO pio, uint sm){
    Matriz_leds_config tres = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(tres, pio, sm);
}

void numero_4(PIO pio, uint sm){
    Matriz_leds_config quatro = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(quatro, pio, sm);
}

void numero_5(PIO pio, uint sm){
    Matriz_leds_config cinco = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(cinco, pio, sm);
}

void numero_6(PIO pio, uint sm){
    Matriz_leds_config seis = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(seis, pio, sm);
}

void numero_7(PIO pio, uint sm){
    Matriz_leds_config sete = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(sete, pio, sm);
}

void numero_8(PIO pio, uint sm){
    Matriz_leds_config oito = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(oito, pio, sm);
}

void numero_9(PIO pio, uint sm){
    Matriz_leds_config nove = {
        //  Coluna 0          Coluna 1          Coluna 2          Coluna 3          Coluna 4
        // R    G    B       R    G    B       R    G    B       R    G    B       R    G    B
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
        {{0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
    };
    
   imprimir_desenho(nove, pio, sm);
}

void acao(int numero, PIO pio, uint sm){
    switch (numero){
        case 1:
            numero_1(pio, sm);
            break;
        case 2:
            numero_2(pio, sm);
            break;
        case 3:
            numero_3(pio, sm);
            break;
        case 4:
            numero_4(pio, sm);
            break;
        case 5:
            numero_5(pio, sm);
            break;
        case 6:
            numero_6(pio, sm);
            break;
        case 7:
            numero_7(pio, sm);
            break;
        case 8:
            numero_8(pio, sm);
            break;
        case 9:
            numero_9(pio, sm);
            break;
        case 0:
            numero_0(pio, sm);
            break;
        default:
            numero_0(pio, sm);
            break;
    }
}


int main() {
    stdio_init_all();

    PIO pio = pio0;
    uint sm = configurar_matriz(pio);

    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);

    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    
    uart_init(uart0, 115200);
    uart_set_irq_enables(uart0, true, false);
    gpio_set_function(0, GPIO_FUNC_UART); // Pino TX
    gpio_set_function(1, GPIO_FUNC_UART); // Pino RX


    int baudrate = 400*1000; // 400kHz baud rate for i2c communication
    i2c_init(I2C_PORT, baudrate);
    gpio_set_function(DISPLAY_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_SDA);
    gpio_set_function(DISPLAY_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_SCL);

    irq_set_exclusive_handler(UART0_IRQ, uart_callback);
    irq_set_enabled(UART0_IRQ, true);

    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_RISE, true, &callback_botao);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_RISE, true, &callback_botao);

    init_oled();
    
    while (true) {
        exibir_texto_oled('c');
        if (c != '\0') {
            exibir_texto_oled(c);
            if (c >= '0' && c <= '9') {
                acao((c - '0'), pio, sm);
            }
        }
        sleep_ms(100);
        
    }
}
