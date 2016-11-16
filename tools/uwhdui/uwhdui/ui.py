try:
    from Tkinter import *
except ImportError:
    from tkinter import *

import time

def sized_frame(master, height, width):
   F = Frame(master, height=height, width=width)
   F.pack_propagate(0)
   return F

def SizedLabel(root, var, bg, fg, font, height, width):
  sf = sized_frame(root, height, width)
  l = Label(sf, textvariable=var, bg=bg, fg=fg, font=font)
  l.pack(fill=BOTH, expand=1)
  return sf

def SizedButton(root, callback, text, bg, fg, font, height, width):
  sf = sized_frame(root, height, width)
  b = Button(sf, text=text, command=callback, bg=bg, fg=fg, font=font,
             highlightbackground="dark grey", highlightthickness=4, relief='flat',
             activebackground=bg)
  b.pack(fill=BOTH, expand=1)
  return sf

class ConfirmManualEditScore(object):
  def __init__(self, master, tb_offset, cancel_continuation, manual_continuation):
    self.root = Toplevel(master)
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 310, 0, 170 + tb_offset))

    self.root.overrideredirect(1)
    self.root.transient(master)

    manual_edit_button = SizedButton(self.root, lambda : self.manual_edit_clicked(),
                                     "MANUALLY EDIT SCORE", "orange", "black", ("Consolas", 50),
                                     180, 800)
    manual_edit_button.grid(row=0, column=0)

    cancel_button = SizedButton(self.root, lambda : self.cancel_clicked(),
                                "CANCEL", "red", "black", ("Consolas", 36),
                                130, 800)
    cancel_button.grid(row=1, column=0)

    self.manual_continuation = manual_continuation
    self.cancel_continuation = cancel_continuation

    self.root.mainloop()

  def manual_edit_clicked(self):
    self.root.destroy()
    self.manual_continuation()

  def cancel_clicked(self):
    self.root.destroy()
    self.cancel_continuation()

class ManualEditScore(object):
  def __init__(self, master, tb_offset, white_score, black_score,
               cancel_continuation, submit_continuation):
    self.cancel_continuation = cancel_continuation
    self.submit_continuation = submit_continuation

    self.root = Toplevel(master)
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 310, 0, 170 + tb_offset))

    self.root.overrideredirect(1)
    self.root.transient(master)

    self.white_score = white_score
    self.black_score = black_score
    self.white_new_var = StringVar()
    self.black_new_var = StringVar()
    self.redraw()

    button_font = ("Consolas", 36)
    label_font  = ("Consolas", 96)

    cancel_button = SizedButton(self.root, lambda : self.cancel_clicked(),
                                "CANCEL", "red", "black", button_font,
                                130, 400)
    cancel_button.grid(row=2, column=0, columnspan=2)

    submit_button = SizedButton(self.root, lambda : self.submit_clicked(),
                                "SUBMIT", "green", "black", button_font,
                                130, 400)
    submit_button.grid(row=2, column=2, columnspan=2)

    white_new = SizedLabel(self.root, self.white_new_var, "black", "white", label_font,
                           160, 300)
    white_new.grid(row=0, rowspan=2, column=1)

    black_new = SizedLabel(self.root, self.black_new_var, "black", "blue", label_font,
                           160, 300)
    black_new.grid(row=0, rowspan=2, column=2)

    white_up_button = SizedButton(self.root, lambda : self.white_up(),
                                  "+", "light blue", "black", button_font,
                                  80, 100)
    white_up_button.grid(row=0, column=0)

    white_dn_button = SizedButton(self.root, lambda : self.white_dn(),
                                  "-", "grey", "black", button_font,
                                  80, 100)
    white_dn_button.grid(row=1, column=0)

    black_up_button = SizedButton(self.root, lambda : self.black_up(),
                                  "+", "light blue", "black", button_font,
                                  80, 100)
    black_up_button.grid(row=0, column=3)

    black_dn_button = SizedButton(self.root, lambda : self.black_dn(),
                                  "-", "grey", "black", button_font,
                                  80, 100)
    black_dn_button.grid(row=1, column=3)

  def redraw(self):
    self.white_new_var.set('%d' % (self.white_score,))
    self.black_new_var.set('%d' % (self.black_score,))

  def white_up(self):
    self.white_score = self.white_score + 1
    self.redraw()

  def white_dn(self):
    self.white_score = self.white_score - 1
    self.redraw()

  def black_up(self):
    self.black_score = self.black_score + 1
    self.redraw()

  def black_dn(self):
    self.black_score = self.black_score - 1
    self.redraw()

  def cancel_clicked(self):
    self.root.destroy()
    self.cancel_continuation()

  def submit_clicked(self):
    self.root.destroy()
    self.submit_continuation(self.white_score, self.black_score)

class ConfirmRefTimeOut(object):
  def __init__(self, master, tb_offset, game_clock, edit_continuation,
               resume_continuation):
    self.root = Toplevel(master)
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 190, 0, 115 + 170 + tb_offset))

    self.root.overrideredirect(1)
    self.root.transient(master)

    self.game_clock = game_clock

    resume_button = SizedButton(self.root, lambda : self.resume_clicked(),
                                "RESUME\nPLAY", "green", "black", ("Consolas", 50),
                                190, 400)
    resume_button.grid(row=0, column=0)

    edit_button = SizedButton(self.root, lambda : self.edit_clicked(),
                              "EDIT TIME", "orange", "black", ("Consolas", 50),
                              190, 400)
    edit_button.grid(row=0, column=1)

    self.edit_continuation = edit_continuation
    self.resume_continuation = resume_continuation

    self.root.mainloop()

  def resume_clicked(self):
    self.root.destroy()
    self.resume_continuation(self.game_clock)

  def edit_clicked(self):
    self.root.destroy()
    self.edit_continuation()


class ManualEditTime(object):
  def __init__(self, master, tb_offset, clock_at_pause,
               cancel_continuation, submit_continuation):
    self.cancel_continuation = cancel_continuation
    self.submit_continuation = submit_continuation

    self.root = Toplevel(master)
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 310, 0, 170 + tb_offset))

    self.root.overrideredirect(1)
    self.root.transient(master)

    self.clock_at_pause = clock_at_pause
    self.orig_clock_at_pause = clock_at_pause
    self.clock_running_var = StringVar()
    self.game_clock_var = StringVar()
    self.redraw()

    button_font = ("Consolas", 36)
    label_font  = ("Consolas", 96)
    playpause_button_font = ("Consolas", 36)
    game_clock_font = ("Consolas", 72)

    cancel_button = SizedButton(self.root, lambda : self.cancel_clicked(),
                                "CANCEL", "red", "black", button_font,
                                150, 400)
    cancel_button.grid(row=2, column=0, columnspan=2)

    submit_button = SizedButton(self.root, lambda : self.submit_clicked(),
                                "SUBMIT", "green", "black", button_font,
                                150, 400)
    submit_button.grid(row=2, column=2, columnspan=2)

    m_up_button = SizedButton(self.root, lambda : self.game_clock_m_up(),
                              u"Min \u2191", "light blue", "black", button_font,
                              80, 200)
    m_up_button.grid(row=0, column=0)

    m_dn_button = SizedButton(self.root, lambda : self.game_clock_m_dn(),
                              u"Min \u2193", "grey", "black", button_font,
                              80, 200)
    m_dn_button.grid(row=1, column=0)

    s_up_button = SizedButton(self.root, lambda : self.game_clock_s_up(),
                              u"Sec \u2191", "light blue", "black", button_font,
                              80, 200)
    s_up_button.grid(row=0, column=3)

    s_dn_button = SizedButton(self.root, lambda : self.game_clock_s_dn(),
                              u"Sec \u2193", "grey", "black", button_font,
                              80, 200)
    s_dn_button.grid(row=1, column=3)


    game_clock_new = SizedLabel(self.root, self.game_clock_var, "black", "blue", game_clock_font,
                           160, 400)
    game_clock_new.grid(row=0, rowspan=2, column=1, columnspan=2)

  def redraw(self):
    self.game_clock_var.set('%d:%02d' % (self.clock_at_pause // 60,
                                         self.clock_at_pause % 60))

  def game_clock_s_up(self):
    self.clock_at_pause = self.clock_at_pause + 1
    self.redraw()

  def game_clock_s_dn(self):
    self.clock_at_pause = self.clock_at_pause - 1
    self.redraw()

  def game_clock_m_up(self):
    self.clock_at_pause = self.clock_at_pause + 60
    self.redraw()

  def game_clock_m_dn(self):
    self.clock_at_pause = self.clock_at_pause - 60
    self.redraw()

  def cancel_clicked(self):
    self.root.destroy()
    self.cancel_continuation(self.orig_clock_at_pause)

  def submit_clicked(self):
    self.root.destroy()
    self.submit_continuation(self.clock_at_pause)


class NormalView(object):
  def __init__(self, mgr, iomgr, NO_TITLE_BAR):
    self.mgr = mgr
    self.iomgr = iomgr
    self.first_game_started = False

    self.root = Tk()
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 480, 0, 0))
    if NO_TITLE_BAR:
      self.root.overrideredirect(1)
      self.tb_offset = 0
    else:
      self.tb_offset = 70
    #self.root.mainloop()

    score_font = ("Consolas", 96)
    label_font = ("Consolas", 36)
    status_font = label_font
    button_font = label_font
    gong_font = button_font
    time_change_font = gong_font

    score_width  = 200
    score_height = 120
    label_width = score_width
    label_height = 50
    clock_height = score_height
    clock_width = 400
    status_height = label_height
    status_width = clock_width
    button_width = score_width
    button_height = 120
    gong_width = clock_width
    gong_height = button_height
    penalty_height = 190
    penalty_width = score_width
    ref_signal_height = penalty_height
    ref_signal_width = clock_width
    time_change_height = penalty_height
    time_change_width = clock_width

    refresh_ms = 50

    # Vars
    ###########################################################################
    game_clock = self.mgr.gameClock()
    self.game_clock_mins = game_clock // 60
    self.game_clock_secs = game_clock % 60

    self.white_score_var = StringVar()
    self.white_score_var.set("##")
    self.game_clock_var = StringVar()
    self.game_clock_var.set("##:##")
    self.black_score_var = StringVar()
    self.black_score_var.set("##")

    self.status_var = StringVar()
    self.status_var.set("FIRST HALF")

    # Left Column
    ###########################################################################
    white_score_label = SizedLabel(self.root, self.white_score_var, "black",
                                   "white", score_font, score_height,
                                   score_width)
    white_score_label.grid(row=0, column=0)
    def refresh_white(self):
      self.white_score_var.set("%d" % (self.mgr.whiteScore(),))
      white_score_label.after(refresh_ms, lambda : refresh_white(self))
    white_score_label.after(refresh_ms, lambda : refresh_white(self))

    white_label_var = StringVar()
    white_label_var.set("WHITE")
    white_label = SizedLabel(self.root, white_label_var, "white", "black",
                             label_font, label_height, label_width)
    white_label.grid(row=1, column=0)

    white_button = SizedButton(self.root, lambda : self.score_change_clicked(),
                               "WHITE\nSCORE", "dark cyan", "black",
                               button_font, button_height, button_width)
    white_button.grid(row=2, column=0)

    white_penalty = Frame(self.root, height=penalty_height, width=penalty_width,
                          bg="black")
    white_penalty.grid(row=3, column=0)

    # Center Column
    ###########################################################################
    self.game_clock_label = SizedLabel(self.root, self.game_clock_var, "black", "#000fff000",
                                       score_font, clock_height, clock_width)
    self.game_clock_label.grid(row=0, column=1)

    self.status_label = SizedLabel(self.root, self.status_var, "black", "#000fff000", status_font,
                                   status_height, status_width)
    self.status_label.grid(row=1, column=1)

    def refresh_time(self):
      game_clock = self.mgr.gameClock()
      game_mins = game_clock // 60
      game_secs = game_clock % 60
      self.game_clock_var.set("%02d:%02d" % (game_mins, game_secs))

      if game_clock <= 0:
        self.mgr.setGameClockRunning(False)
        if self.mgr.gameStateFirstHalf():
          self.mgr.setGameStateHalfTime()
          #FIXME:
          #self.mgr.setGameClock(HALF_TIME_DURATION)
          self.gong_clicked()
          self.mgr.setGameClockRunning(True)
          self.root.update()
        elif self.mgr.gameStateHalfTime():
          self.mgr.setGameStateSecondHalf()
          #FIXME:
          #self.mgr.setGameClock(HALF_PLAY_DURATION)
          self.gong_clicked()
          self.mgr.setGameClockRunning(True)
          self.root.update()
        elif self.mgr.gameStateSecondHalf():
          self.mgr.setGameStateGameOver()
          #FIXME:
          #self.mgr.setGameClock(GAME_OVER_DURATION)
          self.gong_clicked()
          self.mgr.setGameClockRunning(True)
          self.root.update()
        elif self.mgr.gameStateGameOver():
          self.mgr.setBlackScore(0)
          self.mgr.setWhiteScore(0)
          self.mgr.setGameStateFirstHalf()
          #FIXME:
          #self.mgr.setGameClock(HALF_PLAY_DURATION)
          self.gong_clicked()
          self.mgr.setGameClockRunning(True)

      if self.mgr.gameStateFirstHalf():
        self.status_var.set("FIRST HALF")
      elif self.mgr.gameStateHalfTime():
        self.status_var.set("HALF TIME")
      elif self.mgr.gameStateSecondHalf():
        self.status_var.set("SECOND HALF")
      elif self.mgr.gameStateGameOver():
        self.status_var.set("GAME OVER")
      elif self.mgr.gameStateRefTimeOut():
        self.status_var.set("REF TIMEOUT")
      self.root.update()

      self.game_clock_label.after(refresh_ms, lambda : refresh_time(self))
    self.game_clock_label.after(refresh_ms, lambda : refresh_time(self))
    self.refresh_time = refresh_time

    gong_button = SizedButton(self.root, lambda : self.gong_clicked(), "GONG",
                              "red", "black", gong_font, gong_height,
                              gong_width)
    gong_button.grid(row=2, column=1)

    time_button = SizedButton(self.root, lambda : self.ref_timeout_clicked(),
                              "REF TIMEOUT", "yellow", "black", time_change_font,
                              time_change_height, time_change_width)
    time_button.grid(row=3, column=1)

    #ref_signal_cover = Frame(self.root, height=ref_signal_height,
    #                         width=ref_signal_width, bg="black")
    #ref_signal_cover.grid(row=3, column=1)

    # Right Column
    ###########################################################################
    black_score_label = SizedLabel(self.root, self.black_score_var, "black",
                                   "blue", score_font, score_height,
                                   score_width)
    black_score_label.grid(row=0, column=2)
    def refresh_black(self):
      self.black_score_var.set("%d" % (self.mgr.blackScore(),))
      black_score_label.after(refresh_ms, lambda : refresh_black(self))
    black_score_label.after(refresh_ms, lambda : refresh_black(self))

    self.black_label_var = StringVar()
    self.black_label_var.set("BLACK")
    black_label = SizedLabel(self.root, self.black_label_var, "black", "blue",
                             label_font, label_height, label_width)
    black_label.grid(row=1, column=2)

    black_button = SizedButton(self.root, lambda: self.score_change_clicked(),
                               "BLACK\nSCORE", "dark cyan", "black",
                               button_font, button_height, button_width)
    black_button.grid(row=2, column=2)

    black_penalty = Frame(self.root, height=penalty_height, width=penalty_width,
                          bg="black")
    black_penalty.grid(row=3, column=2)

    def poll_clicker(self):
      if self.iomgr.readClicker():
        print("remote clicked")
        self.gong_clicked()
      else:
        self.iomgr.setSound(0)
      self.root.after(refresh_ms, lambda : poll_clicker(self))
    self.root.after(refresh_ms, lambda : poll_clicker(self))


    self.root.mainloop()

  def gong_clicked(self):
    print("gong clicked")
    if not self.first_game_started:
      self.first_game_started = True
      self.mgr.setGameClockRunning(True)
    self.iomgr.setSound(1)
    time.sleep(1)
    self.iomgr.setSound(0)

  def score_change_clicked(self):
    def manual_continuation():
      def submit_clicked(white_score, black_score):
        self.mgr.setWhiteScore(white_score)
        self.mgr.setBlackScore(black_score)

      ManualEditScore(self.root, self.tb_offset,
                      self.mgr.whiteScore(), self.mgr.blackScore(),
                      lambda : None, submit_clicked)

    ConfirmManualEditScore(self.root,
                           self.tb_offset,
                           lambda : None,
                           manual_continuation)

  def ref_timeout_clicked(self, save_state=True):
    # The awkward sequence here is to work around a bug in the c++ code,
    # which can't easily be fixed at the moment: it is baked into the displays.
    #
    # The bug itself is that self.mgr.gameClock() returns the wrong value
    # when it is called while the clock is paused. It does produce the correct
    # value when the clock is running, so we save it off, stop the clock, and
    # write the saved value so that all of the clock displays get the correct
    # paused time.
    clock_at_pause = self.mgr.gameClock()
    self.mgr.setGameClockRunning(False)
    self.mgr.setGameClock(max(clock_at_pause,0))

    if save_state:
      if self.mgr.gameStateFirstHalf():
        self.state_before_pause = "FIRST HALF"
      elif self.mgr.gameStateHalfTime():
        self.state_before_pause = "HALF TIME"
      elif self.mgr.gameStateSecondHalf():
        self.state_before_pause = "SECOND HALF"
      elif self.mgr.gameStateGameOver():
        self.state_before_pause = "GAME OVER"
      elif self.mgr.gameStateRefTimeOut():
        self.state_before_pause = "REF TIMEOUT"

      self.mgr.setGameStateRefTimeOut()

    self.refresh_time(self)

    def edit_continuation(self):
      def submit_clicked(game_clock):
        self.mgr.setGameClock(max(game_clock, 0))
        self.ref_timeout_clicked(save_state=False)

      def cancel_clicked(game_clock):
        self.mgr.setGameClock(max(clock_at_pause, 0))
        self.ref_timeout_clicked(save_state=False)

      ManualEditTime(self.root, self.tb_offset, clock_at_pause,
                     cancel_clicked, submit_clicked)

    def resume_continuation(self, pause_time):
      if self.state_before_pause == "FIRST HALF":
        self.mgr.setGameStateFirstHalf()
      elif self.state_before_pause == "HALF TIME":
        self.mgr.setGameStateHalfTime()
      elif self.state_before_pause == "SECOND HALF":
        self.mgr.setGameStateSecondHalf()
      elif self.state_before_pause == "GAME OVER":
        self.mgr.setGameStateGameOver()
      elif self.state_before_pause == "REF TIMEOUT":
        self.mgr.setGameStateFirstHalf()
        print("something strange in the resume continuation")
      self.mgr.setGameClockRunning(True)
      self.mgr.setGameClock(max(pause_time, 0))

    ConfirmRefTimeOut(self.root,
                      self.tb_offset,
                      clock_at_pause,
                      lambda : edit_continuation(self),
                      lambda pause_time : resume_continuation(self, pause_time))

