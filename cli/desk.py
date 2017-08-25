#/usr/bin/python3

import argparse
import requests
import time

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--url", default="http://192.168.0.145", help="URL for make request to")
    parser.add_argument("time", type=float, default=0.5, help="time to run command for")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-s", "--stop", action="store_true", help="stop desk")
    group.add_argument("-u", "--up", action="store_true", help="raise desk")
    group.add_argument("-d", "--down", action="store_true", help="lower desk")

    args = parser.parse_args()

    if args.stop:
        endpoint = "/stop"
        print("stopping ...")
    elif args.up:
        endpoint = "/up"
        print("Going up...")
    elif args.down:
        endpoint = "/down"
        print("Going down...")


    url = args.url + endpoint
    r = requests.get(url);

    time.sleep(args.time)

    url = args.url + "/stop"
    r = requests.get(url);


if __name__ == "__main__":
    main()
