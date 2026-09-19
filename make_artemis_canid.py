# make_artemis_canid.py:
# Uses "CAN messages definition" spreadsheet to generate `artemis_canid.hpp`
# My goal is to make this work without any external libraries

import urllib.request
import csv

SPREADSHEET_URL = "https://docs.google.com/spreadsheets/d/18iwNaZD5jmWbgMh4tzVz-yI6ByvRYDDgnM9hWbDSNEU"
SPREADSHEET_EXPORT_URL = f"{SPREADSHEET_URL}/export?format=csv"

with urllib.request.urlopen(SPREADSHEET_EXPORT_URL) as f:
    content: bytes = f.read()
    print(content.decode())

