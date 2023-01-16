library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity Lab3 is
	port (
		BP  : in std_logic;
		clk : in  std_logic ;
		reset : in std_logic;
		display1 : out  STD_LOGIC_VECTOR (7 downto 0);
		display2 : out  STD_LOGIC_VECTOR (7 downto 0);
		display3 : out  STD_LOGIC_VECTOR (7 downto 0);
		display4 : out  STD_LOGIC_VECTOR (7 downto 0);
		Signe : out std_logic;
		I2C_clk: inout std_logic;
		I2C_data: inout std_logic;
		CS_bar: out std_logic;
		ALT_adress: out std_logic	
	);
end entity Lab3;

architecture rtl of Lab3 is

component Lab3_qsys is
         port (
            clk_clk               : in    std_logic                     := 'X'; -- clk
            output_i2c_scl_pad_io : inout std_logic                     := 'X'; -- scl_pad_io
            output_i2c_sda_pad_io : inout std_logic                     := 'X'; -- sda_pad_io
            reset_reset_n         : in    std_logic                     := 'X'; -- reset_n
            afficheur_export      : out   std_logic_vector(16 downto 0);        -- export
            bp_export             : in    std_logic                     := 'X'  -- export
        );
    end component Lab3_qsys;
	 
	 component decode is
	port (                    
		input : in STD_LOGIC_VECTOR (3 downto 0);
		output: out STD_LOGIC_VECTOR (7 downto 0)
	);
	end component;
	 
	 signal readbin: std_logic_vector(16 downto 0);
	 
begin
	 
	 u0 : component Lab3_qsys
        port map (
            clk_clk               => clk,  --        clk.clk
				output_i2c_sda_pad_io => I2C_data,  --           .sda_pad_io				
            output_i2c_scl_pad_io => I2C_clk, -- output_i2c.scl_pad_io
				reset_reset_n         => reset,         --      reset.reset_n
				afficheur_export      => readbin,
				bp_export             => BP );
				
				
		u1: component decode port map (readbin(3 downto 0), display1);
		u2: component decode port map (readbin(7 downto 4), display2);
		u3: component decode port map (readbin(11 downto 8), display3);
		u4: component decode port map (readbin(15 downto 12), display4);
				
  Signe <= readbin(16);			
  CS_bar<='1';
  ALT_adress<='1';
end architecture;