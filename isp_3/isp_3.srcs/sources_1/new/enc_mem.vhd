LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY enc_mem IS
    PORT (
        clk_i : IN STD_LOGIC;
        btn_i : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
        sw_i : IN STD_LOGIC_VECTOR (7 DOWNTO 0);
        digit_i : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)
    );
END enc_mem;

ARCHITECTURE Behavioral OF enc_mem IS
BEGIN
    encoder : PROCESS (clk_i) IS
        VARIABLE value : INTEGER;
        VARIABLE digit_index : INTEGER;
        VARIABLE pattern : STD_LOGIC_VECTOR(6 DOWNTO 0);
    BEGIN
        IF rising_edge(clk_i) THEN
            IF btn_i /= "0000" THEN
                value := to_integer(unsigned(sw_i(3 DOWNTO 0)));
                CASE value IS
                    WHEN 0 =>
                        pattern := "0000001";
                    WHEN 1 =>
                        pattern := "1001111";
                    WHEN 2 =>
                        pattern := "0010010";
                    WHEN 3 =>
                        pattern := "0000110";
                    WHEN 4 =>
                        pattern := "1001100";
                    WHEN 5 =>
                        pattern := "0100100";
                    WHEN 6 =>
                        pattern := "0100000";
                    WHEN 7 =>
                        pattern := "0001111";
                    WHEN 8 =>
                        pattern := "0000000";
                    WHEN 9 =>
                        pattern := "0000100";
                    WHEN 10 => -- A
                        pattern := "0001000";
                    WHEN 11 => -- B
                        pattern := "1100000";
                    WHEN 12 => -- C
                        pattern := "0110001";
                    WHEN 13 => -- D
                        pattern := "1000010";
                    WHEN 14 => -- E
                        pattern := "0110000";
                    WHEN 15 => -- F
                        pattern := "0111000";
                    WHEN OTHERS =>
                        pattern := "1111111"; -- Default case (for values not covered above)
                END CASE;

                CASE btn_i IS
                    WHEN "0001" =>
                        digit_index := 0;
                    WHEN "0010" =>
                        digit_index := 1;
                    WHEN "0100" =>
                        digit_index := 2;
                    WHEN "1000" =>
                        digit_index := 3;
                    WHEN OTHERS =>
                        digit_index := 3;
                END CASE;

                digit_i((digit_index + 1) * 8 - 1 DOWNTO digit_index * 8 + 1) <= pattern;
            END IF;
            digit_i(24) <= NOT sw_i(7);
            digit_i(16) <= NOT sw_i(6);
            digit_i(8) <= NOT sw_i(5);
            digit_i(0) <= NOT sw_i(4);

        END IF;

    END PROCESS;

END Behavioral;