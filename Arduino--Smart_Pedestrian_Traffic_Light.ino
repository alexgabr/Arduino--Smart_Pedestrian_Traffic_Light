//
// Alexandru Vrincianu
// 2024
//

#define BUILT_IN 13
#define GREEN 20000

short wait_period; // the waiting period until the green led turns on
float last_push = 0, last_time = 0;

float millis_to_seconds(unsigned long int millis) {
  return millis / 1000.00;
}

void set_timer(float time) {
  short q = 0;
  //se masoara intervalul dintre ultimul semnal verde si apasarea butonului si se adauga sau se scade din timpul de asteptare
  if (time - last_time < 50) q = 5;
  else if (time - last_time == 50) q = 0;
  else q = -5;

  //se calculeaza timpul de asteptare pana la semnalul verde
  if (time - last_push >= 50) {
    wait_period = 15 + q;
  } 
  else if (time - last_push < 50) {
    wait_period = 25 + q;
  }

  //last_push este momentul in care a fost apasat ultima data butonul
  last_push = time;

  Serial.print("q: "), Serial.println(q);

  Serial.print("Ultima apasare: "), Serial.println(last_push);

  Serial.print("Perioada de asteptare: "), Serial.println(wait_period);
}

void setup() {
  pinMode(2, INPUT); //push button
  pinMode(5, OUTPUT); //led secunde
  pinMode(6, OUTPUT); //semnal baza tranzistor si led verde

  pinMode(BUILT_IN, OUTPUT);
  digitalWrite(BUILT_IN, LOW);

  Serial.begin(9600);
}

void loop() {
  Serial.print("Ultimul timp: "), Serial.println(last_time);

  bool button = digitalRead(2);
  if(button) {
    //timpul cand butonul este apasat
    float time = millis_to_seconds(millis());

    Serial.print("Timp: "), Serial.println(time);
    
    set_timer(time);

    //se incepe innumaratoarea inversa, marcata de aprinderea intermitenta a unui led la o secunda
    for(int i = wait_period; i > 0; i--) {
      digitalWrite(5, HIGH);
      delay(500);
      digitalWrite(5, LOW);
      delay(500);
    }

    //se stinge semnalul rosu prin introducerea unui semnal electric in baza tranzistorului si se aprinde led-ul verde
    digitalWrite(6, HIGH);

    //constant green time 20 sec
    delay(GREEN); 

    //se memoreaza in secunde ultima data cand s-a stins led-ul verde
    last_time = millis_to_seconds(millis() - 70); // prin -70 ms se aproximeaza mai bine timpul, existand o eroare de 70 - 130 ms 

    //led-ul rosu se aprinde si cel verde se stinge
    digitalWrite(6, LOW);
  }
}
