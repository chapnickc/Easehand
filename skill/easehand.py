import time
import logging
import os

from flask import Flask
from flask_ask import Ask, request, session, question, statement

from bluepy import btle


app = Flask(__name__)
ask = Ask(app, "/")
logging.getLogger('flask_ask').setLevel(logging.DEBUG)


UART_UUID = btle.UUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E")
TX_UUID = btle.UUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E")


global tx_char

@ask.launch
def launch():
    global tx_char
    dev = btle.Peripheral("D0:E0:99:0B:B1:C2", 'random')
    UART_service = dev.getServiceByUUID(UART_UUID)
    tx_char = UART_service.getCharacteristics(TX_UUID)[0]
    speech_text = 'Welcome to the Easehand, you can say open hand, or close hand'
    return question(speech_text).reprompt(speech_text).simple_card('Easehand', speech_text)


@ask.intent('CloseHandIntent')
def close_hand():
    speech_text = 'Disconnected'
    if tx_char:
        tx_char.write('C'.encode())
        speech_text = 'Closing hand'
        return statement(speech_text).simple_card('Easehand', speech_text)
    else:
        return statement(speech_text).simple_card('Easehand', speech_text)

@ask.intent('OpenHandIntent')
def open_hand():
    speech_text = 'Disconnected'
    if tx_char:
        tx_char.write('O'.encode())
        speech_text = 'Opening Hand'
        return statement(speech_text).simple_card('Easehand', speech_text)
    else:
        return statement(speech_text).simple_card('Easehand', speech_text)


@ask.intent('HelloIntent')
def hello_world():
    speech_text = 'Hello there'
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
