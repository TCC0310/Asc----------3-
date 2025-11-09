/*******************************************************************************
 * @file        VOFA+�Ĵ����շ�
 * @author      eternal_fu
 * @version     V0.0.1
 * @brief       
 * @Date 2024-12-11 01:05:50
 * @LastEditTime 2024-12-11 04:28:17
 * @**************************************************************************** 
 * @attention   �޸�˵��
 * @    �汾��      ����                ����      ˵��
 *      0.0.1       2024��12��11��      fu      ��֧��FireWater�������桢�շ�����
 *******************************************************************************/
#include "uart_vofa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // �����ַ�������������ͷ�ļ�
#include "pid_demo.h"

/* VOFA����������� */
uint8_t g_ucVofaBuf[VOFA_DATAPACK_MAXLEN];

static float vofa_get_data(uint8_t, uint8_t);
static void vofa_set_sram_data(uint8_t, float);
static void vofa_set_flash_data(float);

/* ����������� */

/*******************************************************************************
 * @brief   ͨ��������VOFA+�������ݣ��������
 * @return {*}
 * @note    none
 * @param {uint8_t} _dataflag   ��ǰ��ӡ���������[��־λ]
 *******************************************************************************/
void vofa_draw_graphical(uint8_t _dataflag)
{
/*
    FireWaterЭ���ʽ:
        "<any>:ch0,ch1,ch2,...,chN\n"
        - any��ð�ſ���Ϊ�գ�������(\n)����ʡ�ԣ�
        - any������Ϊ"image"�����ǰ׺���ڽ���ͼƬ���ݣ�
        - �˴�\nΪ���У�����ָ�ַ�б��+�ַ�n��
        - \nҲ����Ϊ\n\r����\r\n��
    ����:
        VOFA_printf("%.0f,"		
					"%.0f,"
					"%.4f,"
					"%.4f,"
                    "%.4f\r\n"
					,dis_pid.target, dis_pid.current, dis_pid.kp, dis_pid.kd,  dis_pid.out);
        ����4������:
        VOFA_printf("channels: 1.386578,0.977929,-0.628913,-0.942729\n");
        VOFA_printf("1.386578,0.977929,-0.628913,-0.942729\n")
*/

	/* ����дһ��ö�٣�ͨ��ö��ѡ���ӡָ������ */
    switch(_dataflag)
	{
        /* ����-�ٶȻ� */
		case PID_LEFT_MOTOR:
            VOFA_printf("%.2f,%.2f,%.2f,%2f\r\n", 
						g_tAnglePID.current, g_tAnglePID.target, g_tAnglePID.kp, g_tAnglePID.kd);
			break;
		/* �ǶȻ� */
		case PID_ANGLE:
            VOFA_printf("%.2f,%.2f,%.2f,%2f\r\n",
						g_tAnglePID.current, g_tAnglePID.target, g_tAnglePID.kp, g_tAnglePID.kd);
			break;
/*		 ����-�ٶȻ� 
		case PID_LEFT_MOTOR:
            VOFA_printf("%.2f,%.2f,%.2f,%2f\r\n", car_pid_kp, turn_pid_kp, now_speed, aim_speed);
			now_speed+=0.02;
			break;*/
        default:
            break;
	}
#if 0 /* ��������ο�(��ɾ��) */
    switch(_dataflag)
	{
		case _left_motor_pid:
			//�����ٶȻ��������ͺͽ��ա�
			VOFA_printf("%.2f,"		
						"%.2f,"
						"%.2f,"
						"%.2f\r\n"
						,left_Motor.pid.target, left_Motor.pid.current, left_Motor.pid.kp, left_Motor.pid.ki);
			break;
		case _right_motor_pid:
			//�����ٶȻ��������ͺͽ��ա�
			VOFA_printf("%f,"
						"%f,"
						"%f,"
						"%f\r\n"
						,right_Motor.pid.target, right_Motor.pid.current, right_Motor.pid.kp, right_Motor.pid.ki);
			break;
		case _turn_pid:
			//Ѳ��ת��(����ͷ)��
			VOFA_printf("%.2f,"		
						"%.2f,"
						"%.2f\r\n"
						,turn_pid.target, turn_pid.current, turn_pid.kp);
			break;
		case _angle_pid:
			//������ת�򻷣�
			VOFA_printf("%.0f,"		
						"%.0f,"
						"%.2f,"
						"%.0f\r\n"
						,angle_pid.target, angle_pid.current, angle_pid.kp, angle_pid.kd);
			break;
		case _track_pid:
			//Ѳ�ߣ��Ҷȣ�ת�򻷣�
			VOFA_printf("%.0f,"		
						"%.0f,"
						"%.5f,"
						"%.0f\r\n"
						,track_pid.target, track_pid.current, track_pid.kp, track_pid.kd);
			break;
        case _dis_pid:
			//�Ʋ�����
			VOFA_printf("%.0f,"		
						"%.0f,"
						"%.4f,"
						"%.4f,"
                        "%.4f\r\n"
						,dis_pid.target, dis_pid.current, dis_pid.kp, dis_pid.kd,  dis_pid.out);
			break;
	}
#endif
}

/*******************************************************************************
 * @brief   ���˺������Ӵ��ڴ����ֽ����ĳ�����(�紮���жϽ�����)
 *          ���������յ���VOFA+�������������ִ�ж�Ӧ����
 * @return {*}
 * @note    Ŀǰ��ʵ����VOFA�������ݵĽ���
 *******************************************************************************/
void vofa_set_data(uint8_t _rx_byte)
{
    static uint8_t end_pos = 0;         // ����vofa����������¼β��λ�á����鵱ǰλ��
    static uint8_t head_pos = 0;

    /* �������� */
    g_ucVofaBuf[end_pos] = _rx_byte;

    if (_rx_byte == VOFA_DATAPACK_HEAD) {   /* �ҵ�֡ͷ */
        head_pos = end_pos;                 /* ���鵱ǰλ�þ���֡ͷ */
    }
    else if(_rx_byte == VOFA_DATAPACK_END && g_ucVofaBuf[head_pos] == VOFA_DATAPACK_HEAD) { /* �ҵ�֡β��֡ͷʱ */
        /* ����VOFA���� */
        float VofaData = vofa_get_data(head_pos, end_pos);
        
        /* �����꣬д��洢��SRAM�ı����� */
        vofa_set_sram_data(head_pos, VofaData);

        /* ��������������� */
        end_pos = 0;
        head_pos = 0;
        memset(g_ucVofaBuf, 0x00, VOFA_DATAPACK_MAXLEN);
    }

    /* ������������С����� */
    if(++end_pos > VOFA_DATAPACK_MAXLEN) {
        end_pos = 0;
        memset(g_ucVofaBuf, 0x00, VOFA_DATAPACK_MAXLEN);
    }
}

/*******************************************************************************
 * @brief   
 * @return {*}
 * @note    _head��֡ͷλ�ã�_end��֡βλ�ã�����Ϊ��ʼ��_head+1
 *          c��׼������, �����ͷ�ļ� stdlib.h
 *          - atof()�ִ�ת��Ϊ������
 *          - atoi()�ִ�ת��Ϊ����
 *******************************************************************************/
static float vofa_get_data(uint8_t _head, uint8_t _end)
{
    char _VofaData[_end - (_head + 1)];
	for(uint8_t i = 0; i < _end; i++)
		_VofaData[i] = g_ucVofaBuf[i + _head + 1]; 

    return atof(_VofaData);	/* �ַ���ת���� */
}

/*******************************************************************************
 * @brief   �����յ������ֱ��д��SRAM�ռ䣬���綪ʧ
 *          �����ǰѽ��յ���ֵ������Ҫ�ı�����
 * @return {*}
 * @note    �ڴ˴�������ص��˶�����
 * ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
 * |   ���ݺ�(���ڷ������ĸ���ʲô������,1-2byte)   |   ֡ͷ('=',1byte)   |   ����λ(n byte)   |   ��β('\n',1byte)   |
 * ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
 *   ���磺
 *       "P1=%.2f\n"
 *       "V1=%.2f\n"
 * 
 *  ���ǵ�P1/V1�����ݺš��𣬾����������ڷֱ���������˭��
 *  �����Ҳ���
 *      P1 - car_pid_kp
 *      P2 - turn_pid_kp
 *      I1 - car_pid_ki
 *		I2=%.2f\n
 *  �Դ����ƶ����ԣ�
 * 
 *******************************************************************************/
static void vofa_set_sram_data(uint8_t _head, float _data)
{
    uint8_t _id_pos1 = _head - 2;   /* ���ݺŵĵ�1λ - P/I/... */
    uint8_t _id_pos2 = _head - 1;   /* ���ݺŵĵ�2λ - 1/2/3/4/...*/
	
// �ǶȻ�[1] ������������������������������������������������������������������������
    /* P1 - g_tAnglePID.kp */
	if (g_ucVofaBuf[_id_pos1] == 'P' && g_ucVofaBuf[_id_pos2] == '1') { 
        g_tAnglePID.kp = _data;
    }
    /* D1 - g_tAnglePID.kd */
    else if (g_ucVofaBuf[_id_pos1] == 'D' && g_ucVofaBuf[_id_pos2] == '1') {
        g_tAnglePID.kd = _data;
    }
	/* T1 - g_tAnglePID.target */
    else if (g_ucVofaBuf[_id_pos1] == 'T' && g_ucVofaBuf[_id_pos2] == '1') {
        g_tAnglePID.target = _data;
    }
// �����ٶȻ�[2] ������������������������������������������������������������������������
    /* P2 - g_tLeftMotorPID.kp */
	else if (g_ucVofaBuf[_id_pos1] == 'P' && g_ucVofaBuf[_id_pos2] == '2') { 
        g_tLeftMotorPID.kp = _data;
    }
    /* I2 - g_tLeftMotorPID.ki */
    else if (g_ucVofaBuf[_id_pos1] == 'I' && g_ucVofaBuf[_id_pos2] == '2') {
        g_tLeftMotorPID.ki = _data;
    }
	/* T2 - g_tAnglePID.target */
    else if (g_ucVofaBuf[_id_pos1] == 'T' && g_ucVofaBuf[_id_pos2] == '2') {
        g_tLeftMotorPID.target = _data;
    }

    /* �Դ����� */
    /* ע�������int����-�ǵ�����������ת��(int) */
}

/*******************************************************************************
 * @brief   �����յ������ֱ��д��FLASH�ռ䣬���粻��ʧ
 *          ����ǰ��FLASH�ռ俪���˲����ռ䣬ʵ�����ڲ�FLASH��API��
 * @return {*}
 * @note    none
 *******************************************************************************/
static void vofa_set_flash_data(float _data)
{
    
}