# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
include(pico_sdk_import.cmake)

project(comunicacaoserial C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Define a placa utilizada
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Adicionar arquivos-fonte do projeto
add_executable(comunicacaoserial 
    comunicacaoserial.c 
    matriz_leds.c
    inc/ssd1306.c  # Caminho correto para o ssd1306.c
)

# Definir nome e versão do programa
pico_set_program_name(comunicacaoserial "comunicacaoserial")
pico_set_program_version(comunicacaoserial "0.1")

# Habilitar saída via UART e USB
pico_enable_stdio_uart(comunicacaoserial 1)
pico_enable_stdio_usb(comunicacaoserial 1)

# Adicionar bibliotecas necessárias
target_link_libraries(comunicacaoserial
    pico_stdlib
    hardware_timer
    hardware_pio
    hardware_clocks
    hardware_pwm
    hardware_i2c
    hardware_uart
    pico_multicore # Se estiver usando múltiplos núcleos no RP2040
)

# Incluir diretórios de cabeçalhos
target_include_directories(comunicacaoserial PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/inc  # Adicionar o diretório 'inc' para os arquivos de cabeçalho
)

# Gerar arquivos de cabeçalho a partir dos arquivos PIO
pico_generate_pio_header(comunicacaoserial ${CMAKE_CURRENT_LIST_DIR}/pio_matrix.pio)

# Adicionar saída extra (como UF2 e binários)
pico_add_extra_outputs(comunicacaoserial)
