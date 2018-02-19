from selenium import webdriver
from selenium.common.exceptions import NoSuchElementException

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

