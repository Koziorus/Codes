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

BEGIN

END Behavioral;