# -*- coding: utf-8 -*-
import scrapy
from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.contrib.linkextractors import LinkExtractor

class TwooUserItem(scrapy.Item):
    userName = scrapy.Field()
    userId = scrapy.Field()

class TwooSpider(scrapy.Spider):
    #Name of the spider
    name = "twoo"

    #The domains that are allowed for the spider to crawl
    allowed_domains = ["twoo.com"]

    #Empty global arrays to fill in using command line arguments
    start_urls = []
    page_no = '0';
    locationID = 'null'

    #Overriding the init method to read command line arguments
    def __init__(self, *args, **kwargs):
        super(TwooSpider, self).__init__(*args, **kwargs)
        #Splitting the command line arguments from the args command line
        #argument.
        cmdLineArgs = kwargs.get("args").split(',')
        self.start_urls.append(cmdLineArgs[0])
        self.page_no = cmdLineArgs[1]
        self.locationID = cmdLineArgs[2]

        #Prints to ensure that the command line arguments are printed as
        #passed.
        print self.start_urls[0]
        print self.page_no
        print self.locationID

    #This function parses the response after the crawl.
    def parse(self, response):
        #Debug print to indicate that the parse after crawl has begun
        print "parse fn"

        #The form data on the Twoo search page is modified to ensure that the
        #search is done in the correct region.
        #The search is restricted between ages 18 and 50.
        #Radius 1 indicates that the search would be performed within a 15 mile radius
        #The locationID indicates the location for which the search is being done.
        request = [scrapy.FormRequest(url = self.start_urls[0] + '?page=' + str(self.page_no),
        formdata = {'minAge' : '18', 'maxAge' : '50', 'radius' : '1', 'locationID' : str(self.locationID)},
        callback = self.afterChangeForm)]
        return request

    #This function is registered as a callback from the parse function and gets called
    #after the crawl has been completed.
    def afterChangeForm(self, response):
        #Debug print to indicate entry to this function.
        print "after fn"

        #Create an empty users list
        users = []

        i = 0
        #Loop through the response for 20 times because the response on a particular
        #page has at max 20 users. Extract the userId using the appropriate xpath
        #expression
        while i < 20:
            j = i + 1;

            #xpath expression to extract jth user
            xpathString = "//*[@id=\"searchPage_" + str(self.page_no)+ "\"]/div[2]/a[" + str(j) + "]/@href"

            #Extract the user string and break if found to be empty.
            userString = str(response.xpath(xpathString).extract())
            if userString == "[]":
                break

            #Debug print to ensure that the user string is being extracted appropriately.
            print userString

            #Split with a hiphen because Twoo hyperlinks have username-userID
            userInfo = userString.split("-")

            #Create a user object and store the userName and User ID
            user = TwooUserItem()
            userInfo[0] = userInfo[0][4:]
            userInfo[1] = userInfo[1][:-2]

            #Store the user info in the user object.
            user['userName'] = userInfo[0]
            user['userId'] = userInfo[1]

            #Add the user to the list of users.
            users.append(user)
            i += 1

        #Return the entire user list. Depending on how the spider is run,
        #the input might get written to a CSV or a JSON file.
        return users
