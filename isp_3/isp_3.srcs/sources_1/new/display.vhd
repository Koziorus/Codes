LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY display IS
    PORT (
        clk_i : IN STD_LOGIC;
        rst_i : IN STD_LOGIC;
        digit_i : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
        led7_an_o : OUT STD_LOGIC_VECTOR (3 DOWNTO 0);
        led7_seg_o : OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
    );
END display;

ARCHITECTURE Behavioral OF display IS
    SIGNAL clock_ticks : INTEGER := 0;
    CONSTANT display_clock_step : INTEGER := 10000; -- for a global 100MHz clock the step is 10000 resulting in a local 1kHz clock

    SIGNAL digit_index : INTEGER := 0;
    SHARED VARIABLE i : INTEGER := 0;

BEGIN
    disp : PROCESS (clk_i) IS
    BEGIN
        IF rising_edge(clk_i) THEN

            clock_ticks <= clock_ticks + 1;

            IF clock_ticks = display_clock_step THEN
                clock_ticks <= 0; -- Reset clock_ticks
                digit_index <= digit_index - 1; -- Increment digit_index

                IF digit_index = 0 THEN
                    digit_index <= 3;
                END IF;

            END IF;

            led7_an_o <= "0000";
            led7_an_o(digit_index) <= '1';
        END IF;

    END PROCESS;

    led7_seg_o <= digit_i(((digit_index + 1) * 8 - 1) DOWNTO (digit_index * 8)); -- this may cause flickering of the display (so maybe it should be in the pro

END Behavioral;