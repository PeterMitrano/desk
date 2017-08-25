#/usr/bin/python3

import argparse
import requests
import time

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--url", default="http://192.168.0.145", help="URL for make request to")
    parser.add_argument("-t", "--time", type=float, default=0.5, help="time to run command for")
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

    move(args.url, endpoint, args.time)


def move(base_url, endpoint, dt):
    url = base_url + endpoint
    r = requests.get(url);

    time.sleep(dt)

    url = base_url + "/stop"
    r = requests.get(url);


if __name__ == "__main__":
    main()
