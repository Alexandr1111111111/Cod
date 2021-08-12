
import requests
from bs4 import BeautifulSoup
import csv
import os

URL = 'https://auto.drom.ru/porsche/all/'
HEADERS = {'user-agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.164 Safari/537.36 OPR/77.0.4054.277 (Edition Yx)', 'accept': '*/*'}
HOST = 'https://auto.drom.ru'
FILE = 'main_content.csv'

def str_counter(html):
    soup = BeautifulSoup(html, 'html.parser')
    count_pages = soup.find_all('div', 'e15hqrm30')
    if count_pages:
        return int(count_pages[-1].get_text())
    else:
        return 1

def get_html(URL, params=None):
    req = requests.get(URL, headers=HEADERS, params=params)
    return req


def content(html):
    soup = BeautifulSoup(html, 'html.parser')
    items = soup.find_all('a', 'css-1psewqh')
    main_content = []
    for item in items:
        main_content.append({
            'title': item.find('div', 'css-1svsmzw').get_text(),
            'inf': item.find('div', 'css-3xai0o').get_text(),
            'price': item.find('div', 'css-1dv8s3l').get_text(),
            'city': item.find('span', 'css-fbscyn').get_text(),
        })
    return main_content

def save_file(items, path):
    with open(path, 'w', newline='', encoding="utf-8") as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(['NAZWANIE', 'INFORMAZIA', 'ZENA', 'GOROD'])
        for item in items:
            writer.writerow([item['title'], item['inf'], item['price'], item['city']])

def main():
    html = get_html(URL)
    if html.status_code == 200:
        main_content = []
        count_pages = str_counter(html.text)
        for page in range(1, count_pages + 1):
            print(f'Parsing str  {page}  iz  {count_pages} ...')
            html = get_html(URL, params={'page': page})
            main_content.extend(content(html.text))
        print(f'polycheno {len(main_content)} avto')
        print(main_content)
        save_file(main_content, FILE)
        os.startfile(FILE)
    else:
        print('Error')

main()
