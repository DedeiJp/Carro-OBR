/*
*** Codigo para o robô autonomo de competição Walter14 ***

  Autores:
    - Dyego Matheus Vigilato de Souza
    - João Pedro Alvim Vigilato
    - Nicolle Emilly Dias

  Versão 1.1 | Data: 7/11/2020
    Adicionado sugestões do avaliador da OBR
      - Correção de indentação
      - Adição de mais domumentação
*/

// Essa biblioteca facilita a utilização de multiplos ultrasons, mais documentação em: https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib
#include <HCSR04.h>
/*
  Inicio
  1- Inicia motor desligado
  2- ler a distância
  3- ler a linha
  4- Se a distância for maior que 10 cm e linha estiver presente
    4.1 ligar motor, seguir em frente
    4.2 repetir passo 2
  5- Senão
    5.1 chamar funçao Desviar  ();
    5.2 ler a linha;
    5.3 Se a linha estiver presente;
      5.3.1 virar para esquerda (1 s);
      5.3.2 repetir passo 2;
  Fim

      Função Desviar
   Inicio
   1- desligar motor, parar o carro
   2- ler a distância
   3- Se a distância for maior que 10
     3.1- Va para tras (1 s);
     3.2- vira para esquerda (1 s);
     3.3 repetir o passo 2;
   4- Senão
     4.1 vira para esquerda (0.5);
     4.2 va para frente (2 s);
     4.3 vira para direita (1 s);
     4.4 va para frente (2 s);
     4.5 retornar para oprograma principal
   Fim

     Função Frente (Velocidade final, Tempo até velocidade máxima)
  Inicio
  1- Mudar direção para frente;
  2- Acelerar até velocidade final;
  Fim

     Função FrenteM (Velocidade desejada)
  Inicio
  Definir velocidade dos motores para velocidade desejada;
  Fim

     Função Parar (Velocidade atual do carrinho, Tempo até velocidade máxima)
  Inicio
  1- Se o tempo de delay > 0;
    1.1 desacelerar o carrinho utilizando o tempo de delay;
  2- Se não;
    2.1 Parar o carrinho;
  Fim

     Função Tras (Velocidade final, Tempo até velocidade máxima)
  Inicio
  1- Mudar direção para tras;
  2- Acelerar até velocidade final;
  Fim

     Função VirarM (Velocidade atual do carrinho, Tempo, intencidade da curva, direção da curva)
  Inicio
  1- Se direção = 0 (Esquerda)
    1.1 Faz a esteira esquerda ir um pouco para mais devagar do que a direita;
    1.2 Anda pelo tempo;
    1.3 Retorna ambas as esterias a velocidade normal;
    1.4 Fim da Função;
  2- Se direção = 1 (Direita)
    1.1 Faz a esteira direita ir um pouco para mais devagar do que a esquerda;
    1.2 Anda pelo tempo;
    1.3 Retorna ambas as esterias a velocidade normal;
    1.4 Fim da Função;
  Fim

     Função ChecDist ()
  Inicio
  1- Epera por 500ms
  2- Lê a distancia do ultrason
  3- Se a distancia for maior que 20;
    3.1- Finaliza a função e retorna "0";
  4- Se não;
    4.1- Finaliza a função e retorna "1";
  Fim
*/

// Pinos ---------------------------------------


// Esteiras =================================
#define MS1  3  // PWM - Velocidade do motor 1  AMARELO
#define MD11 22 // Motor 1, direção 1           MARROM
#define MD12 23 // Motor 1, direção 2           BRANCO
#define MS2  2  // PWM - Velocidade do motor 2  VERDE
#define MD21 24 // Motor 1, direção 1           VERMELHO
#define MD22 25 // Motor 2, direção 2           CINZA
// ==========================================

#define Debuzer 32

// Ultrasons ================================ // Revisar
#define UT 26 // Triger do Ultrason 1 LARANJA
#define UE1 27 // Echo do Ultrason 1 PRETO
//===========================================

// LDR's ==================================== // Queria usar #define aqui, mas estava tendo um erro que dizia que precissava de um ")" antes de A1
int LDRE = A2; // LDR esquerdo VERDE
int LDRD = A1; // LDR direito BRANCO
// ========================================== // Lembrar que: Quanto mais luz, menor o valor
// ---------------------------------------------


// Variaveis -----------------------------------
// Ultrasons ================================
int distance1; // variavel distance para o ultrason 1
int velGlobal; // Velocidade do carrinho
// ==========================================

// LDR's ==================================== -Isso é aluminosidade dos LDR's. Aqui em casa o meu máximo foi de 920 e mínimo de 200. Para tranformar o maximo mara 255 apenas multiplicar por 0.27
int LE; // LDR Esquerdo
int LD; // LDR Direito
// ==========================================

// Outros ===================================
int MODE; // Modo de linha(1) ou resgate(2) // Temporario
// ==========================================
// ---------------------------------------------


// Bibliotecas ==============================
HCSR04 hc(UT, new int[1] {UE1}, 1); // Revisar // Isso requer a biblioteca HCSR04, basicamente permite o uso de multiplos ultasons, // "UT" é um triguer unico para todos os ultrasons e o "UE1" é o echo para o ultason 1.
// ==========================================


// Funções -------------------------------------
// Movimento ================================
int Frente();
int MudarVel();
int Tras();
void Parar();
void VirarM();
void VirarP();
int LerDist();
void DB();
// ==========================================
// ---------------------------------------------

void setup() {

  Serial.begin(9600);

  // Definições de IN/OUT ---------

  // Esteiras ==============
  pinMode(MS1,  OUTPUT);
  pinMode(MS2,  OUTPUT);
  pinMode(MD11, OUTPUT);
  pinMode(MD12, OUTPUT);
  pinMode(MD21, OUTPUT);
  pinMode(MD22, OUTPUT);
  // =======================

  pinMode(Debuzer, OUTPUT);

  // Ultrasons ============= // A biblioteca´pode mudar
  pinMode(UT,  OUTPUT); // Revisar
  pinMode(UE1,  INPUT); // Revisar
  // =======================

  // LDR'S =================
  pinMode(LDRD, INPUT);
  pinMode(LDRE, INPUT);
  // -------------------------------

  // Definiçoes de Inicialização ----------

  // Esteiras ======================== // Isso define a direção dos motores em tem que estar HIGH e o outro LOW
  digitalWrite(MD11, HIGH);
  digitalWrite(MD12,  LOW);
  digitalWrite(MD21, HIGH);
  digitalWrite(MD22,  LOW);
  // =================================

  // Outros ==========================
  MODE = 1;
  distance1 = 20;
  // =================================

}

void loop() {

  //   int  Valor_Sensor1 = analogRead(LDRE);
  // int  Valor_Sensor2 = analogRead(LDRD);
  // Serial.print("Sensor 1: \n");
  // Serial.print(Valor_Sensor1);
  //
  // Serial.print("Sensor 2: \n");
  // Serial.print(Valor_Sensor2);

  LD = analogRead(LDRD);
  LE = analogRead(LDRE);

  if ((LE > 250) && (LD > 250)) {
    analogWrite(MS1, 80);
    analogWrite(MS2, 80);
  }

  if ((LE < 500) && (LD > 500)) {
    analogWrite(MS1, 0);
    analogWrite(MS2, 100);
  }

  if ((LE > 500) && (LD < 500)) {
    analogWrite(MS1, 100);
    analogWrite(MS2, 0);
  }

  distance1 = LerDist(0);
  if (distance1 < 10) {
    Desviar();
  }

  Serial.println(distance1);

}

// Lembrar que todos os valores de tempo são em milisegundos

//Essa função muda a direção do carrinho para frente, o acelera para a velocidade escolhida no tempo escolhido
int Frente(int vel, int tempo) {

  if (velGlobal == vel) {
    return vel;
  }

  if (velGlobal == 0) {

    digitalWrite(MD11, HIGH);
    digitalWrite(MD12,  LOW);
    digitalWrite(MD21, HIGH);
    digitalWrite(MD22,  LOW);

    int tempo2 = tempo / vel;
    for (int i = 0; i < vel; i++) {
      analogWrite(MS1, i);
      analogWrite(MS2, i);
      delay(tempo2);
    }
    velGlobal = vel;
    return vel;
  }

  return vel;

}

//Essa função muda a velocidade do carrinho para a desejada, seja essa maior ou menor, no tempo escolhido
int MudarVel(int vel, int tempo) {
  int tempo2 = tempo / (velGlobal - vel);

  if (velGlobal < vel) { // Se a velocidade do carrinho for menor que a velocidade desejada
    for (int i = velGlobal; i < vel; i++) {
      analogWrite(MS1, i);
      analogWrite(MS2, i);
      delay(abs(tempo2));
    }
    velGlobal = vel;
    return vel;
  }

  if (velGlobal > vel) { // Se a velocidade do carrinho for maior que a velocidade desejada
    for (int i = velGlobal; i > vel; i--) {
      analogWrite(MS1, i);
      analogWrite(MS2, i);
      delay(abs(tempo2));
    }
    velGlobal = vel;
    return vel;
  }
}

//Essa função muda a direção do carrinho para trâs, o acelera para a velocidade escolhida no tempo escolhido
int Tras(int vel, int tempo) {
  digitalWrite(MD11,  LOW);
  digitalWrite(MD12, HIGH);
  digitalWrite(MD21,  LOW);
  digitalWrite(MD22, HIGH);

  int tempo2 = tempo / vel;
  for (int i = 0; i < vel; i++) {
    analogWrite(MS1, i);
    analogWrite(MS2, i);
    delay(tempo2);
  }
  return vel;
}

//Essa função para o carrinho instantaneamente ou o desacelera até para-lo no tempo escolhido
void Parar(int tempo) {
  if (tempo > 0) {
    int tempo2 = tempo / velGlobal;
    for (int i = velGlobal; i > 0; i--) {
      analogWrite(MS1, i);
      analogWrite(MS2, i);
      delay(tempo2);
    }
    velGlobal = 0;
  }
  else {
    analogWrite(MS1, 0);
    analogWrite(MS2, 0);
    velGlobal = 0;
  }
}

//Essa função serve para virar o carrinho enquanto o mesmo está em movimento
void VirarM(int tempo, int inten, int dire) { //inten provavelmente vai ser um numero fixo, por enquanto é uma variavel por motivos de teste, o M indica que o carro entá em movimento
  if (dire == 0) { // Esquerda
    analogWrite(MS1, velGlobal + inten);
    analogWrite(MS2, velGlobal - inten);
    delay(tempo);
    analogWrite(MS1, velGlobal);
    analogWrite(MS2, velGlobal);
    return;
  }
  if (dire == 1) { // Direita
    analogWrite(MS1, velGlobal - inten);
    analogWrite(MS2, velGlobal + inten);
    delay(tempo);
    analogWrite(MS1, velGlobal);
    analogWrite(MS2, velGlobal);
    return;
  }
}

//Essa função serve para virar o carrinho enquanto o mesmo está parado
void VirarP(int tempo, int inten, int dire) {
  if (dire == 0) { // Esquerda
    digitalWrite(MD11, HIGH);
    digitalWrite(MD12, LOW);
    digitalWrite(MD21, LOW);
    digitalWrite(MD22, HIGH);
    int tempo2 = 1000 / inten;
    for (int i = 0; i < inten; i++) {
      analogWrite(MS1, i);
      analogWrite(MS2, i);
      delay(tempo2);
    }
    delay(tempo);
    analogWrite(MS1, 0);
    analogWrite(MS2, 0);
    return;
  }
  if (dire == 1) { // Direita
    digitalWrite(MD11, LOW);
    digitalWrite(MD12, HIGH);
    digitalWrite(MD21, HIGH);
    digitalWrite(MD22, LOW);
    int tempo2 = 1000 / inten;
    for (int i = 0; i < inten; i++) {
      analogWrite(MS1, i);
      analogWrite(MS2, i);
      delay(tempo2);
    }
    delay(tempo);
    analogWrite(MS1, 0);
    analogWrite(MS2, 0);
    return;
  }
}

//Essa função serve para ler a distancia de um sensor ultrassonico escolhido e retornar o valor lido
int LerDist(int NUS) {
  int distancia = hc.dist(NUS);
  return distancia;
}

//Essa função serve para acionar um buzzer uma escolhida quantidade de vezes com o delay de 200ms entre cada acionamento
void DB(int NUN) {

  for (int i = NUN; i > 0; i--) {

    digitalWrite(Debuzer, HIGH);
    delay(200);
    digitalWrite(Debuzer, LOW);
    delay(200);

  }

}

//Essa função chamas outras funções em sequencia para efetuar o desvio de um obstaculo
void Desviar() {
  Parar(0);
  delay(100);
  VirarP(800, 100, 0);
  Frente(255, 1000);
  Parar(0);
  VirarP(800, 100, 1);
  Frente(100, 10);
  delay(800);
  Parar(0);
  VirarP(600, 100, 1);
  Frente(100, 10);
  delay(50);
  DB(1);
  return;
}
