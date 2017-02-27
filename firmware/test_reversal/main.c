/*
 * main.c
 *
 *  Created on: 27.02.2017
 *      Author: Medvedev
 */

void main(){
	//get wx, wy, wz
	double wx, wy, wz;

	const double pi= 3.14;
	double dt;
	double R0, R1, R2, R3;				//���������� � t0
	double P1, P2, P3, P0;				//���������� ��������������� �� dt
	double SP, SR;
	double Q0, Q1, Q2, Q3;				// ���������� � t0+dt
	double u1, u2, u3; 					//����������� �������
	double k1, k2;						//������������ ����������
	int Mmx, Mpx, Mmy, Mpy, Mmz, Mpz; 	//���

	P1 = wx*dt*0.5;
	P2 = wy*dt*0.5;
	P3 = wz*dt*0.5;
	SP = P1*P1 + P2*P2 + P3*P3;
	SR = R0*R0 + R1*R1 + R2*R2 + R3*R3;
	P0 = (3 - SP - SR)*0.5;

	//������ ���������� ��������������� �� ����� dt
	Q0=R0*P0 - R1*P1 - R2*P2 - R3*P3;
	Q1=R0*P1 + R1*P0 + R2*P3 - R3*P2;
	Q2=R0*P2 + R2*P0 + R3*P1 - R1*P3;
	Q3=R0*P3 + R3*P0 + R1*P2 - R2*P1;

	//������������ ������������ ������� u

	/*u1=k1*2*Q0*Q1 + k2 * wx;
	u2=k1*2*Q0*Q2 + k2 * wy;
	u3=k1*2*Q0*Q3 + k2 * wz;*/

	u1 = 0;
	u2 = 0;
	u3 = -(k1*wz + k2*(Q0*Q3-(pi/2)));

	//������������ ��������
	if (u1>0){
		Mmx = 0;
		Mpx = 1;
	}
	else if (u1<0){
		Mmx = 1;
		Mpx = 0;
	}
	else{
		Mmx = 0;
		Mpx = 0;
	}

	if (u2>0){
		Mmy = 0;
		Mpy = 1;
	}
	else if (u2<0){
		Mmy = 1;
		Mpy = 0;
	}
	else{
		Mmy = 0;
		Mpy = 0;
	}

	if (u3>0){
		Mmz = 0;
		Mpz = 1;
	}
	else if (u3<0){
		Mmz = 1;
		Mpz = 0;
	}
	else{
		Mmz = 0;
		Mpz = 0;
	}

	//send Mmx, Mpx, Mmy, Mpy, Mmz, Mpz

}
