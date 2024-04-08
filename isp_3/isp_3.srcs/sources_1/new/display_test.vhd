----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 02/28/2024 05:03:37 PM
-- Design Name: 
-- Module Name: tb - Behavioral
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

ENTITY display_test IS

END display_test;

ARCHITECTURE Behavioral OF display_test IS

    COMPONENT display IS
        PORT (
            clk_i : IN STD_LOGIC;
            rst_i : IN STD_LOGIC;
            digit_i : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            led7_an_o : OUT STD_LOGIC_VECTOR (3 DOWNTO 0);
            led7_seg_o : OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
        );
    END COMPONENT;

    SIGNAL clk_i : STD_LOGIC;
    SIGNAL rst_i : STD_LOGIC;
    SIGNAL digit_i : STD_LOGIC_VECTOR (31 DOWNTO 0);
    SIGNAL led7_an_o : STD_LOGIC_VECTOR (3 DOWNTO 0);
    SIGNAL led7_seg_o : STD_LOGIC_VECTOR (7 DOWNTO 0);

    -- internal signals from display
    SIGNAL clock_ticks : INTEGER;
    SIGNAL digit_index : INTEGER;

    CONSTANT clock_period : TIME := 100 ns;
BEGIN
    display1 : display PORT MAP(
        clk_i => clk_i,
        rst_i => rst_i,
        digit_i => digit_i,
        led7_an_o => led7_an_o,
        led7_seg_o => led7_seg_o
    );

    -- accessing internal signals
    clock_ticks <= << SIGNAL .display_test.display1.clock_ticks : INTEGER >> ;
    digit_index <= << SIGNAL .display_test.display1.digit_index : INTEGER >> ;

    rst_i <= '0';

    clock : PROCESS
    BEGIN
        clk_i <= '0';
        WAIT FOR clock_period / 2;
        clk_i <= '1';
        WAIT FOR clock_period / 2;
    END PROCESS;

    testing : PROCESS
    BEGIN
        -- example digits
        digit_i <= "11001001111101101000101001101101";
        WAIT FOR 20 ms;
        digit_i <= "10101001111000110110011101001000";
        WAIT FOR 20 ms;
        digit_i <= "00111101010110110110100111100001";
        WAIT FOR 20 ms;
        digit_i <= "10000110110101011001001010101110";
        WAIT FOR 20 ms;
        digit_i <= "01111101011010101011001110001101";
        WAIT FOR 20 ms;
        digit_i <= "11100110101011100100101101101010";
        WAIT FOR 20 ms;
        digit_i <= "01001111010011111100001000110011";
        WAIT FOR 20 ms;
        digit_i <= "10111001001010111001101111101100";
        WAIT FOR 20 ms;
        digit_i <= "00100101101101010010011010011010";
        WAIT FOR 20 ms;
        digit_i <= "11011100010001111110011000011101";
    END PROCESS;

END Behavioral;