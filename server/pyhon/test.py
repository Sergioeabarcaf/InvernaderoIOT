# The following lines require manual changes 
username = "idiProteinlab" # enter your username
password = "Proteinlab2017!" # enter your password 
# This example establishes a https connection, but doesn't provide the server certificate validation. 
# Production code should implement certificate validation.
# -------------------------------------------------
import httplib
import base64 

# create HTTP basic authentication string, this consists of 
# "username:password" base64 encoded 
auth = base64.encodestring("%s:%s"%(username,password))[:-1]
webservice = httplib.HTTPSConnection("remotemanager.digi.com")

# to what URL to send the request with a given HTTP method
webservice.putrequest("GET", "/ws/v1/streams/inventory")

# add the authorization string into the HTTP header
webservice.putheader("Authorization", "Basic %s"%auth)

webservice.endheaders()

# get the response
response = webservice.getresponse()
statuscode = response.status
statusmessage = response.reason
response_body = response.read()

# print the output to standard out
print (statuscode, statusmessage)
print response_body

