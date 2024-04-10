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

ENTITY tb IS

END tb;

ARCHITECTURE Behavioral OF tb IS

    COMPONENT top IS
        PORT (
            clk_i : IN STD_LOGIC;
            rst_i : IN STD_LOGIC;
            RXD_i : IN STD_LOGIC;
            led7_an_o : OUT STD_LOGIC_VECTOR (3 DOWNTO 0);
            led7_seg_o : OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
        )
    END COMPONENT;

    SIGNAL clk_i : STD_LOGIC;
    SIGNAL rst_i : STD_LOGIC;
    SIGNAL RXD_i : STD_LOGIC := '1';
    SIGNAL led7_an_o : STD_LOGIC_VECTOR (3 DOWNTO 0);
    SIGNAL led7_seg_o : STD_LOGIC_VECTOR (7 DOWNTO 0);

    CONSTANT clock_period : TIME := 10 ns;
BEGIN
    uut : top PORT MAP(
        clk_i => clk_i,
        rst_i => rst_i,
        RXD_i => RXD_i,
        led7_an_o => led7_an_o,
        led7_seg_o => led7_seg_o);

    clock : PROCESS
    BEGIN
        clk_i <= '0';
        WAIT FOR TIME;
        clk_i <= '1';
        WAIT FOR TIME;
    END PROCESS;

    tb : PROCESS
    BEGIN
        -- sending data
        RXD_i <= '0';
        wait for 1 ms
        wait for 9600
    END PROCESS;

END Behavioral;