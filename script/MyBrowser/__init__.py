__author__ = 'mjan'

import requests, os
from cookielib import LWPCookieJar

class MyBrowser(object):
    """HTML Browser """
    headers = {
        'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:47.0) Gecko/20100101 Firefox/47.0'
    }

    def __init__(self):
        #requests.packages.urllib3.disable_warnings()
        self.cookies = {}
        self.session = requests.Session()
        self.session.headers = self.headers

    def get(self, url):
        """
        stahne html zadaneho url metodou get
        """
        #print 'GET %s' % url
        
        self._loadCookies()
        r = self.session.get(url)
        r.encoding = 'utf-8'
        r.raise_for_status()        
        self._saveCookies()           
        return r.text

    def post(self, url, params, jsonStr = "", **kwargs):
        """
        stahne html zadaneho url metodou post
        """        
        #print 'POST %s' % url
        #print 'Params: %s' % params
        
        self._loadCookies()
        if (jsonStr==""):
            r = self.session.post(url, data=params, verify=False)
        else:             
            r = self.session.post(url, data=params, verify=False, json=jsonStr)
        r.raise_for_status()
        self._saveCookies()
        return r.text

    def _loadCookies(self):
        self.session.cookies = LWPCookieJar('cookiejar')
        if not os.path.exists('cookiejar'):
            # Create a new cookies file and set our Session's cookies
            #print('setting cookies')
            self.session.cookies.save()
        else:
            # Load saved cookies from the file and use them in a request
            #print('loading saved cookies')
            self.session.cookies.load(ignore_discard=True)
            
    def _saveCookies(self):
        self.session.cookies.save(ignore_discard=True)
            
        
#    def save_cookies(self,filename):
#        requests_cookiejar = self.session.cookies
#        with open(filename, 'wb') as f:
#            pickle.dump(requests_cookiejar, f)
#
#    def load_cookies(self,filename):
#        with open(filename, 'rb') as f:
#            return pickle.load(f)