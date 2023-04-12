import pandas as pd
import matplotlib.pyplot as plt

def get_data(csv_file):
    data_frame = pd.read_csv('kursUSD.csv')
    data_frame['Data'] = pd.to_datetime(data_frame['Data'])
    return data_frame

def get_EMA(data, N, starting_day):
    # EMA = sum(A_i/B_i)
    a = 1   `
    A = 0
    B = 0
    alpha = 2.0 / (N + 1)
    for i in range(0, N):
        A += a * data[starting_day - i]
        B += a
        a *= (1 - alpha)

    EMA = A / B
    return EMA


def get_MACD(data, starting_day):
    MACD = [0.0] * len(data)  # creates

    for i in range(starting_day, len(data)):
        MACD[i] = (get_EMA(data, 12, i) - get_EMA(data, 26, i))

    return MACD


def get_SIGNAL(data, starting_day):
    SIGNAL = [0.0] * len(data)

    for i in range(starting_day, len(data)):
        SIGNAL[i] = get_EMA(data, 9, i)

    return SIGNAL


def get_intersections(MACD, SIGNAL, starting_day):
    buy_points = []
    sell_points = []

    previous_state = ('below' if MACD[starting_day] < SIGNAL[starting_day] else 'over')

    for i in range(starting_day, len(MACD)):
        current_state = ('below' if MACD[i] < SIGNAL[i] else 'over')
        if previous_state != current_state:
            if current_state == 'over':
                buy_points.append(i)
            else:  # 'below'
                sell_points.append(i)

        previous_state = current_state

    return sell_points, buy_points


def buy_sell(buy_points, sell_points, usd_prices, balance):
    stock = 0

    tasks = [''] * len(usd_prices)

    for i in buy_points:
        tasks[i] = 'buy'

    for i in sell_points:
        tasks[i] = 'sell'

    balance_x_top = [0] * len(tasks)
    balance_x_bottom = [0] * len(tasks)

    up = 0
    down = 0

    for i in range(len(tasks)):
        if tasks[i] == 'buy':
            amount = 0.5 * balance / usd_prices[i]
            balance -= amount * usd_prices[i]
            down += amount * usd_prices[i]
            stock += amount
        elif tasks[i] == 'sell':
            amount = 0.5 * stock
            balance += amount * usd_prices[i]
            up += amount * usd_prices[i]
            stock -= amount
        balance_x_bottom[i] = balance
        balance_x_top[i] = stock * usd_prices[i]

    balance += usd_prices[len(usd_prices)-1] * stock  # the rest of money

    print("Liczba syngałów sprzedaży: " + str(len(sell_points)))
    print("Liczba syngałów kupna: " + str(len(buy_points)))

    return balance, (balance_x_bottom, balance_x_top)


def get_buy_sell_plot(buy_points, sell_points, data_frame):
    buy_x = []
    buy_y = []
    for i in range(len(buy_points)):
        buy_x.append(data_frame['Data'][buy_points[i]])
        buy_y.append(data_frame['Kurs'][buy_points[i]])

    sell_x = []
    sell_y = []
    for i in range(len(sell_points)):
        sell_x.append(data_frame['Data'][sell_points[i]])
        sell_y.append(data_frame['Kurs'][sell_points[i]])

    return buy_x, buy_y, sell_x, sell_y


if __name__ == '__main__':
    to_plot = True

    df = get_data('kursUSD.csv')
    usd_prices = df['Kurs'].tolist()

    MACD = get_MACD(usd_prices, 35)
    SIGNAL = get_SIGNAL(MACD, 9)
    buy_points, sell_points = get_intersections(MACD, SIGNAL, 35)

    balance_in = 1000.0
    print("Kapitał wejściowy: " + str(round(balance_in, 2)) + " PLN")
    balance_out, balance_x = buy_sell(buy_points, sell_points, usd_prices, balance_in)
    print("Kapitał wyjściowy: " + str(round(balance_out, 2)) + " PLN")

    if(to_plot):
        fig_MACD = plt.figure()
        ax_MACD = fig_MACD.add_subplot(111)
        ax_MACD.plot(df['Data'], MACD, label='MACD')
        ax_MACD.plot(df['Data'], SIGNAL, label='SIGNAL')
        ax_MACD.set_title('MACD/SIGNAL')
        ax_MACD.set_xlabel('Data')
        ax_MACD.legend()

        fig = plt.figure()
        ax = fig.add_subplot(111)
        ax.plot(df['Data'], df['Kurs'], 'darkblue')
        ax.set_xlabel('Data')
        ax.set_ylabel('Wartość kursu USD')
        ax.set_title('Kurs USD')

        # side-by-side (price) (MACD, SIGNAL)
        fig_seperate = plt.figure()

        ax1 = fig_seperate.add_subplot(211)
        ax2 = fig_seperate.add_subplot(212)
        ax1.plot(df['Data'], df['Kurs'], 'darkblue')
        ax1.set_xlabel('Data')
        ax1.set_ylabel('Wartość kursu USD')
        ax1.set_title('Kurs USD')

        ax2.plot(df['Data'], MACD, label='MACD')
        ax2.plot(df['Data'], SIGNAL, label='SIGNAL')
        ax2.set_title('MACD/SIGNAL')

        ax2.set_xlabel('Data')

        ax2.legend()

        fig_seperate.set_figwidth(15)
        fig_seperate.set_figheight(5)

        fig_seperate.tight_layout()

        plt.show()

        # all-in-one (price, MACD, SIGNAL)
        fig_combined = plt.figure()

        axis_combined = fig_combined.add_subplot(111)
        axis_combined.plot(df['Data'], df['Kurs'], '-')
        axis_combined.plot(df['Data'], list(map(lambda x: x * 4 + 3.1, MACD)), label='MACD')
        axis_combined.plot(df['Data'], list(map(lambda x: x * 4 + 3.1, SIGNAL)), label='SIGNAL')
        axis_combined.legend()
        axis_combined.set_title('Kurs USD i wskaźnik MACD')

        fig_combined.set_figwidth(15)
        fig_combined.set_figheight(5)
        plt.xlabel('data')

        plt.show()

        buy_x, buy_y, sell_x, sell_y = get_buy_sell_plot(buy_points, sell_points, df)

        fig_buy_sell = plt.figure()
        axis_buy_sell = fig_buy_sell.add_subplot(111)
        axis_buy_sell.plot(df['Data'], df['Kurs'], '-')
        axis_buy_sell.plot(buy_x, buy_y, 'ro', label="kupno")
        axis_buy_sell.plot(sell_x, sell_y, 'go', label="sprzedaż")
        axis_buy_sell.legend()
        axis_buy_sell.set_title("Sprzedaż i kupno według MACD")
        axis_buy_sell.set_xlabel('Data')
        axis_buy_sell.set_ylabel('Wartość kursu USD')
        plt.show()

        fig_balance = plt.figure()
        axis_balance = fig_balance.add_subplot(111)
        axis_balance.bar(df['Data'], balance_x[0], width=1.0, label='aktualny kapitał')
        axis_balance.bar(df['Data'], balance_x[1], bottom=balance_x[0], width=1.0, label='wartość zakupionych akcji')
        axis_balance.legend()
        axis_balance.set_title("Kapitał i wartość akcji")
        axis_balance.set_xlabel('Data')
        axis_balance.set_ylabel('PLN')

        plt.show()