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
            when "0000" => output <= "00000011";
            when "0001" => output <= "10011111";
            when "0010" => output <= "00100101";
            when "0011" => output <= "00001101";
            when "0100" => output <= "10011001";
            when "0101" => output <= "01001001";
            when "0110" => output <= "01000001";
            when "0111" => output <= "00011111";
            when "1000" => output <= "00000001";
            when "1001" => output <= "00001001";
            when others => output <= "01100000";
        end case;
end process;
end architecture;