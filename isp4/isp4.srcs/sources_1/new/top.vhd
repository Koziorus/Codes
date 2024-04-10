----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 02/28/2024 04:29:25 PM
-- Design Name: 
-- Module Name: top - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
ENTITY top IS
    PORT (
        clk_i : IN STD_LOGIC;
        rst_i : IN STD_LOGIC;
        RXD_i : IN STD_LOGIC;
        led7_an_o : OUT STD_LOGIC_VECTOR (3 DOWNTO 0);
        led7_seg_o : OUT STD_LOGIC_VECTOR (7 DOWNTO 0));
END top;
ARCHITECTURE Behavioral OF top IS
    SIGNAL clock_ticks : INTEGER;
    SIGNAL started : STD_LOGIC;
    SIGNAL data_index : INTEGER;

    CONSTANT data_len : INTEGER := 8;
    CONSTANT
BEGIN
    rxd_synch : PROCESS (clk_i)
    BEGIN
        IF rising_edge(clk_i) THEN
            IF started = '1' THEN
                IF clock_ticks * 96 > 1000000 THEN -- if clock_ticks > ceil(9600 Hz / 100Mhz) -> if in a new cycle
                    clock_ticks <= 1;

                    IF data_index = data_len + 1 THEN -- if data and stop has been reached
                        started <= '0';
                    ELSE
                        IF data_index < data_len THEN -- if stop hasn't been reached
                            led7_an_o(data_index) <= RXD_i;
                        END IF;

                        data_index <= data_index + 1;
                    END IF;
                ELSE
                    clock_ticks <= clock_ticks + 1;
                END IF;
            ELSIF RXD_i = '0' THEN --  started
                clock_ticks <= 1;
                started <= '1';
                data_index <= 0;
            END IF;
        ELSIF rst_i = '1' THEN
            clock_ticks <= 0;
            bit_index <= 0;
            started <= '0';
        END IF;
    END PROCESS;
END Behavioral;