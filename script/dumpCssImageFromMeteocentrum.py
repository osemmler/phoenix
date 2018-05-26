#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, re
from PIL import Image
im = Image.open("weather.png")

f = open("x")
css = f.read()

states = re.findall("state-(\d+):after {",css)
size = [40,35]

for state in states:
    start = re.findall("state-"+state+":after {\n background-position:(-?\d+)p?x? (-?\d+)p?x?",css)[0]
    start = [-int(start[0]),-int(start[1])]
    crop_rectangle = (start[0], start[1], start[0]+size[0], start[1]+size[1])    
    cropped_im = im.crop(crop_rectangle)
    cropped_im.save("state_"+state+".png")
