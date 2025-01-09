#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>
#include <ArduinoSTL.h>
#include <vector>

#define LED_RED_PIN 3
#define LED_blue_PIN 5
#define LED_green_PIN 4
#define BUTON_1 8
#define JOYSTICK1_VRX A0
#define JOYSTICK2_VRY A3
#define BUZZERPIN 9
#define DEBOUNCE_DELAY 50

int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
int buttonState = 0;
const int START_BUTTON = BUTON_1;

LiquidCrystal_I2C lcd(0x27, 16, 2);
LedControl lc = LedControl(11, 13, 10, 1);

void matriceSetPixel(int x, int y, int value)
{

    if (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        lc.setLed(0, x, y, value);
    }
}

void setLed(int red, int green, int blue)
{
    analogWrite(LED_RED_PIN, red);
    analogWrite(LED_green_PIN, green);
    analogWrite(LED_blue_PIN, blue);
}

class Pair
{
public:
    int x;
    int y;

    Pair() : x(0), y(0) {}

    Pair(int x, int y) : x(x), y(y) {}

    bool operator==(const Pair &other) const
    {
        return x == other.x && y == other.y;
    }
};

class Bullet
{
private:
    Pair coord;

public:
    Bullet(Pair coord) : coord(coord) {}

    Pair getCoord() const { return coord; }

    void move()
    {
        matriceSetPixel(coord.x, coord.y, 0);
        coord.x--;
        matriceSetPixel(coord.x, coord.y, 1);
    }
};

class Meteorit
{
private:
    Pair coord;

public:
    Meteorit(Pair coord) : coord(coord) {}

    Pair getCoord() const { return coord; }

    void display()
    {
        matriceSetPixel(coord.x, coord.y, 1);
    }

    void fall()
    {
        matriceSetPixel(coord.x, coord.y, 0);
        coord.x++;
        matriceSetPixel(coord.x, coord.y, 1);
    }

    void destroy()
    {
        matriceSetPixel(coord.x, coord.y, 0);
    }
};

class Nava
{
private:
    Pair left;
    Pair right;
    Pair up;
    Pair down;

public:
    Nava(Pair left, Pair right, Pair up, Pair down)
        : left(left), right(right), up(up), down(down) {}

    Pair getLeft() const { return left; }
    Pair getRight() const { return right; }
    Pair getUp() const { return up; }
    Pair getDown() const { return down; }

    void display()
    {
        matriceSetPixel(left.x, left.y, 1);
        matriceSetPixel(right.x, right.y, 1);
        matriceSetPixel(up.x, up.y, 1);
        matriceSetPixel(down.x, down.y, 1);
    }

    void moveLeft()
    {

        matriceSetPixel(left.x, left.y, 0);
        matriceSetPixel(right.x, right.y, 0);
        matriceSetPixel(up.x, up.y, 0);
        matriceSetPixel(down.x, down.y, 0);

        if (left.y > 0)
        {
            left.y--;
            right.y--;
            up.y--;
            down.y--;
        }

        matriceSetPixel(left.x, left.y, 1);
        matriceSetPixel(right.x, right.y, 1);
        matriceSetPixel(up.x, up.y, 1);
        matriceSetPixel(down.x, down.y, 1);
    }

    void moveRight()
    {

        matriceSetPixel(left.x, left.y, 0);
        matriceSetPixel(right.x, right.y, 0);
        matriceSetPixel(up.x, up.y, 0);
        matriceSetPixel(down.x, down.y, 0);

        if (right.y < 7)
        {
            left.y++;
            right.y++;
            up.y++;
            down.y++;
        }

        matriceSetPixel(left.x, left.y, 1);
        matriceSetPixel(right.x, right.y, 1);
        matriceSetPixel(up.x, up.y, 1);
        matriceSetPixel(down.x, down.y, 1);
    }

    void shoot(std::vector<Meteorit> &meteor, int &randNrMet)
    {
        Pair coordBullet(up.x - 1, up.y);
        Bullet bullet(coordBullet);

        for (int i = 0; i < 7; i++)
        {
            unsigned long currentTime = millis();
            matriceSetPixel(bullet.getCoord().x, bullet.getCoord().y, 1);
            while (millis() - currentTime < 25)
            {
                for (auto it = meteor.begin(); it != meteor.end();)
                {
                    if (it->getCoord() == bullet.getCoord())
                    {
                        it->destroy();
                        it = meteor.erase(it);
                        randNrMet--;
                        return;
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            matriceSetPixel(bullet.getCoord().x, bullet.getCoord().y, 0);
            bullet.move();
        }
    }
};

Pair left(7, 3);
Pair right(7, 5);
Pair up(6, 4);
Pair down(7, 4);

Nava nava(left, right, up, down);
std::vector<Meteorit> meteor;
int frequency[] = {0, 0, 0, 0, 0, 0};
byte heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000};

int numarVieti = 5;

void startround(int nrMeteoriti, int vitezaMeteoriti)
{
    while (nrMeteoriti)
    {

        meteor.clear();
        for (int i = 0; i <= 5; i++)
            frequency[i] = 0;

        int randNrMeteoriti;
        do
        {
            randNrMeteoriti = random(1, 4);
        } while (randNrMeteoriti > nrMeteoriti);

        nrMeteoriti -= randNrMeteoriti;

        for (int i = 1; i <= randNrMeteoriti; i++)
        {
            int randomCoord;
            do
            {
                randomCoord = random(0, 6);
            } while (frequency[randomCoord] == 1);
            frequency[randomCoord] = 1;
        }

        bool flagRight = false;
        bool flagLeft = false;
        bool flagShoot = false;

        for (int i = 0; i <= 5; i++)
            if (frequency[i] == 1)
            {
                Pair coord(0, i + 1);
                meteor.push_back(coord);
                meteor.back().display();
            }

        for (int i = 0; i < 8; i++)
        {

            unsigned long timpCadere = millis();

            while ((millis() - timpCadere < vitezaMeteoriti) && randNrMeteoriti > 0)
            {

                if (numarVieti <= 0)
                {
                    setLed(255, 0, 0);
                    lcd.clear();
                    lcd.setCursor(5, 0);
                    lcd.print("Ai pierdut!");
                    delay(2000);
                    return;
                }

                lcd.clear();
                lcd.setCursor(5, 0);
                for (int i = 0; i < numarVieti; i++)
                {
                    lcd.write(byte(0));
                }

                for (auto it = meteor.begin(); it != meteor.end();)
                {
                    if (it->getCoord() == nava.getUp() || it->getCoord() == nava.getDown() ||
                        it->getCoord() == nava.getLeft() || it->getCoord() == nava.getRight())
                    {
                        tone(BUZZERPIN, 200, 300);
                        it = meteor.erase(it);
                        randNrMeteoriti--;
                        numarVieti--;
                    }
                    else
                    {
                        ++it;
                    }
                }

                int joystick1 = analogRead(JOYSTICK1_VRX);
                int joystick2 = analogRead(JOYSTICK2_VRY);

                if (joystick1 >= 900 && joystick1 <= 1000 && flagRight == false)
                {
                    nava.moveRight();
                    flagRight = true;
                }
                else if (joystick1 >= 10 && joystick1 <= 100 && flagLeft == false)
                {
                    nava.moveLeft();
                    flagLeft = true;
                }
                else if (joystick1 >= 470 && joystick1 <= 520)
                {
                    flagRight = false;
                    flagLeft = false;
                }

                if (joystick2 >= 20 && joystick2 <= 60 && flagShoot == false)
                {
                    flagShoot = true;
                    tone(BUZZERPIN, 1000, 25);
                    nava.shoot(meteor, randNrMeteoriti);
                }
                else if (joystick2 >= 430 && joystick2 <= 530)
                    flagShoot = false;

                Serial.print("Joystick 2 (VRY): ");
                Serial.println(joystick1);
            }

            for (auto it = meteor.begin(); it != meteor.end(); ++it)
            {
                if (i < 7)
                    it->fall();
            }
        }

        for (auto it = meteor.begin(); it != meteor.end(); ++it)
        {
            it->destroy();
        }
        numarVieti -= meteor.size();
    }
}

void setup()
{
    Serial.begin(9600);
    randomSeed(1234);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);

    lcd.createChar(0, heart);

    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(BUTON_1, INPUT_PULLUP);
    pinMode(BUZZERPIN, OUTPUT);
    noTone(BUZZERPIN);

    lc.shutdown(0, false);
    lc.setIntensity(0, 8);
    lc.clearDisplay(0);
}

void countdown()
{
    for (int i = 3; i > 0; i--)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Incepe runda in");
        lcd.setCursor(0, 1);
        lcd.print(i);
        delay(1000);
    }
}

void startGame()
{

    numarVieti = 5;

    // Round 1
    lc.clearDisplay(0);
    nava.display();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Runda 1");
    delay(1000);
    countdown();
    lcd.clear();
    setLed(0, 255, 0);
    startround(8, 500);
    setLed(255, 0, 0);
    if (numarVieti <= 0)
    {
        delay(2000);
        lcd.clear();
        lc.clearDisplay(0);
        return;
    }
    delay(3000);

    lc.clearDisplay(0);
    nava.display();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Runda 2");
    delay(1000);
    countdown();
    lcd.clear();
    setLed(0, 255, 0);
    startround(13, 400);
    setLed(255, 0, 0);
    if (numarVieti <= 0)
    {
        delay(2000);
        lcd.clear();
        lc.clearDisplay(0);
        return;
    }
    delay(3000);

    // Round 3
    lc.clearDisplay(0);
    nava.display();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Runda 3");
    delay(1000);
    countdown();
    lcd.clear();
    setLed(0, 255, 0);
    startround(20, 300);
    setLed(255, 0, 0);
    if (numarVieti <= 0)
    {
        delay(2000);
        lcd.clear();
        lc.clearDisplay(0);
        return;
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("FELICITARI!");
        lcd.setCursor(0, 1);
        lcd.print("Ai castigat !");
    }
}

void loop()
{

    setLed(255, 0, 0);

    unsigned long currentTime = millis();

    int reading = digitalRead(START_BUTTON); 

    if (reading != lastButtonState)
    {
        lastDebounceTime = currentTime;
    }

    if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY)
    {
        if (reading == LOW && buttonState == 0)
        {
            buttonState = 1;
            Serial.println("Start Game!");
            delay(1000);
            startGame();
        }
    }

    if (reading == HIGH)
    {
        buttonState = 0;
    }

    lastButtonState = reading;
}
