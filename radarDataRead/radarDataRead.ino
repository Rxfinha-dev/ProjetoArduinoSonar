#include <Servo.h>

// set output pins
const int TriggerPin = 11;
const int EchoPin = 12;
const int motorSignalPin = 9;
const int buzzerPin = 4; // buzzer no pino D4

// starting location
const int startAngle = 90;

// rotation limits
const int minimumAngle = 6;
const int maximumAngle = 175;

// speed
const int degreesPerCycle = 1;

// distância limite para ativar o buzzer (em cm)
const int detectionThreshold = 20;

// Library class instance
Servo motor;

void setup(void) 
{
    pinMode(TriggerPin, OUTPUT);
    pinMode(EchoPin, INPUT);
    pinMode(buzzerPin, OUTPUT); // configura buzzer como saída

    motor.attach(motorSignalPin);
    Serial.begin(9600);
}

void loop(void)
{
    static int currentAngle = startAngle;
    static int motorRotateAmount = degreesPerCycle;

    // move motor
    motor.write(currentAngle);
    delay(10);

    // calcula distância
    int distance = CalculateDistance();

    // envia dados pela serial
    SerialOutput(currentAngle, distance);

    // verifica se há objeto próximo e faz beep
    if (distance > 0 && distance <= detectionThreshold) {
        beep(); 
    } else {
        noTone(buzzerPin); // garante que o buzzer fique desligado se não tiver nada perto
    }

    // atualiza ângulo do motor
    currentAngle += motorRotateAmount;

    // se atingir os limites, inverte a direção
    if (currentAngle <= minimumAngle || currentAngle >= maximumAngle) {
        motorRotateAmount = -motorRotateAmount;
    }
}

int CalculateDistance(void)
{
    // trigger do sensor ultrassônico
    digitalWrite(TriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TriggerPin, LOW);
    
    long duration = pulseIn(EchoPin, HIGH);
    // converter tempo para distância em cm
    float distance = duration * 0.017F;
    return int(distance);
}

void SerialOutput(const int angle, const int distance)
{
    // envia ângulo e distância pela serial
    Serial.println(String(angle) + "," + String(distance));
}

// função para emitir beep curto no buzzer
void beep()
{
    tone(buzzerPin, 1000); // 1000 Hz -> frequência do beep
    delay(100);            // duração do beep (ms)
    noTone(buzzerPin);     // desliga o buzzer
}
