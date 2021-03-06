#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Load data from weather website.
#
# Output format for one day: "hightemp#lowtemp#iconfile\n"

import re, sys
from datetime import date, datetime
from bs4 import BeautifulSoup
from MyBrowser import MyBrowser
import urllib2
import json

reload(sys)
sys.setdefaultencoding('utf8')

def wunderground():
    f = urllib2.urlopen('http://api.wunderground.com/api/WU_4377582/forecast10day/q/CA/San_Francisco.json')
    json_string = f.read()
    print json_string
    #parsed_json = json.loads(json_string)
    #location = parsed_json['location']['city']
    #temp_f = parsed_json['current_observation']['temp_f']
    #print "Current temperature in %s is: %s" % (location, temp_f)
    f.close()

def accuweather():

    # mapping based on https://developer.accuweather.com/weather-icons
    iconMap = {
    "1" : "sun.svg",
    "2" : "sun.svg",
    "3" : "cloud-sun.svg",
    "4" : "cloud-sun.svg",
    "5" : "clouds-sun.svg",
    "6" : "clouds-sun.svg",
    "7" : "cloud.svg",
    "8" : "clouds.svg",
    "11": "fog.svg",
    "12": "cloud-drizzle.svg",
    "13": "cloud-drizzle-sun.svg",
    "14": "cloud-drizzle-sun.svg",
    "15": "cloud-rain-lightning.svg",
    "16": "cloud-rain-lightning-sun.svg",
    "17": "cloud-rain-lightning-sun.svg",
    "18": "cloud-rain.svg",
    "19": "cloud-snow.svg",                 #Flurries
    "20": "cloud-snow-sun.svg",
    "21": "cloud-snow-sun.svg",
    "22": "cloud-snow.svg",                 #Snow
    "23": "cloud-snow-sun.svg",
    "24": "cloud-hail.svg",                 #Ice
    "25": "cloud-hail.svg",                 #Sleet
    "26": "cloud-rain.svg",                 #Freezing Rain 
    "29": "cloud-rain.svg",                 #Rain and Snow 
    "30": "thermometer-100.svg",            #Hot
    "31": "thermometer-25.svg",             #Cold
    "32": "wind.svg",                       #Windy
    "33": "moon-50.svg",                    #Night Clear
    "34": "moon-50.svg",                    #Night Mostly Clear
    "35": "cloud-moon.svg",                 #Night Partly Cloudy 
    "36": "cloud-moon.svg",                 #Night Intermittent Cloudy 
    "37": "clouds-moon.svg",                #Hazy Moonlight
    "38": "clouds-moon.svg",                #Night Mostly Cloudy 
    "38": "clouds-moon.svg",                #Night Mostly Cloudy 
    "39": "cloud-drizzle-moon.svg",         #Night Partly Cloudy w/ Showers 
    "40": "cloud-drizzle-moon.svg",         #Night Mostly Cloudy w/ Showers 
    "41": "cloud-rain-lightning-moon.svg",  #Night Partly Cloudy w/ T-Storms 
    "42": "cloud-rain-lightning-moon.svg",  #Night Mostly Cloudy w/ T-Storms  
    "43": "cloud-snow-moon.svg",            #Night Mostly Cloudy w/ Flurries 
    "44": "cloud-snow-moon.svg",            #Night Mostly Cloudy w/ Snow  
    }    
    
    #url = "https://www.accuweather.com/en/cz/prague/125594/daily-weather-forecast/125594"
    url = "https://www.meteocentrum.cz/predpoved-pocasi/cz/6250/praha"

    browser = MyBrowser()
    html = browser.get(url)
    soup = BeautifulSoup(html, "html.parser")
    
    #with open('x.html', 'w') as f:
    #    f.write(html)
    #sys.exit(1)      

    i = 0
    days = soup.find("div",id="snippet--days")
    for day in days.find_all("li"):
        
        temp = day.find("span", "tabs__temp").text
        temp_h = temp.split("/")[0][:-1]
        temp_l = temp.split("/")[1][:-1]
        
        #icon = fday_html.find("div","icon").get('class', [])[1]
        
        #iconKey = icon.split('-')[1]
        #if iconKey in iconMap:
            #iconfile = "accuweather/"+str(iconMap[iconKey])
        #else:
            #iconfile = iconKey            
    
        sys.stdout.write(temp_h)
        sys.stdout.write("#")
        sys.stdout.write(temp_l)
        sys.stdout.write("#")
        sys.stdout.write("1")
        sys.stdout.write("\n")

        i=i+1
        if (i==5) : break

accuweather()
#wunderground()

