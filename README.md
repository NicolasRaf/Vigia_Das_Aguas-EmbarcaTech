# Vigia das Águas: Sistema para Prevenção de Enchentes

Projeto final desenvolvido para o curso de Análise e Desenvolvimento de Sistemas do Instituto Federal de Educação, Ciência e Tecnologia do Piauí – Campus Teresina Central. O objetivo é implementar um sistema de monitoramento em tempo real do nível da água em rios e outros corpos d'água, contribuindo para a prevenção de enchentes.

[Link para documentação completa](https://docs.google.com/document/d/1IkiMZ2We-Jzd_8WtnYCGIQL5CQdvDJh0wnUJxQ2tYYc/edit?usp=sharing)

## Sumário

- [Objetivos do Projeto](#objetivos-do-projeto)
- [Descrição do Funcionamento](#descrição-do-funcionamento)
  - [Modos de Funcionamento](#modos-de-funcionamento)
  - [Níveis de Alarme](#níveis-de-alarme)
  - [Interfaces e Telas](#interfaces-e-telas)
- [Justificativa](#justificativa)
- [Especificação do Hardware](#especificação-do-hardware)
  - [Componentes Utilizados](#componentes-utilizados)
  - [Pinagem Utilizada](#pinagem-utilizada)
- [Especificação do Firmware](#especificação-do-firmware)
- [Execução do Projeto](#execução-do-projeto)

## Objetivos do Projeto

O projeto tem como objetivo principal desenvolver um sistema que:

- Realize o monitoramento em tempo real do nível da água em rios e corpos d’água;
- Colete dados precisos da distância da água por meio de sensores, permitindo a avaliação dos níveis e a taxa de aumento;
- Emita alertas visuais e sonoros para notificar comunidades ribeirinhas em situações de risco;
- Seja uma solução de baixo custo e fácil implantação, utilizando a placa BitDogLab com Raspberry Pi Pico W e o sensor ultrassônico HC-SR04;
- Tenha um software modular e eficiente, desenvolvido em linguagem C, facilitando a manutenção e a expansão do sistema.

## Descrição do Funcionamento

O sistema é implementado com a placa didática BitDogLab, que integra o microcontrolador Raspberry Pi Pico W, e utiliza o sensor ultrassônico HC-SR04 para medir a distância até a superfície da água. As leituras são realizadas em intervalos configuráveis, e os dados são processados para determinar se o nível da água está em condições normais ou se há risco iminente.

### Modos de Funcionamento

- **Modo Primário (Média das Leituras):**  
  Calcula a média de todas as leituras coletadas em um período, atenuando ruídos e valores atípicos para uma medição mais estável e confiável.

- **Modo Secundário (Última Leitura):**  
  Exibe a leitura mais recente do sensor, oferecendo uma resposta imediata às variações rápidas do nível da água, embora possa ser mais suscetível a ruídos.

### Níveis de Alarme

O sistema define três estados de alarme baseados na distância medida pelo sensor:

- **Estado Seguro:**  
  Distância ≥ 40 cm – LED verde aceso, sem alerta sonoro.
- **Modo de Atenção:**  
  20 cm ≤ Distância < 40 cm – Combinação de LEDs (verde e vermelho) com alarme sonoro de intensidade média.
- **Nível de Emergência:**  
  Distância < 20 cm – LED vermelho aceso e alarme sonoro com notas frequentes e de alta intensidade.

### Interfaces e Telas

- **Tela Principal:**  
  Exibe a distância (média ou última leitura), o tempo até a próxima medição e a taxa de crescimento do nível da água, calculada através de regressão linear com base nas últimas 10 leituras.

- **Tela de Logs:**  
  Mostra as últimas 10 leituras para permitir a análise de tendências e identificação de variações críticas.

- **Tela de Opções:**  
  Permite a configuração do intervalo de leitura (com opções que variam de 1 segundo a 30 minutos) e a troca entre os modos de operação.

## Justificativa

A implementação do Vigia das Águas responde à necessidade crescente de monitoramento dos níveis de água para prevenir enchentes, proteger comunidades ribeirinhas e combater o negacionismo climático. O sistema busca oferecer dados precisos e confiáveis para ações emergenciais e planejamento a longo prazo, considerando a intensificação dos eventos extremos devido ao aquecimento global.

## Especificação do Hardware

### Componentes Utilizados

- **Placa BitDogLab com Raspberry Pi Pico W:**  
  Possui o microcontrolador RP2040 e integra sensores e atuadores, como o display OLED.

- **Sensor Ultrassônico HC-SR04:**  
  Emite pulsos ultrassônicos e mede o tempo de retorno para calcular a distância com precisão de aproximadamente 3 mm, funcionando entre 2 cm e 4 metros.

- **Buzzer:**  
  Dispositivo responsável pela emissão de alertas sonoros em situações de risco.

- **LED RGB:**  
  Fornece feedback visual, indicando os diferentes níveis de alarme através de cores (verde, amarelo e vermelho).

- **Display OLED:**  
  Apresenta informações em tempo real sobre o monitoramento do nível da água.

### Pinagem Utilizada

| Componente           | Pino no Raspberry Pi Pico W (BitDogLab) | Função                                 |
|----------------------|-----------------------------------------|----------------------------------------|
| HC-SR04 Trigger      | GPIO 17                                 | Envio do sinal ultrassônico            |
| HC-SR04 Echo         | GPIO 16                                 | Recepção do sinal ultrassônico         |
| Buzzer               | GPIO 21                                 | Emissão de alertas sonoros             |
| LED Verde            | GPIO 11                                 | Indicação de estado seguro             |
| LED Vermelho         | GPIO 13                                 | Indicação de emergência                |
| Display OLED (SDA)   | GPIO 14                                 | Comunicação I2C – Dados                |
| Display OLED (SCL)   | GPIO 15                                 | Comunicação I2C – Clock                |

## Especificação do Firmware

Desenvolvido em linguagem C, o firmware utiliza o SDK oficial do Raspberry Pi Pico W e a biblioteca pico-ssd1306 para o display OLED. Suas principais funcionalidades incluem:

- **Inicialização dos Periféricos:**  
  Configuração do sensor ultrassônico, display OLED e outros componentes logo após a energização.

- **Leitura e Processamento dos Dados:**  
  Realiza leituras periódicas do sensor e processa os dados para determinar o nível da água e calcular a taxa de crescimento usando a seguinte equação de regressão linear:

  m = (n *Σ(tᵢ * Lᵢ) − (Σtᵢ)*(ΣLᵢ)) / (n* Σ(tᵢ²) − (Σtᵢ)²)

  Onde:  
  - m é a taxa de crescimento,  
  - n é o número de leituras (10),  
  - tᵢ é o tempo da i-ésima leitura, e  
  - Lᵢ é o nível da água na i-ésima leitura.

- **Interface de Usuário:**  
  Permite a interação através dos botões A (troca de modos) e B (exibição dos logs).

- **Controle de Alertas:**  
  Gerencia os estados de alarme (seguro, atenção, emergência) e aciona LEDs e buzzer conforme os níveis medidos.

## Execução do Projeto

A execução do Vigia das Águas seguiu uma abordagem modular, com as seguintes etapas:

1. **Pesquisa e Levantamento Tecnológico:**  
   Análise de soluções existentes e viabilidade do uso de sensores ultrassônicos para monitoramento de água.

2. **Escolha e Configuração do Hardware:**  
   Seleção da placa BitDogLab, Raspberry Pi Pico W, sensor HC-SR04 e demais componentes, considerando custo, eficiência e compatibilidade.

3. **Desenvolvimento do Software:**  
   Definição das funcionalidades necessárias, como leitura dos sensores, processamento dos dados, interface com o usuário e emissão de alertas.

4. **Estruturação e Modularização do Código:**  
   Implementação em linguagem C, utilizando o SDK oficial e bibliotecas específicas para integração com o hardware, facilitando a manutenção e futuras expansões.

5. **Testes e Validação:**  
   Realização de testes em ambiente controlado para garantir a precisão das medições e a eficácia do sistema de alertas, confirmando a utilidade do cálculo por regressão linear.
