from connect4 import Connect4

connect4 = Connect4(width=7, height=6)

for iter_four in connect4.iter_fours():
    print(iter_four)