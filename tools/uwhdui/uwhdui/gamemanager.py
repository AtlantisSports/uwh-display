class GameState(object):
    game_over = 0
    first_half = 1
    half_time = 2
    second_half = 3
    time_out = 4

class GameManager(object):
    def __init__(self):
        self._white_score = 0
        self._black_score = 0
        self._game_clock = 0
        self._is_clock_running = False
        self._game_state = GameState.game_over

    def gameClock(self):
        return self._game_clock

    def setGameClock(self, n):
        self._game_clock = n

    def whiteScore(self):
        return self._white_score

    def setWhiteScore(self, n):
        self._white_score = n

    def blackScore(self):
        return self._black_score

    def setBlackScore(self, n):
        self._black_score = n

    def setGameClockRunning(self, b):
        self._is_clock_running = b

    def gameStateFirstHalf(self):
        return self._game_state == GameState.first_half

    def setGameStateFirstHalf(self):
        self._game_state = GameState.first_half

    def gameStateHalfTime(self):
        return self._game_state == GameState.half_time

    def setGameStateHalfTime(self):
        self._game_state = GameState.half_time

    def gameStateSecondHalf(self):
        return self._game_state == GameState.second_half

    def setGameStateSecondHalf(self):
        self._game_state = GameState.second_half

    def gameStateGameOver(self):
        return self._game_state == GameState.game_over

    def setGameStateGameOver(self):
        self._game_state = GameState.game_over

    def gameStateRefTimeOut(self):
        return self._game_state == GameState.time_out

    def setGameStateRefTimeOut(self):
        self._game_state = GameState.time_out

