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
USE ieee.numeric_std.ALL;

ENTITY top IS
    PORT (
        clk_i : IN STD_LOGIC;
        btn_i : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
        sw_i : IN STD_LOGIC_VECTOR (7 DOWNTO 0);
        led7_an_o : OUT STD_LOGIC_VECTOR (3 DOWNTO 0);
        led7_seg_o : OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
    );
END top;

ARCHITECTURE Behavioral OF top IS
    SIGNAL digit_i : STD_LOGIC_VECTOR (31 DOWNTO 0);
    SIGNAL rst_i : STD_LOGIC;

    COMPONENT display IS
        PORT (
            clk_i : IN STD_LOGIC;
            rst_i : IN STD_LOGIC;
            digit_i : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            led7_an_o : OUT STD_LOGIC_VECTOR (3 DOWNTO 0);
            led7_seg_o : OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
        );
    END COMPONENT;

    COMPONENT enc_mem IS
        PORT (
            clk_i : IN STD_LOGIC;
            btn_i : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
            sw_i : IN STD_LOGIC_VECTOR (7 DOWNTO 0);
            digit_i : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)
        );
    END COMPONENT;
BEGIN

    rst_i <= '0';

    enc_mem1 : enc_mem PORT MAP(
        clk_i => clk_i,
        btn_i => btn_i,
        sw_i => sw_i,
        digit_i => digit_i
    );

    display1 : display PORT MAP(
        clk_i => clk_i,
        rst_i => rst_i,
        digit_i => digit_i,
        led7_an_o => led7_an_o,
        led7_seg_o => led7_seg_o);

END Behavioral;