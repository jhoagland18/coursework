'''
@author: Jackson
built for python3.5
'''
import tweepy, sys, threading, time


#enter the corresponding information from your Twitter application:
CONSUMER_KEY = 'redacted'#keep the quotes, replace this with your con$
CONSUMER_SECRET = 'redacted'#keep the quotes$
ACCESS_KEY = 'redacted'#keep the quotes, rep$
ACCESS_SECRET = 'redacted'#keep the quotes, repla$
auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)
auth.set_access_token(ACCESS_KEY, ACCESS_SECRET)
api = tweepy.API(auth)

filterWords=sys.argv
with open("mentions.txt","w+") as f:
            f.seek(0)
            f.write(str(0))
            f.truncate()

numStreams=0

mentions=0
lock = threading.Lock()

class MyStreamListener(tweepy.StreamListener):

    def on_status(self, status):
        lock.acquire()
        try:
            global mentions
            mentions+=1
        finally:
            lock.release()
        print(status.text.encode('utf-8'))
        #print("")


    def on_error(self,status):
        SystemExit



myStreamListener = MyStreamListener()
myStream = tweepy.Stream(auth = api.auth, listener=myStreamListener)
myStream.filter(track=filterWords,async=True) # Open the stream to work on asynchronou$
while(True):
        with open("mentions.txt","w+") as f:
                                f.seek(0)
                                f.write(str(mentions))
                                f.truncate()
        time.sleep(.5)
