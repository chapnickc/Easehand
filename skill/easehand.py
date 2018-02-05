import time
import logging
import os

from flask import Flask
from flask_ask import Ask, request, session, question, statement

from bluepy import btle


app = Flask(__name__)
ask = Ask(app, "/")
logging.getLogger('flask_ask').setLevel(logging.DEBUG)


global tx_char, dev

DEVICE_ADDR = "D0:E0:99:0B:B1:C2"
UART_UUID = btle.UUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E")
TX_UUID = btle.UUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E")
dev = btle.Peripheral()


@ask.launch
def launch():
    speech_text = 'Welcome to the Alexa Eashand Skill. Say connect to connect the device'
    return question(speech_text).reprompt(speech_text).simple_card('Easehand', speech_text)

@ask.intent('ConnectIntent')
def connect_device():
    global tx_char, dev
    dev.connect(DEVICE_ADDR, 'random')
    UART_service = dev.getServiceByUUID(UART_UUID)
    tx_char = UART_service.getCharacteristics(TX_UUID)[0]
    speech_text = 'Connected to ease hand!'
    return statement(speech_text).simple_card('Easehand', speech_text)

@ask.intent('DisconnectIntent')
def disconnect_device():
    global dev
    dev.disconnect()
    speech_text = 'Disconnected from ease hand!'
    return statement(speech_text).simple_card('Easehand', speech_text)

@ask.intent('CloseHandIntent')
def close_hand():
    global tx_char
    try:
        tx_char.write('C'.encode())
        speech_text = 'Closing hand'
        return statement(speech_text).simple_card('Easehand', speech_text)
    except (btle.BTLEException, NameError):
        speech_text = 'Device not connected'
        return statement(speech_text).simple_card('Easehand', speech_text)

@ask.intent('OpenHandIntent')
def open_hand():
    global tx_char
    try:
        tx_char.write('O'.encode())
        speech_text = 'Opening Hand'
        return statement(speech_text).simple_card('Easehand', speech_text)
    except (btle.BTLEException, NameError):
        speech_text = 'Device not connected'
        return statement(speech_text).simple_card('Easehand', speech_text)


@ask.intent('HelloWorldIntent')
def hello_world():
    speech_text = 'Hello there!'
    return statement(speech_text).simple_card('Easehand', speech_text)


@ask.intent('AMAZON.HelpIntent')
def help():
    speech_text = 'You can say hello to me!'
    return question(speech_text).reprompt(speech_text).simple_card('HelloWorld', speech_text)


@ask.session_ended
def session_ended():
    return "{}", 200


if __name__ == '__main__':
    if 'ASK_VERIFY_REQUESTS' in os.environ:
        verify = str(os.environ.get('ASK_VERIFY_REQUESTS', '')).lower()
        if verify == 'false':
            app.config['ASK_VERIFY_REQUESTS'] = False
    app.run(debug=True)
