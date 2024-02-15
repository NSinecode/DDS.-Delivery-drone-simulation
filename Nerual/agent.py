import torch
import random
import NeuralMefedron
from GameDrone import DroneAI, Direction
from typing import List
import numpy as np
from collections import deque
from model import Linear_QNet, QTrainer

import pyqtgraph as pg
from PyQt6.QtWidgets import QApplication
import sys

MAX_MEMORY = 100_000
BATCH_SIZE = 1000
LR = 0.001 #learning rate

Position: List[float] = [0, 2, 0]
Vel: List[float] = [0, 0, 0]
Ax: List[float] = [0, 0, 0]
Angle = float(0)
isrunning = bool(0)

def NewTarger(b):
    points = NeuralMefedron.Received()
    target = points[b]

class Agent: 
    
    def __init__(self):
        self.n_games = 0
        self.epsilon = 0 # randomness
        self.gamma = 0.9   # discount rate
        self.memory = deque(maxlen=MAX_MEMORY) # popleft()
        self.model = Linear_QNet(4, 256, 3)
        self.trainer = QTrainer(self.model, lr=LR, gamma=self.gamma)
    
    def get_state(self, game):
        head = Position
        
        dir_l = GameDrone.direction == Direction.LEFT
        dir_r = GameDrone.direction == Direction.RIGHT
        dir_u = GameDrone.direction == Direction.UP
        
        sc25 = False
        sc50 = False
        sc100 = False
        sc200 = False
        if game.score >= 25:
            sc25 = True
        if game.score >= 50:
            sc50 = True
        if game.score >= 100:
            sc100 = True
        if game.score >= 200:
            sc200 = True
        
        state = [
            
            # Move direction
            dir_l,
            dir_r,
            dir_u,
            
            # point location
            target[0] < Position[0],
            target[0] > Position[0],
            target[6] > Position[6],
            target[6] < Position[6],
            
            # Score more then:
            sc25,   #25
            sc50,   #50
            sc100,  #100
            sc200,  #200
        ]
        
        return np.array(state, dtype=int)
    
    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done)) # popleft if MAX_MEMORY is reached
    
    def train_long_memore(self):
        if len(self.memory) > BATCH_SIZE:
            mini_sample = random.sample(self.memory, BATCH_SIZE) # list of tuples
        else:
            mini_sample = self.memory
            
        states, actions, rewards, next_states, dones = zip(*mini_sample)
        self.trainer.train_step(states, actions, rewards, next_states, dones)
    
    def train_short_memore(self, state, action, reward, next_state, done):
        self.trainer.train_step(state, action, reward, next_state, done)
    
    def get_action(self, state):
        # random moves: tradeoff exploration/exploitation
        self.epsilon = 80 - self.n_games
        final_move = [0, 0, 0]
        if random.randint(0, 200) < self.epsilon:
            move = random.randint(0, 2)
            final_move[move] = 1
        else:
            state0 = torch.tensor(state, dtype=torch.float)
            prediction = self.model(state0)
            move = torch.argmax(prediction).item()
            final_move[move] = 1
            
        return final_move
    
def train():
    games = []
    plot_scores = []
    plot_mean_scores = []
    total_score = 0
    record = 0
    agent = Agent()
    game = DroneAI()
    while True:
        state_old = agent.get_state(game)
        
        final_move = agent.get_action(state_old)
        
        reward, done, score = GameDrone.play_step(final_move)
        state_new = agent.get_state(game)
        
        # train short memory
        agent.train_short_memore(state_old, final_move, reward, state_new, done)
        
        agent.remember(state_old, final_move, reward, state_new, done)
        
        if done:
            # train long memory, plot results
            #Функция перезапуска
            agent.n_games += 1
            games.append(agent.n_games)
            agent.train_long_memore()
            
            if score > record:
                record = score
                agent.model.save()
                
            print('Game', agent.n_games, 'Score', score, 'Record:', record)
            
            # plot
            plot_scores.append(score)
            total_score += score
            mean_score = total_score / agent.n_games
            plot_mean_scores.append(mean_score)
            
            # Display new data
            scores_curve.setData(games, plot_scores)
            mean_scores_curve.setData(games, plot_mean_scores)

            # Set a limit for the y axis (not lower than 0)
            win.setYRange(0, max(max(plot_scores), max(plot_mean_scores)))

            # Add text to the last points on the graph
            if plot_scores:
                # Create a text label for the last point of the curve
                last_score = plot_scores[-1]
                last_mean_score = plot_mean_scores[-1]
                last_game = games[-1]

                # Create text labels
                last_score_text = pg.TextItem('Last Score: {}'.format(last_score), color=(255, 255, 255))
                last_mean_score_text = pg.TextItem('Last Mean Score: {:.2f}'.format(last_mean_score), color=(255, 255, 255))
                
                # Set the text to bold
                last_score_text.setFont(pg.QtGui.QFont("Ebrima", 8, pg.QtGui.QFont.Weight.Bold))
                last_mean_score_text.setFont(pg.QtGui.QFont("Ebrima", 8, pg.QtGui.QFont.Weight.Bold))

                # Set text label positions next to the corresponding curves
                last_score_text.setPos(last_game, last_score)
                last_mean_score_text.setPos(last_game, last_mean_score)
                
                # Remove previous text labels
                for item in win.getPlotItem().items[:]:
                    if isinstance(item, pg.TextItem):
                        win.removeItem(item)

                # Add new text labels to the chart
                win.addItem(last_score_text)
                win.addItem(last_mean_score_text)

            # Refresh the window
            app.processEvents()

if __name__ == "__main__":
    
    app = QApplication([])  # Initializing QGuiApplication

    win = pg.plot(title="Training...")  # Creating a graphic element

    win.setLabel('bottom', 'Number of games')
    win.setLabel('left', 'Score')
    win.setYRange(0, 1)
    # Set the plot background color
    win.setBackground('#262626')
    
    scores_curve = win.plot(pen='r', name='Scores')
    mean_scores_curve = win.plot(pen='g', name='Mean Scores')
    # Set the thickness of the curves
    scores_curve.setPen(pg.mkPen('r', width=3))
    mean_scores_curve.setPen(pg.mkPen('g', width=3))
    
    win.addLegend()
    win.plotItem.legend.addItem(scores_curve, name='Score')
    win.plotItem.legend.addItem(mean_scores_curve, name='Mean Score')
    
    train()
