import copy

from exceptions import AgentException


# x -> max
# o -> min

class MinMaxAgent:
    def __init__(self, depth, my_token='x'):
        self.my_token = my_token
        self.search_depth = depth
        assert my_token == 'x'

    def heuristic(self, s):
        # h is positive for 'x'

        h_iter_fours = 0  # h_iter_fours e <-1, 1>
        iter_fours = list(s.iter_fours())
        for iter_four in iter_fours:
            value = 0
            for slot in iter_four:
                if slot == 'x':
                    value += 1
                elif slot == 'o':
                    value -= 1
            value /= 4.0
            h_iter_fours += value
        h_iter_fours /= len(iter_fours)

        h_center_column = 0  # h_iter_fours e <-1, 1>
        center_column = s.center_column()
        for slot in center_column:
            if slot == 'x':
                h_center_column += 1
            elif slot == 'o':
                h_center_column -= 1
        h_center_column /= s.height

        h = 0.5 * h_iter_fours + 0.5 * h_center_column

        return h

    def minmax(self, s, x, d):
        if s.game_over == True:
            if s.wins == 'x':
                return 1
            elif s.wins == 'o':
                return -1
            else:  # tie
                return 0
        elif d == 0:
            return self.heuristic(s)
        else:
            if x == 'x':
                max_quality = -1
                for possible_drop in s.possible_drops():
                    new_s = copy.deepcopy(s)
                    new_s.drop_token(possible_drop)
                    quality = self.minmax(new_s, 'o', d - 1)
                    if quality > max_quality:
                        max_quality = quality

                return max_quality
            elif x == 'o':
                min_quality = 1
                for possible_drop in s.possible_drops():
                    new_s = copy.deepcopy(s)
                    new_s.drop_token(possible_drop)
                    quality = self.minmax(new_s, 'x', d - 1)
                    if quality < min_quality:
                        min_quality = quality

                return min_quality

    def decide(self, connect4):
        if connect4.who_moves != self.my_token:
            raise AgentException('not my round')

        best_decision = 0
        best_decision_quality = -1
        for possible_drop in connect4.possible_drops():
            new_connect4 = copy.deepcopy(connect4)
            new_connect4.drop_token(possible_drop)
            decision_quality = self.minmax(new_connect4, ('o' if self.my_token == 'x' else 'x'), self.search_depth)
            if decision_quality >= best_decision_quality:
                best_decision_quality = decision_quality
                best_decision = possible_drop

        return best_decision
