# app.py

from flask import Flask, render_template, request, redirect
import sqlite3
from datetime import datetime

app = Flask(__name__)

def get_db_connection():
    conn = sqlite3.connect('attendance.db')
    conn.row_factory = sqlite3.Row
    return conn

@app.route('/')
def index():
    conn = get_db_connection()
    attendance = conn.execute('SELECT * FROM attendance').fetchall()
    conn.close()
    return render_template('index.html', attendance=attendance)

@app.route('/register', methods=['POST'])
def register():
    name = request.form['name']
    date = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    
    conn = get_db_connection()
    conn.execute('INSERT INTO attendance (name, date) VALUES (?, ?)', (name, date))
    conn.commit()
    conn.close()
    
    return redirect('/')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, debug=False)
