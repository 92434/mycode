#!/usr/bin/env python
 
import logging
import logging.config
import time
import os
 
#specify logging config file
logging.config.fileConfig("logging.conf")
 
#create and start listener on port 9999
t = logging.config.listen(9999)
t.start()
 
#create logger
logger = logging.getLogger("simpleExample")
 
#watch for existence of file named "f"
#loop through the code while this file exists

#cleanup
logging.config.stopListening()
t.join()

logger.debug("debug message")
logger.info("info message")
logger.warn("warn message")
logger.error("error message")
logger.critical("critical message")

