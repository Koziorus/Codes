import time

from exceptions import GameplayException
from connect4 import Connect4
from randomagent import RandomAgent
from minmaxagent import MinMaxAgent
from alphabetaagent import AlphaBetaAgent

agent1_timer = 0.0
agent2_timer = 0.0
rounds = 0

connect4 = Connect4(width=7, height=6)
# agent1 = RandomAgent('o')
agent1 = MinMaxAgent(depth=4, my_token='x')
agent2 = AlphaBetaAgent(depth=4, my_token='o')
while not connect4.game_over:
    connect4.draw()
    try:
        if connect4.who_moves == agent1.my_token:
            time0 = time.perf_counter()
            n_column = agent1.decide(connect4)
            agent1_timer += time.perf_counter() - time0
        else:
            time0 = time.perf_counter()
            n_column = agent2.decide(connect4)
            agent2_timer += time.perf_counter() - time0
        connect4.drop_token(n_column)
    except (ValueError, GameplayException):
        print('invalid move')
    rounds += 1

connect4.draw()

print("\nAverage time per turn:\n")
print(type(agent1).__name__, ": ", agent1_timer/rounds)
print(type(agent2).__name__, ": ", agent2_timer/rounds)

