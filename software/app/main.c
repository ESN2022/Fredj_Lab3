#include "sys/alt_stdio.h"
#include <alt_types.h>
#include <stdio.h>
#include "system.h"



  alt_16 axe_x=0;
  alt_16 axe_y=0;
  alt_16 axe_z=0;

alt_8 Read_I2c (alt_8 adress){
	 alt_8 data=0;
	 I2C_start(OPENCORES_I2C_0_BASE,0x1D,0);
	 I2C_write(OPENCORES_I2C_0_BASE,adress,0);
	 I2C_start(OPENCORES_I2C_0_BASE,0x1D,1);
	 data=I2C_read(OPENCORES_I2C_0_BASE,1);
	 return data;
}

void set_axes(){
	axe_x=Read_I2c(0x33);
	axe_x=((axe_x <<8)| Read_I2c(0x32));

	axe_y=Read_I2c(0x35);
	axe_y=((axe_y <<8)| Read_I2c(0x34));


	axe_z=Read_I2c(0x37);
	axe_z=((axe_z <<8)| Read_I2c(0x36));
}

int main()
{
  I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_FREQ,100000);
  if (I2C_start(OPENCORES_I2C_0_BASE,0x1D,0)==0){
	  alt_printf("I2C BUENO");
  }
  else alt_printf("NO BUENO\n");

  while (1){
	  set_axes();
	  printf("axe_x = %d , axe_y=%d, axe_z= %d \n",axe_x,axe_y,axe_z);

  usleep(50000);
  }

  return 0;
}
