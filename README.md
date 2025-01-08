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
    - 
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
