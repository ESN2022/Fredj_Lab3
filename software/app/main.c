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

#define add_OFSX 0x1E // OFFSET register for X axe
#define add_OFSY 0x1F // OFFSET register for Y axe
#define add_OFSZ 0x20 // OFFSET register for Z axe

  alt_16 axe_x=0;  //value of X axe
  alt_16 axe_y=0;  //value of Y axe
  alt_16 axe_z=0;  //value of Z axe

  alt_16 x_1; //MSB value of X axe
  alt_16 y_1; //MSB value of Y axe
  alt_16 z_1; //MSB value of Z axe

  alt_16 x_0; //LSB value of X axe
  alt_16 y_0; //LSB value of Y axe
  alt_16 z_0; //LSB value of Z axe

  alt_8 axe_x_offset=3; //offset value of X axe
  alt_8 axe_y_offset=1; //offset value of Y axe
  alt_8 axe_z_offset=2; //offset value of Z axe

  int switch_axes=0;  //Value deciding which axe to display on 7-segment




/*****************************************************************************************/

/*                              Read a register with I²C                                 */

/*****************************************************************************************/
int Read_I2c (alt_8 adress){
	 int data=0; // output data
	 I2C_start(OPENCORES_I2C_0_BASE,0x1D,0); //address the ACCELEROMETRE in write mode
	 I2C_write(OPENCORES_I2C_0_BASE,adress,0);
	 I2C_start(OPENCORES_I2C_0_BASE,0x1D,1); // read mode
	 data=I2C_read(OPENCORES_I2C_0_BASE,1); // will read the input register and stop the communication
	 return data;
}
/**********************************************************************************************/

/*               Write to a register with I²C                                                 */

/**********************************************************************************************/
//this function is used to write the offset value to the registers OFSX, OFSY and OFSZ
void Write_I2c (alt_8 value, alt_8 adress){

	I2C_start(OPENCORES_I2C_0_BASE,0x1D,0);  //address the ACCELEROMETRE in write mode
	I2C_write(OPENCORES_I2C_0_BASE,adress,0); // write to starting address of the offset adresse
	I2C_write(OPENCORES_I2C_0_BASE,value,1);  // write the data
}
/**********************************************************************************************/

/*                       SET THE 3 AXES                                                       */

/**********************************************************************************************/
void set_axes(){
// this function is used to set the value of the 3 axes in axe_x,axe_y and axe_z
	x_0=Read_I2c(0x32); // Read the LSB of the X AXE
	x_1 = Read_I2c(0x33); // Read the MSB of the X AXE
	axe_x=((x_1 <<8)| x_0); //We shift the MSB and add the LSB to get the X axe value

	y_0=Read_I2c(0x34); // Read the LSB of the Y AXE
	y_1 = Read_I2c(0x35);// Read the MSB of the Y AXE
	axe_y=((y_1 <<8)|y_0); //We shift the MSB and add the LSB to get the Y axe value


	z_0=Read_I2c(0x36); // Read the LSB of the Z AXE
	z_1 = Read_I2c(0x37);// Read the MSB of the Z AXE
	axe_z=((z_1<<8)| z_0);//We shift the MSB and add the LSB to get the Z axe value
}
/*********************************************************************************************/

/*                     Calibration function                                                  */

/*********************************************************************************************/
void Calibrate(){
//this function is used to calibrate the ADXL345 AXES with writing those value in their offset registers
	Write_I2c(axe_x_offset,add_OFSX); // Write the OFFSET value of X calculated to OFSX
	usleep(500);

	Write_I2c(axe_y_offset,add_OFSY); // Write the OFFSET value of Y calculated to OFSY
	usleep(500);

	Write_I2c(axe_z_offset,add_OFSZ); // Write the OFFSET value of Z calculated to OFSZ
	usleep(500);
}
/***********************************************************************************************/

/*                 PUSH BUTTON interruption                                                    */

/***********************************************************************************************/
static void irqhandler_BP(void * context, alt_u32 id){
	//IRQ handler for the push button
	alt_printf("Interrupt BP\n"); // showing this message every time there's a BP interruption (for debug)
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BP_BASE, 0x01); //clear the interruption
	if (switch_axes==2) {switch_axes=0;} //return to the X axe after pushing the BP while showing Z axe
	else
		switch_axes=switch_axes+1;  // or increment to the next axe
}

void BP_(){
	// initializing the IRQ
	alt_irq_register(BP_IRQ, NULL, (void*)irqhandler_BP); //register the BP interrupt service routine (ISR)
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BP_BASE , 0xf); //enable the interrupt of BP
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BP_BASE, 0x01); //disable the interrupt of BP

}
/**************************************************************************************************/

/*       DISPLAYING ON THE 7-segment                                                              */


/**************************************************************************************************/
void affichage_7seg (int valeur){
// this function is used to display the values on the 5 7-segments displays
	int aff=0; // initialise the displayed value
	if (valeur <0 ) { //if the axe is negative
		valeur = -valeur; //the opposite value if negative
		aff = (valeur % 10) | (((valeur / 10) % 10) << 4) | (((valeur /100)%10) << 8) | (valeur/1000)<<12 ; // BINARY to BCD
		IOWR_ALTERA_AVALON_PIO_DATA(SIGNE_BASE,0x01);} // displaying the negative symbole on the 5th 7-segment

	else { // if the variable is positive
		aff = (valeur % 10) | (((valeur / 10) % 10) << 4) | (((valeur /100)%10) << 8) | (valeur/1000)<<12  ; // BINARY to BCD
		IOWR_ALTERA_AVALON_PIO_DATA(SIGNE_BASE,0x00);} //removing the negative symbole

     IOWR_ALTERA_AVALON_PIO_DATA(AFFICHEUR_BASE,aff); // displaying the value on the 4 7-segments
}
/***********************************************************************************************************************/

/*                             TIMER INTERRUPTION                                                           */

/************************************************************************************************************************/
static void irqhandler_TIMER_0(void* context,alt_u32 id){
	//IRQ handler for the Timer0
	alt_printf("Interrupt timer\n"); // showing this message every time there's a TIMER interruption (for debug)
	set_axes(); // call for set_axes to give every axe variable it value
	 printf("axe_x = %d , axe_y=%d, axe_z= %d\n",axe_x*4,axe_y*4,axe_z*4); // displaying the 3 values of the axes
	 switch (switch_axes){ //depending on the AXE selected from the BP
		  	  case 0 :affichage_7seg (axe_x*4); // Displaying AXE X
		  	          alt_printf("WATCHING X:\n"); // to know which axe displayed on the terminal
		  	  	  	  break;
		  	  case 1 :affichage_7seg (axe_y*4); // Displaying AXE Y
		  	  	  	  alt_printf("WATCHING Y:\n"); // to know which axe displayed on the terminal
		  	  	  	  break;
		  	  case 2 :affichage_7seg (axe_z*4); // Displaying AXE Z
		  	  	  	  alt_printf("WATCHING Z:\n"); // to know which axe displayed on the terminal
		  	  	  	  break;
		  	  }
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0); // reset the timer status register
}

void Timer_(){
	// initializing the IRQ
	alt_irq_register(TIMER_0_IRQ, NULL, (void*)irqhandler_TIMER_0);//register the TIMER0 interrupt service routine (ISR)
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x07); //ALTERA_AVALON_TIMER_CONTROL_ITO_MSK (0x01)+ALTERA_AVALON_TIMER_CONTROL_CONT_MSK (0x02)+ALTERA_AVALON_TIMER_CONTROL_START_MSK (0x04)
}
/**************************************************************************************************************************/

/*                            MAIN FUNCTION                                                                               */

/**************************************************************************************************************************/
int main()
{
  I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_FREQ,100000); //initializing the I2C communication and set the i2C frequency to 100 Khz
  Calibrate(); // call tge calibrate function
  Timer_(); //set up the timer interrupt function
  BP_(); //set up the BP interrupt function
  while (1){}
  return 0;
}

/**************************************************************************************************************************/
