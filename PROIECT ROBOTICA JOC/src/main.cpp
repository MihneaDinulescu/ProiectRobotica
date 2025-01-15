#include <Arduino.h>
#include <LiquidCrystal_I2C.h> // Bibliotecă pentru controlul afișajului LCD I2C.
#include <LedControl.h>        // Bibliotecă pentru controlul matricei LED MAX7219.
#include <ArduinoSTL.h>        // Bibliotecă pentru utilizarea STL (Standard Template Library) pe Arduino.
#include <vector>              // Bibliotecă pentru manipularea vectorilor din STL.

// Definirea pinilor hardware.
#define LED_RED_PIN 3     // Pin pentru LED-ul roșu.
#define LED_BLUE_PIN 5    // Pin pentru LED-ul albastru.
#define LED_GREEN_PIN 4   // Pin pentru LED-ul verde.
#define BUTON_1 8         // Pin pentru butonul de start.
#define JOYSTICK1_VRX A0  // Pin pentru axa X a primului joystick.
#define JOYSTICK2_VRY A3  // Pin pentru axa Y a celui de-al doilea joystick.
#define BUZZERPIN 9       // Pin pentru buzzer.
#define DEBOUNCE_DELAY 50 // Întârzierea pentru anti-debounce la buton.

int lastButtonState = HIGH;         // Starea precedentă a butonului.
unsigned long lastDebounceTime = 0; // Timpul ultimei schimbări a stării butonului.
int buttonState = 0;                // Starea curentă a butonului.
const int START_BUTTON = BUTON_1;   // Pinul alocat butonului de start.

// Inițializare afișaj LCD cu adresa 0x27 și dimensiuni 16x2.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inițializare matrice LED MAX7219: Data, Clock, Load/CS, număr matrice.
LedControl lc = LedControl(11, 13, 10, 1);

// Funcție pentru setarea unui pixel în matricea LED.
void matriceSetPixel(int x, int y, int value)
{
    // Verifică dacă coordonatele sunt valide (între 0 și 7).
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        lc.setLed(0, x, y, value); // Setează pixelul la valoarea dorită 
    }
}

// Funcție pentru setarea culorii LED-ului RGB.
void setLed(int red, int green, int blue)
{
    analogWrite(LED_RED_PIN, red);     // Controlează intensitatea roșului.
    analogWrite(LED_GREEN_PIN, green); // Controlează intensitatea verde.
    analogWrite(LED_BLUE_PIN, blue);   // Controlează intensitatea albastru.
}

// Clasa pentru reprezentarea unei perechi de coordonate (x, y).
class Pair
{
public:
    int x; // Coordonata X.
    int y; // Coordonata Y.

    // Constructor implicit: inițializează coordonatele la 0.
    Pair() : x(0), y(0) {}

    // Constructor cu parametri.
    Pair(int x, int y) : x(x), y(y) {}

    // Operator pentru compararea a două obiecte Pair.
    bool operator==(const Pair &other) const
    {
        return x == other.x && y == other.y; // Returnează true dacă coordonatele sunt identice.
    }
};

// Clasa pentru reprezentarea unui glonț (Bullet).
class Bullet
{
private:
    Pair coord; // Coordonatele glonțului.

public:
    // Constructor care inițializează coordonatele glonțului.
    Bullet(Pair coord) : coord(coord) {}

    // Getter pentru coordonatele glonțului.
    Pair getCoord() const { return coord; }

    // Funcție pentru deplasarea glonțului în sus.
    void move()
    {
        matriceSetPixel(coord.x, coord.y, 0); // Șterge pixelul curent.
        coord.x--;                            // Deplasează glonțul cu o linie în sus.
        matriceSetPixel(coord.x, coord.y, 1); // Afișează noua poziție.
    }
};

// Clasa pentru reprezentarea unui meteorit (Meteorit).
class Meteorit
{
private:
    Pair coord; // Coordonatele meteoritului.

public:
    // Constructor care inițializează coordonatele meteoritului.
    Meteorit(Pair coord) : coord(coord) {}

    // Getter pentru coordonatele meteoritului.
    Pair getCoord() const { return coord; }

    // Funcție pentru afișarea meteoritului pe matrice.
    void display()
    {
        matriceSetPixel(coord.x, coord.y, 1); // Afișează meteoritul la coordonatele curente.
    }

    // Funcție pentru deplasarea meteoritului în jos.
    void fall()
    {
        matriceSetPixel(coord.x, coord.y, 0); // Șterge pixelul curent.
        coord.x++;                            // Deplasează meteoritul cu o linie în jos.
        matriceSetPixel(coord.x, coord.y, 1); // Afișează noua poziție.
    }

    // Funcție pentru distrugerea meteoritului.
    void destroy()
    {
        matriceSetPixel(coord.x, coord.y, 0); // Șterge pixelul de pe matrice.
    }
};

// Clasa pentru reprezentarea navei spațiale (Nava).
class Nava
{
private:
    Pair left;  // Coordonatele colțului stâng.
    Pair right; // Coordonatele colțului drept.
    Pair up;    // Coordonatele colțului de sus.
    Pair down;  // Coordonatele colțului de jos.

public:
    // Constructor care inițializează coordonatele navei.
    Nava(Pair left, Pair right, Pair up, Pair down)
        : left(left), right(right), up(up), down(down) {}

    // Getteri pentru coordonatele părților navei.
    Pair getLeft() const { return left; }
    Pair getRight() const { return right; }
    Pair getUp() const { return up; }
    Pair getDown() const { return down; }

    // Funcție pentru afișarea navei pe matrice.
    void display()
    {
        matriceSetPixel(left.x, left.y, 1);
        matriceSetPixel(right.x, right.y, 1);
        matriceSetPixel(up.x, up.y, 1);
        matriceSetPixel(down.x, down.y, 1);
    }

    // Funcție pentru deplasarea navei la stânga.
    void moveLeft()
    {
        // Șterge pixelii navei din poziția curentă.
        matriceSetPixel(left.x, left.y, 0);
        matriceSetPixel(right.x, right.y, 0);
        matriceSetPixel(up.x, up.y, 0);
        matriceSetPixel(down.x, down.y, 0);

        // Verifică dacă nava nu depășește limita stângă a matricei.
        if (left.y > 0)
        {
            left.y--;
            right.y--;
            up.y--;
            down.y--;
        }

        // Afișează noua poziție a navei.
        matriceSetPixel(left.x, left.y, 1);
        matriceSetPixel(right.x, right.y, 1);
        matriceSetPixel(up.x, up.y, 1);
        matriceSetPixel(down.x, down.y, 1);
    }

    // Funcție pentru deplasarea navei la dreapta.
    void moveRight()
    {
        // Șterge pixelii navei din poziția curentă.
        matriceSetPixel(left.x, left.y, 0);
        matriceSetPixel(right.x, right.y, 0);
        matriceSetPixel(up.x, up.y, 0);
        matriceSetPixel(down.x, down.y, 0);

        // Verifică dacă nava nu depășește limita dreaptă a matricei.
        if (right.y < 7)
        {
            left.y++;
            right.y++;
            up.y++;
            down.y++;
        }

        // Afișează noua poziție a navei.
        matriceSetPixel(left.x, left.y, 1);
        matriceSetPixel(right.x, right.y, 1);
        matriceSetPixel(up.x, up.y, 1);
        matriceSetPixel(down.x, down.y, 1);
    }

    // Funcție pentru tragerea cu un glonț din poziția navei.
    void shoot(std::vector<Meteorit> &meteor, int &randNrMet)
    {
        // Inițializează glonțul în poziția de deasupra navei.
        Pair coordBullet(up.x - 1, up.y);
        Bullet bullet(coordBullet);

        // Glonțul avansează pe matrice timp de 7 poziții sau până lovește un meteorit.
        for (int i = 0; i < 7; i++)
        {
            unsigned long currentTime = millis();                         // Timpul curent pentru temporizare.
            matriceSetPixel(bullet.getCoord().x, bullet.getCoord().y, 1); // Afișează glonțul.

            // Verifică dacă glonțul lovește un meteorit.
            while (millis() - currentTime < 25)
            {
                for (auto it = meteor.begin(); it != meteor.end();)
                {
                    if (it->getCoord() == bullet.getCoord())
                    {                          // Verifică coliziunea.
                        it->destroy();         // Distruge meteoritul.
                        it = meteor.erase(it); // Elimină meteoritul din listă.
                        randNrMet--;           // Decrementează numărul de meteoriți activi.
                        return;                // Oprește funcția dacă un meteorit este lovit.
                    }
                    else
                    {
                        ++it;
                    }
                }
            }

            matriceSetPixel(bullet.getCoord().x, bullet.getCoord().y, 0); // Șterge poziția anterioară a glonțului.
            bullet.move();                                                // Mută glonțul cu o poziție în sus.
        }
    }
};

// Pozițiile inițiale ale navei.
Pair left(7, 3);
Pair right(7, 5);
Pair up(6, 4);
Pair down(7, 4);

// Inițializare navă și vector pentru meteoriți.
Nava nava(left, right, up, down);
std::vector<Meteorit> meteor;

// Frecvența coloanelor ocupate de meteoriți.
int frequency[] = {0, 0, 0, 0, 0, 0};

// Model de inimă pentru afișajul LCD.
byte heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000};

int numarVieti = 5; // Numărul inițial de vieți.

// Funcție pentru începerea unei runde de joc.
void startround(int nrMeteoriti, int vitezaMeteoriti)
{
    while (nrMeteoriti)
    {
        meteor.clear(); // Curăță lista de meteoriți de la runda precedentă.

        // Resetează frecvența coloanelor.
        for (int i = 0; i <= 5; i++)
            frequency[i] = 0;

        int randNrMeteoriti;

        // Determină câți meteoriți să fie generați în această rundă.
        do
        {
            randNrMeteoriti = random(1, 4);
        } while (randNrMeteoriti > nrMeteoriti);

        nrMeteoriti -= randNrMeteoriti; // Actualizează numărul de meteoriți rămași.

        // Generează poziții aleatorii pentru meteoriți fără suprapuneri.
        for (int i = 1; i <= randNrMeteoriti; i++)
        {
            int randomCoord;
            do
            {
                randomCoord = random(0, 6);
            } while (frequency[randomCoord] == 1);
            frequency[randomCoord] = 1;
        }

        bool flagRight = false; // Indicator pentru mișcare dreapta.
        bool flagLeft = false;  // Indicator pentru mișcare stânga.
        bool flagShoot = false; // Indicator pentru tragere.

        // Afișează meteoriții inițiali.
        for (int i = 0; i <= 5; i++)
        {
            if (frequency[i] == 1)
            {
                Pair coord(0, i + 1);
                meteor.push_back(coord);
                meteor.back().display();
            }
        }

        // Simulează mișcarea și interacțiunea în runda curentă.
        for (int i = 0; i < 8; i++)
        {
            unsigned long timpCadere = millis();

            // Simulează căderea meteoriților pentru viteza specificată.
            while ((millis() - timpCadere < vitezaMeteoriti) && randNrMeteoriti > 0)
            {
                // Verifică dacă viețile sunt epuizate.
                if (numarVieti <= 0)
                {
                    setLed(255, 0, 0); // Setează LED-ul pe roșu.
                    lcd.clear();
                    lcd.setCursor(5, 0);
                    lcd.print("Ai pierdut!"); // Mesaj de înfrângere.
                    delay(2000);
                    return;
                }

                lcd.clear();
                lcd.setCursor(5, 0);

                // Afișează numărul de vieți rămase.
                for (int i = 0; i < numarVieti; i++)
                {
                    lcd.write(byte(0));
                }

                // Verifică coliziunea dintre meteoriți și navă.
                for (auto it = meteor.begin(); it != meteor.end();)
                {
                    if (it->getCoord() == nava.getUp() || it->getCoord() == nava.getDown() ||
                        it->getCoord() == nava.getLeft() || it->getCoord() == nava.getRight())
                    {
                        tone(BUZZERPIN, 200, 300); // Sunet pentru coliziune.
                        it = meteor.erase(it);     // Șterge meteoritul.
                        randNrMeteoriti--;         // Decrementează numărul de meteoriți.
                        numarVieti--;              // Decrementează viețile.
                    }
                    else
                    {
                        ++it;
                    }
                }

                // Controlează mișcarea și tragerea cu joystick-ul.
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

                if (joystick2 >= 40 && joystick2 <= 90 && flagShoot == false)
                {
                    flagShoot = true;
                    tone(BUZZERPIN, 1000, 25); // Sunet pentru tragere.
                    nava.shoot(meteor, randNrMeteoriti);
                }
                else if (joystick2 >= 430 && joystick2 <= 530)
                {
                    flagShoot = false;
                }

                // Debug: Afișează valoarea joystick-ului pe Serial Monitor.
                Serial.print("Joystick 2 (VRY): ");
                Serial.println(joystick2);
            }

            // Mișcă meteoriții în jos
            for (auto it = meteor.begin(); it != meteor.end(); ++it)
            {
                if (i < 7)
                    it->fall();
            }
        }

        // Șterge meteoriții rămași după rundă.
        for (auto it = meteor.begin(); it != meteor.end(); ++it)
        {
            it->destroy();
        }
        numarVieti -= meteor.size(); // Decrementează viețile pentru meteoriții rămași.
    }
}

// Configurare inițială.
void setup()
{
    Serial.begin(9600);  // Inițializează comunicarea serială.
    randomSeed(1234);    // Setează sămânța pentru funcția random.
    lcd.init();          // Inițializează LCD-ul.
    lcd.backlight();     // Activează iluminarea de fundal.
    lcd.setCursor(0, 0); // Setează poziția cursorului LCD.

    lcd.createChar(0, heart); // Creează caracterul personalizat (inimă).

    // Setează pini pentru ieșiri și intrări.
    pinMode(5, OUTPUT);             // Pin pentru LED roșu.
    pinMode(4, OUTPUT);             // Pin pentru LED verde.
    pinMode(3, OUTPUT);             // Pin pentru LED albastru.
    pinMode(BUTON_1, INPUT_PULLUP); // Buton pentru intrare cu rezistență internă activată.
    pinMode(BUZZERPIN, OUTPUT);     // Pin pentru buzzer.
    noTone(BUZZERPIN);              // Dezactivează tonul inițial al buzzer-ului.

    // Inițializează afișajul matricei LED.
    lc.shutdown(0, false); // Activează afișajul.
    lc.setIntensity(0, 8); // Setează intensitatea luminozității.
    lc.clearDisplay(0);    // Curăță afișajul.
}

// Funcție pentru afișarea unui numărătoare inversă înainte de o rundă.
void countdown()
{
    for (int i = 3; i > 0; i--)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Incepe runda in");
        lcd.setCursor(0, 1);
        lcd.print(i); // Afișează numărătoarea.
        delay(1000);  // Așteaptă 1 secundă.
    }
}

// Funcție pentru începerea jocului.
void startGame()
{
    numarVieti = 5; // Resetare număr vieți.

    // Runda 1
    lc.clearDisplay(0);
    nava.display(); // Afișează nava pe matrice.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Runda 1");
    delay(1000);
    countdown(); // Afișează numărătoarea inversă.
    lcd.clear();
    setLed(0, 255, 0);  // Setează LED-ul pe verde.
    startround(8, 500); // Pornește runda cu 8 meteoriți și viteza 500ms.
    setLed(255, 0, 0);  // Setează LED-ul pe roșu după runda.
    if (numarVieti <= 0)
    { // Dacă toate viețile sunt pierdute, termină jocul.
        delay(2000);
        lcd.clear();
        lc.clearDisplay(0);
        return;
    }
    delay(3000);

    // Runda 2
    lc.clearDisplay(0);
    nava.display();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Runda 2");
    delay(1000);
    countdown();
    lcd.clear();
    setLed(0, 255, 0);
    startround(13, 400); // Pornește runda cu 13 meteoriți și viteza 400ms.
    setLed(255, 0, 0);
    if (numarVieti <= 0)
    {
        delay(2000);
        lcd.clear();
        lc.clearDisplay(0);
        return;
    }
    delay(3000);

    // Runda 3
    lc.clearDisplay(0);
    nava.display();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Runda 3");
    delay(1000);
    countdown();
    lcd.clear();
    setLed(0, 255, 0);
    startround(20, 300); // Pornește runda cu 20 meteoriți și viteza 300ms.
    setLed(255, 0, 0);
    if (numarVieti <= 0)
    { // Dacă viețile sunt pierdute, termină jocul.
        delay(2000);
        lcd.clear();
        lc.clearDisplay(0);
        return;
    }
    else
    { // Mesaj de câștig.
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("FELICITARI!");
        lcd.setCursor(0, 1);
        lcd.print("Ai castigat !");
    }
}

// Funcția principală de buclă.
void loop()
{
    setLed(255, 0, 0); // Setează LED-ul pe roșu în mod implicit.

    unsigned long currentTime = millis(); // Citește timpul curent.

    int reading = digitalRead(START_BUTTON); // Citește starea butonului START.

    // Detectează schimbarea stării butonului.
    if (reading != lastButtonState)
    {
        lastDebounceTime = currentTime; // Actualizează timpul ultimei schimbări.
    }

    // Verifică dacă timpul de debounce a trecut.
    if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY)
    {
        if (reading == LOW && buttonState == 0)
        {                                  // Dacă butonul este apăsat.
            buttonState = 1;               // Actualizează starea butonului.
            Serial.println("Start Game!"); // Mesaj de pornire a jocului.
            delay(1000);                   // Pauză pentru claritate.
            startGame();                   // Pornește jocul.
        }
    }

    // Resetează starea butonului dacă nu mai este apăsat.
    if (reading == HIGH)
    {
        buttonState = 0;
    }

    lastButtonState = reading; // Actualizează starea anterioară a butonului.
}
