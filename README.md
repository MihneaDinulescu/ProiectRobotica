# Space Defender  

## Descriere  
**Space Defender** este un joc interactiv dezvoltat pe platforma Arduino, care utilizează două joystick-uri, o matrice LED 8x8, un LCD, un buzzer și un buton. Scopul jocului este de a controla o navă spațială poziționată la baza matricei pentru a distruge meteoriții care cad din cer înainte ca aceștia să lovească nava.Jucătorul dispune de 5 vieți și trebuie să reziste cele 3 runde pentru a câștiga.  

## Funcționalități  
- **Controlul navei spațiale**:  
  - Deplasarea navei pe axa X este realizată prin mișcarea unui joystick stânga-dreapta.  

- **Tragerea cu nava**:  
  - Folosind al doilea joystick, mișcarea acestuia în sus declanșează o tragere a unui glonț.  
  - Fiecare lovitură este semnalizată sonor de buzzer.  

- **Mecanică dinamică a meteoriților**:  
  - Meteorii cad din partea de sus a matricei spre navă.  
  - Viteza și numărul meteoriților cresc progresiv pe măsură ce se avansează in runde.  
  - Lovirea unui meteorit de nava îl distruge.  
  - Dacă un meteorit lovește nava sau nu este lovit la timp, se pierde o viață.
  - Daca un meteorit lovește nava se semnalizează sonor de buzzer. 

- **Sistem de vieți și niveluri**:  
  - Jucătorul începe cu 5 vieți, afișate pe LCD.  
  - Jocul se încheie când toate viețile sunt pierdute sau când jucătorul reușește să termine cele 3 runde fără să își piardă viețile.  
  - După eliminarea unui număr prestabilit de meteoriți, nivelul crește, iar viteza și multiplicitatea meteoriților se intensifică.  

- **Feedback vizual și sonor**:  
  - Matricea LED 8x8 afișează poziția navei și a meteoriților în timp real.  
  - **Buzzer-ul semnalizează**:
    - Când nava trage.  
    - Când jucătorul trece la un nivel următor.  
    - Când un meteorit lovește nava.  

- **Afișare detalii pe LCD**:   
  - Numărul vieților rămase în timpul rundei.  
  - Numărul rundei ce urmează să înceapă.  

- **Utilizarea butonului**:  
  - **Buton 1 - Start**:  
    - O apăsare pornește jocul.

## Flow-ul Jocului

### 1. **Buton Start Apăsat**
- Jocul începe atunci când butonul START este apăsat.
- Este implementată o verificare anti-debounce pentru a preveni declanșările accidentale.

### 2. **Countdown**
- Pe LCD este afișată o numărătoare inversă de la 3 la 1 înainte de începerea rundei.

### 3. **Afișarea Navei**
- Nava jucătorului este afișată pe matricea LED, ocupând patru poziții (sus, jos, stânga, dreapta).

### 4. **Generarea Meteoriților**
- Un număr aleator de meteoriți este generat și poziționat pe prima linie a matricei LED.
- Pozițiile sunt unice și nu există suprapuneri între meteoriți.

### 5. **Controlul Navei**
- **Joystick pentru mișcare:**
  - Mișcarea joystick-ului în stânga sau dreapta deplasează nava pe matrice, respectând limitele (coloanele 0 și 7).
- **Joystick pentru tragere:**
  - Mișcarea joystick-ului în sus trage un glonț care urcă matricea.
  - Dacă glonțul lovește un meteorit, acesta este distrus, iar glonțul dispare.

### 6. **Meteoriții Cad**
- Meteoriții se deplasează treptat în jos, rând cu rând, în funcție de viteza rundei curente.
- Pentru fiecare meteorit, se verifică:
  - **Coliziune cu nava:**
    - Dacă un meteorit atinge o poziție ocupată de navă (sus, jos, stânga, dreapta), o viață este pierdută, iar meteoritul dispare.
  - **Depășirea matricei:**
    - Dacă un meteorit ajunge sub ultima linie a matricei fără să fie distrus, o viață este pierdută.

### 7. **Actualizarea Vieților**
- Viețile rămase sunt afișate pe LCD sub formă de inimioare.
- Dacă viețile ajung la 0, jocul se termină.

### 8. **Finalizarea Rundei**
- Runda se încheie dacă toți meteoriții sunt distruși sau au părăsit matricea (cu pierderile de vieți corespunzătoare).
- Jocul trece la următoarea rundă, care include mai mulți meteoriți și o viteză mai mare de cădere.

### 9. **Finalul Jocului**
- **Victorie:** Dacă toate rundele sunt completate fără pierderea tuturor vieților, pe LCD apare mesajul: **„FELICITĂRI! Ai câștigat!”**
- **Înfrângere:** Dacă viețile ajung la 0 în orice moment, pe LCD apare mesajul: **„Ai pierdut!”**, iar jocul se oprește.

### 10. **Reînceperea Jocului**
- După finalizarea jocului, jucătorul poate apăsa butonul START pentru a începe o nouă sesiune.

## Tehnologii utilizate  
- **Hardware**:
  - Arduino Uno  
  - Matrice LED MAX7219 8x8  
  - LCD  
  - 2 joystick-uri analogice  
  - 1 buton  
  - Buzzer  
  - Rezistențe  

- **Software**:  
  - Programare în C++ utilizând platforma Arduino IDE.

## Schema bloc
*(https://wokwi.com/projects/417630519742962689)*
