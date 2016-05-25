#!/usr/bin/env python

from Tkinter import *
from multiprocessing import Process, Queue
from datetime import datetime
import time
import sys

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
             highlightbackground="dark grey", highlightthickness=4, relief='flat')
  b.pack(fill=BOTH, expand=1)
  return sf

class ConfirmManualEditScore(object):
  def __init__(self, master, cancel_continuation, manual_continuation):
    self.root = Toplevel(master)
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 240, 0, 240))

    self.root.overrideredirect(1)

    manual_edit_button = SizedButton(self.root, lambda : self.manual_edit_clicked(),
                                     "MANUALLY EDIT SCORE", "orange", "black", ("Consolas", 36),
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
  def __init__(self):
    self.root = Tk()
    self.root.resizable(width=FALSE, height=FALSE)
    self.root.geometry('{}x{}+{}+{}'.format(800, 480, 0, 0))

    score_font = ("Consolas", 72)
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

    refresh_ms = 250

    # Vars
    ###########################################################################
    self.black_score  = 4
    self.white_score = 11
    self.game_clock_mins = 1
    self.game_clock_secs = 42

    self.white_score_var = StringVar()
    self.white_score_var.set("##")
    self.game_clock_var = StringVar()
    self.game_clock_var.set("##:##")
    self.black_score_var = StringVar()
    self.black_score_var.set("##")

    # Left Column
    ###########################################################################
    white_score_label = SizedLabel(self.root, self.white_score_var, "black",
                                   "white", score_font, score_height,
                                   score_width)
    white_score_label.grid(row=0, column=0)
    def refresh_white(self):
      white_score = 11
      self.white_score_var.set("%d" % (white_score,))
      white_score_label.after(refresh_ms, lambda : refresh_white(self))
    white_score_label.after(refresh_ms, lambda : refresh_white(self))

    white_label_var = StringVar()
    white_label_var.set("WHITE")
    white_label = SizedLabel(self.root, white_label_var, "white", "black",
                             label_font, label_height, label_width)
    white_label.grid(row=1, column=0)

    white_button = SizedButton(self.root, lambda : self.white_clicked(),
                               "WHITE\nSCORE", "dark cyan", "black",
                               button_font, button_height, button_width)
    white_button.grid(row=2, column=0)

    white_penalty = Frame(self.root, height=penalty_height, width=penalty_width,
                          bg="black")
    white_penalty.grid(row=3, column=0)

    # Center Column
    ###########################################################################
    game_clock_label = SizedLabel(self.root, self.game_clock_var, "black", "green",
                                  score_font, clock_height, clock_width)
    game_clock_label.grid(row=0, column=1)
    def refresh_time(self):
      now = datetime.now()
      game_mins = now.minute
      game_secs = now.second
      self.game_clock_var.set("%02d:%02d" % (game_mins, game_secs))
      game_clock_label.after(refresh_ms, lambda : refresh_time(self))
    game_clock_label.after(refresh_ms, lambda : refresh_time(self))

    status = StringVar()
    status.set("GAME TIME")
    status_label = SizedLabel(self.root, status, "black", "green", status_font,
                              status_height, status_width)
    status_label.grid(row=1, column=1)

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
      black_score = 1
      self.black_score_var.set("%d" % (black_score,))
      black_score_label.after(refresh_ms, lambda : refresh_black(self))
    black_score_label.after(refresh_ms, lambda : refresh_black(self))

    self.black_label_var = StringVar()
    self.black_label_var.set("BLACK")
    black_label = SizedLabel(self.root, self.black_label_var, "black", "blue",
                             label_font, label_height, label_width)
    black_label.grid(row=1, column=2)

    black_button = SizedButton(self.root, lambda: self.black_clicked(),
                               "BLACK\nSCORE", "dark cyan", "black",
                               button_font, button_height, button_width)
    black_button.grid(row=2, column=2)

    black_penalty = Frame(self.root, height=penalty_height, width=penalty_width,
                          bg="black")
    black_penalty.grid(row=3, column=2)

    self.root.mainloop()

  def black_clicked(self):
    print "black clicked"
    def manual_continuation():
      print "manual"
    ConfirmManualEditScore(self.root,
                           lambda : None,
                           manual_continuation)

  def gong_clicked(self):
    print "gong clicked"

  def white_clicked(self):
    print "white clicked"
    def manual_continuation():
      print "manual"
    ConfirmManualEditScore(self.root,
                           lambda : None,
                           manual_continuation)

def main():
  nv = NormalView()

if __name__=="__main__":
  main()
