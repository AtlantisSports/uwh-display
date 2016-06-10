#!/usr/bin/env python

from Tkinter import *
from multiprocessing import Process, Queue
from datetime import datetime
import time
import sys
import uwhdnodisp as uwhd
import pigpio
import os

HALF_PLAY_DURATION = 11 * 60
HALF_TIME_DURATION = 2 * 60
GAME_OVER_DURATION = 6 * 60

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
  def __init__(self, master, cancel_continuation, manual_continuation):
    self.root = Toplevel(master)
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 240, 0, 240))

    self.root.overrideredirect(1)
    self.root.transient(master)

    manual_edit_button = SizedButton(self.root, lambda : self.manual_edit_clicked(),
                                     "MANUALLY EDIT SCORE", "orange", "black", ("Consolas", 50),
                                     180, 800)
    manual_edit_button.grid(row=0, column=0)

    cancel_button = SizedButton(self.root, lambda : self.cancel_clicked(),
                                "CANCEL", "red", "black", ("Consolas", 36),
                                80, 800)
    cancel_button.grid(row=1, column=0)

    self.manual_continuation = manual_continuation
    self.cancel_continuation = cancel_continuation

    self.root.mainloop()

  def manual_edit_clicked(self):
    self.manual_continuation()
    self.root.destroy()

  def cancel_clicked(self):
    self.cancel_continuation()
    self.root.destroy()

class ManualEditScore(object):
  def __init__(self, master, white_score, black_score,
               cancel_continuation, submit_continuation):
    self.cancel_continuation = cancel_continuation
    self.submit_continuation = submit_continuation

    self.root = Toplevel(master)
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 240, 0, 240))

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
                                80, 400)
    cancel_button.grid(row=2, column=0, columnspan=2)

    submit_button = SizedButton(self.root, lambda : self.submit_clicked(),
                                "SUBMIT", "green", "black", button_font,
                                80, 400)
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
    self.cancel_continuation()
    self.root.destroy()

  def submit_clicked(self):
    self.submit_continuation(self.white_score, self.black_score)
    self.root.destroy()

class ConfirmManualEditTime(object):
  def __init__(self, master, cancel_continuation, manual_continuation):
    self.root = Toplevel(master)
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 240, 0, 240))

    self.root.overrideredirect(1)
    self.root.transient(master)

    manual_edit_button = SizedButton(self.root, lambda : self.manual_edit_clicked(),
                                     "MANUALLY EDIT TIME", "orange", "black", ("Consolas", 50),
                                     180, 800)
    manual_edit_button.grid(row=0, column=0)

    cancel_button = SizedButton(self.root, lambda : self.cancel_clicked(),
                                "CANCEL", "red", "black", ("Consolas", 36),
                                80, 800)
    cancel_button.grid(row=1, column=0)

    self.manual_continuation = manual_continuation
    self.cancel_continuation = cancel_continuation

    self.root.mainloop()

  def manual_edit_clicked(self):
    self.manual_continuation()
    self.root.destroy()

  def cancel_clicked(self):
    self.cancel_continuation()
    self.root.destroy()


class NormalView(object):
  def __init__(self, mgr, iomgr):
    self.mgr = mgr
    self.iomgr = iomgr
    self.first_game_started = False

    self.root = Tk()
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 480, 0, 0))
    self.root.overrideredirect(1)
    #self.root.mainloop()

    score_font = ("Consolas", 96)
    label_font = ("Consolas", 36)
    status_font = label_font
    button_font = label_font
    gong_font = button_font

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
    penalty_height = 240
    penalty_width = score_width
    ref_signal_height = penalty_height
    ref_signal_width = clock_width

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
        self.mgr.setGameClockRunning(0)
        if self.mgr.gameStateFirstHalf():
          self.mgr.setGameStateHalfTime()
          self.mgr.setGameClock(HALF_TIME_DURATION)
          print "to half time"
          self.gong_clicked()
          self.mgr.setGameClockRunning(1)
          self.status_var.set("HALF TIME")
          self.root.update()
        elif self.mgr.gameStateHalfTime():
          self.mgr.setGameStateSecondHalf()
          self.mgr.setGameClock(HALF_PLAY_DURATION)
          print "to second half"
          self.gong_clicked()
          self.mgr.setGameClockRunning(1)
          self.status_var.set("SECOND HALF")
          self.root.update()
        elif self.mgr.gameStateSecondHalf():
          self.mgr.setGameStateGameOver()
          self.mgr.setGameClock(GAME_OVER_DURATION)
          print "to game over"
          self.gong_clicked()
          self.mgr.setGameClockRunning(1)
          self.status_var.set("GAME OVER")
          self.root.update()
        elif self.mgr.gameStateGameOver():
          self.mgr.setBlackScore(0)
          self.mgr.setWhiteScore(0)
          self.mgr.setGameStateFirstHalf()
          self.mgr.setGameClock(HALF_PLAY_DURATION)
          print "to first half"
          self.gong_clicked()
          self.mgr.setGameClockRunning(1)
          self.status_var.set("FIRST HALF")
          self.root.update()
      self.game_clock_label.after(refresh_ms, lambda : refresh_time(self))
    self.game_clock_label.after(refresh_ms, lambda : refresh_time(self))

    gong_button = SizedButton(self.root, lambda : self.gong_clicked(), "GONG",
                              "red", "black", gong_font, gong_height,
                              gong_width)
    gong_button.grid(row=2, column=1)

    ref_signal_cover = Frame(self.root, height=ref_signal_height,
                             width=ref_signal_width, bg="black")
    ref_signal_cover.grid(row=3, column=1)

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
        print "remote clicked"
        self.gong_clicked()
      else:
        self.iomgr.setSound(0)
      self.root.after(refresh_ms, lambda : poll_clicker(self))
    self.root.after(refresh_ms, lambda : poll_clicker(self))


    self.root.mainloop()

  def gong_clicked(self):
    print "gong clicked"
    self.iomgr.setSound(1)
    time.sleep(1)
    self.iomgr.setSound(0)
    if not self.first_game_started:
      self.first_game_started = True
      self.mgr.setGameClockRunning(1)

  def score_change_clicked(self):
    def manual_continuation():
      def submit_clicked(white_score, black_score):
        self.mgr.setWhiteScore(white_score)
        self.mgr.setBlackScore(black_score)

      ManualEditScore(self.root, self.mgr.whiteScore(), self.mgr.blackScore(),
                      lambda : None, submit_clicked)

    ConfirmManualEditScore(self.root,
                           lambda : None,
                           manual_continuation)

class IOManager(object):
  def __init__(self):
    os.system("sudo pigpiod")
    sys.stdout.write("Initing IO")
    sys.stdout.flush()
    for i in range(1, 5):
      time.sleep(1)
      sys.stdout.write(".%d" %(4-i,))
      sys.stdout.flush()

    self.io = pigpio.pi()
    self.io.set_mode(4, pigpio.INPUT)
    self.io.set_pull_up_down(4, pigpio.PUD_UP)
    self.io.set_mode(18, pigpio.OUTPUT)
    self.io.set_mode(26, pigpio.OUTPUT)

  def turnOnWetDisplays(self):
    self.io.write(4, 1)

  def readClicker(self):
    return not self.io.read(4)

  def setSound(self, setting):
    self.io.write(26, setting)

def main():
  print "Starting gpio..."
  iomgr = IOManager()

  print "Turning on wet displays"
  iomgr.turnOnWetDisplays()

  print "Starting xbee comms..."
  mgr = uwhd.GameModelManager()
  xbee = uwhd.CreateXBeeSyncServer()
  xbee.Init()
  xbee.setMgr(mgr)

  mgr.setGameStateFirstHalf()
  mgr.setGameClockRunning(0)
  mgr.setBlackScore(0)
  mgr.setWhiteScore(0)
  mgr.setGameClock(HALF_PLAY_DURATION)

  nv = NormalView(mgr, iomgr)

if __name__=="__main__":
  main()
