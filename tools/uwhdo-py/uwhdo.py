#!/usr/bin/env python

import sys
import uwhdo
import time
#from termcolor import colored
from Tkinter import *

from multiprocessing import Process, Queue
from datetime import datetime
import time
import sys


#if __name__ == '__main__':
#  coordinator = False
#  xbee_sync = uwhdo.CreateXBeeSync(coordinator)
#
#  mgr = xbee_sync.getMgr()

#  s = True
#Call black score with mgr.scoreBlack()
#Call white score with mgr.scoreWhite()
#call time with mgr.gameTime()


def sized_frame(master, height, width):
   F = Frame(master, height=height, width=width)
   F.pack_propagate(0)
   return F

def SizedLabel(root, var, bg, fg, font, height, width, anchor):
  sf = sized_frame(root, height, width)
  l = Label(sf, textvariable=var, bg=bg, fg=fg, font=font, anchor=anchor)
  l.pack(fill=BOTH, expand=1)
  return sf

class Example(Frame):

    def __init__(self, parent):
        Frame.__init__(self, parent)

        self.parent = parent
        self.initUI()

    def initUI(self):

        self.parent.title("TimeShark Scores")
        self.pack(fill=BOTH, expand=1)

        #Creating Variables
        # scoreAlign_x    = 80
        # teamAlign_x     = 20
        # timeAlign_x     = 30
        # whiteAlign_y    = 20
        # blackAlign_y    = 45
        # timeAlign_y     = 70
        # labelFont       = "Purisa"
        # whiteColor      = "#ffffff"
        # blackColor      = "#0000ff"
        # timeColor       = "#00ff00"
        # boxMargin       = 5
        # boxWidth        = 150
        # boxHeight       = 75
        # boxOutline1     = "#222222"
        # boxColor1       = "#000000"
        # fontSize        = 20

        score_font = ("Verdana", 12, "bold")
        time_font = score_font

        score_width  = 20
        score_height = 25
        label_width = 3* score_width
        label_height = score_height
        clock_height = 1.1 * score_height
        clock_width = score_width + label_width

        #Setting Colors
        white_bg = "black"
        white_fg = "white"
        black_bg = white_bg
        black_fg = "#666666fff"
        score_bg = white_bg
        score_fg = "#000fff000"

        refresh_ms = 250

        # Vars
        ###########################################################################
        self.black_score  = 4
        self.white_score = 10
        self.game_clock_time = 609
        self.white_team = "White"
        self.black_team = "Black"

        self.white_score_var = StringVar()
        self.white_score_var.set("%d" % (self.white_score,))
        self.white_team_var = StringVar()
        self.white_team_var.set("%s" % (self.white_team,))
        self.game_clock_var = StringVar()
        self.game_clock_var.set("%02d:%02d" % (self.game_clock_time//60,self.game_clock_time%60))
        self.black_team_var = StringVar()
        self.black_team_var.set("%s" % (self.black_team,))
        self.black_score_var = StringVar()
        self.black_score_var.set("%d" % (self.black_score,))

        w = Canvas(self)
        w.pack(fill=BOTH, expand=1)

        #w.create_rectangle(boxMargin+2*boxWidth, boxMargin, boxMargin+boxWidth, boxMargin+boxHeight, fill=boxColor1)
        #w.create_rectangle(teamAlign_x - boxMargin, whiteAlign_y - boxMargin, scoreAlign_x - 2 * boxMargin, whiteAlign_y + boxMargin, outline=boxOutline1)

        #Creating a small row and column that give a small margin

        white_team = SizedLabel(w, self.white_team_var, white_bg,
            white_fg, score_font, score_height, score_width, E)
        white_team.grid(row=1, column=0)
        white_score_label = SizedLabel(w, self.white_score_var, white_bg,
            white_fg, score_font, score_height, score_width, E)
        white_score_label.grid(row=1, column=1, ipadx=5)
        black_team = SizedLabel(w, self.black_team_var, black_bg,
                black_fg, score_font, score_height, score_width, E)
        black_team.grid(row=2, column=0)
        black_score_label = SizedLabel(w, self.black_score_var, black_bg,
            black_fg, score_font, score_height, score_width, E)
        black_score_label.grid(row=2, column=1, ipadx=5)
        game_clock_label = SizedLabel(w, self.game_clock_var, score_bg,
            score_fg, time_font, clock_height, clock_width, CENTER)
        game_clock_label.grid(row=0, column=0, columnspan=2)

def main():

    root = Tk()
    ex = Example(root)
    # make it cover the entire screen
    w, h = root.winfo_screenwidth(), root.winfo_screenheight()
    root.geometry("%dx%d-0+0" % (w, h))
    root.mainloop()

if __name__ == '__main__':
    main()
