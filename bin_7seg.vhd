library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity decode is
	port (                    
		input : in STD_LOGIC_VECTOR (3 downto 0);
		output: out STD_LOGIC_VECTOR (7 downto 0)
	);
end entity decode;

architecture rtl of decode is
begin
decodage : process(input)
    begin
        case input is
            when "0000" => output <= "00000011";-- case 0
            when "0001" => output <= "10011111";-- case 1
            when "0010" => output <= "00100101";-- case 2
            when "0011" => output <= "00001101";-- case 3
            when "0100" => output <= "10011001";-- case 4
            when "0101" => output <= "01001001";-- case 5
            when "0110" => output <= "01000001";-- case 6
            when "0111" => output <= "00011111";-- case 7
            when "1000" => output <= "00000001";-- case 8
            when "1001" => output <= "00001001";-- case 9
            when others => output <= "01100000";
        end case;
end process;
end architecture;