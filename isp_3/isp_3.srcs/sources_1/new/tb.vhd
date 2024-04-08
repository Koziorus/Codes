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

ENTITY tb IS

END tb;

ARCHITECTURE Behavioral OF tb IS

    COMPONENT top IS
        PORT (
            clk_i : IN STD_LOGIC;
            btn_i : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
            sw_i : IN STD_LOGIC_VECTOR (7 DOWNTO 0);
            led7_an_o : OUT STD_LOGIC_VECTOR (3 DOWNTO 0);
            led7_seg_o : OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
        );
    END COMPONENT;

    SIGNAL clk_i : STD_LOGIC;
    SIGNAL btn_i : STD_LOGIC_VECTOR (3 DOWNTO 0) := "0000";
    SIGNAL sw_i : STD_LOGIC_VECTOR (7 DOWNTO 0) := "00000000";
    SIGNAL led7_an_o : STD_LOGIC_VECTOR (3 DOWNTO 0);
    SIGNAL led7_seg_o : STD_LOGIC_VECTOR (7 DOWNTO 0);

    SIGNAL digit_i : STD_LOGIC_VECTOR (31 DOWNTO 0);

    CONSTANT clock_period : TIME := 100 ns;
BEGIN
    uut : top PORT MAP(
        clk_i => clk_i,
        btn_i => btn_i,
        sw_i => sw_i,
        led7_an_o => led7_an_o,
        led7_seg_o => led7_seg_o);

    digit_i <= << SIGNAL .tb.uut.enc_mem1.digit_i : STD_LOGIC_VECTOR (31 DOWNTO 0) >> ;

    clock : PROCESS
    BEGIN
        clk_i <= '0';
        WAIT FOR clock_period / 2;
        clk_i <= '1';
        WAIT FOR clock_period / 2;
    END PROCESS;

    tb : PROCESS
    BEGIN
        WAIT FOR 5 ms;
        sw_i <= "10000001";
        WAIT FOR 5 ms;

        btn_i <= "0001";
        WAIT FOR 1 ms;
        btn_i <= "0000";

        WAIT FOR 5 ms;
        sw_i <= "01101101";
        WAIT FOR 5 ms;

        btn_i <= "1000";
        WAIT FOR 1 ms;
        btn_i <= "0000";
    END PROCESS;

END Behavioral;