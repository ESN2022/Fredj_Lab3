library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity signeAXE is
	port (                    
		input : in STD_LOGIC_VECTOR (3 downto 0);
		output: out STD_LOGIC_VECTOR (7 downto 0)
	);
end entity signeAXE;

architecture rtl of signeAXE is
begin
decodage : process(input)
    begin
        case input is
            when "0000" => output <= "11111111";-- case positive
            when "0001" => output <= "11111101";-- case negative
            when others => output <= "11111101";
        end case;
end process;
end architecture;