#!/usr/bin/env python

import sys
from bs4 import BeautifulSoup
from MyBrowser import MyBrowser

reload(sys)
sys.setdefaultencoding('utf8')

def meteopocasi():
    browser = MyBrowser()
    html = browser.get("http://www.meteo-pocasi.cz/maps/cz/praha/962-meteostanice-praha-lysolaje/")
    soup = BeautifulSoup(html, "html.parser")    
    #with open('x.html', 'w') as f:
    #    f.write(html)    
    svalues = soup.find_all("div","svalue")
    print svalues[0].text

def agrobiologie():
    browser = MyBrowser()
    html = browser.get("http://meteostanice.agrobiologie.cz/")
    soup = BeautifulSoup(html, "html.parser")
    svalues = soup.find_all("span","ramecek")
    print svalues[0].text[:-3].replace(',','.')


#meteopocasi()
agrobiologie()
