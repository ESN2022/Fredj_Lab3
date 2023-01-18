library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity Lab3 is
	port (
		BP  : in std_logic; -- the push button
		clk : in  std_logic ; -- clock
		reset : in std_logic; -- reset button
		display1 : out  STD_LOGIC_VECTOR (7 downto 0); -- the 1st 7-segments
		display2 : out  STD_LOGIC_VECTOR (7 downto 0); -- the 2nd 7-segments
		display3 : out  STD_LOGIC_VECTOR (7 downto 0); -- the 3rd 7-segments
		display4 : out  STD_LOGIC_VECTOR (7 downto 0); -- the 4th 7-segments
		display5 : out  STD_LOGIC_VECTOR (7 downto 0); -- the 5th  7-segments containing the SYMBOLE
		I2C_clk: inout std_logic; -- I2C clock (100 khz)
		I2C_data: inout std_logic; -- I2C data 
		CS_bar: out std_logic;  -- TO use I2C as communication
		ALT_adress: out std_logic	-- To use the 0x1D adress
	);
end entity Lab3;

architecture rtl of Lab3 is

component Lab3_qsys is
        port (
            afficheur_export      : out   std_logic_vector(15 downto 0);        -- export
            bp_export             : in    std_logic                     := 'X'; -- export
            clk_clk               : in    std_logic                     := 'X'; -- clk
            output_i2c_scl_pad_io : inout std_logic                     := 'X'; -- scl_pad_io
            output_i2c_sda_pad_io : inout std_logic                     := 'X'; -- sda_pad_io
            reset_reset_n         : in    std_logic                     := 'X'; -- reset_n
            signe_export          : out   std_logic_vector(3 downto 0)          -- export
        );
    end component Lab3_qsys;

	 
component decode is -- to decode from 4 bits to 8 bits compatible with the 7-segments
	port (                    
		input : in STD_LOGIC_VECTOR (3 downto 0);
		output: out STD_LOGIC_VECTOR (7 downto 0)
	);
	end component;
	
component signeAXE is -- to show the symbole on the last 7-segment
	port (                    
		input : in STD_LOGIC_VECTOR (3 downto 0);
		output: out STD_LOGIC_VECTOR (7 downto 0)
	);
   end component signeAXE;

	 
	 signal readbin: std_logic_vector(15 downto 0); -- variable were we will stock the 16 bits numbers ( every digit in 4 bits)
	 signal Signeax: STD_LOGIC_VECTOR (3 downto 0); -- variable were we will stock the symbole
	 
begin
	 
	 u0 : component Lab3_qsys
        port map (
		  		afficheur_export      => readbin,
				bp_export             => BP,
            clk_clk               => clk,  --        clk.clk
				output_i2c_scl_pad_io => I2C_clk, -- output_i2c.scl_pad_io
				output_i2c_sda_pad_io => I2C_data,  --.sda_pad_io				
				reset_reset_n         => reset,       --      reset.reset_n
				signe_export          => Signeax
				 );
				
		u1: component decode port map (readbin(3 downto 0), display1); -- to display every 4 bits in their 7 segments requirement in 8 bits
		u2: component decode port map (readbin(7 downto 4), display2);
		u3: component decode port map (readbin(11 downto 8), display3);
		u4: component decode port map (readbin(15 downto 12), display4);
		u5: component signeAXE port map (Signeax, display5); -- to display the symbole on the last 7-segment
				
  CS_bar<='1'; -- config I2C
  ALT_adress<='1'; -- config I2C
end architecture;