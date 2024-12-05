# Space Defender  

## Descriere  
**Space Defender** este un joc interactiv dezvoltat pe platforma Arduino, care utilizează două joystick-uri, o matrice LED 8x8, un LCD, un buzzer și două butoane. Scopul jocului este de a controla o navă spațială poziționată la baza matricei pentru a distruge meteoriții care cad din cer înainte ca aceștia să lovească nava. Jucătorul dispune de trei vieți și trebuie să obțină un scor cât mai mare.  

## Funcționalități  
- **Controlul navei spațiale**:  
  - Deplasarea navei pe axa X este realizată prin mișcarea unui joystick stânga-dreapta.  

- **Tragerea cu nava**:  
  - Folosind al doilea joystick, mișcarea acestuia în sus declanșează o lovitură.  
  - Fiecare lovitură este semnalizată sonor de buzzer.  

- **Mecanică dinamică a meteoriților**:  
  - Meteorii cad din partea de sus a matricei spre navă.  
  - Viteza și numărul meteoriților cresc progresiv pe măsură ce nivelurile avansează.  
  - Lovirea unui meteorit îl distruge, contribuind la creșterea scorului.  
  - Dacă un meteorit lovește nava, se pierde o viață.  

- **Sistem de vieți și niveluri**:  
  - Jucătorul începe cu trei vieți, afișate pe LCD.  
  - Jocul se încheie când toate viețile sunt pierdute, iar scorul final este afișat.  
  - După eliminarea unui număr prestabilit de meteoriți, nivelul crește, iar viteza și multiplicitatea meteoriților se intensifică.  

- **Feedback vizual și sonor**:  
  - Matricea LED 8x8 afișează poziția navei și a meteoriților în timp real.  
  - **Buzzer-ul semnalizează**:
    - Când nava trage.  
    - Când jucătorul trece la un nivel următor.  
    - Când un meteorit lovește nava.  

- **Afișare detalii pe LCD**:  
  - Scorul curent.  
  - Numărul vieților rămase.  
  - Nivelul actual al jocului.  

- **Utilizarea butoanelor**:  
  - **Buton 1 - Start-Stop**:  
    - Apăsarea scurtă pornește jocul.  
    - Menținerea apăsată timp de ≥1s oprește complet jocul.  
  - **Buton 2 - Pauză**:  
    - Orice apăsare pune jocul pe pauză și păstrează starea curentă.  

## Tehnologii utilizate  
- **Hardware**:
  - Arduino Uno  
  - Matrice LED MAX7219 8x8  
  - LCD  
  - 2 joystick-uri analogice  
  - 2 butoane  
  - Buzzer  
  - Rezistențe  

- **Software**:  
  - Programare în C++ utilizând platforma Arduino IDE.  
