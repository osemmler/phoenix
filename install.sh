#!/bin/bash

##############################################################################
##############################################################################
####                                                                      ####
#### This script install monster-phoenix on current raspberry pi device.  ####
####                                                                      ####
##############################################################################
##############################################################################

##
## Install deb packages
##

sudo apt-get install python-pip qt5-default libqt5svg5-dev

##
## Install python packages
##

sudo pip install beautifulsoup4