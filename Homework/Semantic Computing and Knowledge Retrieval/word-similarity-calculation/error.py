from datetime import datetime


def handle_error(s: str):
    print(s)
    with open('./log/error.log', 'a', encoding='utf-8') as f:
        print(datetime.now().strftime('%Y-%m-%d %H:%M:%S') + ' ' + s, file=f)
