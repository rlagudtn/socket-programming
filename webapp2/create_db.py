# create_db.py

import sqlite3

def create_database():
    conn = sqlite3.connect('attendance.db')
    c = conn.cursor()
    c.execute('''CREATE TABLE IF NOT EXISTS attendance
                 (id INTEGER PRIMARY KEY AUTOINCREMENT,
                  name TEXT NOT NULL,
                  date TEXT NOT NULL)''')
    conn.commit()
    conn.close()

if __name__ == '__main__':
    create_database()
