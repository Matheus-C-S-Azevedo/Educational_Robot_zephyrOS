Educational Robot - Zephyr RTOS 🤖

Este repositório contém o firmware para um robô educativo desenvolvido utilizando o Zephyr Real-Time Operating System (RTOS). O projeto demonstra a implementação de controlo de movimento, leitura de sensores e gestão de tarefas em tempo real para sistemas embebidos.

🚀 Visão Geral

O projeto utiliza as capacidades de multithreading e a abstração de hardware do Zephyr para controlar um robô móvel. A arquitetura foi pensada para ser modular, facilitando a adição de novos sensores ou comportamentos (como seguimento de linha ou desvio de obstáculos).

Principais Funcionalidades

Controlo de Motores: Implementação de sinais PWM para ajuste de velocidade e direção.

Integração de Sensores: Leitura de sensores ultrassónicos (HC-SR04) e infravermelhos.

Multithreading: Gestão de threads independentes para a lógica de controlo e para a monitorização de segurança.

Portabilidade: Utilização de Devicetree Overlays para fácil adaptação a diferentes placas (ESP32, STM32, etc.).

🛠️ Tecnologias e Ferramentas

Linguagem: C

RTOS: Zephyr Project

Build System: CMake & West

Hardware Alvo: ESP32 (configurável via Devicetree)

📋 Pré-requisitos

Para compilar este projeto, é necessário configurar o ambiente de desenvolvimento do Zephyr:

Instale o Zephyr SDK.

Instale a ferramenta West.

Configure as dependências do sistema conforme a documentação oficial do Zephyr.

🔧 Como Compilar e Carregar

1. Clonar o Repositório

git clone [https://github.com/Matheus-C-S-Azevedo/Educational_Robot_zephyrOS.git](https://github.com/Matheus-C-S-Azevedo/Educational_Robot_zephyrOS.git)
cd Educational_Robot_zephyrOS


2. Inicializar o Workspace

Se estiver a usar este repositório como um módulo isolado:

west init -l .
west update


3. Compilar para a sua Placa

Por exemplo, para a ESP32 DevKitC V4:

west build -b esp32_devkitc_v4


4. Flash (Carregar no Hardware)

west flash


📂 Estrutura do Projeto

├── src/
│   └── main.c           # Lógica principal do firmware
├── boards/
│   └── esp32.overlay    # Definição de pinos (PWM, Sensores)
├── prj.conf             # Configurações do Kernel (Kconfig)
├── CMakeLists.txt       # Script de construção do projeto
└── README.md            # Documentação


⚙️ Configurações (Kconfig)

As funcionalidades do kernel utilizadas estão definidas no ficheiro prj.conf. Algumas das flags essenciais incluem:

CONFIG_GPIO=y - Ativa suporte a entrada/saída digital.

CONFIG_PWM=y - Ativa controlo de motores.

CONFIG_SENSOR=y - Ativa a stack de sensores.

🤝 Contribuições

Sinta-se à vontade para abrir uma Issue ou submeter um Pull Request para melhorias:

Faça um Fork do projeto.

Crie uma Branch (git checkout -b feature/MinhaMelhoria).

Faça Commit das alterações.

Faça Push para a branch.

Abra um Pull Request.

Desenvolvido por Matheus Azevedo
