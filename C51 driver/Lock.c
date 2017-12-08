#include <REG52.h>
#include<intrins.h>

#define LCM_Data  P0		  //��P0�ڶ���ΪLCM_Data
#define uchar unsigned char
#define uint  unsigned int
#define w 6				//��������λ��

//1602�Ŀ��ƽ�
sbit lcd1602_rs=P2^7;
sbit lcd1602_rw=P2^6;
sbit lcd1602_en=P2^5;

sbit Scl=P3^4;			//24C02����ʱ��
sbit Sda=P3^5;			//24C02��������

sbit ALAM = P2^1;		//����
sbit KEY = P3^6;		//����

bit  operation=0;		//������־λ
bit  pass=0;			//������ȷ��־
bit  ReInputEn=0;		//�������������־
bit  s3_keydown=0;		//3�밴����־λ
bit  key_disable=0;		//�������̱�־
bit  flag_REC =0;       //���ڽ��ܱ�־λ
unsigned char buff[5]=0;
unsigned char countt0,second;	//t0�жϼ�����,�������

void Delay5Ms(void);		//������ʱ����

unsigned char code a[]={0xFE,0xFD,0xFB,0xF7}; 				//����ɨ����Ʊ�

//Һ����ʾ��������
unsigned char code start_line[]	= {"password:       "};
unsigned char code name[] 	 	= { "===Coded Lock==="};	//��ʾ����
unsigned char code Correct[] 	= {"     correct    "};		//������ȷ
unsigned char code Error[]   	= {"      error     "};  	//�������
unsigned char code codepass[]	= {"      pass      "};
unsigned char code LockOpen[]	= {"      open      "};		//OPEN
unsigned char code SetNew[] 	= {"SetNewWordEnable"};
unsigned char code Input[]   	= {"input:          "};		//INPUT
unsigned char code ResetOK[] 	= {"ResetPasswordOK "};
unsigned char code initword[]	= {"Init password..."};
unsigned char code Er_try[]		= {"error,try again!"};
unsigned char code again[]		= {"input again     "};
unsigned char code Dynamic[]		= {"input dynamic   "};

unsigned char InputData[16];									//���������ݴ���
unsigned char DynamicData[7]={'P','1','1','1','1','1','1'};///////////////////////
unsigned char CurrentPassword[16]={'1','3','1','4','2','0'}; 			//����Ա���루ֻ���ڳ������޸ģ�
unsigned char TempPassword[16];
unsigned char N=0,M=0,flag=0;				//��������λ������
unsigned char ErrorCont;			//�����������
unsigned char CorrectCont;			//��ȷ�������
unsigned char DynamicCont;			//��ȷ�����?
unsigned char ReInputCont; 			//�����������
unsigned char code initpassword[16]={'0','0','0','0','0','0'};			//�������Ա����������ʼΪ000000
unsigned char MM[2]={0,0};

//=====================5ms��ʱ==============================
void Delay5Ms(void)
{
	unsigned int TempCyc = 5552;
	while(TempCyc--);
}

//===================400ms��ʱ==============================
void Delay400Ms(void)
{
	unsigned char TempCycA = 5;
	unsigned int TempCycB;
	while(TempCycA--)
	{
		TempCycB=7269;
		while(TempCycB--);
	}
}

//=============================================================================================
//================================24C02========================================================
//=============================================================================================

void mDelay(uint t) //��ʱ
{
	uchar i;
	while(t--)
	{
		for(i=0;i<125;i++)
		{;}
	}
}

void Nop(void)		  //�ղ���
{
	_nop_();		  //������ʱ��һ������Լ1us
 	_nop_();
 	_nop_();
 	_nop_();
}

/*****24c02�������24c02ʱ��ͼ*****/
/*��ʼ����*/

void Start(void)
{
 	Sda=1;
 	Scl=1;
 	Nop();
 	Sda=0;
 	Nop();
}

/*ֹͣ����*/
void Stop(void)
{
	Sda=0;
 	Scl=1;
 	Nop();
 	Sda=1;
 	Nop();
}

/*Ӧ��λ*/
void Ack(void)
{
	Sda=0;
	Nop();
	Scl=1;
	Nop();
	Scl=0;
}

/*����Ӧ��λ*/
void NoAck(void)
{
 	Sda=1;
 	Nop();
 	Scl=1;
 	Nop();
 	Scl=0;
}

/*���������ӳ���DataΪҪ���͵�����*/
void Send(uchar Data)
{
	uchar BitCounter=8;
	uchar temp;
	do
	{
		temp=Data;					   //�������������ݴ�temp
		Scl=0;
		Nop();
		if((temp&0x80)==0x80)		   //������������&0x80
		Sda=1;
		else
		Sda=0;
		Scl=1;
		temp=Data<<1;				   //��������
		Data=temp;					   //�������ƺ����¸�ֵData
		BitCounter--;				   //�ñ�������0ʱ������Ҳ�ʹ��������
	}
	while(BitCounter);				   //�ж��Ƿ������
	Scl=0;
}

/*��һ�ֽڵ����ݣ������ظ��ֽ�ֵ*/
uchar Read(void)
{
	uchar temp=0;
	uchar temp1=0;
	uchar BitCounter=8;
	Sda=1;
	do
	{
		Scl=0;
		Nop();
		Scl=1;
		Nop();
		if(Sda)				   //����λ�Ƿ�Ϊ1
			temp=temp|0x01;	   //Ϊ1 temp�����λΪ1��|0x01�����ǽ����λ��Ϊ1��
		else				   //���Ϊ0
			temp=temp&0xfe;	   //temp���λΪ0��&0xfe��11111110�����λ����0��
		if(BitCounter-1)	   //BitCounter��1���Ƿ�Ϊ��
		{
			temp1=temp<<1;	   //temp����
			temp=temp1;
		}
		BitCounter--;		   //BitCounter����0ʱ�����ݾͽ�������
	}
	while(BitCounter);		   //�ж��Ƿ�������
	return(temp);
}

void WrToROM(uchar Data[],uchar Address,uchar Num)
{
	uchar i;
	uchar *PData;
	PData=Data;
	for(i=0;i<Num;i++)
	{
		Start();
		Send(0xa0);
		Ack();
		Send(Address+i);
		Ack();
		Send(*(PData+i));
		Ack();
		Stop();
		mDelay(20);
	}
}

void RdFromROM(uchar Data[],uchar Address,uchar Num)
{
	uchar i;
	uchar *PData;
	PData=Data;
	for(i=0;i<Num;i++)
	{
		Start();
		Send(0xa0);
		Ack();
		Send(Address+i);
		Ack();
		Start();
		Send(0xa1);
		Ack();
		*(PData+i)=Read();
		Scl=0;
		NoAck();
		Stop();
	}
}

//==================================================================================================
//=======================================LCD1602====================================================
//==================================================================================================

#define yi 0x80 //LCD��һ�еĳ�ʼλ��,��ΪLCD1602�ַ���ַ��λD7�㶨Ϊ1��100000000=80��
#define er 0x80+0x40 //LCD�ڶ��г�ʼλ�ã���Ϊ�ڶ��е�һ���ַ�λ�õ�ַ��0x40��

//----------------��ʱ���������澭������----------------------
void delay(uint xms)//��ʱ�������вκ���
{
	uint x,y;
	for(x=xms;x>0;x--)
	for(y=110;y>0;y--);
}

//--------------------------дָ��---------------------------
void write_1602com(uchar com)//****Һ��д��ָ���****
{
	lcd1602_rs=0;//����/ָ��ѡ����Ϊָ��
	lcd1602_rw=0; //��дѡ����Ϊд
	P0=com;//��������
	delay(1);
	lcd1602_en=1;//����ʹ�ܶˣ�Ϊ������Ч���½�����׼��
	delay(1);
	lcd1602_en=0;//en�ɸ߱�ͣ������½��أ�Һ��ִ������
}

//-------------------------д����-----------------------------
void write_1602dat(uchar dat)//***Һ��д�����ݺ���****
{
	lcd1602_rs=1;//����/ָ��ѡ����Ϊ����
	lcd1602_rw=0; //��дѡ����Ϊд
	P0=dat;//��������
	delay(1);
	lcd1602_en=1; //en�øߵ�ƽ��Ϊ�����½�����׼��
	delay(1);
	lcd1602_en=0; //en�ɸ߱�ͣ������½��أ�Һ��ִ������
}

//-------------------------��ʼ��-------------------------
void lcd_init(void)
{
	write_1602com(0x38);//����Һ������ģʽ����˼��16*2����ʾ��5*7����8λ����
	write_1602com(0x0c);//����ʾ����ʾ���
	write_1602com(0x06);//�������ƶ�������Զ�����
	write_1602com(0x01);//����ʾ
}
//========================================================================================
//=========================================================================================

//==============������ֵ����Ϊ��ֵ=========================
unsigned char coding(unsigned char m)
{
	unsigned char k;
	
	switch(m)
	{
		case (0x11):
			k='1';
			break;
		case (0x21):
			k='2';
			break;
		case (0x41):
			k='3';
			break;
		case (0x81):
			k='A';
			break;
		case (0x12):
			k='4';
			break;
		case (0x22):
			k='5';
			break;
		case (0x42):
			k='6';
			break;
		case (0x82):
			k='B';
			break;
		case (0x14):
			k='7';
			break;
		case (0x24):
			k='8';
			break;
		case (0x44):
			k='9';
			break;
		case (0x84):
			k='C';
			break;
		case (0x18):
			k='*';
			break;
		case (0x28):
			k='0';
			break;
		case (0x48):
			k='#';
			break;
		case (0x88):
			k='D';
			break;
	}
	return(k);
}

//=====================������Ⲣ���ذ���ֵ===============================
unsigned char keynum(void)
{
	unsigned char row,col,i;
	
	if(flag_REC==1)				    //
	{
		flag_REC=0;
		if(buff[0]=='O'&&buff[1]=='N')	//��һ���ֽ�ΪO���ڶ����ֽ�ΪN���������ֽ�Ϊ������
			switch(buff[2])
			{
				case '1' :
					return(0x11);
					break;
				case '2':
					return(0x21);
					break;
				case '3':
					return(0x41);
					break;
				case '4':
					return(0x12);
					break;
				case '5':
					return(0x22);
					break;
				case '6':
					return(0x42);
					break;
				case '7':
					return(0x14);
					break;
				case '8':
					return(0x24);
					break;
				case '9':
					return(0x44);
					break;
				case 'A':
					return(0x84);
					break;
				case 'B':
					return(0x18);
					break;
				case 'C':
					return(0x48);
					break;
				case 'D':
					return(0x88);
					break;
				case 'E':
					return(0x28);
					break;
			}
	}
	P1=0xf0;
 	if((P1&0xf0)!=0xf0)
 	{
		Delay5Ms();
		Delay5Ms();
		if((P1&0xf0)!=0xf0)
		{
			row=P1^0xf0;          //ȷ������
			i=0;
			P1=a[i];	          //��ȷ��λ
			while(i<4)
			{
	 			if((P1&0xf0)!=0xf0)
				{
					col=~(P1&0xff);	  //ȷ������
					break;            //�Ѷ�λ����ǰ�˳�
				}
				else
				{
					i++;
					P1=a[i];
				}
			}
		}
		else
		{
			return 0;
		}

		while((P1&0xf0)!=0xf0);

		return (row|col);	 		//������������Ϻ󷵻�
 	}
 	else return 0;	         		//�޼�����ʱ����0

}

//=======================һ����ʾ������ʾ��Ч����========================
void OneAlam(void)
{
	ALAM=0;
	Delay5Ms();
	ALAM=1;
}

//========================������ʾ������ʾ�����ɹ�========================
void TwoAlam(void)
{
	ALAM=0;
	Delay5Ms();
	ALAM=1;
	Delay5Ms();
	ALAM=0;
	Delay5Ms();
	ALAM=1;
}

//========================������ʾ��,��ʾ����========================
void ThreeAlam(void)
{
	ALAM=0;
	Delay5Ms();
	ALAM=1;
	Delay5Ms();
	ALAM=0;
	Delay5Ms();
	ALAM=1;
	Delay5Ms();
	ALAM=0;
	Delay5Ms();
	ALAM=1;
}

//=======================��ʾ��ʾ����=========================
void DisplayChar(void)
{
	unsigned char i;
	
	if(pass==1)
	{
		write_1602com(er);				   //�ڶ��п�ʼ��ʾ
		for(i=0;i<16;i++)
		{
			write_1602dat(LockOpen[i]);	   //��ʾopen �����ɹ�
		}
	}
	else
	{
		if(N==0)
		{
			write_1602com(er);
			for(i=0;i<16;i++)
			{
				write_1602dat(Error[i]);	//��ʾ����
			}
		}
		else
		{
			write_1602com(er);
			for(i=0;i<16;i++)
			{
				write_1602dat(start_line[i]);//��ʾ��ʼ����
			}
		}
	}
}

//========================��������==================================================
//==================================================================================
void ResetPassword(void)
{
	unsigned char i;
	unsigned char j;
	
	write_1602com(yi);//������ʾ�̶����Ŵӵ�һ�е�0��λ��֮��ʼ��ʾ
	for(i=0;i<16;i++)
	{
		write_1602dat(SetNew[i]);//��Һ����д��������
	}
	if(pass==0)
	{
		pass=0;
		DisplayChar();	   //��ʾ����
		ThreeAlam();	   //û����ʱ�����������뱨��3��
	}
	else				   //����״̬�²��ܽ����������ó���
	{
		if(ReInputEn==1)   //����״̬�£�ReInputEn��1��������������
		{
			if(N==M)	   //����6λ����
			{
				ReInputCont++;			//�����������
				if(ReInputCont==2)		//������������
				{
					for(i=0;i<M;)
					{
						if(TempPassword[i]==InputData[i])	//��������������������Ա�
							i++;
						else								//������ε����벻ͬ
						{
							write_1602com(er);
							for(j=0;j<16;j++)
							{
								write_1602dat(Error[j]);	//��ʾ����Error
							}
							ThreeAlam();			//������ʾ
							pass=0;					//����
							ReInputEn=0;			//�ر����ù��ܣ�
							ReInputCont=0;
							DisplayChar();
							break;
						}
					}
					if(i==M)
					{
						write_1602com(er);
						for(j=0;j<16;j++)
						{
							write_1602dat(ResetOK[j]);	  //�����޸ĳɹ�����ʾ
						}

						TwoAlam();				//�����ɹ���ʾ
						MM[0]=M;
					 	WrToROM(TempPassword,0,M);		//��������д��24C02�洢
						WrToROM(MM,17,1);
						ReInputEn=0;
					}
					ReInputCont=0;
					CorrectCont=0;
				}
				else					  //����һ������ʱ
				{
					OneAlam();
					write_1602com(er);
					for(j=0;j<16;j++)
					{
						write_1602dat(again[j]);			//��ʾ������һ��
					}
					for(i=0;i<M;i++)
					{
						TempPassword[i]=InputData[i];		//����һ������������ݴ�����
					}
				}

				N=0;						//��������λ������������
			}
		}
	}
}

//=======================����������󳬹���������������������======================
void Alam_KeyUnable(void)
{
	P1=0x00;
	{
		ALAM=~ALAM;				 //������һֱ��˸����
		Delay5Ms();
	}
}

//=======================ȡ�����в���============================================
void Cancel(void)
{
	unsigned char i;
	unsigned char j;

	write_1602com(yi);//������ʾ�̶����Ŵӵ�һ�е�0��λ��֮��ʼ��ʾ
	for(i=0;i<16;i++)
	{
		write_1602dat(name[i]);//��Һ����д��������
	}
	write_1602com(er);
	for(j=0;j<16;j++)
	{
		write_1602dat(start_line[j]);	  //��ʾ���������������
	}
	TwoAlam();				//��ʾ��
	for(i=0;i<16;i++)
	{
		InputData[i]=0;		//��������������
	}
	KEY=1;					//�ر���
	ALAM=1;					//������
	operation=0;			//������־λ����
	pass=0;					//������ȷ��־����
	ReInputEn=0;			//������������־����
	ErrorCont=0;			//������������������
	CorrectCont=0;			//������ȷ�����������
	ReInputCont=0;			//�������������������

	s3_keydown=0;
	key_disable=0;			//�������̱�־����
	N=0;					//����λ������������
}

//==========================ȷ�ϼ�����ͨ����Ӧ��־λִ����Ӧ����===============================
void Ensure(void)
{
	unsigned char i,j,k,t;
	
	write_1602com(yi);//������ʾ�̶����Ŵӵ�һ�е�0��λ��֮��ʼ��ʾ
	for(i=0;i<16;i++)
	{
		write_1602dat(name[i]);//��Һ����д��������
	}
	RdFromROM(MM,17,1);
	M=MM[0];
	if(M==0)
	M=6;
	RdFromROM(CurrentPassword,0,M); 					//��24C02������洢����
		
	//��ʼ
	if(N==6)
	{
		if((InputData[0]=='1')&&(InputData[1]=='3')&&(InputData[2]=='1')&&(InputData[3]=='4')&&(InputData[4]=='2')&&(InputData[5]=='0'))
		{
			flag=1;
			MM[0]=6;
			WrToROM(MM,17,1);
			WrToROM(initpassword,0,6); 				//ǿ�ƽ���ʼ����д��24C02�洢
			write_1602com(er);
			for(j=0;j<16;j++)
			{
				write_1602dat(initword[j]);				//��ʾ��ʼ������
			}
			TwoAlam();									//�ɹ���ʾ��
			Delay400Ms();								//��ʱ400ms
			TwoAlam();									//�ɹ���ʾ��
			N=0;										//����λ������������
		}
		else
		flag=0;
	}

	if(N==M&&N!=0)
	{
	  if(ReInputEn==0)							//�������빦��δ����
		{
			if(CorrectCont==0)
			{
				for(i=0;i<M;)
				{
					if(CurrentPassword[i]==InputData[i])	//�ж����������24c02�е������Ƿ���ͬ
					{
						i++;								//��ͬһλ i��+1
					}
					else 									//��������벻ͬ
					{
						ErrorCont++;						//�������++
						if(ErrorCont==3)			//�����������������ʱ����������������
						{
							write_1602com(er);
							for(i=0;i<16;i++)
							{
								write_1602dat(Error[i]);
							}
							do
							Alam_KeyUnable();
							while(1);
						}
						else					 //�������С��3��ʱ����������3�룬Ȼ�����¿�������
						{
							write_1602com(er);
							for(i=0;i<16;i++)
							{
								write_1602dat(Error[i]);
							}
							ThreeAlam();
							pass=0;					//passλ����
							break;					//����
						}
					}
				}
				
				if(i==M)					 //���������ʱ
				{
					CorrectCont++;
					for(j=0;j<M;j++)								//���������
					{
						InputData[j]=0;								//����������λ����
					}
					write_1602com(yi);
					for(j=0;j<16;j++)
					{
						write_1602dat(Dynamic[j]);				//��ʾ��ʼ������
					}
					write_1602com(er);
					for(j=0;j<16;j++)
					{
						write_1602dat(' ');				//��ʾ��ʼ������
					}					
					TwoAlam();
				}	
			}				
			else
			{
				for(j=0;j<7;j++)
				{
					if(DynamicData[j+1]==InputData[j])	//�ж����������24c02�е������Ƿ���ͬ
					{
						k++;								//��ͬһλ i��+1
					}
						
				}
				if(k==6)
				{
					write_1602com(er);
					for(t=0;t<16;t++)
					{
						write_1602dat(LockOpen[t]);		  //��ʾopen��������
					}
					CorrectCont=0;
					k=0;
					TwoAlam();			//�����ɹ���ʾ��
					ReInputEn=1;									//����������������
					KEY=0;											//����
					pass=1;											//����ȷ��־λ
					TR0=1;											//������ʱ						
					for(t=0;t<16;t++)								//���������
					{
						InputData[t]=0;								//����������λ����
					}										
				}
				else
				{
					ErrorCont++;						//�������++
					if(ErrorCont==3)			//�����������������ʱ����������������
					{
						write_1602com(er);
						for(j=0;j<16;j++)
						{
							write_1602dat(Error[j]);
						}
						do
						Alam_KeyUnable();
						while(1);
					}
					else					 //�������С��3��ʱ����������3�룬Ȼ�����¿�������
					{
						write_1602com(er);
						for(j=0;j<16;j++)
						{
							write_1602dat(Error[j]);
						}
						ThreeAlam();
						pass=0;					//passλ����
					}
				}
			}

		}
		else											//���Ѿ������������빦��ʱ�������¿�������
		{
			write_1602com(er);
			for(j=0;j<16;j++)
			{
				write_1602dat(Er_try[j]);			  //��������������
			}
			ThreeAlam();							  //������ʾ��
		}
	}

	if(N<M&&flag!=1&&N!=0)
	{
		write_1602com(er);
		for(j=0;j<16;j++)
		{
			write_1602dat(Error[j]);		 //��ʾ����
		}

 		ThreeAlam();										//������ʾ��
		pass=0;
	}
	N=0;													//���������ݼ��������㣬Ϊ��һ��������׼��
	operation=1;
}

//==============================������===============================
void main(void)
{
 	unsigned char KEY,NUM;
	unsigned char i,j;
	
 	P1=0xFF; 				   //P1�ڸ�λ
	TMOD=0x21;				   //���幤����ʽ
 	TL0=0xB0;
 	TH0=0x3C;				   //��ʱ������ֵ
 	EA=1;					   //���ж��ܿ���
 	ET0=1;					   //���ж�������
 	TR0=0;					   //�򿪶�ʱ������
	TH1=0Xfd;
	TL1=0Xfd;	    //9600
	TR1=1; 		    // ��ʱ��1����
	SM0=0;	    // ���ô��ڵĹ���ģʽ
	SM1=1; 	    //��ʽ1
	REN=1; 		 // �����ڽ�������
	ES=1; 	     // �����ж�Ӧ��

 	Delay400Ms(); 	//�����ȴ�����LCM���빤��״̬
 	lcd_init(); 	//LCD��ʼ��
	write_1602com(yi);//������ʾ�̶����Ŵӵ�һ�е�0��λ��֮��ʼ��ʾ
	for(i=0;i<16;i++)
	{
		write_1602dat(name[i]);//��Һ����д��������
	}
	write_1602com(er);
	for(i=0;i<16;i++)
	{
		write_1602dat(start_line[i]);//д��������ȴ�����
	}
	write_1602com(er+9);	//���ù��λ��
	write_1602com(0x0f);	//���ù��Ϊ��˸
 	Delay5Ms(); //��ʱƬ��(�ɲ�Ҫ)

 	N=0;														//��ʼ����������λ��
	RdFromROM(MM,17,1);
	M=MM[0];
	if(M==0||M>16)
	{
		M=6;
		MM[0]=6;
		WrToROM(MM,17,1);
	}
 	while(1)		 //����ѭ��
 	{
		if(key_disable==1)						//�������̱�־Ϊ1ʱ
			Alam_KeyUnable();					//����������
		else
			ALAM=1;								//�ر���

		KEY=keynum();							//��������λ����
		if(KEY!=0)								//���а�������ʱ
		{
			if(key_disable==1)					//�������̱�־Ϊ1ʱ
			{
				second=0;						//������
			}
			else								//û����������ʱ
			{
				NUM=coding(KEY);				//���ݰ�����λ�ý�����룬����ֵ��ֵ��NUM
				{
					switch(NUM)					//�жϰ���ֵ
					{
						case ('A'):
							break;
						case ('B'):
							break;
						case ('C')://ɾ����
							write_1602com(yi);//������ʾ�̶����Ŵӵ�һ�е�0��λ��֮��ʼ��ʾ
							for(i=0;i<16;i++)
							{
								write_1602dat(name[i]);//��Һ����д��������
							}
							if(N>=1) N--;	OneAlam();								//������ʾ��
							for(j=N;j<16;j++)
							{
								write_1602com(er+j);
							  write_1602dat(' ');
							}
						 	for(j=0;j<N;j++)
							{
								write_1602com(er+j);				//��ʾλ�����������Ӷ�����
								write_1602dat('*');					//������ʾʵ�����֣���*����
							}
							InputData[N]=N+4;
							break;		//ABC���޶��尴��
						case ('D'):
							ResetPassword();
							break;      //������������
						case ('*'):
							Cancel();
							break;      //ȡ����ǰ����
						case ('#'):
							Ensure();
							break;   	//ȷ�ϼ���
						default: 										//������ǹ��ܼ�����ʱ���������ּ�����
						{
							write_1602com(yi);
							for(i=0;i<16;i++)
							{
								write_1602dat(Input[i]);				//��ʾ���뻭��
							}
						  operation=0;								//������־����
							if(N<16)                   					//���������������6λʱ���������벢���棬����6λʱ����Ч��
							{
								OneAlam();								//������ʾ��
						 		for(j=0;j<16;j++)
								{
									if(j>N)
									{
									  write_1602com(er+j);
									  write_1602dat(' ');
									}
									if(j<=N)
									{
										write_1602com(er+j);				//��ʾλ�����������Ӷ�����
										write_1602dat('*');					//������ʾʵ�����֣���*����
									}
								}
								InputData[N]=NUM;						//�����ּ����븳ֵ��InputData[]�����ݴ�			
								N++;									//����λ����
							}
							else										//��������λ������6�󣬺�������
							{
								N=16;									//�����������6λʱ������������
						 		break;
							}
							write_1602com(er+N);
							if(ReInputEn==1&&ReInputCont==0)
							M=N;
						}
					}
				}
			}
	 	}
	}
}

//*********************************�жϷ�����**************************************
void  time0_int(void) interrupt 1 		   //��ʱ��T0
{
 	TL0=0xB0;
 	TH0=0x3C;		  //��ʱ�����¸���ֵ
 	//TR0=1;
 	countt0++;			   //��ʱ�����ӣ���1��ʱ50ms
	if(countt0==20)		   //�ӵ�20�ξ���1s
	{
		countt0=0;		   //��������
		second++;		   //���
		if(pass==1)		   //����״̬ʱ
		{
			if(second==1)			//��ӵ�1sʱ
			{
				TR0=0;				//�ض�ʱ��
				TL0=0xB0;
 				TH0=0x3C;			//�ٴθ���ֵ
				second=0;			//������
			}
		}
		else						//���ڿ���״̬ʱ
		{
			if(second==3)			//��ӵ�3ʱ
			{
				TR0=0;				//�رն�ʱ��
				second=0;			//������
				key_disable=0;		//������������
				s3_keydown=0;
				TL0=0xB0;
				TH0=0x3C;			//���¸���ֵ
			}
			else
			TR0=1;				//�򿪶�ʱ��
		}
	}
}

//  ͨѶ�жϽ��ճ���   �жϺ����޷���ֵ
void uart_rx(void)  interrupt 4	using 3	  //�������� �ͷ���main����������һ����
{
	unsigned char i;
	unsigned char j;
	unsigned char flag;
	unsigned char dat;
	
	if(RI)	                 //�Ƿ�����ж�
	{
		RI=0;
		dat=SBUF;
		if(dat=='O'&&(i==0)) //�������ݵ�һ֡
		{
			buff[i]=dat;
			flag=1;        //��ʼ��������
		}
		else if(flag==1)
		{
			i++;
			buff[i]=dat;					
			if(i>=2)
			{
				i=0;
				flag=0;
				flag_REC=1 ;		
			}  // ֹͣ����
		}	
		
		if(dat=='P'&&(j==0)) //�������ݵ�һ֡
		{
			DynamicData[j]=dat;
			flag=2;        //��ʼ������?				
		}
		else if(flag==2)
		{
			j++;
			DynamicData[j]=dat;
			if(j>=7)
			{							
				j=0;
				flag=0;
				flag_REC=3 ;												
			}  // ֹͣ����
		}				
	}
}
