# Projeto de Temporização com Raspberry Pi Pico W
Este projeto implementa um sistema de temporização para controle de LEDs utilizando a Raspberry Pi Pico W. O sistema é acionado por um botão (pushbutton) e utiliza a função add_alarm_in_ms() do Pico SDK para criar um temporizador one-shot que gerencia a sequência de acionamento dos LEDs.

## Funcionamento do Projeto
### Descrição Geral
O projeto consiste em um circuito com três LEDs e um botão. Quando o botão é pressionado, os três LEDs são ligados simultaneamente. Após um intervalo de 3 segundos, dois LEDs são desligados, deixando apenas dois acesos. Após mais 3 segundos, apenas um LED permanece aceso. Finalmente, após mais 3 segundos, todos os LEDs são desligados. O botão só pode iniciar uma nova sequência quando a anterior for concluída.

## Componentes Utilizados
Raspberry Pi Pico W: Microcontrolador principal.
LEDs: Três LEDs conectados aos GPIOs 11, 12 e 13 (LED RGB).
Botão: Um pushbutton conectado ao GPIO 5.
Resistores: Resistores adequados para limitar a corrente dos LEDs e do botão.

## Funcionalidades Implementadas
### Acionamento dos LEDs:
Ao pressionar o botão, os três LEDs são ligados.
Após 3 segundos, dois LEDs são desligados.
Após mais 3 segundos, apenas um LED permanece aceso.
Após mais 3 segundos, todos os LEDs são desligados.

### Temporizador One-Shot:
A função add_alarm_in_ms() é usada para criar um temporizador que dispara uma única vez após 3 segundos.
A cada disparo do temporizador, o estado dos LEDs é atualizado.

### Debounce do Botão:
Uma rotina de software debounce é implementada para evitar leituras falsas do botão causadas por bouncing.

### Bloqueio de Acionamento:
O botão só pode iniciar uma nova sequência quando a anterior for concluída (todos os LEDs desligados).

#### Implementação do Temporizador One-Shot
A função add_alarm_in_ms() é usada para criar um temporizador one-shot. Essa função agenda uma chamada de callback após um intervalo de tempo especificado (em milissegundos). No projeto, o intervalo é de 3 segundos (3000 ms).

#### Função timer_callback()
A função timer_callback() é chamada a cada 3 segundos e é responsável por:
Atualizar o estado dos LEDs com base no estado atual (STATE_ALL_ON, STATE_TWO_ON, STATE_ONE_ON, STATE_ALL_OFF).
Agendar o próximo alarme (se necessário) usando add_alarm_in_ms().
Finalizar a temporização quando todos os LEDs estiverem desligados.

## Estrutura do Código
O código está organizado da seguinte forma:

### Configuração dos GPIOs:
Os pinos dos LEDs e do botão são configurados como saídas e entrada, respectivamente.
O botão é configurado com um resistor pull-up.

### Atualização dos LEDs:
A função update_leds() controla o estado dos LEDs com base no estado atual.

### Debounce do Botão:
A função debounce_button() verifica se o botão foi pressionado de forma estável.

### Loop Principal:
O loop principal verifica o estado do botão e inicia a temporização quando necessário.

## Como Executar
Conecte os LEDs aos GPIOs 11, 12 e 13.
Conecte o botão ao GPIO 5.
Compile e carregue o código na Raspberry Pi Pico W.
Pressione o botão para iniciar a sequência de acionamento dos LEDs.
