namespace time
{
    constexpr unsigned long max_ulong = 4294967295;

    void wrap(unsigned long& wrapping_type, unsigned long& rollover_type, unsigned long wrap_at) {
        if (wrapping_type >= wrap_at) {
            wrapping_type = wrapping_type % wrap_at;
            rollover_type++;
        }
    }
    
  void sleep(unsigned long seconds) {
      for (unsigned long t = 0; t < seconds; ++t) {
          delay(1000);
      }
  }
  
  class AsyncDelay {
  private:
      unsigned long expires;
      unsigned long delta;
  public:
      void delay(unsigned long ms) {
          auto current_time = millis();
          if (current_time > max_ulong - ms) {
              delta = max_ulong - current_time + 1;
          }
          else {
              delta = 0;
          }
          expires = current_time + ms;
      }
      bool expired() {
          auto current_time = millis();
          if (expires + delta <= current_time + delta) {
              return true;
          }
          return false;
      }
  };

    class Timer {
    public:
        Timer() : async_delay{} {
            async_delay.delay(delta_ms);
        }
        void update() {
            if (async_delay.expired()) {
                ms += delta_ms;
                wrap(ms, s, 1000);
                wrap(s, m, 60);
                wrap(m, h, 60);
                wrap(h, d, 24);
                async_delay.delay(delta_ms);
            }
        }
//        void print() {
//            Serial.print("day:");
//            Serial.print(d);
//            Serial.print(" time:");
//            Serial.print(h);
//            Serial.print(':');
//            Serial.print(m);
//            Serial.print(':');
//            Serial.println(s);
//        }
        void reset() {
            ms = 0;
            s = 0;
            m = 0;
            h = 0;
            d = 0;
        }
        unsigned long ms{ 0 };
        unsigned long s{ 0 };
        unsigned long m{ 0 };
        unsigned long h{ 0 };
        unsigned long d{ 0 };
      private:
        unsigned long delta_ms{1000};
        AsyncDelay async_delay;
    };
}


constexpr auto relay_on = LOW;
constexpr auto relay_off = HIGH;

struct Pump {
public:
    Pump(int pin) :pin{ pin } {
        digitalWrite(pin, relay_off);
        pinMode(pin, OUTPUT);
    }
    void pump(unsigned long seconds) {
      digitalWrite(pin, relay_on);
      Serial.println("pumping");
      time::sleep(seconds);
      Serial.println("pumping complete");
      digitalWrite(pin, relay_off);
    }
private:
    int pin;
};

time::Timer timer{};
Pump pump(13);

void setup() {
  Serial.begin(9600);
  Serial.println("starting");
  time::sleep(10);
  pump.pump(15);
}

void loop() {
    timer.update();
    //timer.print();
    if (timer.d >= 3) {
        timer.reset();
        pump.pump(15);
    }
}
