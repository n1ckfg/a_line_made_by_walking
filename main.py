#!/usr/bin/env python

import os, json, model
from housepy import config, log, strings, tornado_server
from housepy import process as pc
from process import process_walk

pc.secure_pid(os.path.join(os.path.dirname(__file__), "run"))


class Home(tornado_server.Handler):

    def get(self, page=None):
        log.info("Home.get")
        return self.render("home.html", {'walks': model.fetch_walks()})

    def post(self, nop=None):
        log.info("Home.post")
        try:
            data = json.loads(self.get_argument('walk_data'))
            log.debug(data)
        except Exception as e:
            return self.error(log.exc(e))
        if not len(data['accel_data']):
            return self.error("NO DATA")
        walk_id = model.insert_walk(data)
        log.info("Processing data...")
        try:    
            process_walk(data['accel_data'], walk_id)
        except Exception as e:
            return self.error("Could not process: %s" % log.exc(e))
        log.info("--> done")
        return self.text("OK")


class Sequence(tornado_server.Handler):

    def get(self, walk_id=None):
        log.info("Sequence.get %s" % walk_id)
        data = model.fetch_squence(walk_id)
        return self.json(data)


def main():
    handlers = [
        (r"/sequence/?([^/]*)", Sequence),    
        (r"/?([^/]*)", Home),
    ]
    tornado_server.start(handlers)      
                     
if __name__ == "__main__":
    main()