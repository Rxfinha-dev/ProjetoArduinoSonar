#include <Servo.h>

// pinos do ultrassom
const int TriggerPin = 11;
const int EchoPin = 12;

// pino do servo
const int motorSignalPin = 9;

// pino do buzzer (D4)
const int buzzerPin = 4;

// posição inicial do servo
const int startAngle = 90;

// limites do servo
const int minimumAngle = 6;
const int maximumAngle = 175;

// velocidade
const int degreesPerCycle = 1;

// distância limite para ativar o buzzer (em cm)
const int detectionThreshold = 20;

// objeto da biblioteca Servo
Servo motor;

void setup(void) 
{
    pinMode(TriggerPin, OUTPUT);
    pinMode(EchoPin, INPUT);
    
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

    // calcula a distância
    int distance = CalculateDistance();

    // mostra no serial
    SerialOutput(currentAngle, distance);

    // verifica se tem objeto perto
    if (distance > 0 && distance < detectionThreshold) {
        // faz um beep de 100 ms em 1000 Hz
        tone(buzzerPin, 1000, 100);
    } else {
        noTone(buzzerPin); // garante que fica desligado quando não há nada
    }

    // atualiza posição do servo
    currentAngle += motorRotateAmount;

    // inverte direção ao chegar no limite
    if(currentAngle <= minimumAngle || currentAngle >= maximumAngle) 
    {
        motorRotateAmount = -motorRotateAmount;
    }
}

int CalculateDistance(void)
{
    // envia pulso de 10us
    digitalWrite(TriggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TriggerPin, LOW);

    // mede tempo de resposta
    long duration = pulseIn(EchoPin, HIGH, 30000); // timeout 30ms (~5m)
    
    if (duration == 0) return -1; // sem leitura válida

    // converte tempo em distância (cm)
    float distance = duration * 0.017F;
    return int(distance);
}

void SerialOutput(const int angle, const int distance)
{
    Serial.println(String(angle) + "," + String(distance));
}
