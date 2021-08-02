
#include "bluetooth.h"

#include "main.h"
#include <intrins.h>
void UartInit()		//9600bps@11.0592MHz
{
	P_SW1 &= ~(3<<6);
	P_SW1 |= 1<<6;			//RXD/P3.2, TXD/P3.3
	P3M0 |= 1<<3;
	P3M1 &= ~(1<<3); //TX�������
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ,����Ƶ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xE0;		//���ö�ʱ��ʼֵ
	TH1 = 0xFE;		//���ö�ʱ��ʼֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	ES=1;						//�򿪽����ж�	
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
}


void Timer0Init()		//1000΢��@11.0592MHz
{
	P5M0 |=1<<4;
	P5M1 &=~(1<<4);	//P54���ģʽ
	
	AUXR |= 0x80;		//��ʱ��0ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ʼֵ
	TH0 = 0xD4;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}

void ADCInit()
{
    P5M0 &= ~(1<<5);                                //����P5.5ΪADC��
    P5M1 |= 1<<5;


		P_SW2 |= 0X80;
		ADCTIM=0X3F;
		P_SW2 &= 0X7F; 
	
    ADCCFG |= 0x0f;                              //����ADCʱ��Ϊϵͳʱ��/2/16/16
		ADC_CONTR |=5;  													//����ͨ��5
    ADC_CONTR |= 0x80;                           //ʹ��ADCģ��
	

}


u16 voltage=0;
void TASK_ADC()		//1000΢��@11.0592MHz
{

	ADC_CONTR |= 0x40;                      //����ADת��
	while (!(ADC_CONTR & 0x20)) ;           //��ѯADC��ɱ�־
   _nop_();
	 _nop_();
	ADC_CONTR &= ~0x20;                     //����ɱ�־
	voltage = ADC_RES*4+ADC_RESL/64;                           //��ȡADC���
	voltage = voltage *32/5;
	mcu_dp_value_update(DPID_VOLTAGE,voltage);//�ϱ�

	
}
u8 f_task_ad=0;
void main()
{	
	UartInit();  //	���ڳ�ʼ��
	Timer0Init();// ��ʱ��0��ʼ��
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
	SBUF=value;//�����յ������ݷ��뵽���ͼĴ���
	while(!TI);			 //�ȴ������������
	TI=0;						 //���������ɱ�־λ	
}

void Usart() interrupt 4
{
	u8 receiveData;

	receiveData=SBUF;//��ȥ���յ�������
	RI = 0;//��������жϱ�־λ
	uart_receive_input(receiveData);
}


u16  fre_set=1;
void Timer0() interrupt 1
{
	static u16 i=0;
	static u16 count=0;
	if(fre_set>=100)//Ƶ�ʴ��ڵ���100Hz
	{
		TL0 = (65535-11059/fre_set*500)%256;
		TH0 = (65535-11059/fre_set*500)/256;
		i=0;
		P54 =~P54;
		count++;
	}
	else if(fre_set<100)//0.1ms
	{
		TL0 = 0xAE;		//���ö�ʱ��ʼֵ
		TH0 = 0xFB;		//���ö�ʱ��ʼֵ
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


