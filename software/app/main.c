#include <unistd.h>
#include "system.h"
#include "sys/alt_stdio.h"
#include <sys/alt_irq.h>
#include <alt_types.h>
#include <stdio.h>
#include "opencores_i2c.h"
#include "altera_avalon_timer_regs.h"
#include <io.h>
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"

#define add_OFSX 0x1E
#define add_OFSY 0x1F
#define add_OFSZ 0x20

  alt_16 axe_x=0;
  alt_16 axe_y=0;
  alt_16 axe_z=0;

  alt_16 x_1;
  alt_16 y_1;
  alt_16 z_1;

  alt_16 x_0;
  alt_16 y_0;
  alt_16 z_0;

  alt_8 axe_x_offset=3;
  alt_8 axe_y_offset=4;
  alt_8 axe_z_offset=0;

  int switch_axes=0;

//  int C2_offset( alt_16 axe){
//	int a;
//	a= axe*15.6 / 2000;
//	if (a>127) { a=a-256;}
//	a=~a+ 1;
//	return a;
//}

int Read_I2c (alt_8 adress){
	 int data=0;
	 I2C_start(OPENCORES_I2C_0_BASE,0x1D,0); // write mode
	 I2C_write(OPENCORES_I2C_0_BASE,adress,0);
	 I2C_start(OPENCORES_I2C_0_BASE,0x1D,1); // read mode
	 data=I2C_read(OPENCORES_I2C_0_BASE,1); // will read the input register and send stop
	 return data;
}

void Write_I2c (alt_8 value, alt_8 adress){

	I2C_start(OPENCORES_I2C_0_BASE,0x1D,0);
	I2C_write(OPENCORES_I2C_0_BASE,adress,0);
	I2C_write(OPENCORES_I2C_0_BASE,value,1);
}

void set_axes(){
	x_0=Read_I2c(0x32);
	x_1 = Read_I2c(0x33);
	axe_x=((x_1 <<8)| x_0);

	y_0=Read_I2c(0x34);
	y_1 = Read_I2c(0x35);
	axe_y=((y_1 <<8)|y_0);


	z_0=Read_I2c(0x36);
	z_1 = Read_I2c(0x37);
	axe_z=((z_1<<8)| z_0);
}

void Calibrate(){

	Write_I2c(axe_x_offset,add_OFSX);

	Write_I2c(axe_y_offset,add_OFSY);

	Write_I2c(axe_z_offset,add_OFSZ);
}

static void irqhandler_BP (void * context, alt_u32 id)
{
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BP_BASE, 0x01); //clear the interruption
	if (switch_axes==2) {switch_axes=0;}
	else
		switch_axes=switch_axes+1;
}
//////////////////////////////////////////Affihage de 7 segment////////////////////////////////////////////////////
void affichage_7seg (int valeur){
	int aff=0;
	if (valeur <0 ) {
		valeur = -1*valeur;
		aff = (valeur % 10) | (((valeur / 10) % 10) << 4) | (((valeur /100)%10) << 8) | (valeur/1000)<<12 | (1 <<16)  ;}
	else
		aff = (valeur % 10) | (((valeur / 10) % 10) << 4) | (((valeur /100)%10) << 8) | (valeur/1000)<<12 | (1 <<16)  ;
	IOWR_ALTERA_AVALON_PIO_DATA(AFFICHEUR_BASE,aff);
}

///////////////////////////////////////////timer////////////////////////////////////////////////////////////////////
static void irqhandler_TIMER_0(void* context,alt_u32 id){
	 //axe_x_offset=Read_I2c(add_OFSX);
	 //axe_y_offset=Read_I2c(add_OFSY);
	 //axe_z_offset=Read_I2c(add_OFSZ);
	set_axes();
		  //printf("axe_x_offset = %d , axe_y_offset=%d, axe_z_offset= %d \n",axe_x_offset,axe_y_offset,axe_z_offset);
		  //printf("axe_x = %d , axe_y=%d, axe_z= %d \n",axe_x*4,axe_y*4,axe_z*4);
	 switch (switch_axes){
		  	  case 0 :affichage_7seg (axe_x);
		  	  	  	  break;
		  	  case 1 :affichage_7seg (axe_y);
		  	  	  	  	  break;
		  	  case 2 :affichage_7seg (axe_z);
		  	  	  	  	  break;
		  	  }

	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0); // clear the reset
}

int main()
{

alt_ic_isr_register(BP_IRQ_INTERRUPT_CONTROLLER_ID, BP_IRQ, irqhandler_BP , NULL, 0x0);
IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BP_BASE, 0x01); //enable the interrupt of the button

 alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID,TIMER_0_IRQ, (void*)irqhandler_TIMER_0, NULL, 0);
IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x07);

  I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_FREQ,100000);

  Calibrate();
  set_axes();

  while (1){  }

  return 0;
}
