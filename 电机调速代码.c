/*
 * PID相关代码实现：
 * 
 * 准备：复制之前的测速代码 + 添加控制电机转动的代码 + PID库安装成功
 * 修改:
 *  1.修改了测速的单位时间（2000 -> 50）；
 *  2.添加电机转向与PWM控制引脚；
 * 步骤：
 *   1.包含头文件
 *   2.创建PID对像
 *   3.setUp中启用PID对象
 *   4.调用Computer（）输出控制电机的PWM值
 *   
 */

#include <PID_v1.h> 
//1.将使用的引脚封装为变量，封装计数变量；
 int encoder_A = 21;//2
 int encoder_B = 20;//3
 volatile int count = 0;
 int DIR_LEFT = 4;
 int PWM_LEFT = 5;
 
//4.计数逻辑实现
void count_a(){
  //先判断A是否跳变到高电压
  if(digitalRead(encoder_A) == HIGH){
    //再判断B的电压
    if(digitalRead(encoder_B) == HIGH){
      count++;
    } else {
      count--;
    }
  } else{
    if(digitalRead(encoder_B) == LOW){
      count++;
    } else {
      count--; 
    }
  }
}
void count_b(){
  if(digitalRead(encoder_B) == HIGH){
    if(digitalRead(encoder_A) == LOW){
      count++;
    } else {
      count--;
    }
  } else {
    if(digitalRead(encoder_A) == HIGH){
      count++;
    } else {
      count--;
    }
  }
}

//测试流程:
/*
 * 1.封装变量 --- 开始时间,单位时间，减速比，一圈输出的脉冲数，使用的N倍频测速
 * 2.实现逻辑
 *  2-1.获取时时时间戳（当前时间）
 *  2-2.if（当前时间 - 开始时间 >= 单位时间）{
 *        //取消中断
 *        //计算转速（count）
 *        //将count置0
 *        //将开始时间重置为当前时间
 *        //重启中断
 *         *      
 *  }
 */
long start_time = millis();
int interval_time = 50;
int per_round = 90 * 11 * 4;
double vel;
void get_current_vel(){
  //获取时时时间戳（当前时间）
  long right_now = millis();
  //判断逝去的时间是否大于单位时间
  long past_time = right_now - start_time;
  if(past_time >= interval_time){
    //取消中断
    noInterrupts();
    //计算转速（count）
    vel = (double)count / per_round / past_time * 1000 * 60;
    Serial.println(vel);
    //将count置0
    count = 0;
    //将开始时间重置为当前时间
    start_time = right_now;
    //重启中断
    interrupts();
    
  }
}
/*
 * 参数1：输入（当前转速）
 * 参数2：输出（根据PID算法计算的PWM值 --- 油门）
 * 参数3：目标值；
 * 参数4 - 参合6： PID
 * 参数7：DIRECT | REVERSE （后者的P，I，D值会取反）
 * 
 * 
 */
double pwm;
double target = 80;
double Kp=1.5, Ki=4, Kd=0.1;
PID pid(&vel,&pwm,&target,Kp,Ki,Kd,DIRECT);

//调速函数
void update_vel(){
  //调用测速参数
  get_current_vel();
  //调用 Compute 函数生成 PWM 值
  pid.Compute();
  //将PWM写入电机
  digitalWrite(DIR_LEFT,HIGH);
  analogWrite(PWM_LEFT,pwm);

}
void setup() {
  // put your setup code here, to run once:
  //2.setup中设置引脚的操作模式（INPUT），设置波特率；
  Serial.begin(57600);
  pinMode(encoder_A,INPUT);
  pinMode(encoder_B,INPUT);
  pinMode(DIR_LEFT,4);
  pinMode(PWM_LEFT,5);
  
  //3.为引脚添加终端函数（attachInterrupt);
  //参数1：中段口 参数2：回调函数 参数3：触发时机
  attachInterrupt(2,count_a,CHANGE);//单倍频或双倍频只需要为编码器的A相添加中断函数
  attachInterrupt(3,count_b,CHANGE);
  pid.SetMode(AUTOMATIC);
}

void loop() {
//  delay(2000);
//  Serial.println(count);
//  get_current_vel();
    delay(10);
    update_vel();
}