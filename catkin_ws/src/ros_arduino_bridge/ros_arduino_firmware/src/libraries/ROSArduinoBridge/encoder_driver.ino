/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(ARDUINO_ENC_COUNTER)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
  static const int8_t ENC_STATES [] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};  //encoder lookup table
    
  /* Interrupt routine for LEFT encoder, taking care of actual counting */
  ISR (PCINT2_vect){
  	static uint8_t enc_last=0;
        
	enc_last <<=2; //shift previous state two places
	enc_last |= (PIND & (3 << 2)) >> 2; //read the current state into lowest 2 bits
  
  	left_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  ISR (PCINT1_vect){
        static uint8_t enc_last=0;
          	
	enc_last <<=2; //shift previous state two places
	enc_last |= (PINC & (3 << 4)) >> 4; //read the current state into lowest 2 bits
  
  	right_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
  }
#elif defined ARDUINO_MY_COUNTER
  //功能：实现左右电机的脉冲计数
  //1.定义计数器
  volatile long left_count = 0L;
  volatile long right_count = 0L;
  //2.初始化
  void initEncoder(){
    pinMode(LEFT_A,INPUT);
    pinMode(LEFT_B,INPUT);
    pinMode(RIGHT_A,INPUT);
    pinMode(RIGHT_B,INPUT);
    //添加中断
    attachInterrupt(2,leftEncoderEventA,CHANGE);
    attachInterrupt(3,leftEncoderEventB,CHANGE);
    attachInterrupt(5,rightEncoderEventA,CHANGE);
    attachInterrupt(4,rightEncoderEventB,CHANGE);
  }

  //3.编写中断的回调函数
  void leftEncoderEventA(){
    
    if(digitalRead(LEFT_A) == HIGH){
  
      if(digitalRead(LEFT_B) == HIGH){//A 高 B 高
        left_count++;  
      } else {//A 高 B 低
        left_count--;  
      }
  
  
    } else {
  
      if(digitalRead(LEFT_B) == LOW){//A 低 B 低
        left_count++;  
      } else {//A 低 B 高
        left_count--;  
      }  
  
    }
  }
  void leftEncoderEventB(){
    if(digitalRead(LEFT_B) == HIGH){
  
      if(digitalRead(LEFT_A) == LOW){//B 高 A 低
        left_count++;
      } else {//B 高 A 高
        left_count--;
      }
  
  
    } else {
  
      if(digitalRead(LEFT_A) == HIGH){//B 低 A 高
        left_count++;
      } else {//B 低 A 低
        left_count--;
      }

    }
  }
  void rightEncoderEventA(){
    if(digitalRead(RIGHT_A) == HIGH){
  
      if(digitalRead(RIGHT_B) == HIGH){//A 高 B 高
        right_count++;  
      } else {//A 高 B 低
        right_count--;  
      }
  
  
    } else {
  
      if(digitalRead(RIGHT_B) == LOW){//A 低 B 低
        right_count++;  
      } else {//A 低 B 高
        right_count--;  
      }  
  
    }
  }
  void rightEncoderEventB(){
    if(digitalRead(RIGHT_B) == HIGH){
  
      if(digitalRead(RIGHT_A) == LOW){//B 高 A 低
        right_count++;
      } else {//B 高 A 高
        right_count--;
      }
  
  
    } else {
  
      if(digitalRead(RIGHT_A) == HIGH){//B 低 A 高
        right_count++;
      } else {//B 低 A 低
        right_count--;
      }

    }
  }
  //4.实现编码器数据读和重置的函数
  //i 取值是 LEFT 或 RIGHT,是左右轮的标记
  long readEncoder(int i){
    if (i == LEFT) return left_count;
    else return right_count;
    }
  void resetEncoder(int i){
     if (i == LEFT){
      left_count=0L;
      return;
    } else { 
      right_count=0L;
      return;
    }
  }
  
#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif
