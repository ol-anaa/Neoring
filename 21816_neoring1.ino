#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 24

Adafruit_NeoPixel ring(LED_COUNT,
                      LED_PIN,
                      NEO_RGB + NEO_KHZ800);
int cnt = 0;
int cnt_old = 0;
bool Final = false; //Vai indicar se chegou no último led


void setup()
{
  //Configuração do timer1
  TCCR1A = 0; // modo de operação normal (Timer Counter Register 1)
  TCCR1B = 0; 
  TCCR1B |= (1<<CS10) | (1<<CS12); //prescale 10 (deslocamento de bits)
  //contador para estouro em 0,5 segundos
  // 0xC2F7 -> 1 segundo
  // 0x85ED -> 0,5 segundos
  // 65535 - (16M/1024/t(1) = 49911) -> 0xC2F7
  TCNT1 = 0xC2F7;
  TIMSK1 |= (1<<TOIE1); // Timer Overflow Interrupt 1
}

void loop()
{
  if(cnt != cnt_old){
    if (!Final){
      	//Faz a divisão por 2, se sobra resto acende o impar se não par
        if(cnt%2 == 0){
          ring.setPixelColor(cnt, 144,238,144);
          ring.show();
        }
        if(cnt%2 != 0){
          ring.setPixelColor(cnt, 123,104,238);
          ring.show();
        }
        // Final recebe true para indicar que chegou ao fim.
        if(cnt == 24){
          Final = true;
        }
    }
    
    if (Final){
        // Recebe a cor 0,0,0 para "apagar".
        ring.setPixelColor(cnt, 0,0,0);
        ring.show();
    }
      //Chegou no começo Final zera.
      if (cnt == 0){
        Final = false;
      }
    cnt_old = cnt;
  }
}

ISR(TIMER1_OVF_vect){
  TCNT1 = 0xC2F7;
 
  //Se for diferente de final ele vai incrementar para rodar acendendo no sentido horario
  if (!Final)
    cnt++;
  
  // Se não, siginifica que ele ja terminou e então ele começa a voltar
  else 
    cnt--; 
}
