#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pinos dos botões
const int botao1 = 2;
const int botao2 = 3;
const int botao3 = 4;
const int botao4 = 5;
const int botao5 = 6;
const int botao6 = 7;

// Pinos dos LEDs
const int led1 = 8;
const int led2 = 9;
const int led3 = 10;
const int led4 = 11;

// Pino do potenciômetro
const int potPin = A0;

// Estados dos LEDs
bool led1Estado = false;
bool led2Estado = false;
bool led3Estado = false;
bool led4Estado = false;

// Estados antigos dos botões
bool botao1EstadoAntigo = false;
bool botao2EstadoAntigo = false;
bool botao3EstadoAntigo = false;
bool botao4EstadoAntigo = false;

// LCD e nomes dos patches
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço corrigido para 0x27
String nomes[] = {"limpo", "crunch", "high"};
int indiceNome = 0;

void setup() {
  // Inicialização dos pinos
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
  pinMode(botao3, INPUT_PULLUP);
  pinMode(botao4, INPUT_PULLUP);
  pinMode(botao5, INPUT_PULLUP);
  pinMode(botao6, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Inicializa LCD
  lcd.init();
  lcd.backlight();
  delay(100);

  lcd.setCursor(0, 0);
  lcd.print("Vol: 0%");

  lcd.setCursor(0, 1);
  lcd.print("Nome: ");
  lcd.print(nomes[indiceNome]);

  Serial.begin(9600);
}

void verificarBotao(int botaoPin, bool &ledEstado, int ledPin, bool &botaoEstadoAntigo, String nomeBotao) {
  bool botaoEstadoAtual = digitalRead(botaoPin) == LOW;
  
  if (botaoEstadoAtual && !botaoEstadoAntigo) {
    ledEstado = !ledEstado;
    digitalWrite(ledPin, ledEstado ? HIGH : LOW);
    Serial.println(nomeBotao + " foi pressionado");
  }
  
  botaoEstadoAntigo = botaoEstadoAtual;
}

void atualizarVolumeLCD(int porcentagem) {
  lcd.setCursor(0, 0);
  lcd.print("Vol:");
  if (porcentagem < 10) lcd.print("  "); // Alinha 1 a 2 dígitos
  else if (porcentagem < 100) lcd.print(" "); // Alinha 2 a 3 dígitos
  lcd.print(porcentagem);
  lcd.print("% ");
}

void atualizarNomePatch() {
  lcd.setCursor(0, 1);
  lcd.print("Nome:           ");  // Limpa linha
  lcd.setCursor(6, 1);
  lcd.print(nomes[indiceNome]);
}

void loop() {
  // Atualiza estado dos botões e LEDs
  verificarBotao(botao1, led1Estado, led1, botao1EstadoAntigo, "Botao 1");
  verificarBotao(botao2, led2Estado, led2, botao2EstadoAntigo, "Botao 2");
  verificarBotao(botao3, led3Estado, led3, botao3EstadoAntigo, "Botao 3");
  verificarBotao(botao4, led4Estado, led4, botao4EstadoAntigo, "Botao 4");

  // Leitura do potenciômetro
  int leitura = analogRead(potPin);
  int porcentagem = map(leitura, 0, 1023, 0, 100);
  atualizarVolumeLCD(porcentagem);

  // Alternância entre nomes (botões 5 e 6)
  if (digitalRead(botao5) == LOW) {
    if (indiceNome < 2) {
      indiceNome++;
      atualizarNomePatch();
      Serial.println("Botao 5 foi pressionado");
      delay(200);
    }
  }

  if (digitalRead(botao6) == LOW) {
    if (indiceNome > 0) {
      indiceNome--;
      atualizarNomePatch();
      Serial.println("Botao 6 foi pressionado");
      delay(200);
    }
  }

  delay(50);
}
