#include "icm.h"


#define CPU_F ((double)4000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/5000.0))

struct _acc acc;
struct _acc filter_acc;
struct _gyro gyro;
struct _SI_float   nacc;
struct _gyro filter_gyro;
struct _out_angle out_angle;
struct _SI_float SI_acc;
struct _SI_float SI_gyro;
 
//�������¶Ȳ���
#define	ICM20_XG_OFFS_TC_H				0x04
#define	ICM20_XG_OFFS_TC_L				0x05
#define	ICM20_YG_OFFS_TC_H				0x07
#define	ICM20_YG_OFFS_TC_L				0x08
#define	ICM20_ZG_OFFS_TC_H				0x0A
#define	ICM20_ZG_OFFS_TC_L				0x0B
//���ٶ��Լ����(����ʱ���ã��������û����Լ����ֵ�Ƚϣ�
#define	ICM20_SELF_TEST_X_ACCEL			0x0D
#define	ICM20_SELF_TEST_Y_ACCEL			0x0E
#define	ICM20_SELF_TEST_Z_ACCEL			0x0F
//�����Ǿ�̬ƫ��
#define	ICM20_XG_OFFS_USRH				0x13
#define	ICM20_XG_OFFS_USRL				0x14
#define	ICM20_YG_OFFS_USRH				0x15
#define	ICM20_YG_OFFS_USRL				0x16
#define	ICM20_ZG_OFFS_USRH				0x17
#define	ICM20_ZG_OFFS_USRL				0x18

#define	ICM20_SMPLRT_DIV				0x19//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	ICM20_CONFIG					0x1A
#define	ICM20_GYRO_CONFIG				0x1B
#define	ICM20_ACCEL_CONFIG				0x1C
#define	ICM20_ACCEL_CONFIG2				0x1D
#define	ICM20_LP_MODE_CFG				0x1E

//�˶����Ѽ��ٶ���ֵ
#define	ICM20_ACCEL_WOM_X_THR			0x20
#define	ICM20_ACCEL_WOM_Y_THR			0x21
#define	ICM20_ACCEL_WOM_Z_THR			0x22


#define	ICM20_FIFO_EN					0x23
#define	ICM20_FSYNC_INT					0x36
#define	ICM20_INT_PIN_CFG				0x37
//#define	ICM20_INT_ENABLE				0x38
#define	ICM20_FIFO_WM_INT_STATUS		0x39
#define	ICM20_INT_STATUS				0x3A

//���ٶ����
#define	ICM20_ACCEL_XOUT_H				0x3B
#define	ICM20_ACCEL_XOUT_L				0x3C
#define	ICM20_ACCEL_YOUT_H				0x3D
#define	ICM20_ACCEL_YOUT_L				0x3E
#define	ICM20_ACCEL_ZOUT_H				0x3F
#define	ICM20_ACCEL_ZOUT_L				0x40
//�¶����
#define	ICM20_TEMP_OUT_H				0x41
#define	ICM20_TEMP_OUT_L				0x42
//���ٶ����
#define	ICM20_GYRO_XOUT_H				0x43
#define	ICM20_GYRO_XOUT_L				0x44
#define	ICM20_GYRO_YOUT_H				0x45
#define	ICM20_GYRO_YOUT_L				0x46
#define	ICM20_GYRO_ZOUT_H				0x47
#define	ICM20_GYRO_ZOUT_L				0x48
//�������Լ����
#define	ICM20_SELF_TEST_X_GYRO			0x50
#define	ICM20_SELF_TEST_Y_GYRO			0x51
#define	ICM20_SELF_TEST_Z_GYRO			0x52

#define	ICM20_FIFO_WM_TH1				0x60
#define	ICM20_FIFO_WM_TH2				0x61
#define	ICM20_SIGNAL_PATH_RESET			0x68
#define	ICM20_ACCEL_INTEL_CTRL 			0x69
#define	ICM20_USER_CTRL					0x6A
//��Դ����
#define	ICM20_PWR_MGMT_1				0x6B //��Դ��������ֵ��0x00(��������)
#define	ICM20_PWR_MGMT_2				0x6C

#define	ICM20_I2C_IF					0x70
#define	ICM20_FIFO_COUNTH				0x72
#define	ICM20_FIFO_COUNTL				0x73
#define	ICM20_FIFO_R_W					0x74

#define	ICM20_WHO_AM_I 					0x75
//���ٶȾ�̬ƫ��
#define	ICM20_XA_OFFSET_H				0x77
#define	ICM20_XA_OFFSET_L				0x78
#define	ICM20_YA_OFFSET_H				0x7A
#define	ICM20_YA_OFFSET_L				0x7B
#define	ICM20_ZA_OFFSET_H				0x7D
#define	ICM20_ZA_OFFSET_L 				0x7E
//===========================================================

#define CS_L()  P2OUT &= ~BIT0
#define CS_H()  P2OUT |= BIT0
#define ICM20602_ADDRES   0xD2

//��д����
void icm20602_write(unsigned char REG,unsigned char DATA)
{
	CS_L();//CS������ΪSPIģʽ
	spi_send(REG & 0x7f);//���ͼĴ�����ַ+д����
	spi_send(DATA);
	CS_H();
}

unsigned char icm20602_read(unsigned char REG)
{
	unsigned char data;
	CS_L();
	spi_send(REG | 0x80);//���ͼĴ�����ַ+������
	data = spi_send(0xff);
	CS_H();
	return data;
}

void icm20602_read_nbyte(unsigned char REG,unsigned char len,unsigned char *buf)
{
	CS_L();
	spi_send(REG | 0x80);//���ͼĴ�����ַ+������
	while(len--)
		*buf++ = spi_send(0xff);
	CS_H();
}

void ICM20602_rest(void)
{
	icm20602_write(ICM20_PWR_MGMT_1, 0x80);
	delay_ms(20);
}

unsigned char ICM20602_Init(void) //��ʼ��
{	
	spi_init();
	delay_ms(10);
	icm20602_write(  ICM20_PWR_MGMT_1, 0x80);	//��λ������
	delay_ms(100);//�ȴ���λ���
	icm20602_write(  ICM20_SMPLRT_DIV, 0x00);	//�����ǲ����ʣ����1kHZ
	delay_ms(10);
	icm20602_write(  ICM20_PWR_MGMT_1, 0x00);	//�����豸ʱ��Դ��������Z��  0x01
	//�¼����
	icm20602_write(  ICM20_PWR_MGMT_2, 0x00);//���3������//Ĭ��Ҳ���
	delay_ms(10);
	icm20602_write(  ICM20_CONFIG, 0x04);   //��ͨ�˲�Ƶ�ʣ�0x03(42Hz) 0x04(20HZ)
	delay_ms(10);//
	//��������
	icm20602_write(  ICM20_GYRO_CONFIG, (3 << 3));//+-2000deg/s 0x1B
	delay_ms(10);
	icm20602_write(  ICM20_ACCEL_CONFIG, (2 << 3));//+-8G  0x1C
	//���ٶȼ��˲����� 20hz
	//����
	icm20602_write(  ICM20_ACCEL_CONFIG2,0x04);
	//��������о���������Ƶ�ʺ���
	delay_ms(10);
        //FIFO ��Դ
	icm20602_write(  0x1E, 0x00);//�رյ͹���
	delay_ms(10);
	//����
	icm20602_write(  0x23, 0x00);//�ر�FIFO DMP����Ŵ� 
        delay_ms(10);
	if(icm20602_read(  ICM20_WHO_AM_I)!= 0x12) //�����ַ ����ȷ
		return 0;
	return 1;
}


void ICM20602_Updata_get(void) //��ȡ����
{
    uint8_t buffer[14];
	  //ICM20602_Offset();
    icm20602_read_nbyte(ICM20_ACCEL_XOUT_H, 14, buffer);
    acc.x = ((buffer[0] << 8)  | buffer[1] );
    acc.y = ((buffer[2] << 8)  | buffer[3] );
    acc.z = ((buffer[4] << 8)  | buffer[5] );
    gyro.x =((buffer[8] << 8)  | buffer[9] );
    gyro.y =((buffer[10] << 8) | buffer[11]);
    gyro.z= ((buffer[12] << 8) | buffer[13]);
}





