from selenium.common.exceptions import NoSuchElementException
from selenium import webdriver
import time


class MoveMaker:
    def __init__(self):
        self.browser = webdriver.Chrome('/Users/jrobles/Desktop/School/CIS/330/twitch-plays-chess/chromedriver')
        self.browser.get('http://chess.com/')
        self.squareSize = None
        self.constant = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H']

    def signin(self):
        self.browser.find_element_by_xpath('//*[@id="username"]').send_keys('cis330playschess')
        time.sleep(.2)
        self.browser.find_element_by_xpath('//*[@id="password"]').send_keys('cs330norris')
        time.sleep(.2)
        self.browser.find_element_by_xpath('//*[@id="sidebar"]/section[2]/form/div[5]/button').click()

    def startComputerGame(self):
        self.browser.get('https://www.chess.com/play/computer')
        try:
            self.browser.find_element_by_xpath('//*[@id="content"]/div[1]/div/div/div/h2/button').click()

        except NoSuchElementException:
            print('There was no ad')

        self.board = self.browser.find_element_by_xpath('//*[@id="chessboard_boardarea"]')

    def drag(self, i, j):
        # pieceToMove and whereToMove are strings that give the location of the squares that a piece is located and
        # where to move to
        # String should be in format A-H are the columns and 1-8 are the rows
        # Ex. A1 is the bottom left corner, H8 is the top right corner
        # This function runs assuming that the move is already valid. That will be checked before being called
        self.squareSize = self.board.get_attribute('style').split()[-1]
        self.squareSize = int(int(self.squareSize[:-3]) / 8)
        action = webdriver.common.action_chains.ActionChains(self.browser)
        x1 = (self.constant.index(i[0]))
        y1 = (8 - int(i[1]))
        print(x1, y1)
        x2 = self.constant.index(j[0]) - x1
        y2 = (8 - int(j[1])) - y1
        print(x2, y2)

        action.move_to_element_with_offset(self.board, x1*self.squareSize + self.squareSize/2, y1*self.squareSize + self.squareSize/2).click_and_hold().perform()
        for i in range(int(self.squareSize/2)):
            action.move_by_offset(2*x2, 2*y2)
        action.release()
        action.perform()

    def move(self, pieceToMove, whereToMove):
        # pieceToMove and whereToMove are strings that give the location of the squares that a piece is located and
        # where to move to
        # String should be in format A-H are the columns and 1-8 are the rows
        # Ex. A1 is the bottom left corner, H8 is the top right corner
        # This function runs assuming that the move is already valid. That will be checked before being called
        self.squareSize = self.board.get_attribute('style').split()[-1]
        self.squareSize = int(int(self.squareSize[:-3]) / 8)
        action = webdriver.common.action_chains.ActionChains(self.browser)
        print("Moving piece at {} to {}".format(pieceToMove, whereToMove))

        for i in [pieceToMove, whereToMove]:
            action.move_to_element_with_offset(self.board, (self.constant.index(i[0]) * self.squareSize) + self.squareSize/2, (8 - int(i[1])) * self.squareSize + self.squareSize/2).perform()
            action.click().perform()

    def get_piece(self):
        pass


def main():
    print("Starting up")
    mm = MoveMaker()
    print("Signing in")
    mm.signin()
    print("Starting game with comp")
    mm.startComputerGame()
    while True:
        move = input("Please enter your move: ")
        if move in "exit":
            break

        elif move in "reset":
            mm.browser.refresh()

        else:
            try:
                l = move.split()
                mm.move(l[0], l[1])
            except Exception as e:
                print("That was not a valid move. Try Again")


if __name__ == "__main__":
    main()
