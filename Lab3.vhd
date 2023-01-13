library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity Lab3 is
	port (
	
		clk : in  std_logic ;
		reset : in std_logic;
		I2C_clk: inout std_logic;
		I2C_data: inout std_logic;
		CS_bar: out std_logic;
		ALT_adress: out std_logic	
	);
end entity Lab3;

architecture rtl of Lab3 is

component Lab3_qsys is
        port (
            clk_clk               : in    std_logic := 'X'; -- clk
            reset_reset_n         : in    std_logic := 'X'; -- reset_n
            output_i2c_scl_pad_io : inout std_logic := 'X'; -- scl_pad_io
            output_i2c_sda_pad_io : inout std_logic := 'X'  -- sda_pad_io
        );
    end component Lab3_qsys;
	 
begin
	 
	 u0 : component Lab3_qsys
        port map (
            clk_clk               => clk,               --        clk.clk
            reset_reset_n         => reset,         --      reset.reset_n
            output_i2c_scl_pad_io => I2C_clk, -- output_i2c.scl_pad_io
            output_i2c_sda_pad_io => I2C_data  --           .sda_pad_io
        );
  CS_bar<='1';
  ALT_adress<='1';
end architecture;