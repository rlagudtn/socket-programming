# -*-coding:utf-8-*-

# 필요한 라이브러리를 불러옵니다.
import RPi.GPIO as GPIO
from flask import Flask, render_template, request

app = Flask(__name__)

# 불필요한 warning 제거, GPIO핀의 번호 모드 설정
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

# pins란 딕셔너리를 만들어 GPIO 18, 23, 25 핀을 저장합니다
pins = {
    18: {'name': 'RED LED', 'state': GPIO.LOW},
    23: {'name': 'Yellow LED', 'state': GPIO.LOW},
    25: {'name': 'Green LED', 'state': GPIO.LOW}
}

# pins 내에 있는 모든 핀들을 출력으로 설정하고 초기화합니다
for pin in pins:
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)

# 웹서버의 URL 주소로 접근하여 아래의 main() 함수를 실행합니다
@app.route("/")
def main():
    # pins 내에 있는 모든 핀의 현재 핀 상태(ON/OFF)를 업데이트
    for pin in pins:
        pins[pin]['state'] = GPIO.input(pin)
    # templateData에 저장
    templateData = {
        'pins': pins
    }
    # 업데이트된 templateData 값을 homeLED.html로 리턴
    return render_template('homeLED.html', **templateData)

# URL 주소 끝에 "/핀번호/action"을 붙여서 접근시 action에 따라 동작
@app.route("/<changePin>/<action>")
def action(changePin, action):
    # 현재 핀번호를 URL 주소로 받은 핀번호로 설정
    changePin = int(changePin)
    # 핀번호에 설정된 이름값을 불러옴
    deviceName = pins[changePin]['name']
    # action 값이 'on'일 때
    if action == "on":
        GPIO.output(changePin, GPIO.HIGH)
    # action 값이 'off'일 때
    if action == "off":
        GPIO.output(changePin, GPIO.LOW)
    # GPIO 핀의 ON/OFF 상태 저장
    pins[changePin]['state'] = GPIO.input(changePin)
    # 핀들의 값을 업데이트 해서 templateData에 저장
    templateData = {
        'pins': pins
    }
    # 업데이트 된 templateData 값을 homeLED.html로 리턴
    return render_template('homeLED.html', **templateData)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80, debug=False)
