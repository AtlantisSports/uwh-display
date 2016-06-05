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

        #Creating layout variables
        ###########################################################################
        #Fonts
        score_font = ("Verdana", 20, "bold")
        time_font = score_font

        #Element sizes
        score_width  = 48
        score_height = 40
        label_width = 2.5 * score_width
        label_height = score_height
        clock_height = 1 * score_height
        clock_width = score_width + label_width
        border_side_width = score_width
        border_top_width = clock_width + 2 * border_side_width
        border_side_height = 3 * score_height

        #Setting colors
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
        self.white_team = " White"
        self.black_team = " Black"
        self.border_text = ""

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
        self.border_text_var = StringVar()
        self.border_text_var.set("%s" % (self.border_text,))

        w = Canvas(self)
        w.pack(fill=BOTH, expand=1)

        #w.create_rectangle(boxMargin+2*boxWidth, boxMargin, boxMargin+boxWidth, boxMargin+boxHeight, fill=boxColor1)
        #w.create_rectangle(teamAlign_x - boxMargin, whiteAlign_y - boxMargin, scoreAlign_x - 2 * boxMargin, whiteAlign_y + boxMargin, outline=boxOutline1)

        #Table layout for time and score.

        #Border around table
        border_top_label = SizedLabel(w, self.border_text_var, score_bg,
            score_fg, time_font, clock_height, border_top_width, CENTER)
        border_bottom_label = SizedLabel(w, self.border_text_var, score_bg,
            score_fg, time_font, clock_height, border_top_width, CENTER)
        border_left_label = SizedLabel(w, self.border_text_var, score_bg,
            score_fg, time_font, border_side_height, border_side_width, CENTER)
        border_right_label = SizedLabel(w, self.border_text_var, score_bg,
            score_fg, time_font, border_side_height, border_side_width, CENTER)
        border_top_label.grid(row=0, column=0, columnspan=4)
        border_bottom_label.grid(row=10, column=0, columnspan=4)
        border_left_label.grid(row=1, column=0, rowspan=3)
        border_right_label.grid(row=1, column=3, rowspan=3)


        game_clock_label = SizedLabel(w, self.game_clock_var, score_bg,
            score_fg, time_font, clock_height, clock_width, CENTER)
        game_clock_label.grid(row=1, column=1, columnspan=2)

        white_team = SizedLabel(w, self.white_team_var, white_bg,
            white_fg, score_font, score_height, label_width, W)
        white_team.grid(row=2, column=1)
        white_score_label = SizedLabel(w, self.white_score_var, white_bg,
            white_fg, score_font, score_height, score_width, E)
        white_score_label.grid(row=2, column=2)

        black_team = SizedLabel(w, self.black_team_var, black_bg,
                black_fg, score_font, score_height, label_width, W)
        black_team.grid(row=3, column=1)
        black_score_label = SizedLabel(w, self.black_score_var, black_bg,
            black_fg, score_font, score_height, score_width, E)
        black_score_label.grid(row=3, column=2)



def main():

    root = Tk()
    ex = Example(root)
    # make it cover the entire screen
    w, h = root.winfo_screenwidth(), root.winfo_screenheight()
    root.geometry("%dx%d-0+0" % (w, h))
    root.attributes('-fullscreen', True)
    root.mainloop()

if __name__ == '__main__':
    main()
