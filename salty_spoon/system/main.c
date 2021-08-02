
#include "bluetooth.h"

#include "main.h"
#include <intrins.h>
void UartInit()		//9600bps@11.0592MHz
{
	P_SW1 &= ~(3<<6);
	P_SW1 |= 1<<6;			//RXD/P3.2, TXD/P3.3
	P3M0 |= 1<<3;
	P3M1 &= ~(1<<3); //TX推挽输出
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式,不分频
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE0;		//设置定时初始值
	TH1 = 0xFE;		//设置定时初始值
	ET1 = 0;		//禁止定时器1中断
	ES=1;						//打开接收中断	
	TR1 = 1;		//定时器1开始计时
}


void Timer0Init()		//1000微秒@11.0592MHz
{
	P5M0 |=1<<4;
	P5M1 &=~(1<<4);	//P54输出模式
	
	AUXR |= 0x80;		//定时器0时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初始值
	TH0 = 0xD4;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0 = 1;		//定时器0开始计时
}

void ADCInit()
{
    P5M0 &= ~(1<<5);                                //设置P5.5为ADC口
    P5M1 |= 1<<5;


		P_SW2 |= 0X80;
		ADCTIM=0X3F;
		P_SW2 &= 0X7F; 
	
    ADCCFG |= 0x0f;                              //设置ADC时钟为系统时钟/2/16/16
		ADC_CONTR |=5;  													//设置通道5
    ADC_CONTR |= 0x80;                           //使能ADC模块
	

}


u16 voltage=0;
void TASK_ADC()		//1000微秒@11.0592MHz
{

	ADC_CONTR |= 0x40;                      //启动AD转换
	while (!(ADC_CONTR & 0x20)) ;           //查询ADC完成标志
   _nop_();
	 _nop_();
	ADC_CONTR &= ~0x20;                     //清完成标志
	voltage = ADC_RES*4+ADC_RESL/64;                           //读取ADC结果
	voltage = voltage *32/5;
	mcu_dp_value_update(DPID_VOLTAGE,voltage);//上报

	
}
u8 f_task_ad=0;
void main()
{	
	UartInit();  //	串口初始化
	Timer0Init();// 定时器0初始化
	bt_protocol_init();
	ADCInit();
	while(1)
	{
		bt_uart_service();
		if(f_task_ad)
		{
			f_task_ad=0;
			TASK_ADC();
		}
	}
}


void Uart_PutChar(unsigned char value)
{
	SBUF=value;//将接收到的数据放入到发送寄存器
	while(!TI);			 //等待发送数据完成
	TI=0;						 //清除发送完成标志位	
}

void Usart() interrupt 4
{
	u8 receiveData;

	receiveData=SBUF;//出去接收到的数据
	RI = 0;//清除接收中断标志位
	uart_receive_input(receiveData);
}


u16  fre_set=1;
void Timer0() interrupt 1
{
	static u16 i=0;
	static u16 count=0;
	if(fre_set>=100)//频率大于等于100Hz
	{
		TL0 = (65535-11059/fre_set*500)%256;
		TH0 = (65535-11059/fre_set*500)/256;
		i=0;
		P54 =~P54;
		count++;
	}
	else if(fre_set<100)//0.1ms
	{
		TL0 = 0xAE;		//设置定时初始值
		TH0 = 0xFB;		//设置定时初始值
		if(fre_set==0)
		{
			P54=1;
		}
		else
		{
			i++;		
			if(i>(5000/fre_set))
			{
				P54 =~P54;
				i=0;
				count++;
			}
		}
	}
	if(count>(4*fre_set))
	{
		count=0;
		f_task_ad=1;
	}
}


