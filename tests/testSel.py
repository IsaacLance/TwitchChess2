from selenium import webdriver
from selenium.common.exceptions import NoSuchElementException
from selenium import webdriver

browser = webdriver.Chrome('/Users/jrobles/Desktop/School/CIS/330/twitch-plays-chess/chromedriver')
browser.get('http://chess.com/')

browser.find_element_by_xpath('//*[@id="username"]').send_keys('')
browser.find_element_by_xpath('//*[@id="password"]').send_keys('')
browser.find_element_by_xpath('//*[@id="sidebar"]/section[2]/form/div[5]/button').click()

try:
    browser.find_element_by_xpath('//*[@id="content"]/div[1]/div/div/div/h2/button').click()

except NoSuchElementException:
    print('There was no ad')

browser.get('https://www.chess.com/play/computer')

try:
    browser.find_element_by_xpath('//*[@id="newbie-modal"]/div/div/button').click()
except NoSuchElementException:
    print("No popup")

board = browser.find_element_by_xpath('//*[@id="chessboard_boardarea"]')

while True:
        move = input("Please enter your move: ")
        if move is "exit":
            break

        else:
            action = webdriver.common.action_chains.ActionChains(browser)
            action.move_to_element_with_offset(board, 10, 552)
            action.click()
            action.perform()

