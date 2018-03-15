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
        print("Starting game")
        self.browser.get('https://www.chess.com/play/computer')

        self.board = self.browser.find_element_by_xpath('//*[@id="chessboard_boardarea"]')
        self.squareSize = self.board.get_attribute('style').split()[-1]
        self.squareSize = int(int(self.squareSize[:-3]) / 8)
        time.sleep(1)

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

    def getBoard(self):
        newBoard = [[None] * 8 for x in range(8)]
        l = self.browser.find_elements_by_class_name('chess_com_piece')
        for i in l:
            src = i.get_attribute('src')[-6:][:-4]
            i = i.get_attribute('style').split()
            col = i[-2][10:-3]
            row = i[-1][:-4]
            newBoard[int(row)//self.squareSize][int(col)//self.squareSize] = src

        return newBoard





#def main():
#    print("Starting up")
#    mm = MoveMaker()
#    print("Signing in")
#    mm.signin()
#    print("Starting game with comp")
#    mm.startComputerGame()
#    mm.getBoard()
#    while True:
#        move = raw_input("Please enter your move: ")
#        if move in "exit":
#            break
#
#        elif move in "reset":
#            mm.browser.refresh()
#
#        else:
#            try:
#                l = move.split()
#                mm.move(l[0], l[1])
#                time.sleep(1)
#                mm.getBoard()
#            except Exception as e:
#                print(e)
#                print("That was not a valid move. Try Again")
#

#if __name__ == "__main__":
#    main()
